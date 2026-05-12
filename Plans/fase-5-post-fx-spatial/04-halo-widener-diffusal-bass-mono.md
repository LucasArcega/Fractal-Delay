# Plano: Halo widener, diffusal e bass mono

> **ClickUp:** [Fase 5 — Post-FX e Spatial](https://app.clickup.com/90171225448/v/l/li/901713673882) · **Entrega:** 4/4 nesta pasta  
> **Tarefa:** [Halo widener, diffusal e bass mono](https://app.clickup.com/t/86e1b2ghg)  
> **Dependências:** cadeia de pós-FX anterior; cuidado com **mono compatibilidade** em sistemas club  

## Objetivo

- **Halo / widener:** M/S ou *allpass* + *crossfeed* de altos para ampliar imagem estéreo.
- **Diffusal:** rede de *allpass* curtos (*Schroeder*) para espalhar transientes.
- **Bass mono:** crossover (ex. 120 Hz) — graves somados a mono.

## Escopo

- Controles: largura, quantidade de *diffusion*, frequência de *bass mono*.
- *Solo* de *diffusion* opcional para *debug*.

## Testes unitários

| Teste | Descrição |
|--------|-----------|
| `BassMonoSum` | Abaixo de `fc`, L ≈ R (correlação → 1). |
| `WidthZeroIdentity` | Largura mínima → matriz próxima da identidade. |
| `EnergyConservationMid` | Sinal mono de entrada: energia total de saída não explode (limites). |

## Encerramento do roadmap v1

Com a Fase 5 concluída, o *Fractal-Delay* cobre motor, *feedback*, modos avançados, *ducking* espectral e pós-processamento espacial. Próximos passos típicos: presets, A/B, *installer*, QA em múltiplos *hosts*.
