# Plans — Fractal-Delay

Documentação de **implementação** alinhada às **fases do ClickUp** do workspace [90171225448](https://app.clickup.com/90171225448).

## Mapa de fases (produto)

| Fase ClickUp | Pasta local | Lista de tarefas |
|--------------|---------------|------------------|
| **1** — Motor de Delay Básico | [`fase-1-motor-delay-basico/`](fase-1-motor-delay-basico/) | [Abrir lista](https://app.clickup.com/90171225448/v/l/li/901713673870) |
| **2** — Feedback Engine | [`fase-2-feedback-engine/`](fase-2-feedback-engine/) | [Abrir lista](https://app.clickup.com/90171225448/v/l/li/901713673872) |
| **3** — Modos Avançados | [`fase-3-modos-avancados/`](fase-3-modos-avancados/) | [Abrir lista](https://app.clickup.com/90171225448/v/l/li/901713673875) |
| **4** — Spectral Ducking | [`fase-4-spectral-ducking/`](fase-4-spectral-ducking/) | [Abrir lista](https://app.clickup.com/90171225448/v/l/li/901713673880) |
| **5** — Post-FX e Spatial | [`fase-5-post-fx-spatial/`](fase-5-post-fx-spatial/) | [Abrir lista](https://app.clickup.com/90171225448/v/l/li/901713673882) |

## Documentos por pasta

### Fase 1 — Motor de Delay Básico

1. [`01-configuracao-projeto-juce.md`](fase-1-motor-delay-basico/01-configuracao-projeto-juce.md)
2. [`02-buffer-circular-delay.md`](fase-1-motor-delay-basico/02-buffer-circular-delay.md)
3. [`03-sincronizacao-bpm-host.md`](fase-1-motor-delay-basico/03-sincronizacao-bpm-host.md)
4. [`04-modos-clean-tape.md`](fase-1-motor-delay-basico/04-modos-clean-tape.md)
5. [`05-ui-parametros-motor-delay-basico.md`](fase-1-motor-delay-basico/05-ui-parametros-motor-delay-basico.md)

### Fase 2 — Feedback Engine

1. [`01-saturacao-soft-clipping-feedback.md`](fase-2-feedback-engine/01-saturacao-soft-clipping-feedback.md)
2. [`02-filtros-low-cut-high-cut-loop.md`](fase-2-feedback-engine/02-filtros-low-cut-high-cut-loop.md)
3. [`03-stereo-crossfeed-modulacao-delay.md`](fase-2-feedback-engine/03-stereo-crossfeed-modulacao-delay.md)

### Fase 3 — Modos Avançados

1. [`01-modo-bbd-bucket-brigade.md`](fase-3-modos-avancados/01-modo-bbd-bucket-brigade.md)
2. [`02-modo-lofi-bit-crush-sample-rate.md`](fase-3-modos-avancados/02-modo-lofi-bit-crush-sample-rate.md)
3. [`03-modo-reverse-crossfade.md`](fase-3-modos-avancados/03-modo-reverse-crossfade.md)
4. [`04-modo-granular-engine.md`](fase-3-modos-avancados/04-modo-granular-engine.md)
5. [`05-modos-fractal-bloom-shards.md`](fase-3-modos-avancados/05-modos-fractal-bloom-shards.md)

### Fase 4 — Spectral Ducking

1. [`01-filterbank-8-bandas-analise-espectral.md`](fase-4-spectral-ducking/01-filterbank-8-bandas-analise-espectral.md)
2. [`02-envelope-follower-banda-gain-computer.md`](fase-4-spectral-ducking/02-envelope-follower-banda-gain-computer.md)
3. [`03-visualizacao-spectral-ducking-tempo-real.md`](fase-4-spectral-ducking/03-visualizacao-spectral-ducking-tempo-real.md)

### Fase 5 — Post-FX e Spatial

1. [`01-eq-parametrico-8-bandas-wet.md`](fase-5-post-fx-spatial/01-eq-parametrico-8-bandas-wet.md)
2. [`02-tube-saturation-compressor.md`](fase-5-post-fx-spatial/02-tube-saturation-compressor.md)
3. [`03-clipper-soft-hard-chorus.md`](fase-5-post-fx-spatial/03-clipper-soft-hard-chorus.md)
4. [`04-halo-widener-diffusal-bass-mono.md`](fase-5-post-fx-spatial/04-halo-widener-diffusal-bass-mono.md)

## Testes em todas as fases

Ver [`00-testes-unitarios-visao-geral.md`](00-testes-unitarios-visao-geral.md).

## Convenções do repositório (UI, áudio↔UI, CI, Git)

Ver **[`00-convencoes-repo-ui-testes.md`](00-convencoes-repo-ui-testes.md)** — alinha implementação com o que já está no código (Grid/FlexBox, fila *lock-free*, testes GUI no Windows, `clang-format`, etc.).

## Clean code em C++

Ver **[`00-cpp-clean-code.md`](00-cpp-clean-code.md)** — padrões de C++ moderno, *audio thread*, includes e alinhamento com `clang-format` / `clang-tidy`.

## Skills (Cursor) — ClickUp e GitHub

Ver **[`.cursor/skills/clickup-github-ticket-workflow/SKILL.md`](../.cursor/skills/clickup-github-ticket-workflow/SKILL.md)** — branch por ticket, PR no GitHub, link na tarefa ClickUp, estados após merge (MCP `user-clickup` + `gh`).

## Ordem de execução recomendada

Respeitar a numeração **dentro** de cada pasta; as **fases ClickUp** seguem a ordem 1 → 5 (cada fase pode exigir *milestones* internos em paralelo quando indicado nos planos).
