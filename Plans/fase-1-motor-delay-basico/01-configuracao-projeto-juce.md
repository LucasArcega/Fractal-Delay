# Plano: Configurar projeto JUCE do zero

> **ClickUp:** [Fase 1 — Motor de Delay Básico](https://app.clickup.com/90171225448/v/l/li/901713673870) · **Entrega:** 1/5 nesta pasta  
> **Tarefa:** [Configurar projeto JUCE do zero](https://app.clickup.com/t/86e1b2f2x)  
> **Dependências:** nenhuma  

## Objetivo

Ter um *plugin* de áudio (formato alvo: VST3 e, opcionalmente, AU no macOS / Standalone para depuração) compilando de forma reprodutível, com estrutura de pastas clara, alvo de testes separado do processador e integração mínima com o *pipeline* de áudio do JUCE.

## Escopo

- Repositório com **CMake** (recomendado) ou **Projucer** (`.jucer`); este plano assume **CMake + `juce_add_plugin`** por ser o padrão atual do JUCE e facilitar CI.
- Tipos de *plugin*: efeito (*effect*), mono ou estéreo conforme decisão do produto; documentar canais de entrada/saída.
- Versão mínima do JUCE (fixar no `CMakeLists.txt` ou submódulo `JUCE` com *tag* explícita).
- *Targets*: `FractalDelay` (plugin), `FractalDelay_Standalone` (opcional mas útil), `FractalDelay_Tests` (executável de testes — ver secção de testes e [`../00-testes-unitarios-visao-geral.md`](../00-testes-unitarios-visao-geral.md)).

## Fora de escopo nesta entrega

- DSP de delay completo, UI além do mínimo, presets comerciais.

## Passos de implementação

### 1. Estrutura de diretórios sugerida

```
fractal-delay/
  CMakeLists.txt
  Source/
    PluginProcessor.h / .cpp
    PluginEditor.h   / .cpp
  Tests/
    Main.cpp
    ...
  third_party/ ou modules/
  Plans/
    README.md
    fase-1-motor-delay-basico/
    ...
```

### 2. `CMakeLists.txt` (nível raiz)

- `cmake_minimum_required` compatível com a *toolchain*.
- Incluir JUCE: **submódulo Git** `https://github.com/juce-framework/JUCE` com *tag* fixa, ou **CPM.cmake** / `FetchContent` com `GIT_TAG`.
- `juce_add_plugin` com `PLUGIN_MANUFACTURER_CODE`, `PLUGIN_CODE` únicos; `COPY_PLUGIN_AFTER_BUILD TRUE`; `FORMATS` (ex.: `VST3 Standalone`).
- `target_sources` para `PluginProcessor` / `PluginEditor`.
- Linkar módulos JUCE mínimos; incluir `juce_dsp` quando o DSP desta fase for integrado.

### 3. `AudioProcessor` esqueleto

- Stubs: `prepareToPlay`, `releaseResources`, `processBlock`, `createEditor`; parâmetros placeholder se útil.
- `isBusesLayoutSupported`: layouts suportados explícitos.

### 4. `AudioProcessorEditor` mínimo

- Janela mínima ou `GenericAudioProcessorEditor` até concluir [`05-ui-parametros-motor-delay-basico.md`](05-ui-parametros-motor-delay-basico.md).

### 5. CI

- Compilar e executar testes (mesmo que só *smoke test* nesta entrega).

## Critérios de aceite

- Compilação limpa; plugin abre num *host*; alvo de testes existe.

## Testes unitários

Ver também [`../00-testes-unitarios-visao-geral.md`](../00-testes-unitarios-visao-geral.md).

| Teste | O que valida |
|--------|----------------|
| `ProcessorInstantiation` | Instância do processador e IDs não vazios. |
| `BusesLayoutStereo` | Layout estéreo aceite; inválido rejeitado. |
| `PrepareAndRelease` | `prepareToPlay` / `releaseResources` sem *crash*. |
| `EditorCreates` | Ciclo de vida do editor. |

## Riscos

- Submódulo JUCE vs pacote; códigos de 4 letras únicos.

## Próximo documento nesta pasta

[`02-buffer-circular-delay.md`](02-buffer-circular-delay.md)
