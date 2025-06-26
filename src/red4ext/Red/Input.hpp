#pragma once

#include <RED4ext/RED4ext.hpp>
#include <RED4ext/Scripting/Natives/Generated/EInputKey.hpp>

enum EInputAction {
  IACT_None = 0x0,
  IACT_Press = 0x1,
  IACT_Release = 0x2,
  IACT_Axis = 0x3,
};

enum EInputDevice {
  INVALID = 0x0,
  KBD_MOUSE = 0x1,
  ORBIS = 0x2,
  DURANGO = 0x3,
  STEAM = 0x4,
  XINPUT_PAD = 0x5,
  STADIA = 0x6,
  NINTENDO_SWITCH = 0x7,
  EID_COUNT = 0xC,
};

// input::BufferedInputEvent
struct Input {
  RED4ext::EInputKey key;
  EInputAction action;
  float value;
  uint32_t unk0C;
  uint32_t unk10;
  uint32_t unk14;
  uint32_t unk18; // source, maybe? 2 disables processing in some places
  uint32_t unk1C;
  HWND hwnd;
  uint64_t index;
  uint64_t unk30;
  uint8_t unk38;
  uint8_t unk39;
  uint8_t unk3A;
  uint8_t unk3B;
  uint8_t unk3C;
  uint8_t unk3D;
  uint8_t unk3E;
  uint8_t unk3F;
};

RED4EXT_ASSERT_SIZE(Input, 0x40);

Input *__fastcall UpdateInput(Input *input, RED4ext::EInputKey key, EInputAction action, float value, int a5, int a6, HWND hwnd, uint32_t index);