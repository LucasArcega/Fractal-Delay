# Fractal-Delay

Plugin de áudio (JUCE) em desenvolvimento. Ver [`Plans/README.md`](Plans/README.md) para o *roadmap* por fases (ClickUp).

## Requisitos

- CMake 3.22 ou superior  
- Compilador C++17 (Visual Studio 2022, Xcode ou GCC)  
- Git (para o *fetch* do JUCE e do Catch2 via CMake)  
- Opcional: [Ninja](https://ninja-build.org/) para builds mais rápidos  

## Windows: pacotes (winget)

No **Windows Package Manager** (`winget`), estes são os pacotes mais directos para o fluxo CMake + JUCE:

| Ferramenta | ID `winget` | Notas |
|------------|-------------|--------|
| CMake | `Kitware.CMake` | Versão recente no catálogo (ex.: 4.3.x); satisfaz `cmake_minimum_required(3.22)`. |
| Ninja | `Ninja-build.Ninja` | Gerador rápido; depois: `cmake -B build -G Ninja`. |
| Git | `Git.Git` | Se ainda não tiveres o Git no PATH. |
| Compilador MSVC | `Microsoft.VisualStudio.2022.BuildTools` ou `Microsoft.VisualStudio.2022.Community` | Instala pelo **Visual Studio Installer** a carga **“Desktop development with C++”** (MSVC, Windows SDK). O *winget* sozinho costuma precisar de argumentos de *workload*; o caminho mais simples é abrir o instalador e marcar essa carga. |

Instalação em linha de comando (CMake + Ninja + Git), com confirmação interactiva do `winget`:

```powershell
winget install --id Kitware.CMake -e --source winget
winget install --id Ninja-build.Ninja -e --source winget
winget install --id Git.Git -e --source winget
```

Depois de instalar o **CMake**, abre um **novo** terminal e verifica:

```powershell
cmake --version
ninja --version
```

**Dependências do projecto:** JUCE e Catch2 vêm pelo **CMake `FetchContent`** (rede na primeira configuração); **não** é necessário vcpkg nem submódulo para esse passo.

### O que costuma faltar no Windows

1. **`cmake -G "Visual Studio 17 2022"` falha** com “could not find any instance of Visual Studio”, mesmo tendo a pasta `Community` — o **Visual Studio Installer** por vezes não regista a instalação (cópia manual, *drive* mudado, etc.). **Reparação:** abre o *Visual Studio Installer* → **Modify** na tua instalação → garante a carga **“Desktop development with C++”** (MSVC + Windows SDK). Depois volta a correr o CMake.  
2. **Ninja + terminal normal** — o `cl.exe` não está no `PATH` até carregares o ambiente MSVC. Usa um dos caminhos abaixo.  
3. **Pasta `build/` a meio** — se mudaste de gerador ou a configuração falhou a meio, apaga `build/` e volta a configurar (`rmdir /s /q build` no *cmd*).

### Atalho: scripts (MSVC + Ninja)

Fluxo **único** no Windows (ambiente VS + configure **Debug** + Ninja + flags do projecto + build + `ctest` com testes de GUI incluídos):

```bat
scripts\all-debug.bat
```

Isto chama `VsDevCmd.bat` (ou `vswhere`), mete o **Ninja** do `winget` no `PATH`, e corre **configure → build → ctest**. As opções de cache partilhadas (ex. testes Catch2 **`[gui]`** no `ctest`) estão em `scripts\cmake-debug-cache-flags.bat` — um sítio só para não duplicar linhas de `cmake`.

Para **só** reconfigurar ou **só** compilar (depois de já teres feito configure pelo menos uma vez):

```bat
scripts\configure-ninja.bat
scripts\build-debug.bat
```

O `configure-ninja.bat` usa as **mesmas** flags que o `all-debug.bat`.

## Configurar e compilar (manual / outros SO)

Na raiz do repositório:

```bash
cmake -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build --config Debug
```

Com Ninja (um gerador por configuração):

```bash
cmake -B build -G Ninja -DCMAKE_BUILD_TYPE=Debug
cmake --build build
```

No Windows, o comando acima só funciona no **“x64 Native Tools Command Prompt for VS 2022”** (ou depois de `VsDevCmd.bat`), salvo se usares os `scripts\` acima.

**Nota:** `cmake` “nu” na shell **não** aplica as mesmas opções que os `scripts\` (ver `scripts\cmake-debug-cache-flags.bat`). Para alinhar com o CI / `all-debug`, passa por exemplo `-DFRACTAL_DELAY_CTEST_INCLUDE_GUI=ON` ao configurar.

### Versão do JUCE

O *tag* Git do JUCE é a variável de cache `JUCE_TAG` (por omissão `8.0.12`). Para fixar outra versão:

```bash
cmake -B build -DJUCE_TAG=8.0.4
```

## Testes

- **Windows (recomendado):** `scripts\all-debug.bat` já corre **`ctest`** no fim (com testes **`[gui]`** activos via cache do configure).
- **Manual:** `ctest --test-dir build --output-on-failure` (o binário está em `build/`, nome `FractalDelay_Tests.exe` no Windows).
- **Linux** sem ecrã: `xvfb-run -a ctest --test-dir build --output-on-failure` (como no CI). Só o teste de GUI: `FractalDelay_Tests "[gui]"`.

## Executar sem DAW

Depois do build, o alvo **Standalone** gera uma aplicação (por exemplo `FractalDelay_artefacts/Debug/Standalone/Fractal Delay.exe` no Windows, caminhos exactos dependem do gerador). Use-a para validar áudio e MIDI sem abrir uma DAW.

O **VST3** fica em `build/…/FractalDelay_artefacts/…/VST3/` após o build. Para copiar automaticamente para a pasta do sistema (ex.: `Common Files\VST3` no Windows, muitas vezes exige *admin*), configura com `-DFRACTAL_DELAY_COPY_PLUGIN_AFTER_BUILD=ON`.
