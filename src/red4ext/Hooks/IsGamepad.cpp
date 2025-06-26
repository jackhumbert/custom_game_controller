#include <Registrar.hpp>
#include <IsJoystick.hpp>

// (key - 136) <= 19 || (key - 255) <= 6 || (key - 262) <= 5
REGISTER_HOOK_HASH(bool, 1763577801, IsPadKey, uint16_t key) {
  return IsPadKey_Original(key) || IsJoystick(key);
}