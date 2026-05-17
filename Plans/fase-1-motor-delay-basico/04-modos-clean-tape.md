# Plano: Implementar modos Clean e Tape

> **ClickUp:** [Fase 1 — Motor de Delay Básico](https://app.clickup.com/90171225448/v/l/li/901713673870) · **Entrega:** 4/5 nesta pasta  
> **Tarefa:** [Implementar modos Clean e Tape](https://app.clickup.com/t/86e1b2few)  
> **Dependências:** [`01`](01-configuracao-projeto-juce.md)–[`03`](03-sincronizacao-bpm-host.md)  

## Objetivo

Dois caminhos no mesmo núcleo: **Clean** (linear, transparente) e **Tape** (saturação + perda de agudos + opcional *wow/flutter*).

## Escopo

- `DelayCharacter { Clean, Tape }` ou parâmetro equivalente.
- Mistura *dry/wet* documentada; Tape com saturação e LP na realimentação ou pós-leitura conforme desenho único de produto.
- Estabilidade com *feedback* alto (*clip* suave ou limite de ganho de laço).
- **Modularização:** Clean/Tape e coloração em **`Source/DSP/`** (módulos dedicados + `target_sources`); não concentrar tudo no `PluginProcessor`. Ver [`00-convencoes-repo-ui-testes.md`](../00-convencoes-repo-ui-testes.md).

## Relação com a Fase 2 (ClickUp)

A [Fase 2 — Feedback Engine](../fase-2-feedback-engine/) aprofunda **saturação dedicada no feedback**, filtros no *loop* e *crossfeed*. Este documento cobre a **coloração base** do motor (Clean/Tape) da Fase 1; evitar duplicar responsabilidades — extrair `IDelayColorProcessor` ou blocos reutilizáveis para a Fase 2 estender.

## Critérios de aceite

- Troca de modo com *smoothing*; Clean com THD baixo; Tape com harmónicos crescentes com *drive*.

## Testes unitários

| Teste | Descrição |
|--------|-----------|
| `CleanUnityDry` | *Wet* = 0 → saída ≈ entrada. |
| `CleanLinearity` | Escala de amplitude proporcional. |
| `TapeStabilityMaxFeedback` | Envelope controlado em *feedback* máximo. |
| `ProcessorReset` | `reset()` reprodutível. |

**Nota:** quando houver medidores ou *meters* na UI, usar a fila áudio→UI documentada em [`00-convencoes-repo-ui-testes.md`](../00-convencoes-repo-ui-testes.md).

## Próximo documento nesta pasta

[`05-ui-parametros-motor-delay-basico.md`](05-ui-parametros-motor-delay-basico.md)
