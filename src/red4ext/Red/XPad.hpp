#pragma once

#include <RED4ext/RED4ext.hpp>
#include <Red/IPad.hpp>

class XPad : public IPad {
  // static constexpr const uint32_t VFT = 499584564;
  // virtual uintptr_t Destructor(char a1) override {
  //   RED4ext::UniRelocFunc<decltype(&XPad::Destructor)> call(VFT, 0x00);
  //   return call(this, a1);
  // }
  virtual ~XPad() = delete;

  uint8_t unk08[0x18];
};
