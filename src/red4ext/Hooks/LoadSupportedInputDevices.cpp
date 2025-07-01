#include <Registrar.hpp>
#include <RED4ext/RED4ext.hpp>
#include <RED4ext/Handle.hpp>
#include <RED4ext/Scripting/Natives/Generated/ink/InputKeyIconManager.hpp>
#include <RED4ext/Scripting/Natives/Generated/ink/TextureAtlas.hpp>
#include <RED4ext/Scripting/Natives/Generated/ink/InputIconMappingJSON.hpp>
#include <RED4ext/Scripting/Natives/Generated/input/EInputDevice.hpp>

using namespace RED4ext;

auto static AddSupportedInputDevice = UniversalRelocFunc<void (*)(ink::InputKeyIconManager *, 
  input::EInputDevice device, Handle<ink::TextureAtlas> const & atlas
)>(2219452775);

auto static LoadTextureAtlas = UniversalRelocFunc<void (*)(ink::InputKeyIconManager *, 
  ResourceAsyncReference<ink::TextureAtlas> & iconAtlas, Handle<ink::TextureAtlas> & atlas
)>(2295409473);

auto static LoadInputDeviceIconMappings = UniversalRelocFunc<void (*)(ink::InputKeyIconManager *, 
  ink::InputIconMappingJSON const &, ink::InputKeyIconManager::InputDeviceTextureData &
)>(783035098);

auto joystickInkAtlas = ResourceAsyncReference<ink::TextureAtlas>("user\\jackhumbert\\icons_joystick.inkatlas");
Handle<ink::TextureAtlas> joystickTextureAtlas;

//void ink::InputKeyIconManager::LoadSupportedInputDevices(void)
// 1
// 2
// 3
// 4
// 5
// 6 not loaded
// 7 not loaded
// 8
// 9
REGISTER_HOOK_HASH(void, 2939295274, LoadSupportedInputDevices, ink::InputKeyIconManager * self) {
  LoadSupportedInputDevices_Original(self);
  LoadTextureAtlas(self, joystickInkAtlas, joystickTextureAtlas);
  AddSupportedInputDevice(self, input::EInputDevice::STADIA, joystickTextureAtlas);
}

