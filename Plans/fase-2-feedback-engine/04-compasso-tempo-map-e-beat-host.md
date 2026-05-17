# Plano: Compasso, tempo map e “beat” do host (extensão do sync)

> **ClickUp:** [Fase 2 — Feedback Engine](https://app.clickup.com/90171225448/v/l/li/901713673872) · **Entrega:** 4/4 nesta pasta  
> **Tarefa:** (criar no ClickUp quando priorizar — *time signature*, *tempo map*, *beat* em 6/8 / compasso variável)  
> **Dependências:** [`03-sincronizacao-bpm-host.md`](../fase-1-motor-delay-basico/03-sincronizacao-bpm-host.md) implementado (BPM + tabela em semínimas + fallback ms); laço de *feedback* da Fase 2 pode exigir o **mesmo contrato de tempo** para modulação / *crossfeed* — alinhar com [`03-stereo-crossfeed-modulacao-delay.md`](03-stereo-crossfeed-modulacao-delay.md) se o delay for modulado em unidades musicais.

## Objetivo

Fechar a lacuna deixada na Fase 1: o sync atual assume **BPM + durações em unidades de semínima** (quarter note), **sem** `time signature` nem **tempo map**. Este documento define como evoluir para:

- **Compassos variados** (3/4, 5/4, 7/8, 6/8, etc.) com comportamento **previsível** e documentado face ao que o host expõe em `AudioPlayHead::PositionInfo`.
- **Opcional:** *delay* ou modulação expressos em **compassos** (ex.: “1 compasso”) ou alinhados a **fronteira de compasso** (PPQ), quando o host fornecer dados suficientes.
- **Tempo map** (mudanças de BPM ao longo do tempo): política por bloco (valor no início do *callback* vs interpolação).

## Contexto (Fase 1 — o que já existe)

- [`DelayTempoMath.h`](../../Source/DSP/DelayTempoMath.h) + `PluginProcessor`: `getBpm()` + tabela de figuras em **batidas = semínimas**; **sem** `getTimeSignature()` nem `ppqPosition` para duração de compasso.
- Isto coincide com muitos DAWs em **4/4** (BPM = semínimas por minuto), mas **não garante** alinhamento “por compasso” nem semântica de **beat** em **6/8 composto** ou mudanças de numerador/denominador.

## Escopo (Fase 2)

1. **Leitura de `PositionInfo`** além de BPM:
   - `getTimeSignature()` → numerador / denominador quando existirem.
   - Opcional: `getPpqPosition()`, `getPpqPositionOfLastBarStart()`, `getBarCount()` — para duração de **um compasso** em unidades PPQ (quando o host preencher).
2. **Camada DSP** (extensão ou módulo novo em `Source/DSP/`, ex. `DelayTempoContext.h/.cpp`):
   - Estrutura imutável por bloco: `{ bpm, timeSig, ppqAtBlockStart?, flags }` normalizada a partir do `Optional` do JUCE.
   - Funções puras: `quarterNoteDurationSeconds(bpm)`, `beatsPerBar(timeSig)` (definição explícita da convenção escolhida pelo produto), `barDurationSeconds(...)`, etc.
3. **Política documentada** para:
   - **6/8** e similares: se o “beat” do host for colcheia vs semínima ponteadada — tabela de figuras pode continuar em **semínimas** OU passar a seguir o **beat do transporte** (decisão de produto; testar em Reaper, Ableton, Studio One, etc.).
   - **BPM inválido** ou campos em falta: manter fallback atual (`delayMs`) da Fase 1.
4. **Testes:** matriz pequena de *time signatures* com BPM fixo (valores esperados em amostras ou segundos); testes sem JUCE onde possível + *golden* com `DummyPlayHead` se necessário.

## Fora de escopo (nesta entrega)

- Sample-accurate automation fina do tempo (continua fora; ver Fase 1 doc `03`).
- UI completa dos novos modos (pode ficar para [`05-ui-parametros-motor-delay-basico.md`](../fase-1-motor-delay-basico/05-ui-parametros-motor-delay-basico.md) ou extensão do editor na Fase 2).

## Passos sugeridos

1. Inventariar o que **cada host** preenche em `getTimeSignature()` / PPQ (tabela interna “Reaper / …”).
2. Definir enum de **modo de interpretação**: `QuarterNoteGrid` (atual) vs `TransportBeat` vs `BarRelative` (futuro).
3. Implementar construtor de `DelayTempoContext` a partir de `AudioPlayHead*` (só `processBlock`).
4. Integrar em `resolveDelaySamples` ou função irmã (evitar *spaghetti* no `PluginProcessor`).
5. Atualizar documentação do [`03`](../fase-1-motor-delay-basico/03-sincronizacao-bpm-host.md) com uma linha “comportamento por modo”.

## Critérios de aceite

- Em **4/4** e **3/4**, com BPM fixo, durações de figuras coincidem com **referência manual** (calculadora ou DAW) dentro de tolerância acordada.
- Mudança de **numerador** (ex. 4/4 → 7/4) altera “1 compasso” em segundos de forma documentada (se modo *bar* existir).
- **Sem NaN** com `Optional` vazio ou transição de compasso; fallback **ms** preservado.
- **Build** + testes `~[gui]` verdes.

## Testes unitários (exemplos)

| Teste | Descrição |
|--------|------------|
| `TimeSigBeatsPerBar` | 4/4 → 4 semínimas por compasso na convenção escolhida; 6/8 → documentar resultado esperado. |
| `BarDurationSeconds` | BPM 120, 4/4 → 2 s por compasso (se *bar* = 4 quartos). |
| `MissingTimeSignatureFallback` | Sem `timeSignature` no host → comportamento igual ao modo **QuarterNoteGrid** da Fase 1. |

## Riscos

| Risco | Mitigação |
|-------|-----------|
| Hosts não expõem PPQ / último início de compasso | Degradar para modo semínima-only; não prometer “1 bar” sem dados. |
| Pro Tools e campos opcionais | Documentar na tabela de hosts; testes manuais. |
| Dupla definição de “beat” em 6/8 | Escolher uma convenção no produto e expor na UI (“Sync: grid de semínima” vs “Sync: beat do transporte”). |

---

**Referência transversal:** [`00-convencoes-repo-ui-testes.md`](../00-convencoes-repo-ui-testes.md) — DSP em `Source/DSP/`, *audio thread* sem alocações.

## Próximo foco na Fase 2

Após fechar contrato de tempo: alinhar **modulação de delay** / *crossfeed* [`03-stereo-crossfeed-modulacao-delay.md`](03-stereo-crossfeed-modulacao-delay.md) para consumir o mesmo `DelayTempoContext` (evitar dois cálculos de tempo divergentes).
