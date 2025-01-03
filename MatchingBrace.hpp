#pragma once
#include <ctime>
#include <vector>

struct MatchingBrace {
public:
  static bool IsExist(const int n) {
    for (auto& it : matched_) {
      if (n == it.first || n == it.second) {
        return true;
      }
    }
    return false;
  }
  static void AddMatched(const int pre, const int back) {
    matched_.emplace_back(pre, back );
  }
  static std::pair<int, int> DeleteMatched(int n) {
    std::pair<int, int> result;
    for (auto it = matched_.begin(); it != matched_.end();) {
      if (n == it->first || n == it->second) {
        result = *it;
        it = matched_.erase(it);
        break;
      }
      ++it;
    }
    return result;
  }
  static unsigned int GetColor() {
    static int colors_[10] = { 0xCD853F, 0xFFD700, 0xFFFF00, 0x7FFF00, 0xF5FFFA, 0x00BFFF, 0x8A2BE2, 0xFFEFD5, 0xFFC0CB, 0xD8BFD8 };
    srand(time(NULL));
    return colors_[rand() % 10];
  }
private:
  MatchingBrace() = delete;
  static std::vector<std::pair<int, int>> matched_;
};

ssize_t idaapi HandleMatchingBrace(void* ud, hexrays_event_t event, va_list va);