#pragma once

#include <RED4ext/RED4ext.hpp>

namespace game::input {

class UserMapping {
  /// @hash 2711691758
  void FetchKeysForMapping(CName, bool, red::DynArray<EInputKey> &) const;

  /// @hash 2240420883
  RawMappingCollection const & GetMapping(CName) const;

  // @hash 1385634503
  bool AddMapping(unsigned short key, CName name, EMappingType type, float, CName presetName, float, float, unsigned int, unsigned int);

  enum EMappingType : uint8_t {
    Internal = 0, // maybe
    Axis = 1,
    FakeButton = 2,
    Key = 3,
    Fake = 4,
    Relative = 5,
    Unk6 = 6
  };

};

}