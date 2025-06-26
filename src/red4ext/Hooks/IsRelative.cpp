#include <Registrar.hpp>
#include <IsRelative.hpp>
#include <IsJoystickAxis.hpp>

// IK_MouseX, IK_MouseY, IK_MouseZ, IK_Pad_Fake_LeftAxis, IK_Pad_Fake_RightAxis, IK_Pad_Fake_RelativeLeftAxis, IK_Pad_Fake_RelativeRightAxis
// takes a 16b version of RED4ext::EInputKey
REGISTER_HOOK_HASH(bool, 2490568903, IsRelative, uint16_t key) {
  return IsRelative_Original(key); // && !IsJoystickAxis(key);
}
