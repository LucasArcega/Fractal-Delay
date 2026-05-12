# Plano: Filterbank de 8 bandas para análise espectral

> **ClickUp:** [Fase 4 — Spectral Ducking](https://app.clickup.com/90171225448/v/l/li/901713673880) · **Entrega:** 1/3 nesta pasta  
> **Tarefa:** [Filterbank de 8 bandas para análise espectral](https://app.clickup.com/t/86e1b2g4t)  
> **Dependências:** [Fase 3](../fase-3-modos-avancados/) ou *milestone* intermédio que congele API do *wet* a analisar  

## Objetivo

Decompor o sinal (tipicamente **entrada lateral** ou *wet*) em **8 bandas** para medição de energia / envoltória por banda, alimentando o *ducking*.

## Escopo

- Banco de filtros em cascata ou DFT enxuta; latência uniforme entre bandas (compensação explícita).
- Frequências de *edges* configuráveis ou preset (octavas).
- Saída: vetor de 8 amplitudes RMS ou pico suavizado por bloco.

## Fora de escopo

- *EQ* criativo de 8 bandas no *wet* ([Fase 5](../fase-5-post-fx-spatial/01-eq-parametrico-8-bandas-wet.md)) — aqui o foco é **análise**.

## Testes unitários

| Teste | Descrição |
|--------|-----------|
| `PinkNoiseBandEnergy` | Ruído rosa: energia por banda decresce ~ esperado (tolerância log). |
| `SineInSingleBand` | Tom puro cai principalmente numa banda (isolamento cruzado < limiar). |
| `LatencyMatched` | Soma das bandas reconstruída (se aplicável) alinhada à entrada com atraso conhecido. |

## Próximo

[`02-envelope-follower-banda-gain-computer.md`](02-envelope-follower-banda-gain-computer.md)
