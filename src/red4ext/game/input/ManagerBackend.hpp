#pragma once

#include <RED4ext/RED4ext.hpp>

namespace game::input {

class ManagerBackend {
  /// @hash 989204148
  bool IsKey(uint16_t);

  /// @hash 2490568903
  bool IsAxis(uint16_t);

  /// @hash 1763577801
  bool IsPadKey(uint16_t);
};

}