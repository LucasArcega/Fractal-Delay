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

## Configurar e compilar

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

### Versão do JUCE

O *tag* Git do JUCE é a variável de cache `JUCE_TAG` (por omissão `8.0.12`). Para fixar outra versão:

```bash
cmake -B build -DJUCE_TAG=8.0.4
```

## Testes

```bash
ctest --test-dir build --output-on-failure
```

Ou executar diretamente o binário `FractalDelay_Tests` na pasta de build.

## Executar sem DAW

Depois do build, o alvo **Standalone** gera uma aplicação (por exemplo `FractalDelay_artefacts/Debug/Standalone/Fractal Delay.exe` no Windows, caminhos exactos dependem do gerador). Use-a para validar áudio e MIDI sem abrir uma DAW.

O **VST3** é copiado para a pasta de *plugins* do utilizador quando `COPY_PLUGIN_AFTER_BUILD` está activo (definido no `CMakeLists.txt`).
