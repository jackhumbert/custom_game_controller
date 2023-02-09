#pragma once
#include <RED4ext/RED4ext.hpp>
#include <RED4ext/Scripting/Natives/Generated/ent/Entity.hpp>
#include <RED4ext/Scripting/Natives/Generated/vehicle/BaseObject.hpp>
#include "Addresses.hpp"
#include <RED4ext/Scripting/Natives/Generated/game/EffectSystem.hpp>

// 1.52 RVA: 0x196260 / 1663584
// 1.62 RVA: 0x196960
/// @pattern 48 85 C9 74 2E 53 48 83 EC 20 48 8B D9 48 8D 4C 24 30 E8 09 02 00 00 BA 01 00 00 00 48 8B CB E8
using GamepadDestructor = void __fastcall (void *);

// 1.52 RVA: 0x795360 / 7951200
// 1.6  RVA: 0x79C250 / 7979600
/// @pattern 48 8D 05 ?  ?  ?  02 89 51 08 48 89 01 0F 57 C0 0F 11 41 0C 48 8B C1 C3
using InitializeXPad = void *__fastcall (void *, uint32_t);

// 1.6 RVA: 0x2D6E2C0 / 47637184
/// @pattern  B8 96 00 00 00 66 2B C8 66 83 F9 05 0F 96 C0 C3
using IsAxis = bool __fastcall (uint16_t key);

// 1.6 RVA: 0x2D6E2D0 / 47637200
/// @pattern BA 96 00 00 00 0F B7 C1 66 2B C2 66 83 F8 05 76 21 B8 E4 00 00 00 66 2B C8 66 83 F9 20 77 10 48
using IsButtonToAxis = bool __fastcall (uint16_t key);

// 1.6 RVA: 0x2D6E310 / 47637264
/// @pattern BA 88 00 00 00 0F B7 C1 66 2B C2 66 83 F8 13 76 11 B8 FF 00 00 00 66 2B C8 66 83 F9 07 76 03 32
using IsGamepad = bool __fastcall (uint16_t key);