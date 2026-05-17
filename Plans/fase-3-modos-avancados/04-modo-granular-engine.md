# Plano: Modo Granular — motor granular completo

> **ClickUp:** [Fase 3 — Modos Avançados](https://app.clickup.com/90171225448/v/l/li/901713673875) · **Entrega:** 4/5 nesta pasta  
> **Tarefa:** [Modo Granular — granular engine completo](https://app.clickup.com/t/86e1b2g20)  
> **Dependências:** buffer longo; eventual leitura fracionária; *envelope* de grão  

## Objetivo

Motor **granular** com múltiplas vozes, leitura a partir do buffer de delay com **posição, tamanho e densidade** de grãos controláveis e *windowing* (Hann).

## Escopo

- N vozes; fila de grãos; jitter de posição e de pitch fino (± cents).
- Limites de CPU: máximo de grãos ativos; *fade in/out* por grão.
- Mistura com modos anteriores via parâmetro global de modo.

## Testes unitários

| Teste | Descrição |
|--------|-----------|
| `GrainSchedulerDeterministic` | *Seed* fixa → sequência de offsets reprodutível. |
| `MaxVoicesRespected` | Nunca excede vozes configuradas. |
| `SilenceInputZeroGrains` | Entrada zero → saída com energia abaixo de limiar (sem *explosion*). |

---

**Referência transversal:** convenções de repo, UI, filas áudio↔UI e testes JUCE — [`00-convencoes-repo-ui-testes.md`](../00-convencoes-repo-ui-testes.md).

## Próximo

[`05-modos-fractal-bloom-shards.md`](05-modos-fractal-bloom-shards.md)
