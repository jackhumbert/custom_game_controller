#pragma once
#include <RED4ext/RED4ext.hpp>
#include <RED4ext/Scripting/Natives/Generated/ent/Entity.hpp>
#include <RED4ext/Scripting/Natives/Generated/vehicle/BaseObject.hpp>
#include "Addresses.hpp"
#include <RED4ext/Scripting/Natives/Generated/game/EffectSystem.hpp>

// XInput Destructor
/// @hash 2647855289

// 1.52 RVA: 0x196260 / 1663584
// 1.62 RVA: 0x196960
/// @pattern 48 85 C9 74 2E 53 48 83 EC 20 48 8B D9 48 8D 4C 24 30 E8 09 02 00 00 BA 01 00 00 00 48 8B CB E8
using GamepadDestructor = void __fastcall (void *);

// xpad Allocator/Init
/// @hash 3372825422
using InitializeXPad = void *__fastcall (void *, uint32_t deviceID);

// bool ManagerBackend::IsAxis(uint16_t);
/// @hash 2490568903
using IsAxis = bool __fastcall (uint16_t key);

// bool ManagerBackend::IsKey(uint16_t);
/// @hash 989204148
using IsButtonToAxis = bool __fastcall (uint16_t key);

// bool ManagerBackend::IsPadKey(uint16_t);
/// @hash 1763577801
using IsGamepad = bool __fastcall (uint16_t key);