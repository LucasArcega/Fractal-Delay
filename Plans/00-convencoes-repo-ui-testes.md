# Convenções do repositório — UI, testes e tooling (referência transversal)

Documento **vivo**: reflecte decisões já aplicadas no código (2026). Os planos por fase devem alinhar-se a isto ao implementar GUI, filas áudio↔UI e CI.

## Build e JUCE

- **CMake + `FetchContent`** para JUCE com `JUCE_TAG` fixo no `CMakeLists.txt` (não depender de “memória” de versão).
- **`COPY_PLUGIN_AFTER_BUILD`**: default **OFF** (cópia para `Program Files\…\VST3` costuma exigir admin).
- **Alvos**: `FractalDelay` (VST3 + Standalone), `FractalDelay_Tests` (Catch2).
- **Scripts Windows**: `scripts/configure-ninja.bat`, `all-debug.bat`, `cmake-debug-cache-flags.bat` — configuram flags de cache (incl. opções de `ctest` quando aplicável).

## Estilo e Git

- **`.clang-format`**: `BasedOnStyle: Microsoft`, `NamespaceIndentation: All`, `ColumnLimit: 120`.
- **`.gitattributes`**: `eol=lf` para código; `*.bat` com `crlf`.
- **`.gitignore`**: inclui `build/`, `.claude/`, `compile_commands.json`, etc.

## UI — layout

- **Macro**: `juce::Grid` na janela do editor (linhas: cabeçalho / corpo / rodapé; colunas: IN | centro Delay | OUT). `Grid::Fr` no JUCE 8 usa **inteiros** (ex. `Fr(100)`, `Fr(145)`), não `float`.
- **`performLayout`**: recebe `juce::Rectangle<int>` (não `float`).
- **Micro**: `juce::FlexBox` **dentro** de cada coluna (`juce::Component` “coluna” com filhos).
- **Componentes reutilizáveis**: pasta `Source/Components/`, namespace **`GUI`** (ex.: `GUI::FooterBar`, `GUI::HorizontalMetter`).
- **Medidores no rodapé**: `FooterBar` com saída **L/R** (stereo); mono espelha L→R no DSP.

## UI — áudio → interface (sem estoirar RAM no Windows)

Padrão inspirado no [airwindows/Meter](https://github.com/airwindows/Meter):

- **`AbstractFifo`** / fila *lock-free* de mensagens pequenas (`struct { enum What; float newValue; }`) escrita em **`processBlock`** e lida num **`IdleTimer`** (~30 Hz) no editor (`idle()` drena a fila).
- **`repaint` / actualização de widgets** só quando faz sentido (ex. após mensagem `INCREMENT` ou quando valores mudam), não spam incondicional.
- **Evitar** `Timer` + `isShowing()` / `parentHierarchyChanged` de forma que o timer **nunca arranque** no Standalone (o editor pode não estar “showing” no primeiro frame).
- **`JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR`** em `Component` **subclasses que são membros por valor** de outro `Component` pode impedir construtor por defeito no MSVC — para *widgets* filhos embutidos, omitir o macro de *leak detector* ou usar `unique_ptr` + *heap*.

## Parâmetros e texto

- **`AudioParameterFloatAttributes`**: `withStringFromValueFunction` / `withValueFromStringFunction` para validar texto no *text box* dos sliders (evitar `getFloatValue()` silencioso a 0 em lixo).
- Constante **`silenceDb`** (ex. -80) partilhada entre conversores no `.cpp` (namespace anónimo).

## Testes (Catch2 + JUCE GUI)

- **`JUCE_MODAL_LOOPS_PERMITTED=1`** só no alvo **`FractalDelay_Tests`**, para poder usar `MessageManager::runDispatchLoopUntil` onde for necessário esvaziar a fila após testes com editor.
- **Windows / `ctest`**: por defeito os testes **tag** `[gui]` podem ficar **fora** do `ctest` (fila de mensagens JUCE sem *pump* = RAM a crescer). Correr o `.exe` manualmente ou CI com *display* (Linux: `xvfb-run`).
- **`SKIP_FRACTAL_DELAY_GUI_TESTS`**: ambiente sem GUI — os testes marcam `SKIP`.

## Referência cruzada

- Índice: [`README.md`](README.md).
- Testes por fase: [`00-testes-unitarios-visao-geral.md`](00-testes-unitarios-visao-geral.md).
