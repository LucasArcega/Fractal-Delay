# Plano: Modo Reverse — reprodução reversa com crossfade

> **ClickUp:** [Fase 3 — Modos Avançados](https://app.clickup.com/90171225448/v/l/li/901713673875) · **Entrega:** 3/5 nesta pasta  
> **Tarefa:** [Modo Reverse — playback reverso com crossfade](https://app.clickup.com/t/86e1b2fz4)  
> **Dependências:** buffer com capacidade de leitura não sequencial ou segmentos reversos sobre janelas  

## Objetivo

Janelas de áudio reproduzidas **ao contrário**, com **crossfades** entre segmentos para evitar cliques nas fronteiras.

## Escopo

- Política de disparo: contínua vs. sincronizada ao transporte.
- Tamanho de janela e *crossfade* (cosine / equal-power).
- Interação com *feedback*: definir se o *loop* reverso alimenta o mesmo buffer ou buffer secundário.
- **Modularização:** motor *reverse* em **`Source/DSP/`** + `target_sources`. Ver [`00-convencoes-repo-ui-testes.md`](../00-convencoes-repo-ui-testes.md).

## Testes unitários

| Teste | Descrição |
|--------|-----------|
| `WindowEnergyContinuous` | Sobreposição de janelas conserva energia aproximada (sinal constante). |
| `NoDiscontinuityAtJoin` | Derivada primeira limitada nas junções (opcional, aproximada). |
| `ReverseReadOrder` | Índices de leitura monótonos decrescentes dentro da janela. |

---

**Referência transversal:** convenções de repo, UI, filas áudio↔UI e testes JUCE — [`00-convencoes-repo-ui-testes.md`](../00-convencoes-repo-ui-testes.md).

## Próximo

[`04-modo-granular-engine.md`](04-modo-granular-engine.md)
