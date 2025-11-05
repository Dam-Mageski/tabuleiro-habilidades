#include <stdio.h>

#define NBOARD 10
#define MASKN  5          // tamanho fixo das habilidades (ímpar recomendado)

// --- Utilidades de tabuleiro -------------------------------------------------
void zeraTabuleiro(int b[NBOARD][NBOARD]) {
  for (int i = 0; i < NBOARD; i++)
    for (int j = 0; j < NBOARD; j++)
      b[i][j] = 0; // água
}

// Coloca alguns navios 3 para visualização (exemplo simples, fixo)
void posicionaNaviosExemplo(int b[NBOARD][NBOARD]) {
  // Navio horizontal
  b[2][2] = b[2][3] = b[2][4] = 3;
  // Navio vertical
  b[6][7] = b[7][7] = b[8][7] = 3;
  // Ponto isolado
  b[4][0] = 3;
}

// Imprime o tabuleiro com 0/3/5 separados por espaço
void imprimeTabuleiro(int b[NBOARD][NBOARD], const char *titulo) {
  if (titulo) printf("\n== %s ==\n", titulo);
  for (int i = 0; i < NBOARD; i++) {
    for (int j = 0; j < NBOARD; j++) {
      printf("%d ", b[i][j]); // 0: água, 3: navio, 5: área afetada
    }
    printf("\n");
  }
}

// --- Construção dinâmica das máscaras ----------------------------------------
// CONE: topo na linha 0, abrindo para baixo (ponta no topo).
void constroiMascaraCone(int m[MASKN][MASKN]) {
  int mid = MASKN / 2;
  for (int i = 0; i < MASKN; i++) {
    for (int j = 0; j < MASKN; j++) {
      int raio = i; // abre 1 por linha para baixo
      m[i][j] = (j >= mid - raio && j <= mid + raio) ? 1 : 0;
    }
  }
}

// CRUZ: linha e coluna centrais = 1.
void constroiMascaraCruz(int m[MASKN][MASKN]) {
  int mid = MASKN / 2;
  for (int i = 0; i < MASKN; i++) {
    for (int j = 0; j < MASKN; j++) {
      m[i][j] = (i == mid || j == mid) ? 1 : 0;
    }
  }
}

// OCTAEDRO (losango): distância Manhattan ao centro <= mid.
void constroiMascaraOctaedro(int m[MASKN][MASKN]) {
  int mid = MASKN / 2;
  for (int i = 0; i < MASKN; i++) {
    for (int j = 0; j < MASKN; j++) {
      int di = (i > mid) ? (i - mid) : (mid - i);
      int dj = (j > mid) ? (j - mid) : (mid - j);
      m[i][j] = (di + dj <= mid) ? 1 : 0;
    }
  }
}

// --- Sobreposição no tabuleiro -----------------------------------------------
// Centralizada (para CRUZ e OCTAEDRO)
void sobrepoeCentralizado(int board[NBOARD][NBOARD],
                          int mask[MASKN][MASKN],
                          int origemLinha, int origemColuna,
                          int valorAfetado) {
  int mid = MASKN / 2;
  int iniL = origemLinha - mid;
  int iniC = origemColuna - mid;

  for (int i = 0; i < MASKN; i++) {
    for (int j = 0; j < MASKN; j++) {
      if (mask[i][j] == 0) continue;

      int L = iniL + i;
      int C = iniC + j;

      if (L < 0 || L >= NBOARD || C < 0 || C >= NBOARD) continue;
      if (board[L][C] == 3) continue; // preserva navio

      board[L][C] = valorAfetado;     // 5
    }
  }
}

// Topo alinhado (para CONE): origem = ponta do cone, centralizando colunas
void sobrepoeConeTopo(int board[NBOARD][NBOARD],
                      int mask[MASKN][MASKN],
                      int origemTopoLinha, int origemTopoColuna,
                      int valorAfetado) {
  int mid = MASKN / 2;
  int iniL = origemTopoLinha;           // topo encosta na origem
  int iniC = origemTopoColuna - mid;    // centraliza a coluna

  for (int i = 0; i < MASKN; i++) {
    for (int j = 0; j < MASKN; j++) {
      if (mask[i][j] == 0) continue;

      int L = iniL + i;
      int C = iniC + j;

      if (L < 0 || L >= NBOARD || C < 0 || C >= NBOARD) continue;
      if (board[L][C] == 3) continue;

      board[L][C] = valorAfetado;       // 5
    }
  }
}

int main(void) {
  int board[NBOARD][NBOARD];
  zeraTabuleiro(board);
  posicionaNaviosExemplo(board);

  // Construir máscaras 5x5
  int cone[MASKN][MASKN], cruz[MASKN][MASKN], octa[MASKN][MASKN];
  constroiMascaraCone(cone);
  constroiMascaraCruz(cruz);
  constroiMascaraOctaedro(octa);

  imprimeTabuleiro(board, "Tabuleiro inicial (0=agua, 3=navio)");

  // Origens fixas (permitido no enunciado)
  int origemConeL = 1, origemConeC = 5;  // topo do cone
  int origemCruzL = 4, origemCruzC = 2;  // centro
  int origemOctaL = 7, origemOctaC = 7;  // centro

  // Marcar afetados como 5
  sobrepoeConeTopo(board, cone, origemConeL, origemConeC, 5);
  sobrepoeCentralizado(board, cruz, origemCruzL, origemCruzC, 5);
  sobrepoeCentralizado(board, octa, origemOctaL, origemOctaC, 5);

  imprimeTabuleiro(board, "Tabuleiro com habilidades (5=area afetada)");

  return 0;
}
