#pragma once

#include <RED4ext/RED4ext.hpp>

class IPad {
public:
  // static constexpr const uint32_t VFT = 3559394325;
  // 00
  // virtual uintptr_t Destructor(char a1) {
  //   RED4ext::UniRelocFunc<decltype(&IPad::Destructor)> call(VFT, 0x00);
  //   return call(this, a1);
  // }
  virtual ~IPad() = delete;
};