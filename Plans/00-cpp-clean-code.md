# Clean code em C++ — padrões do Fractal-Delay

Guia **prático** para código novo e revisões. Complementa [`.clang-format`](../.clang-format), [`.clang-tidy`](../.clang-tidy) e [`00-convencoes-repo-ui-testes.md`](00-convencoes-repo-ui-testes.md) (UI, filas, testes).

## Ferramentas como contrato

- **Formatação:** o estilo oficial é o do `.clang-format` (Microsoft + `NamespaceIndentation: All`, coluna 120). Não “inventar” outro estilo no mesmo ficheiro.
- **Análise estática:** antes de PR ou quando mexeres em ficheiros sensíveis, `clang-tidy -p build Source/...` com os *checks* já listados no `.clang-tidy` (init de membros, `nullptr`, *range-for* por referência, etc.).
- **clangd** com `compile_commands.json` no `build/` para erros e includes cedo.

## Nomes e legibilidade

- **Consistência com o código existente:** mesmo tom de nomes (JUCE usa *camelCase* em muitas APIs; seguir o que já está na pasta).
- **Nomes que dizem o quê / unidades:** `delayTimeMs` melhor que `t`; `gainLinear` vs `gainDb` explícitos para não misturar escalas.
- **Evitar abreviações opacas** excepto consensuais (`dsp`, `apvts`, `fifo`).

## Funções e ficheiros

- **Funções curtas** com um propósito claro; se o corpo precisa de muitos comentários “passo a passo”, extrair funções auxiliares (no `.cpp`, `namespace { ... }` quando forem só detalhe de implementação).
- **Cabeçalhos (.h):** API mínima; includes necessários; evitar includes pesados no header quando um *forward declaration* chega.
- **`.cpp`:** includes do próprio header em primeiro lugar; depois JUCE/STL; detalhes de implementação em namespace anónimo quando fizer sentido.

## `const`, tipos e conversões

- **`const` por defeito** em variáveis e referências que não mudam; métodos que não alteram estado → `const`.
- **Conversões float/double/int:** alinhar com o que JUCE pede (cuidado com **C4244** no MSVC); usar `f` em literais `float` quando a API é `float`.
- **Evitar `static_cast` a torto e a direito** — usar quando a intenção é realmente “conversão de tipo”, não para silenciar avisos sem corrigir a causa.

## Recursos, ownership e regra de zero

- **Preferir `std::unique_ptr`** (ou containers) a *ownership* manual com `new`/`delete`.
- **Regra de zero:** se não precisas de destructor especial, não escrevas copy/move à mão — deixa o compilador gerar ou remove operações com `= delete` de forma consciente.
- **Macros JUCE em `Component` membro por valor:** ver nota em [`00-convencoes-repo-ui-testes.md`](00-convencoes-repo-ui-testes.md) (`JUCE_DECLARE_NON_COPYABLE...` + MSVC).

## *Thread* de áudio (DSP)

- **Sem alocações** em `processBlock` (sem `vector::push_back` sem reserva, sem `std::string` a crescer, etc.).
- **Sem locks** que possam bloquear o *callback* do *host*; comunicação com UI → fila já definida no doc de convenções.
- **Sem I/O, logs em excesso ou UI** no *audio thread*.

## Erros e invariantes

- **Preferir estados válidos** a “valores mágicos” espalhados; constantes nomeadas (`namespace` anónimo no `.cpp` para constantes só da unidade de tradução).
- **`jassert` / asserções** onde uma pré-condição é realmente de programador (não substituir validação de entrada do utilizador).

## Comentários

- **Comentar o “porquê”**, não o óbvio que o código já diz.
- **TODO** com contexto (quem/quando) só se for deixado propositadamente; evitar ruído permanente.

## Testes (Catch2)

- **Um cenário principal por `TEST_CASE`** quando possível; nomes de teste que descrevem comportamento esperado.
- **Testes GUI / `[gui]`:** seguir [`00-testes-unitarios-visao-geral.md`](00-testes-unitarios-visao-geral.md) e o doc de convenções.

## O que este documento não substitui

- APIs concretas do JUCE: **confirmar no tag** fixo do projeto (`build/_deps/juce-src/`), não por memória.
- Decisões de produto ou roadmap: pastas `fase-*` nos planos.

## Referência cruzada

- [`README.md`](README.md) · [`00-convencoes-repo-ui-testes.md`](00-convencoes-repo-ui-testes.md) · [`00-testes-unitarios-visao-geral.md`](00-testes-unitarios-visao-geral.md)
