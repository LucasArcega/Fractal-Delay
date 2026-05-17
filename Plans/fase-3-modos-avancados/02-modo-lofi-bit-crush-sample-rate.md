# Plano: Modo Lo-Fi — bit crush e redução de sample rate

> **ClickUp:** [Fase 3 — Modos Avançados](https://app.clickup.com/90171225448/v/l/li/901713673875) · **Entrega:** 2/5 nesta pasta  
> **Tarefa:** [Modo Lo-Fi — bit crush e sample rate reduction](https://app.clickup.com/t/86e1b2fw8)  
> **Dependências:** núcleo de delay + *feedback*; posicionar no *wet* ou no *loop* conforme desenho  

## Objetivo

Degradação intencional: **quantização de bit depth** e **hold** / *downsampling* do sinal atrasado (ou do *loop*) para estética Lo-Fi.

## Escopo

- Bit depth efetivo (float → quantizar para 2–16 bits simulados).
- *Sample-and-hold* com fator inteiro ou fracionário (CIC leve ou *hold* simples).
- Anti-*zipper* em parâmetros; opcional filtro suave pós-*crush*.

## Testes unitários

| Teste | Descrição |
|--------|-----------|
| `QuantizationSteps` | Profundidade mínima → número finito de níveis observados numa rampa. |
| `DownsampleHoldPeriod` | Período de *hold* alinha-se a `factor / sampleRate`. |
| `ParameterSmoothing` | Mudança brusca de bits não gera DC infinito (média limitada). |

---

**Referência transversal:** convenções de repo, UI, filas áudio↔UI e testes JUCE — [`00-convencoes-repo-ui-testes.md`](../00-convencoes-repo-ui-testes.md).

## Próximo

[`03-modo-reverse-crossfade.md`](03-modo-reverse-crossfade.md)
