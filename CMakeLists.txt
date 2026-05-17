cmake_minimum_required(VERSION 3.22)

# compile_commands.json for clangd / clang-tidy (Cursor picks it up from build/ or a symlink at repo root).
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# C needed for CMake C toolchain (deps compile .c); avoids MSVC+Ninja CMAKE_C_COMPILE_OBJECT errors.
project(FractalDelay VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Ninja + MSVC from a plain shell: cl.exe runs without STL/Windows SDK on the command line.
if(MSVC)
    include(CheckCXXSourceCompiles)
    check_cxx_source_compiles(
        "#include <cstddef>\n#include <vector>\nint main() { std::vector<int> v; return static_cast<int>(v.size()); }\n"
        FRACTAL_DELAY_MSVC_STL_VISIBLE
    )
    if(NOT FRACTAL_DELAY_MSVC_STL_VISIBLE)
        message(
            FATAL_ERROR
            "MSVC cannot see C++ standard library headers (e.g. <vector>). "
            "Configure from a Visual Studio environment: run scripts\\configure-ninja.bat from cmd.exe, "
            "or open \"x64 Native Tools Command Prompt for VS 2022\", then delete the build folder and run cmake again. "
            "Plain PowerShell without VsDevCmd breaks Ninja + MSVC."
        )
    endif()
endif()

include(FetchContent)

set(JUCE_TAG "8.0.12" CACHE STRING "JUCE git tag for FetchContent")

FetchContent_Declare(
    juce
    GIT_REPOSITORY https://github.com/juce-framework/JUCE.git
    GIT_TAG "${JUCE_TAG}"
    GIT_SHALLOW TRUE
)

FetchContent_MakeAvailable(juce)

# OFF by default: copying to Program Files\Common Files\VST3 needs elevation and breaks normal builds.
option(
    FRACTAL_DELAY_COPY_PLUGIN_AFTER_BUILD
    "After build, copy plugins to the system folders (e.g. Common Files\\VST3 on Windows; may require admin)"
    OFF)

juce_add_plugin(
    FractalDelay
    COMPANY_NAME "LucasArcega"
    BUNDLE_ID com.lucasarcega.fractaldelay
    IS_SYNTH FALSE
    NEEDS_MIDI_INPUT FALSE
    NEEDS_MIDI_OUTPUT FALSE
    IS_MIDI_EFFECT FALSE
    EDITOR_WANTS_KEYBOARD_FOCUS FALSE
    COPY_PLUGIN_AFTER_BUILD ${FRACTAL_DELAY_COPY_PLUGIN_AFTER_BUILD}
    PLUGIN_MANUFACTURER_CODE Arce
    PLUGIN_CODE Fdly
    FORMATS VST3 Standalone
    PRODUCT_NAME "Fractal Delay"
)

juce_generate_juce_header(FractalDelay)

target_include_directories(FractalDelay PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/Source")

target_sources(
    FractalDelay
    PRIVATE
        Source/PluginProcessor.cpp
        Source/PluginEditor.cpp
        Source/DSP/CircularDelayBuffer.cpp
        Source/Components/FooterBar.cpp
        Source/Components/HorizontalMetter.cpp
)

target_compile_definitions(
    FractalDelay
    PUBLIC
        JUCE_WEB_BROWSER=0
        JUCE_USE_CURL=0
        JUCE_VST3_CAN_REPLACE_VST2=0
)

target_link_libraries(
    FractalDelay
    PUBLIC
        juce::juce_audio_utils
        juce::juce_dsp
        juce::juce_recommended_config_flags
        juce::juce_recommended_lto_flags
        juce::juce_recommended_warning_flags
)

FetchContent_Declare(
    Catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG v3.5.4
    GIT_SHALLOW TRUE
)

FetchContent_MakeAvailable(Catch2)

add_executable(FractalDelay_Tests)
target_sources(FractalDelay_Tests PRIVATE Tests/FractalDelayTests.cpp)
target_link_libraries(FractalDelay_Tests PRIVATE FractalDelay Catch2::Catch2WithMain)

# Só no executável de testes: permite runDispatchLoopUntil para esvaziar a fila após GUI
# (TooltipWindow / Desktop timers + repaints sem dispatch cresciam a vários GB no Windows).
target_compile_definitions(FractalDelay_Tests PRIVATE JUCE_MODAL_LOOPS_PERMITTED=1)

# juce_generate_juce_header only adds JuceLibraryCode to the plugin target; tests include PluginProcessor.h.
target_include_directories(
    FractalDelay_Tests
    PRIVATE
        "${CMAKE_CURRENT_BINARY_DIR}/FractalDelay_artefacts/JuceLibraryCode"
)
add_dependencies(FractalDelay_Tests FractalDelay)

# Catch2: exclude [gui] tests by default (JUCE editor can SIGSEGV without a proper display loop).
# Enable full suite: cmake -B build -DFRACTAL_DELAY_CTEST_INCLUDE_GUI=ON  or  FractalDelay_Tests.exe
option(FRACTAL_DELAY_CTEST_INCLUDE_GUI "ctest runs Catch2 [gui] tests (needs display / xvfb)" OFF)

enable_testing()
# Use generator expression so ctest finds the binary with multi-config generators
# (e.g. Visual Studio: build/Debug/FractalDelay_Tests.exe vs build/FractalDelay_Tests.exe).
if(FRACTAL_DELAY_CTEST_INCLUDE_GUI)
    add_test(NAME FractalDelay_Tests COMMAND "$<TARGET_FILE:FractalDelay_Tests>")
else()
    add_test(NAME FractalDelay_Tests COMMAND "$<TARGET_FILE:FractalDelay_Tests>" "~[gui]")
endif()
