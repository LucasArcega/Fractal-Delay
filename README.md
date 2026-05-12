# Fractal-Delay

Plugin de áudio (JUCE) em desenvolvimento. Ver [`Plans/README.md`](Plans/README.md) para o *roadmap* por fases (ClickUp).

## Requisitos

- CMake 3.22 ou superior  
- Compilador C++17 (Visual Studio 2022, Xcode ou GCC)  
- Git (para o *fetch* do JUCE e do Catch2 via CMake)  
- Opcional: [Ninja](https://ninja-build.org/) para builds mais rápidos  

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
