## Desafio – Habilidades no Tabuleiro (Matrizes)
- Tabuleiro 10x10: `0 = água`, `3 = navio`, `5 = área afetada`.
- Habilidades (máscaras 5x5) geradas dinamicamente:
  - **Cone** (topo no ponto de origem, abrindo para baixo)
  - **Cruz** (linha e coluna centrais)
  - **Octaedro** (losango por distância Manhattan)
- Sobreposição respeita limites do tabuleiro e **não sobrescreve navio (3)**.

# tabuleiro-habilidades
