# Plano: Tube saturation e compressor

> **ClickUp:** [Fase 5 — Post-FX e Spatial](https://app.clickup.com/90171225448/v/l/li/901713673882) · **Entrega:** 2/4 nesta pasta  
> **Tarefa:** [Tube saturation e compressor](https://app.clickup.com/t/86e1b2gch)  
> **Dependências:** [`01-eq-parametrico-8-bandas-wet.md`](01-eq-parametrico-8-bandas-wet.md) (ordem FX: EQ → sat → comp, ou inverter com justificativa)  

## Objetivo

Cadeia **válvula** (saturação assimétrica ou *wavefolder* suave) seguida de **compressor** (VCA ou *feed-forward*) no *wet* ou no *master wet bus*.

## Escopo

- *Oversampling* ×2 recomendado para saturação se CPU permitir.
- Compressor: *threshold*, *ratio*, *attack/release*, *make-up*.

## Testes unitários

| Teste | Descrição |
|--------|-----------|
| `CompressorReduction` | Sinal acima do limiar reduz pico conforme *ratio*. |
| `SaturationAsymmetry` | Meia-onda positiva vs. negativa diferem (métrica simples). |

---

**Referência transversal:** convenções de repo, UI, filas áudio↔UI e testes JUCE — [`00-convencoes-repo-ui-testes.md`](../00-convencoes-repo-ui-testes.md).

## Próximo

[`03-clipper-soft-hard-chorus.md`](03-clipper-soft-hard-chorus.md)
