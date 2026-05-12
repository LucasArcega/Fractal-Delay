# Plano: Sincronização de tempo ao BPM do host

> **ClickUp:** [Fase 1 — Motor de Delay Básico](https://app.clickup.com/90171225448/v/l/li/901713673870) · **Entrega:** 3/5 nesta pasta  
> **Tarefa:** [Sincronização de tempo ao BPM do host](https://app.clickup.com/t/86e1b2fc1)  
> **Dependências:** [`01`](01-configuracao-projeto-juce.md) + [`02`](02-buffer-circular-delay.md)  

## Objetivo

Expressar o delay em **unidades musicais** ou **ms**; com `AudioPlayHead` válido, o atraso em amostras **acompanha BPM** (e *tempo map* se aplicável) de forma estável.

## Escopo

- *Wrapper* para `AudioPlayHead` / `PositionInfo` conforme a versão JUCE.
- Modo musical: subdivisão → segundos → amostras; modo ms independente de BPM.
- Fallback sem *playhead* ou BPM inválido; suavização opcional de `delaySamples`.

## Fora de escopo

- *Sample-accurate* automation fina (iteração futura).

## Passos

1. Funções puras: `beatsToSeconds`, `subdivisionToBeats`, `secondsToSamples`.
2. Estado no processador: `sampleRate`, último BPM, modo.
3. `processBlock`: atualizar alvo de atraso e alimentar o buffer circular.
4. Parâmetros APVTS (integração total com UI em [`05`](05-ui-parametros-motor-delay-basico.md)).

## Critérios de aceite

- Eco alinhado a BPM fixo em *host* compatível; sem *NaN* ao mudar BPM; política em pausa documentada.

## Testes unitários

| Teste | Descrição |
|--------|-----------|
| `BeatsToSeconds` | 1 beat a 60 BPM = 1 s. |
| `SubdivisionTable` | Fatores de semínima corretos. |
| `InvalidBpmFallback` | BPM ≤ 0 → fallback. |
| `PlayHeadAbsent` | Modo ms continua funcional. |

## Riscos

- Diferenças de API entre versões JUCE; encapsular.

## Próximo documento nesta pasta

[`04-modos-clean-tape.md`](04-modos-clean-tape.md)
