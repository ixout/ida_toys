#include <ida.hpp>
#include <idp.hpp>
#include <loader.hpp>
#include <hexrays.hpp>
#include <kernwin.hpp>
#include "MatchingBrace.hpp"

std::vector<std::pair<int, int>> MatchingBrace::matched_;

ssize_t idaapi HandleMatchingBrace(void* ud, hexrays_event_t event, va_list va) {
  vdui_t* vdui = va_arg(va, vdui_t*);
  int shift_state = va_arg(va, int);

  strvec_t& codes = vdui->cfunc->sv;
  ctext_position_t& now_pos = vdui->cpos;
  bool needRefresh = false;
  int now_line = now_pos.lnnum;
  int match_line = now_line;
  if (now_line < codes.size()) {
    qstring line;
    tag_remove(&line, codes[now_line].line);
    const char* pos = strpbrk(line.c_str(), "{}");
    if (pos != NULL) {
      if (!MatchingBrace::IsExist(now_line)) {
        int npos = pos - line.c_str();
        char brace = *pos;
        if (brace == '{') {
          for (;;) {
            tag_remove(&line, codes[++match_line].line);
            char dst = line.at(npos);
            if (dst == '}') {
              break;
            }
          }
        }
        else {
          for (;;) {
            tag_remove(&line, codes[--match_line].line);
            char dst = line.at(npos);
            if (dst == '{') {
              break;
            }
          }
        }
        unsigned color = MatchingBrace::GetColor();
        codes[now_line].bgcolor = color;
        codes[match_line].bgcolor = color;
        MatchingBrace::AddMatched(now_line, match_line);
      }
      else {
        auto [x, y] = MatchingBrace::DeleteMatched(now_line);
        codes[x].bgcolor = 0xffffffff;
        codes[y].bgcolor = 0xffffffff;
      }
      needRefresh = true;
    }
  }

  if (needRefresh) {
    refresh_custom_viewer(vdui->ct);
  }
  return 1;
}

