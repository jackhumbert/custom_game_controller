#pragma once

#include <RED4ext/RED4ext.hpp>

namespace game::input {

class UserMapping {
  /// @hash 2711691758
  void FetchKeysForMapping(CName, bool, red::DynArray<EInputKey> &) const;

  /// @hash 2240420883
  RawMappingCollection const & GetMapping(CName) const;

};

}