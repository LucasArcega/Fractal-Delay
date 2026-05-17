# Plano: Envelope follower por banda e gain computer

> **ClickUp:** [Fase 4 — Spectral Ducking](https://app.clickup.com/90171225448/v/l/li/901713673880) · **Entrega:** 2/3 nesta pasta  
> **Tarefa:** [Envelope follower por banda e gain computer](https://app.clickup.com/t/86e1b2g6c)  
> **Dependências:** [`01-filterbank-8-bandas-analise-espectral.md`](01-filterbank-8-bandas-analise-espectral.md)  

## Objetivo

Por cada banda: **detector de envoltória** (*attack/release*) e **gain computer** que reduz o ganho do *wet* (ou do *bus* alvo) quando a energia lateral ultrapassa limiar — *spectral ducking*.

## Escopo

- Parâmetros: *threshold*, *ratio*, *attack/release* por banda ou global com *link*.
- *Lookahead* opcional (buffer pequeno) vs. causal.
- Mistura final: aplicar ganhos por banda via **filtros de síntese** espelhando a análise ou via **STFT** se arquitetura for frequencial.
- **Modularização:** *envelope* / *gain computer* em **`Source/DSP/`** + `target_sources`. Ver [`00-convencoes-repo-ui-testes.md`](../00-convencoes-repo-ui-testes.md).

## Fora de escopo

- UI de visualização em tempo real ([`03`](03-visualizacao-spectral-ducking-tempo-real.md)).

## Testes unitários

| Teste | Descrição |
|--------|-----------|
| `FollowerAttackFasterThanRelease` | Constantes de tempo respeitam ordem documentada. |
| `DuckingReducesWet` | Sinal lateral acima do limiar → RMS do *wet* cai X dB. |
| `NoDuckWhenSideBelow` | Lateral baixo → ganho ≈ 0 dB de redução. |

---

**Referência transversal:** convenções de repo, UI, filas áudio↔UI e testes JUCE — [`00-convencoes-repo-ui-testes.md`](../00-convencoes-repo-ui-testes.md).

## Próximo

[`03-visualizacao-spectral-ducking-tempo-real.md`](03-visualizacao-spectral-ducking-tempo-real.md)
