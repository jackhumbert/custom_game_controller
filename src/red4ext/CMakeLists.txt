include(Red4ext)

target_link_libraries(custom_game_controller.dll
  PUBLIC
    Detours
    RED4ext.SDK
    spdlog
)