// void ink::InputKeyIconManager::LoadTextureDictionaryJSON(void)
REGISTER_HOOK_HASH(void, 2726303186, LoadTextureDictionaryJSON, ink::InputKeyIconManager * self) {
  LoadTextureDictionaryJSON_Original(self);

  auto textureData = self->inputDevices.Get(input::EInputDevice::STADIA);
  DynArray<ink::InputIconMappingJSON> mappings = {
    { "IK_Pad_A_CROSS", "a" },
    { "IK_Pad_B_CIRCLE", "b" },
    { "IK_Pad_X_SQUARE", "x" },
    { "IK_Pad_Y_TRIANGLE", "y" },
    { "IK_Pad_A_CROSS", "a_hold" },
    { "IK_Pad_B_CIRCLE", "b_hold" },
    { "IK_Pad_X_SQUARE", "x_hold" },
    { "IK_Pad_Y_TRIANGLE", "y_hold" },
    { "IK_Pad_LeftThumb", "left_thumb" },
    { "IK_Pad_RightThumb", "right_thumb" },
    { "IK_Pad_LeftAxisX", "left_axis_right" },
    { "IK_Pad_LeftAxisY", "left_axis_up" },
    { "IK_Pad_RightAxisX", "right_axis_right" },
    { "IK_Pad_RightAxisY", "right_axis_up" },
    { "IK_Pad_LeftAxisX", "left_axis_leftright" },
    { "IK_Pad_LeftAxisY", "left_axis_updown" },
    { "IK_Pad_RightAxisX", "right_axis_leftright" },
    { "IK_Pad_RightAxisY", "right_axis_updown" },
    { "IK_Pad_LeftThumb", "left_thumb" },
    { "IK_Pad_RightThumb", "right_thumb" },
    { "IK_Pad_DigitUp", "dpad_up" },
    { "IK_Pad_DigitLeft", "dpad_left" },
    { "IK_Pad_DigitRight", "dpad_right" },
    { "IK_Pad_DigitDown", "dpad_down" },
    { "ICK_Pad_DigitUpDown", "dpad_up_down" },
    { "ICK_Pad_DigitLeftRight", "dpad_left_right" },
    { "IK_Pad_DigitUp", "dpad_up_hold" },
    { "IK_Pad_DigitLeft", "dpad_left_hold" },
    { "IK_Pad_DigitRight", "dpad_right_hold" },
    { "IK_Pad_DigitDown", "dpad_down_hold" },
    { "ICK_Pad_DigitUpDown", "dpad_up_down_hold" },
    { "ICK_Pad_DigitLeftRight", "dpad_left_right_hold" },
    { "IK_Pad_LeftShoulder", "left_shoulder" },
    { "IK_Pad_RightShoulder", "right_shoulder" },
    { "IK_Pad_LeftTrigger", "left_trigger" },
    { "IK_Pad_RightTrigger", "right_trigger" },
    { "IK_Pad_LeftShoulder", "left_shoulder_hold" },
    { "IK_Pad_RightShoulder", "right_shoulder_hold" },
    { "IK_Pad_LeftTrigger", "left_trigger_hold" },
    { "IK_Pad_RightTrigger", "right_trigger_hold" },
    { "IK_Pad_Start", "start" },
    { "IK_Pad_Back_Select", "back" },
    { "IK_Pad_Start", "start_hold" },
    { "IK_Pad_Back_Select", "back_hold" },
    { "IK_Pad_Fake_LeftAxis", "left_axis_all" },
    { "IK_Pad_Fake_RelativeLeftAxis", "left_axis_all" },
    { "IK_Pad_Fake_RelativeRightAxis", "right_axis_all" },
    { "IK_Pad_Fake_RightAxis", "right_axis_all" },
    { "IK_Pad_Fake_DigitLeftRight", "dpad_left_right" },
    { "IK_Pad_Fake_DigitUpDown", "dpad_up_down" },
    { "IK_JoyX", "joy_x" },
    { "IK_JoyY", "joy_y" },
    { "IK_JoyZ", "joy_z" },
    { "IK_JoyR", "joy_r" },
    { "IK_JoyU", "joy_u" },
    { "IK_JoyV", "joy_v" },
    { "IK_JoySlider1", "joy_s1" },
    { "IK_JoySlider2", "joy_s2" },
    { "IK_Joy1", "joy_b1" },
    { "IK_Joy2", "joy_b2" },
    { "IK_Joy3", "joy_b3" },
    { "IK_Joy4", "joy_b4" },
    { "IK_Joy5", "joy_b5" },
    { "IK_Joy6", "joy_b6" },
    { "IK_Joy7", "joy_b7" },
    { "IK_Joy8", "joy_b8" },
    { "IK_Joy9", "joy_b9" },
    { "IK_Joy10", "joy_b10" },
    { "IK_Joy11", "joy_b11" },
    { "IK_Joy12", "joy_b12" },
    { "IK_Joy13", "joy_b13" },
    { "IK_Joy14", "joy_b14" },
    { "IK_Joy15", "joy_b15" },
    { "IK_Joy16", "joy_b16" },
  };
  for (const auto & mapping : mappings) {
    LoadInputDeviceIconMappings(self, mapping, *textureData);
  }
}

// REGISTER_HOOK_HASH(input::EInputDevice, 1154358952, GetDeviceTypeForName, ink::InputKeyIconManager * self, CName name) {
//   if (name == "joystick")
//     return input::EInputDevice::EID_COUNT;
//   return GetDeviceTypeForName_Original(self, name);
// }