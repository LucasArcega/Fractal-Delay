# Plano: Saturação / soft clipping no feedback

> **ClickUp:** [Fase 2 — Feedback Engine](https://app.clickup.com/90171225448/v/l/li/901713673872) · **Entrega:** 1/3 nesta pasta  
> **Tarefa:** [Saturação / soft clipping no feedback](https://app.clickup.com/t/86e1b2fj4)  
> **Dependências:** concluir [Fase 1 — Motor de Delay Básico](../fase-1-motor-delay-basico/) (buffer + *mix* + *feedback* estáveis)  

## Objetivo

Inserir **não-linearidade controlada** exclusivamente (ou principalmente) no **caminho de realimentação** do delay, para cor e compressão em ecos sucessivos, com **soft clipping** configurável e sem *runaway* numérico.

## Escopo

- Ponto de inserção: após leitura do buffer e antes da soma de *feedback* à escrita (diagrama único no código).
- Curvas: `tanh`, polinómio ímpar suave, ou `juce::dsp::WaveShaper` com *drive* em dB ou linear suavizado (`LinearSmoothedValue`).
- Ordem de processamento em relação a [`02-filtros-low-cut-high-cut-loop.md`](02-filtros-low-cut-high-cut-loop.md): definir (ex.: saturação **depois** dos filtros no *loop* para domar agudos pré-*clip*).
- Parâmetros APVTS: intensidade, eventual *mix* dry do *shaper*.

## Fora de escopo

- Modelagem de válvula completa (reservado à [Fase 5](../fase-5-post-fx-spatial/01-tube-saturation-compressor.md) se overlapping — coordenar timbre).

## Passos de implementação

1. Extrair interface `FeedbackNonlinearity` com `prepare`, `reset`, `processSample` ou processamento por bloco SIMD-friendly.
2. Integrar no laço de *feedback* existente; garantir que *bypass* do efeito = identidade dentro de tolerância.
3. Limitar ganho de laço global (*feedback* × saturação) com telemetria opcional em *debug build*.

## Critérios de aceite

- *Drive* = mínimo → resposta próxima da linearidade (modo comparável ao motor sem sat extra).
- *Self-oscillation* apenas em extremos intencionais; sem *NaN*/*Inf*.

## Testes unitários

| Teste | Descrição |
|--------|-----------|
| `IdentityAtZeroDrive` | Coeficiente mínimo → saída ≈ entrada (epsilon). |
| `MonotonicHarmonics` | Harmónicos crescem monotonicamente com *drive* em tom sinusoidal fixo. |
| `FeedbackLoopStable` | Vetor de impulso com *feedback* alto e *shaper* ativo — energia limitada após N amostras. |
| `SmoothedParameterNoZipper` | Mudança escalonada de *drive* não dispara picos > limiar (opcional, *offline*). |

## Próximo documento nesta pasta

[`02-filtros-low-cut-high-cut-loop.md`](02-filtros-low-cut-high-cut-loop.md)
