#pragma once

#include <RED4ext/RED4ext.hpp>

namespace game::input {

class UserMappingButtonGroup {
  /// @hash 289806230
  unsigned char GetRawKeyIdxByKeyId(unsigned short) const;

  bool IsKeyPartOfButtonGroup(unsigned short) const;

};

}