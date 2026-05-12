# Plano: Stereo crossfeed e modulação de delay

> **ClickUp:** [Fase 2 — Feedback Engine](https://app.clickup.com/90171225448/v/l/li/901713673872) · **Entrega:** 3/3 nesta pasta  
> **Tarefa:** [Stereo crossfeed e modulação de delay](https://app.clickup.com/t/86e1b2fq3)  
> **Dependências:** [`01`](01-saturacao-soft-clipping-feedback.md) + [`02`](02-filtros-low-cut-high-cut-loop.md) (laço de *feedback* completo)  

## Objetivo

- **Crossfeed:** misturar uma fração do canal oposto na entrada do *delay* de cada lado para espacialização estilo *ping-pong* suave.
- **Modulação:** LFO (ou múltiplos) no **tempo de delay** e/ou no **nível de crossfeed**, com profundidade e forma de onda ajustáveis.

## Escopo

- Matriz 2×2 configurável: entradas L/R → entradas efetivas dos *delay lines*.
- LFOs em fase audível (0,05–10 Hz típico) com `LinearSmoothedValue` no *delay time* (interpolação necessária no buffer — pode exigir **atraso fracionário** ou oversampling do índice de leitura).
- Sincronização opcional ao BPM do *host* para velocidade de LFO (reutilizar utilitários da Fase 1).

## Fora de escopo

- *Widener* espacial avançado ([Fase 5](../fase-5-post-fx-spatial/04-halo-widener-diffusal-bass-mono.md)).

## Passos de implementação

1. Parâmetros: `crossfeedAmount`, `lfoRate`, `lfoDepth`, `lfoShape`.
2. Implementar leitura atrasada **fracionária** (interpolação linear ou *lagrange* curto) se ainda não existir — pode ser pré-requisito técnico desta entrega.
3. Testar estabilidade: modulação profunda + *feedback* alto.

## Critérios de aceite

- Crossfeed 0 = comportamento idêntico ao estéreo independente.
- Modulação audível sem *clicks* de descontinuidade no índice de leitura.

## Testes unitários

| Teste | Descrição |
|--------|-----------|
| `CrossfeedZeroIdentity` | *Amount* = 0 → L só alimenta delay L. |
| `CrossfeedFullMatrix` | *Amount* máximo → mistura esperada numérica (matriz documentada). |
| `LfoDepthBounds` | Profundidade limitada para índice sempre válido no buffer. |
| `ModulatedDelayContinuous` | Varredura de `delayOffset` contínua sem saltos > 1 amostra por amostra (ou documentar *artefact* aceite). |

## Próxima fase ClickUp

[Fase 3 — Modos Avançados](../fase-3-modos-avancados/)
