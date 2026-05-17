# Plano: Modo BBD — emulação Bucket Brigade

> **ClickUp:** [Fase 3 — Modos Avançados](https://app.clickup.com/90171225448/v/l/li/901713673875) · **Entrega:** 1/5 nesta pasta  
> **Tarefa:** [Modo BBD — Bucket Brigade emulation](https://app.clickup.com/t/86e1b2ftx)  
> **Dependências:** [Fase 1](../fase-1-motor-delay-basico/) + [Fase 2](../fase-2-feedback-engine/) estáveis  

## Objetivo

Modo de delay que imita **BBD**: banda limitada, *smearing* temporal, não-linearidades leves e eventual **compensação de *clock* / número de estágios** como parâmetros sonoros.

## Escopo

- Arquitetura sugerida: *delay line* principal + **filtros anti-*alias* / reconstrução** agressivos + opcional *waveshaper* leve no caminho.
- Parâmetros: “*stages*” ou *darkness*, *drive*, mistura do modo com o modo anterior (se *dry* de modo).
- Alternativa literatura: modelo baseado em ODEs ou cadeia de *allpass* — escolher uma abordagem e documentar no cabeçalho do módulo.
- **Modularização:** modo BBD como módulo(s) em **`Source/DSP/`** + `target_sources`; não `juce::Component`. Ver [`00-convencoes-repo-ui-testes.md`](../00-convencoes-repo-ui-testes.md).

## Fora de escopo

- Emulação de chip específico com documentação de patente (manter genérico).

## Testes unitários

| Teste | Descrição |
|--------|-----------|
| `BypassMatchesCleanDelay` | Modo BBD “neutro” ou *mix* 0 → equivalência ao núcleo base dentro de epsilon. |
| `HighFreqAttenuation` | Energia > 12 kHz reduzida vs. modo Clean em sinal de teste. |
| `StabilityImpulse` | Impulso não explode após 1 s de *feedback* moderado. |

---

**Referência transversal:** convenções de repo, UI, filas áudio↔UI e testes JUCE — [`00-convencoes-repo-ui-testes.md`](../00-convencoes-repo-ui-testes.md).

## Próximo

[`02-modo-lofi-bit-crush-sample-rate.md`](02-modo-lofi-bit-crush-sample-rate.md)
