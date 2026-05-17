# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project Overview

**Fractal-Delay** is a JUCE-based audio plugin (VST3 + Standalone) currently in development. The project follows a phased roadmap aligned with ClickUp workspace [90171225448](https://app.clickup.com/90171225448), with detailed implementation plans in the `Plans/` directory.

**Current Status**: Early development phase with basic plugin skeleton, metering, and UI framework established.

## Build System

### Quick Build Commands

**Windows (Recommended):**
```bat
scripts\all-debug.bat
```
This single command configures, builds, and runs tests with the correct MSVC environment.

**Manual CMake (cross-platform):**
```bash
# Configure
cmake -B build -DCMAKE_BUILD_TYPE=Debug

# Build
cmake --build build --config Debug

# Run tests
ctest --test-dir build --output-on-failure
```

**With Ninja (faster, requires MSVC environment on Windows):**
```bash
cmake -B build -G Ninja -DCMAKE_BUILD_TYPE=Debug
cmake --build build
```

### Test Commands

```bash
# Run all tests (excludes [gui] tests by default)
ctest --test-dir build --output-on-failure

# Run tests directly (includes GUI tests)
build/Debug/FractalDelay_Tests.exe  # Windows
./build/FractalDelay_Tests          # Linux/macOS

# Run only GUI tests
FractalDelay_Tests "[gui]"

# Linux without display
xvfb-run -a ctest --test-dir build --output-on-failure
```

### CMake Configuration

- **JUCE version**: Controlled by `JUCE_TAG` cache variable in CMakeLists.txt (default: "8.0.12")
- **Change JUCE version**: `cmake -B build -DJUCE_TAG=8.0.4`
- **Enable GUI tests in ctest**: `cmake -B build -DFRACTAL_DELAY_CTEST_INCLUDE_GUI=ON`
- **Copy plugin after build**: `cmake -B build -DFRACTAL_DELAY_COPY_PLUGIN_AFTER_BUILD=ON` (requires admin on Windows)

### Windows-Specific Build Notes

- The `scripts/` directory contains batch files that handle MSVC environment setup automatically
- `configure-ninja.bat`: Configure with Ninja generator
- `build-debug.bat`: Build only (after configure)
- `cmake-debug-cache-flags.bat`: Shared CMake cache flags used by other scripts
- **Common issue**: Running `cmake` directly in PowerShell without loading MSVC environment will fail. Use the scripts or "x64 Native Tools Command Prompt for VS 2022"

## Architecture

### Core Components

**PluginProcessor** (`Source/PluginProcessor.h/cpp`):
- Main DSP processing class extending `juce::AudioProcessor`
- Manages `AudioProcessorValueTreeState` (APVTS) for parameters
- Contains lock-free queue (`LockFreeQueue<AudioToUIMessage>`) for audio→UI communication
- Audio thread sends meter data via `AudioToUIMessage` enum (PEAK_IN, PEAK_OUT, PEAK_OUT_LEFT, PEAK_OUT_RIGHT, INCREMENT)
- Currently supports stereo and mono bus layouts only

**PluginEditor** (`Source/PluginEditor.h/cpp`):
- GUI editor extending `juce::AudioProcessorEditor`
- Uses `juce::Grid` for macro layout (header | 3 columns | footer)
- Uses `juce::FlexBox` within individual columns for micro layout
- Contains `IdleTimer` that calls `idle()` method ~30Hz to drain the audio→UI message queue
- Never use `Timer::startTimer()` in constructor - the editor may not be "showing" in first frame on Standalone

**Reusable Components** (`Source/Components/`, namespace `GUI`):
- `GUI::FooterBar`: Contains stereo output meters (L/R)
- `GUI::HorizontalMetter`: Simple horizontal meter displaying dB levels (-60 to +6 dB range)

### Audio → UI Communication Pattern

Follows [airwindows/Meter](https://github.com/airwindows/Meter) pattern to avoid RAM explosion on Windows:

1. Audio thread writes small messages to lock-free queue (`juce::AbstractFifo`) in `processBlock`
2. UI thread drains queue in `idle()` method called by `IdleTimer` (~30Hz)
3. Only call `repaint()` when values actually change (not unconditionally)
4. GUI tests use `JUCE_MODAL_LOOPS_PERMITTED=1` and `MessageManager::runDispatchLoopUntil()` to drain message queue and prevent memory leaks

**Critical**: Do NOT spam `repaint()` or use polling patterns that don't drain the message queue on Windows.

### Parameter Architecture

- All parameters managed through APVTS (`AudioProcessorValueTreeState`)
- Parameter IDs use `juce::ParameterID` with version number (e.g., `{"inputGainDb", 1}`)
- Text conversion functions handle special cases (e.g., "-inf dB" for silence)
- `silenceDb` constant (-80 dB) shared via anonymous namespace in .cpp files
- Always use `AudioParameterFloatAttributes::withStringFromValueFunction` and `withValueFromStringFunction` to validate text input safely

### UI Layout System

**Macro Layout** (`juce::Grid`):
- 3 rows: header | body | footer
- Body has 3 columns: IN | center (Delay) | OUT
- Grid fractional units use **integers** in JUCE 8: `Grid::Fr(100)`, NOT `Grid::Fr(1.0f)`
- `performLayout` functions receive `juce::Rectangle<int>`, not float

**Micro Layout** (`juce::FlexBox`):
- Used within individual column components
- Each column is a `juce::Component` container with child components laid out via FlexBox

## Testing

### Test Framework

- **Catch2 v3** for all tests
- Tests in `Tests/FractalDelayTests.cpp`
- Test target: `FractalDelay_Tests`

### Test Tags

- `[processor]`: DSP/state tests only, no GUI required
- `[gui]`: Requires JUCE GUI (may be excluded from ctest on Windows)

### GUI Test Considerations

**Windows RAM Issue**: JUCE message queue can grow unbounded without proper message pump, consuming GB of RAM. Solutions:
- `ctest` excludes `[gui]` tests by default (configure with `-DFRACTAL_DELAY_CTEST_INCLUDE_GUI=ON` to include)
- Run test binary directly: `FractalDelay_Tests.exe` includes all tests
- Tests check `SKIP_FRACTAL_DELAY_GUI_TESTS` env var and skip if set
- Tests use `MessageManager::runDispatchLoopUntil()` to drain message queue after editor creation (requires `JUCE_MODAL_LOOPS_PERMITTED=1` on test target only)

**Linux CI**: Use `xvfb-run -a` for headless testing

## Code Style

### clang-format

```
BasedOnStyle: Microsoft
ColumnLimit: 120
NamespaceIndentation: All
```

Apply formatting before commits. Anonymous namespaces are indented.

### Git Configuration

- Line endings: `eol=lf` for code (`.gitattributes`)
- Exception: `*.bat` files use `crlf`
- `.gitignore` includes: `build/`, `.claude/`, `compile_commands.json`

### Naming Conventions

- **Namespace for UI components**: `GUI` (e.g., `GUI::FooterBar`)
- **JUCE leak detector**: Avoid `JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR` on components that are value members if it prevents default construction on MSVC. Use `std::unique_ptr` or omit the macro for embedded child widgets.

## Development Workflow

### Phased Development

Development follows a structured 5-phase roadmap documented in `Plans/README.md`:

1. **Fase 1**: Motor de Delay Básico (basic delay engine)
2. **Fase 2**: Feedback Engine
3. **Fase 3**: Modos Avançados (advanced modes: BBD, Lo-Fi, Reverse, Granular, Fractal)
4. **Fase 4**: Spectral Ducking (8-band filterbank with ducking)
5. **Fase 5**: Post-FX e Spatial (EQ, saturation, stereo width)

Each phase has detailed implementation plans in `Plans/fase-X-*/` directories with numbered markdown files.

### Key Reference Documents

- `Plans/README.md`: Phase roadmap and ClickUp task links
- `Plans/00-convencoes-repo-ui-testes.md`: Repository conventions (UI, audio↔UI, CI, Git)
- `Plans/00-testes-unitarios-visao-geral.md`: Testing strategy across all phases

### Before Implementing Features

1. Read the relevant phase plan in `Plans/fase-X-*/`
2. Check conventions in `Plans/00-convencoes-repo-ui-testes.md`
3. Understand the test requirements in `Plans/00-testes-unitarios-visao-geral.md`
4. Follow existing patterns in `Source/` for UI and DSP separation

## Common Patterns

### Adding a New Parameter

1. Add to `createParameterLayout()` in PluginProcessor.cpp
2. Use `juce::ParameterID` with version number
3. Provide `withStringFromValueFunction` and `withValueFromStringFunction` for text validation
4. Create attachment in PluginEditor
5. Add test to verify parameter exists in APVTS

### Adding a UI Component

1. Create in `Source/Components/` within `GUI` namespace
2. Add forward declaration or include in PluginEditor.h
3. Add as member to PluginEditor (prefer `std::unique_ptr` for complex components)
4. Call `addAndMakeVisible()` in editor constructor
5. Position in `resized()` using Grid or FlexBox

### Sending Meter Data to UI

1. Define message type in `AudioToUIMessage::What` enum
2. Push message in `processBlock`: `audioToUI.push({AudioToUIMessage::PEAK_OUT, peakValue})`
3. Pop and process in editor's `idle()` method
4. Update UI component and call `repaint()` only when value changes

## Plugin Formats

- **VST3**: Primary format, artifacts in `build/FractalDelay_artefacts/*/VST3/`
- **Standalone**: For testing without DAW, artifacts in `build/FractalDelay_artefacts/*/Standalone/`
- **AU**: Not currently configured (macOS only)

## Dependencies

- **JUCE**: Fetched via CMake `FetchContent` (no git submodule)
- **Catch2**: Fetched via CMake `FetchContent` for tests
- No other external dependencies

## Build Artifacts

After successful build, find:
- **Plugin**: `build/FractalDelay_artefacts/Debug/VST3/Fractal Delay.vst3`
- **Standalone**: `build/FractalDelay_artefacts/Debug/Standalone/Fractal Delay.exe` (Windows)
- **Tests**: `build/Debug/FractalDelay_Tests.exe` (Windows) or `build/FractalDelay_Tests` (Unix)

## Troubleshooting

### Windows Build Issues

**"could not find any instance of Visual Studio"**: Open Visual Studio Installer, modify installation, ensure "Desktop development with C++" workload is installed.

**Ninja can't find cl.exe**: Must run from MSVC environment. Use `scripts\all-debug.bat` or open "x64 Native Tools Command Prompt for VS 2022".

**Partial build folder**: Delete `build/` completely and reconfigure: `rmdir /s /q build` (cmd) or `rm -rf build` (bash/PowerShell).

### Test Failures

**GUI tests fail on Windows**: Ensure you're running the test binary directly, not via ctest (unless configured with `FRACTAL_DELAY_CTEST_INCLUDE_GUI=ON`).

**Memory growth during tests**: Ensure `MessageManager::runDispatchLoopUntil()` is called after creating/destroying editors in GUI tests.

**Tests fail on headless Linux**: Use `xvfb-run -a` or set `SKIP_FRACTAL_DELAY_GUI_TESTS=1`.
