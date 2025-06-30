cmake_minimum_required(VERSION 3.24)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/deps/cyberpunk_cmake")
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/deps/red4ext.sdk/cmake")

include(ConfigureVersionFromGit)
configure_version_from_git()

project(custom_game_controller VERSION ${GIT_VERSION_MAJOR}.${GIT_VERSION_MINOR}.${GIT_VERSION_PATCH} LANGUAGES NONE)

include(CyberpunkMod)

configure_mod(
  NAME "Custom Game Controller"
  SLUG ${PROJECT_NAME}
  PREFIX CGC
  AUTHOR "Jack Humbert"
  URL "https://github.com/jackhumbert/custom_game_controller"
  LICENSE "Licensed under the MIT license. See the license.md in the root project for details."
)

set(LOAD_INPUTS_FROM_RED4EXT ON)
set(LOAD_ARCHIVES_FROM_RED4EXT ON)
set(LOAD_REDSCRIPT_FROM_RED4EXT ON)

find_program(ZOLTAN_CLANG_EXE NAMES zoltan-clang.exe PATHS "${MOD_TOOLS_DIR}" CACHE)
find_program(REDSCRIPT_CLI_EXE NAMES redscript-cli.exe PATHS "${MOD_TOOLS_DIR}" CACHE)
find_program(CYBERPUNK_2077_EXE NAMES Cyberpunk2077.exe PATHS "${CYBERPUNK_2077_GAME_DIR}/bin/x64" CACHE DOC "Cyberpunk2077.exe Executable File")


configure_red4ext(src/red4ext)
# configure_red4ext_addresses(Addresses.hpp)

configure_archives(src/custom_game_controller/packed/archive/pc/mod/custom_game_controller.archive src/archive_xl/custom_game_controller.archive.xl)

find_package(RedLib)
find_package(Detours)
find_package(Spdlog)
find_package(InputLoader)
find_package(ArchiveXL)

# target_include_directories(${MOD_SLUG}.dll
#   PUBLIC
#     ${MOD_SOURCE_DIR}/include
# )

target_link_libraries(${MOD_SLUG}.dll
  PUBLIC
    Detours
    spdlog
    ArchiveXL
    RedLib
    input_loader.dll
)

configure_mod_file(src/input_loader/${MOD_SLUG}.xml red4ext/plugins/${MOD_SLUG}/inputs.xml)

configure_redscript(src/redscript)

# target_link_libraries(${MOD_SLUG}.packed.reds PUBLIC mod_settings.packed.reds)

configure_folder_file(readme.md)
configure_folder_file(license.md)

# configure_uninstall()
configure_release(${MOD_SLUG}_${MOD_VERSION_STR}.zip)
configure_install()