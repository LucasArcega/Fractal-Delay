# Plano: EQ paramétrico de 8 bandas no sinal wet

> **ClickUp:** [Fase 5 — Post-FX e Spatial](https://app.clickup.com/90171225448/v/l/li/901713673882) · **Entrega:** 1/4 nesta pasta  
> **Tarefa:** [EQ paramétrico de 8 bandas no sinal wet](https://app.clickup.com/t/86e1b2ga3)  
> **Dependências:** *wet tap* estável pós-Fase 4 ou bypass de *ducking*; latência global contabilizada  

## Objetivo

**EQ mínimo-fase ou bell/shelf** em cadeia (8 bandas) aplicado **só ao *wet***, pós-delay / pós-modos.

## Escopo

- Tipos: *peak*, *low shelf*, *high shelf*, *HPF* opcional na primeira banda.
- Coeficientes `juce::dsp::IIR` ou `EQ` custom; *smoothing* de Q/ganho/freq.
- Presets de fábrica opcionais.
- **Modularização:** cadeia de EQ *wet* em **`Source/DSP/`** (módulos + `target_sources`). Ver [`00-convencoes-repo-ui-testes.md`](../00-convencoes-repo-ui-testes.md).

## Testes unitários

| Teste | Descrição |
|--------|-----------|
| `FlatResponseWhenAllZero` | Todas as bandas neutras → magnitude plana (± dB). |
| `SingleBellBoost` | Uma banda +6 dB em 1 kHz → pico no espectro esperado (FFT offline). |

---

**Referência transversal:** convenções de repo, UI, filas áudio↔UI e testes JUCE — [`00-convencoes-repo-ui-testes.md`](../00-convencoes-repo-ui-testes.md).

## Próximo

[`02-tube-saturation-compressor.md`](02-tube-saturation-compressor.md)
