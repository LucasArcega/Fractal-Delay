# Plano: UI básica — parâmetros do motor de delay (Fase 1 produto)

> **ClickUp:** [Fase 1 — Motor de Delay Básico](https://app.clickup.com/90171225448/v/l/li/901713673870) · **Entrega:** 5/5 nesta pasta  
> **Tarefa:** [UI básica com parâmetros da Fase 1](https://app.clickup.com/t/86e1b2fga)  
> **Dependências:** entregas [`01`](01-configuracao-projeto-juce.md)–[`04`](04-modos-clean-tape.md) (parâmetros APVTS alinhados ao DSP)  

## Objetivo

Interface para os parâmetros do **motor de delay básico** (esta fase ClickUp): tempo (musical/ms), subdivisão, *feedback*, *mix*, modo Clean/Tape, controles de *tape* se existirem, BPM só leitura opcional.

## Nota de nomenclatura

- **“Fase 1” na tarefa ClickUp** = primeira fase de **produto** (motor básico), **não** confundir com a [Fase 5 — Post-FX e Spatial](../fase-5-post-fx-spatial/) do *roadmap* completo.

## Estado já implementado no repo (base para evoluir)

- **Layout:** `juce::Grid` na `AudioProcessorEditor` — linhas: cabeçalho, corpo (3 colunas), rodapé; colunas **IN** | **Delay (placeholder)** | **OUT**.
- **Colunas:** `juce::Component` “coluna” + `juce::FlexBox` em `resized()` (título, *label* de pico, *slider* vertical com altura mínima fixa).
- **JUCE 8:** `Grid::Fr` usa **inteiros**; `grid.performLayout(juce::Rectangle<int>)`.
- **Widgets partilhados:** `Source/Components/`, namespace **`GUI`** (`FooterBar`, `HorizontalMetter`, …).
- **Rodapé:** `GUI::FooterBar` — caixa com dois `GUI::HorizontalMetter` empilhados (**saída L** em cima, **saída R** em baixo), largura ~55% centrada, *gap* e altura de faixa reduzidos.
- **Áudio → UI:** fila *lock-free* (`AbstractFifo`) com mensagens (`PEAK_IN`, `PEAK_OUT`, `PEAK_OUT_LEFT`, `PEAK_OUT_RIGHT`, `INCREMENT`) escrita no `processBlock` e lida em `idle()` chamado por **`IdleTimer`** (~30 Hz), padrão alinhado ao [airwindows/Meter](https://github.com/airwindows/Meter).
- **Parâmetros:** `SliderAttachment` para `inputGainDb` / `outputGainDb`; atributos de `AudioParameterFloat` com conversão texto↔dB segura.

Ver checklist técnico completo: [`../00-convencoes-repo-ui-testes.md`](../00-convencoes-repo-ui-testes.md).

## Escopo (resto da Fase 1 UI)

- `SliderAttachment` / `ComboBoxAttachment` / APVTS para **tempo**, *feedback*, *mix*, modo Clean/Tape, etc., conforme [`02`](02-buffer-circular-delay.md)–[`04`](04-modos-clean-tape.md).
- Manter o mesmo padrão de **fila + `idle()`** para novos medidores ou visualizações (não voltar a `Timer` + `isShowing()` sem *pump* garantido).

## Fora de escopo

- *Skins* complexas; visualização espectral (ver [Fase 4](../fase-4-spectral-ducking/)).

## Testes unitários

| Teste | Descrição |
|--------|-----------|
| `ParameterLayoutCompleteness` | IDs esperados ⊆ APVTS. |
| `DefaultValuesMatchSpec` | *Defaults* da especificação. |
| `StateRoundTrip` | `copyState` / `setStateInformation`. |

Testes **GUI**: ver [`00-testes-unitarios-visao-geral.md`](../00-testes-unitarios-visao-geral.md) e convenções Windows/CI.

## Próxima fase ClickUp

[Fase 2 — Feedback Engine](../fase-2-feedback-engine/) — saturação e filtros no *loop*, *crossfeed* e modulação.
