# Plano: Visualização em tempo real do spectral ducking

> **ClickUp:** [Fase 4 — Spectral Ducking](https://app.clickup.com/90171225448/v/l/li/901713673880) · **Entrega:** 3/3 nesta pasta  
> **Tarefa:** [Visualização em tempo real do spectral ducking](https://app.clickup.com/t/86e1b2g7r)  
> **Dependências:** [`01`](01-filterbank-8-bandas-analise-espectral.md) + [`02`](02-envelope-follower-banda-gain-computer.md)  

## Objetivo

Mostrar ao utilizador **energia por banda** e/ou **redução de ganho aplicada**, atualizado em tempo quase real com custo de CPU controlado.

## Escopo

- Canal **áudio → UI**: `Atomic` / *lock-free FIFO* com *throttling* (ex. 30 Hz UI).
- Componente JUCE: `Timer` + `drawRect` / path por barra; cores para *threshold*.
- Modo *debug*: exportar valores numéricos.
- **Modularização:** amostragem / fila para UI conforme [`00-convencoes-repo-ui-testes.md`](../00-convencoes-repo-ui-testes.md); *widgets* de barras em **`Source/Components/`**; núcleo de agregação de dados que não seja *paint* em **`Source/DSP/`** ou no processador como orquestração fina — evitar lógica de análise pesada dentro de `Component::paint`. Ver o mesmo doc (Organização do código).

## Fora de escopo

- *OpenGL* pesado (só se necessário para performance).

## Testes

- **Unitário:** fila de amostras de níveis → componente agrega sem *crash* (teste de *headless* com *dummy* lookandfeel se viável).
- **Manual:** verificar *frame rate* com buffer 512 @ 48 kHz.

---

**Referência transversal:** convenções de repo, UI, filas áudio↔UI e testes JUCE — [`00-convencoes-repo-ui-testes.md`](../00-convencoes-repo-ui-testes.md). Para visualização em tempo real, alinhar com o padrão de fila + *throttling* já usado no plugin.

## Próxima fase ClickUp

[Fase 5 — Post-FX e Spatial](../fase-5-post-fx-spatial/)
