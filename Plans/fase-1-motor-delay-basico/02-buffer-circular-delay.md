# Plano: Implementar buffer circular de delay

> **ClickUp:** [Fase 1 — Motor de Delay Básico](https://app.clickup.com/90171225448/v/l/li/901713673870) · **Entrega:** 2/5 nesta pasta  
> **Tarefa:** [Implementar buffer circular de delay](https://app.clickup.com/t/86e1b2f6r)  
> **Dependências:** [`01-configuracao-projeto-juce.md`](01-configuracao-projeto-juce.md)  

## Objetivo

**Buffer de atraso circular** por canal, escrita contínua e leitura com atraso em amostras (inteiro na v1; base para atraso fracionário), **sem alocações** no caminho de áudio após `prepareToPlay`.

## Escopo

- Implementação em **`Source/DSP/CircularDelayBuffer.h`** + **`Source/DSP/CircularDelayBuffer.cpp`** (classe pura; **não** `juce::Component`); o `PluginProcessor` apenas orquestra.
- Classe dedicada (nome canónico: `CircularDelayBuffer`), agnóstica de UI.
- Multi-canal; tamanho máximo derivado de `maxDelaySeconds * sampleRate` + margem.
- API sugerida: `prepare`, `reset`, `pushSample` / `pushBlock`, `readDelayed`.
- Interpolação linear ou nenhuma na v1 do buffer — documentar.

## Fora de escopo nesta entrega

- Sincronização musical: [`03-sincronizacao-bpm-host.md`](03-sincronizacao-bpm-host.md).
- Coloração Clean/Tape: [`04-modos-clean-tape.md`](04-modos-clean-tape.md).
- UI dedicada: [`05-ui-parametros-motor-delay-basico.md`](05-ui-parametros-motor-delay-basico.md).
- *Feedback engine* da [Fase 2 — Feedback Engine](../fase-2-feedback-engine/) (saturação/filtros no *loop* vêm depois).

## Design sugerido

- Ficheiros `Source/DSP/CircularDelayBuffer.{h,cpp}`: `std::vector<float>` por canal; `writeIndex`; *clamp* de `delayInSamples`.
- Integração estilo *write pointer* com `processBlock` para testes amostra a amostra.
- Não realocar em `processBlock`.

## Critérios de aceite

- Impulso + leitura atrasada na posição esperada; estéreo isolado; `reset()` previsível.

## Testes unitários

| Teste | Descrição |
|--------|-----------|
| `DelayReadbackInteger` | Leitura após D escritas conforme semântica definida. |
| `StereoIsolation` | Sem mistura L/R indevida. |
| `PrepareBounds` | *Clamp* ou erro documentado fora do máximo. |
| `ResetClearsState` | Estado inicial após `reset()`. |

## Riscos

- *Off-by-one* ms ↔ amostras; *thread safety* se UI ler o buffer (evitar ou duplicar estado).
- Qualquer **áudio → UI** deve seguir o padrão do repo (fila *lock-free* + *timer* no editor), não `std::atomic` + lógica de visibilidade frágil no Standalone — ver [`00-convencoes-repo-ui-testes.md`](../00-convencoes-repo-ui-testes.md).

## Próximo documento nesta pasta

[`03-sincronizacao-bpm-host.md`](03-sincronizacao-bpm-host.md)
