#include <RED4ext/InstanceType.hpp>
#include <RED4ext/RED4ext.hpp>
#include <RED4ext/RTTITypes.hpp>
#include <RED4ext/RED4ext.hpp>
#include <spdlog/spdlog.h>
#include <RED4ext/Scripting/Natives/Generated/EInputKey.hpp>
#include "Utils.hpp"
#include <stdio.h>
#include <Windows.h>
#include <Dbt.h>
#include <deque>
#include <CyberpunkMod.hpp>
#include <RedLib.hpp>
// #include <InputLoader.hpp>
#include <ArchiveXL.hpp>

RED4EXT_C_EXPORT void RED4EXT_CALL RegisterTypes() {
  spdlog::info("Registering classes & types");
}

RED4EXT_C_EXPORT void RED4EXT_CALL PostRegisterTypes() {
  spdlog::info("Registering members & functions");
  RED4ext::CNamePool::Add("array:EInputKey");
  // RED4ext::CNamePool::Add("array:GameControllerSwitchPosition");
}

RED4EXT_C_EXPORT bool RED4EXT_CALL Main(RED4ext::PluginHandle aHandle, RED4ext::EMainReason aReason,
                                        const RED4ext::Sdk *aSdk) {
  switch (aReason) {
  case RED4ext::EMainReason::Load: {

    Utils::CreateLogger();
    spdlog::info("Starting up Custom Game Controller {}", MOD_VERSION_STR);
    auto ptr = GetModuleHandle(nullptr);
    spdlog::info("Base address: {}", fmt::ptr(ptr));

    RED4ext::RTTIRegistrator::Add(RegisterTypes, PostRegisterTypes);
    Red::TypeInfoRegistrar::RegisterDiscovered();

    aSdk->scripts->Add(aHandle, L"packed.reds");
    aSdk->scripts->Add(aHandle, L"module.reds");
    // InputLoader::Add(aHandle, L"inputs.xml");
    ArchiveXL::RegisterArchive(aHandle, "custom_game_controller.archive");
    ModModuleFactory::GetInstance().Load(aSdk, aHandle);

    // while (!aSdk->hooking->Attach(aHandle, RED4EXT_OFFSET_TO_ADDR(InitializeXPadAddr), &InitializeXPad,
    //                               reinterpret_cast<void **>(&InitializeXPad_Original)))
    //   ;
    // while (!aSdk->hooking->Attach(aHandle, RED4EXT_OFFSET_TO_ADDR(IsAxisAddr), &IsAxis,
    //                               reinterpret_cast<void **>(&IsAxis_Original)))
    //   ;
    // while (!aSdk->hooking->Attach(aHandle, RED4EXT_OFFSET_TO_ADDR(IsButtonToAxisAddr), &IsButtonToAxis,
    //                               reinterpret_cast<void **>(&IsButtonToAxis_Original)))
    //   ;
    // while (!aSdk->hooking->Attach(aHandle, RED4EXT_OFFSET_TO_ADDR(IsGamepadAddr), &IsGamepad,
    //                               reinterpret_cast<void **>(&IsGamepad_Original)))
    //   ;

    break;
  }
  case RED4ext::EMainReason::Unload: {

    spdlog::info("Shutting down");
    ModModuleFactory::GetInstance().Unload(aSdk, aHandle);
    // aSdk->hooking->Detach(aHandle, RED4EXT_OFFSET_TO_ADDR(InitializeXPadAddr));
    // aSdk->hooking->Detach(aHandle, RED4EXT_OFFSET_TO_ADDR(IsAxisAddr));
    // aSdk->hooking->Detach(aHandle, RED4EXT_OFFSET_TO_ADDR(IsButtonToAxisAddr));
    // aSdk->hooking->Detach(aHandle, RED4EXT_OFFSET_TO_ADDR(IsGamepadAddr));
    spdlog::shutdown();
    break;
  }
  }

  return true;
}

RED4EXT_C_EXPORT void RED4EXT_CALL Query(RED4ext::PluginInfo *aInfo) {
  aInfo->name = L"Custom Game Controller";
  aInfo->author = L"Jack Humbert";
  aInfo->version = RED4EXT_SEMVER(MOD_VERSION_MAJOR, MOD_VERSION_MINOR, MOD_VERSION_PATCH);
  aInfo->runtime = RED4EXT_RUNTIME_INDEPENDENT;
  aInfo->sdk = RED4EXT_SDK_LATEST;
}

RED4EXT_C_EXPORT uint32_t RED4EXT_CALL Supports() { return RED4EXT_API_VERSION_LATEST; }
