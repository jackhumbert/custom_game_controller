#pragma once

#include <RED4ext/RED4ext.hpp>

namespace game::input {

class ManagerBackend {
  /// @hash 989204148
  bool IsKey(uint16_t);

  // inlined in:
  // * game::input::UserMapping::AddMapping
  // 66 41 2B C1 66 83 F8 05
  // * game::input::ManagerBackend::IsKey
  // 66 2B C2 32 D2 66 83 F8 05
  // * game::input::UserMappingAxis::AddMappingAxis
  // 66 2B C2 66 83 F8 05
  // * game::input::Manager::FunctionalTestsFakeInput
  // 66 2B C1 66 83 F8 05
  
  // 66 2B ?? 66 83 F8 05
  // 66 2B ?? ?? ?? 66 83 F8 05
  // 66 ?? 2B ?? 66 83 F8 05
  bool IsAxis(uint16_t key) {
    return ((key - 150) <= 5);
  }

  /// @hash 1763577801
  bool IsPadKey(uint16_t);

  bool IsKeyboardKey(uint16_t);

  // IK_MouseX, IK_MouseY, IK_MouseZ, IK_Pad_Fake_LeftAxis, IK_Pad_Fake_RightAxis, IK_Pad_Fake_RelativeLeftAxis, IK_Pad_Fake_RelativeRightAxis
  /// @hash 2490568903
  bool IsRelative(uint16_t);
};

}