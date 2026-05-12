# Visão geral: testes unitários em todas as fases

Este documento define **convenções comuns** para testes ao longo do [roadmap em `README.md`](README.md) (fases ClickUp 1–5).

## Princípios

1. **DSP e matemática de tempo em funções puras** — testáveis sem `AudioProcessor` nem GUI.
2. **IDs de parâmetros** num único sítio — verificados por testes na UI / APVTS (principalmente Fase 1 e fases com muitos parâmetros).
3. **Sem alocações no caminho de áudio** — revisão + testes de stress onde aplicável.
4. **CI** executa `FractalDelay_Tests` (ou nome equivalente) em cada PR.

## Framework

- **Catch2 v3** recomendado para lógica/DSP; **`juce::UnitTest`** opcional para integração JUCE.

## Alvo CMake

- Executável de testes que linka módulos DSP sem necessidade de carregar o binário VST completo, quando a arquitetura permitir.

## Cobertura por fase ClickUp

| Fase | Pasta | Foco dos testes |
|------|--------|-----------------|
| 1 | [`fase-1-motor-delay-basico/`](fase-1-motor-delay-basico/) | Processador, *buses*, *prepare/release*, buffer, BPM, Clean/Tape, estado APVTS. |
| 2 | [`fase-2-feedback-engine/`](fase-2-feedback-engine/) | Saturação no *loop*, filtros IIR, matriz estéreo, modulação de atraso / continuidade. |
| 3 | [`fase-3-modos-avancados/`](fase-3-modos-avancados/) | Cada modo: invariantes, estabilidade, *golden files* curtos onde útil. |
| 4 | [`fase-4-spectral-ducking/`](fase-4-spectral-ducking/) | Energia por banda, *ducking* causal, filas UI↔áudio. |
| 5 | [`fase-5-post-fx-spatial/`](fase-5-post-fx-spatial/) | EQ *wet*, dinâmica, *clipper*, imagem estéreo / mono de graves. |

## Ordem sugerida

1. Com a [Fase 1](fase-1-motor-delay-basico/01-configuracao-projeto-juce.md): alvo de testes + *smoke* + testes do processador.
2. Antes de fechar cada entrega numerada noutras pastas: cumprir a tabela de testes desse `.md`.
3. Manter a suíte **rápida** (meta orientadora: menos de 30 s no CI).

Referência de tarefas: [workspace ClickUp](https://app.clickup.com/90171225448).
