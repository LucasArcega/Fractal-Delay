# Plano: Modos Fractal, Bloom e Shards

> **ClickUp:** [Fase 3 — Modos Avançados](https://app.clickup.com/90171225448/v/l/li/901713673875) · **Entrega:** 5/5 nesta pasta  
> **Tarefa:** [Modos Fractal, Bloom e Shards](https://app.clickup.com/t/86e1b2g3c)  
> **Dependências:** granular e/ou *multi-tap*; documento de **design sonoro** prévio (este plano assume definição conjunta produto/áudio)  

## Objetivo

Três variantes de **multi-tap / recursão espectral temporal** (nomes de marketing alinhados ao *Fractal-Delay*):

- **Fractal:** hierarquia de atrasos derivados (ex.: tap em D, D×k, D×k²…) com atenuação controlada.
- **Bloom:** difusão temporal com *smear* e talvez realimentação cruzada entre taps.
- **Shards:** distribuição estocástica ou em cluster de taps com *jitter*.

## Escopo

- Especificação matemática por modo (tabela de taps e ganhos).
- Partilha de infra com motor granular ou *delay line* múltipla.
- Prioridade **baixa** no ClickUp — implementar após modos core (BBD, Lo-Fi, Reverse, Granular).
- **Modularização:** taps / Bloom / Shards em **`Source/DSP/`** (módulos claros + `target_sources`). Ver [`00-convencoes-repo-ui-testes.md`](../00-convencoes-repo-ui-testes.md).

## Testes unitários

| Teste | Descrição |
|--------|-----------|
| `TapCountAndGains` | Número de taps e vetor de ganhos coincide com especificação para preset de teste. |
| `EnergyBound` | Resposta a impulso com *feedback* unitário limitada. |
| `DeterministicShards` | Modo *Shards* com *seed* fixa → saída binária comparável (*golden* file curto). |

---

**Referência transversal:** convenções de repo, UI, filas áudio↔UI e testes JUCE — [`00-convencoes-repo-ui-testes.md`](../00-convencoes-repo-ui-testes.md).

## Próxima fase ClickUp

[Fase 4 — Spectral Ducking](../fase-4-spectral-ducking/)
