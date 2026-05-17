# Plano: Filtros low-cut e high-cut no loop

> **ClickUp:** [Fase 2 — Feedback Engine](https://app.clickup.com/90171225448/v/l/li/901713673872) · **Entrega:** 2/3 nesta pasta  
> **Tarefa:** [Filtros low-cut e high-cut no loop](https://app.clickup.com/t/86e1b2fmw)  
> **Dependências:** [`01-saturacao-soft-clipping-feedback.md`](01-saturacao-soft-clipping-feedback.md) (definir ordem saturação ↔ filtros) ou implementar em paralelo com contrato de ordem fixo  

## Objetivo

**High-pass** e **low-pass** no caminho de realimentação para moldar tonalidade a cada repetição (remover *mud* acumulado, suavizar agudos agressivos).

## Escopo

- Filtros IIR de 1ª ou 2ª ordem por canal (`juce::dsp::IIR` ou `StateVariableTPTFilter`).
- Frequências de corte em Hz com *smoothing*; preparação em `prepareToPlay` com `ProcessorDuplicator` para estéreo.
- Resposta estável: verificar *zipper* ao automatizar cortes.

## Fora de escopo

- *EQ* paramétrico de 8 bandas no *wet* (isso é [Fase 5](../fase-5-post-fx-spatial/01-eq-parametrico-8-bandas-wet.md)).

## Passos de implementação

1. Bloco `FeedbackToneStack` com `setLowCutHz`, `setHighCutHz`, `prepare`, `reset`, `process`.
2. Inserir no *loop* conforme diagrama acordado com [`01`](01-saturacao-soft-clipping-feedback.md).
3. Garantir que cortes extremos não invertam fase de modo inaudível — documentar bypass quando Hz fora de intervalo útil.

## Critérios de aceite

- Espectro de cada eco mostra atenuação esperada em graves/agudos (medição offline ou *FFT* em teste).
- Sem instabilidade com *feedback* alto + saturação.

## Testes unitários

| Teste | Descrição |
|--------|-----------|
| `MagnitudeHighPass` | Sinal DC bloqueado com HP ativo (média → 0). |
| `MagnitudeLowPass` | Energia acima de Nyquist efetivo reduzida vs. bypass. |
| `StereoIndependentState` | L/R com coeficientes iguais mas estados separados. |
| `PrepareSampleRateChange` | `prepare` 44100 → 48000 recalcula coeficientes sem *crash*. |

---

**Referência transversal:** convenções de repo, UI, filas áudio↔UI e testes JUCE — [`00-convencoes-repo-ui-testes.md`](../00-convencoes-repo-ui-testes.md).

## Próximo documento nesta pasta

[`03-stereo-crossfeed-modulacao-delay.md`](03-stereo-crossfeed-modulacao-delay.md)
