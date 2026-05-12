cmake_minimum_required(VERSION 3.22)

project(FractalDelay VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

set(JUCE_TAG "8.0.12" CACHE STRING "JUCE git tag for FetchContent")

FetchContent_Declare(
    juce
    GIT_REPOSITORY https://github.com/juce-framework/JUCE.git
    GIT_TAG "${JUCE_TAG}"
    GIT_SHALLOW TRUE
)

FetchContent_MakeAvailable(juce)

juce_add_plugin(
    FractalDelay
    COMPANY_NAME "LucasArcega"
    BUNDLE_ID com.lucasarcega.fractaldelay
    IS_SYNTH FALSE
    NEEDS_MIDI_INPUT FALSE
    NEEDS_MIDI_OUTPUT FALSE
    IS_MIDI_EFFECT FALSE
    EDITOR_WANTS_KEYBOARD_FOCUS FALSE
    COPY_PLUGIN_AFTER_BUILD TRUE
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

enable_testing()
add_test(NAME FractalDelay_Tests COMMAND FractalDelay_Tests)
