#include <Registrar.hpp>
#include <RED4ext/HashMap.hpp>
#include <Red/BaseGamepad.hpp>
#include <Red/CustomGamepad.hpp>
#include <RED4ext/Scripting/Natives/Generated/EInputKey.hpp>

// called from void input::InputSystemWin32Game::Initialize(void)
// Replaces XPads with our custom gamepad class
// REGISTER_HOOK_HASH(RED4ext::UniquePtr<CustomGamepad>, 3372825422, InitializeXPad, uint32_t & gamepadIndex) {
//   if (gamepadIndex == 0) {
//     // auto custom = (CustomGamepad*)RED4ext::Memory::Allocator<RED4ext::Memory::PoolEngine>::Get()->Alloc(sizeof(CustomGamepad)).memory;
//     // new (custom) CustomGamepad(gamepadIndex);
//     return RED4ext::MakeUnique<CustomGamepad, uint32_t &>(gamepadIndex);
//   } else {
//     return InitializeXPad_Original(gamepadIndex);
//   }
// }
