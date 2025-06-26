#pragma once

#include <RED4ext/Scripting/Natives/Generated/EInputKey.hpp>

// Allows joystick keys to be treated as axes
bool __fastcall IsJoystickAxis(uint16_t key);