# Plano: Clipper soft/hard e chorus

> **ClickUp:** [Fase 5 — Post-FX e Spatial](https://app.clickup.com/90171225448/v/l/li/901713673882) · **Entrega:** 3/4 nesta pasta  
> **Tarefa:** [Clipper soft/hard e chorus](https://app.clickup.com/t/86e1b2gee)  
> **Dependências:** [`02-tube-saturation-compressor.md`](02-tube-saturation-compressor.md) (ou paralelo com ordem documentada)  

## Objetivo

**Clipper** com modo *soft* / *hard* para pegada final e **chorus** leve (1–3 vozes desintonadas) para espessura pós-delay.

## Escopo

- Clipper: limiar em dBFS, *knee* no modo soft.
- Chorus: LFO, profundidade, *delay* curto por voz, filtro opcional.

## Testes unitários

| Teste | Descrição |
|--------|-----------|
| `HardClipNeverExceeds` | Amostras absolutas ≤ 1.0f (ou *headroom* definido). |
| `ChorusModulationDepth` | Profundidade 0 → saída ≈ entrada (bypass interno). |

## Próximo

[`04-halo-widener-diffusal-bass-mono.md`](04-halo-widener-diffusal-bass-mono.md)
