# Plano: UI básica — parâmetros do motor de delay (Fase 1 produto)

> **ClickUp:** [Fase 1 — Motor de Delay Básico](https://app.clickup.com/90171225448/v/l/li/901713673870) · **Entrega:** 5/5 nesta pasta  
> **Tarefa:** [UI básica com parâmetros da Fase 1](https://app.clickup.com/t/86e1b2fga)  
> **Dependências:** entregas [`01`](01-configuracao-projeto-juce.md)–[`04`](04-modos-clean-tape.md) (parâmetros APVTS alinhados ao DSP)  

## Objetivo

Interface mínima para os parâmetros do **motor de delay básico** (esta fase ClickUp): tempo (musical/ms), subdivisão, *feedback*, *mix*, modo Clean/Tape, controles de *tape* se existirem, BPM só leitura opcional.

## Nota de nomenclatura

- **“Fase 1” na tarefa ClickUp** = primeira fase de **produto** (motor básico), **não** confundir com a [Fase 5 — Post-FX e Spatial](../fase-5-post-fx-spatial/) do *roadmap* completo.

## Escopo

- `AudioProcessorEditor` + `SliderAttachment` / `ComboBoxAttachment` / etc.
- Layout simples (`FlexBox`/`Grid`); persistência via estado do processador.

## Fora de escopo

- *Skins* complexas; visualização espectral (ver [Fase 4](../fase-4-spectral-ducking/)).

## Testes unitários

| Teste | Descrição |
|--------|-----------|
| `ParameterLayoutCompleteness` | IDs esperados ⊆ APVTS. |
| `DefaultValuesMatchSpec` | *Defaults* da especificação. |
| `StateRoundTrip` | `copyState` / `setStateInformation`. |

## Próxima fase ClickUp

[Fase 2 — Feedback Engine](../fase-2-feedback-engine/) — saturação e filtros no *loop*, *crossfeed* e modulação.
