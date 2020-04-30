#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#define SIZE 50

typedef struct {
  char filename[SIZE];
  int quantidadePalavras;
  char palavra[SIZE];
  char dica[SIZE];
  char chutes[SIZE];
  int palavrasEscolhidas[SIZE];
}
dados;
dados data;

FILE * p;
char tmp[60];
int numeroDeChutes = 0;
int numerosGerados = 0;
int numeroAleatorio;
int numeroDeDicas = 0;
int numeroDicas = 0;
int mostraDica = 0;

int chutesDados(char letra) {
  int achou = 0;
  for (int j = 0; j < numeroDeChutes; j++) {
    if (data.chutes[j] == letra) {
      achou = 1;
      break;
    }
  }
  return achou;
}

void dicas() {
  if (mostraDica == 1) {
    printf("A dica já está sendo exibida \n");
  }

  if (numeroDicas > 0 && mostraDica == 0) {
    printf("Você acabou de utilizar uma dica: -1\n");
    numeroDicas--;
    mostraDica = 1;
  } else {
    printf("Você não possui dicas para utilizar...\n");
  }
}

int temLetra(char letra) {
  if (letra == '0') {
    return 1;
  }

  for (int j = 0; j < strlen(data.palavra); j++) {
    if (letra == data.palavra[j]) {
      return 1;
    }
  }
  return 0;
}

int chutouErrado() {
  int vezesErrou = 0;
  for (int i = 0; i < numeroDeChutes; i++) {
    if (!temLetra(data.chutes[i])) {
      vezesErrou++;
    }
  }
  return vezesErrou;
}

void zerarPartida() {
  numeroDeChutes = 0;
  mostraDica = 0;
  for (int i = 0; i < strlen(data.chutes); i++) {
    data.chutes[i] = '\0';
  }
}

void printForca() {
  system("clear");

  int vezesErrou = chutouErrado();

  printf("  _______       \n");
  printf(" |/      |      \n");
  printf(" |      %c%c%c  \n", (vezesErrou >= 1 ? '(' : ' '), (vezesErrou >= 1 ? '_' : ' '), (vezesErrou >= 1 ? ')' : ' '));
  printf(" |      %c%c%c  \n", (vezesErrou >= 3 ? '\\' : ' '), (vezesErrou >= 2 ? '|' : ' '), (vezesErrou >= 3 ? '/' : ' '));
  printf(" |       %c     \n", (vezesErrou >= 2 ? '|' : ' '));
  printf(" |      %c %c   \n", (vezesErrou >= 4 ? '/' : ' '), (vezesErrou >= 4 ? '\\' : ' '));
  printf(" |              \n");
  printf("_|___           \n");
  printf("\n\n");
  printf("%d palavra de %d\n", numerosGerados, data.quantidadePalavras);
  printf("Você possui %d dica(s)\n", numeroDicas);
  if (mostraDica == 1) {
    printf("A dica é %s\n", data.dica);
  }
  for (int i = 0; i < strlen(data.palavra); i++) {
    if (chutesDados(data.palavra[i])) {
      printf("%c ", data.palavra[i]);
    } else {
      printf("_ ");
    }
  }
  printf("\n");
}

void chutarLetra() {
  char chute;
  printf("Para utilizar uma dica digite 0...\n");
  printf("Qual letra? ");
  scanf(" %c", & chute);
  if (chute == '0') {
    dicas();
  }
  chute = toupper(chute);

  if (temLetra(chute)) {
    printf("Você acertou: a palavra tem a letra %c\n\n", chute);
  } else {
    printf("\nVocê errou: a palavra NÃO tem a letra %c\n\n", chute);
  }
  data.chutes[numeroDeChutes] = chute;
  numeroDeChutes++;
}

void novaPalavra() {
  p = fopen(data.filename, "r+");

  printf("Digite a nova palavra: ");
  scanf("%[^\n]s", data.palavra);
  getchar();
  printf("Digite a dica para essa palavra: ");
  scanf("%[^\n]s", data.dica);
  getchar();

  for (int i = 0; i < strlen(data.palavra); i++) {
    data.palavra[i] = toupper(data.palavra[i]);
  }

  for (int i = 0; i < strlen(data.dica); i++) {
    data.dica[i] = toupper(data.dica[i]);
  }

  fseek(p, 0, SEEK_END);
  fprintf(p, "%s", data.palavra);
  fprintf(p, ";%s\n", data.dica);
  data.quantidadePalavras++;

  fclose(p);
}

void contarPalavras() {
  char c;
  p = fopen(data.filename, "r");
  data.quantidadePalavras = 0;

  while (!feof(p)) {
    c = fgetc(p);
    if (c == '\n' || c == '\0') {
      data.quantidadePalavras++;
    }
  }
  fclose(p);
}

int verificaNumero() {
  for (int i = 0; i < numerosGerados; i++) {
    if (data.palavrasEscolhidas[i] == numeroAleatorio) {
      return 0;
    }
  }
  return 1;
}

int geraNumero() {

  numeroAleatorio = rand() % data.quantidadePalavras;
  while (!verificaNumero()) {
    numeroAleatorio = rand() % data.quantidadePalavras;
  }
  data.palavrasEscolhidas[numerosGerados] = numeroAleatorio;
  numerosGerados++;
  return numeroAleatorio;
}

void escolhePalavra() {

  p = fopen(data.filename, "r");

  numeroAleatorio = geraNumero();

  for (int i = 0; i <= numeroAleatorio; i++) {
    fscanf(p, "%s", tmp);
  }

  strcpy(data.palavra, strtok(tmp, ";"));
  strcpy(data.dica, strtok(NULL, ";"));

  fclose(p);
}

int ganhou() {
  int acabou = 0;
  if (numerosGerados == data.quantidadePalavras) {
    acabou = 1;
  }
  return acabou;
}

int enforcou() {
  return chutouErrado() >= 5;
}

int acertouPalavra() {
  for (int i = 0; i < strlen(data.palavra); i++) {
    if (!chutesDados(data.palavra[i])) {
      return 0;
    }
  }
  numeroDicas++;
  return 1;
}

void menu() {
  system("clear");
  printf("\n Menu do jogo: \n");
  printf("[1] Novo Jogo \n");
  printf("[2] Cadastrar nova palavra \n");
  printf("[0] Sair do programa \n");
  printf("Qual a opção você deseja: ");
}

void criaArquivo() {
  p = fopen(data.filename, "w");

  char palavras[][50] = {
    "CARRO",
    "BANANA",
    "UNIVATES",
    "ACADEMIA",
    "CACHORRO"
  };
  char dicas[][50] = {
    "AUTOMOVEL",
    "FRUTA",
    "UNIVESIDADE",
    "LOCAL",
    "ANIMAL"
  };

  for (int i = 0; i < 5; i++) {
    fprintf(p, "%s", palavras[i]);
    fprintf(p, ";%s\n", dicas[i]);
  }
	data.quantidadePalavras = 5;
  fclose(p);
}

void verificaArquivo() {
  p = fopen(data.filename, "r");
  if (!p) {
    criaArquivo();
  } else if (p) {
    fclose(p);
    contarPalavras();
  }
}

int main(int argc, char * argv[]) {

  if (argc != 2) {
    strcpy(data.filename, "bd.csv");
  }
  if (argc == 2) {
    strcpy(data.filename, argv[1]);
  }

  int select = 1;
  verificaArquivo();

  while (select != 0) {
    menu();
    scanf("%d", &select);

    switch (select) {
    case 0:
      {
        printf("Você escolheu sair do jogo!\n");
        break;
      }

    case 1:
      {
        while (!ganhou() && !enforcou()) {
          escolhePalavra();
          while (!acertouPalavra() && !enforcou()) {
            printForca();
            chutarLetra();
          }
          printf("A palavra era: %s \n", data.palavra);
          if (enforcou()) {
            printf("Você perdeu, pressione enter para voltar ao menu inicial! \n");
            getchar();
            getchar();
            zerarPartida();
            numerosGerados = 0;
            numeroDicas = 0;
            break;
          }
          if (ganhou()) {
            printf("Parabéns, você acabou de acertar todas as palavras!\n");
            printf("Pressione ENTER para voltar ao menu inicial...\n");
            getchar();
            getchar();
            zerarPartida();
            numerosGerados = 0;
            numeroDicas = 0;
            break;
          }
          getchar();
          getchar();
          printf("Pressione ENTER para continuar...\n");
          zerarPartida();
        }
        break;
      }

    case 2:
      {
        getchar();
        novaPalavra();
        break;
      }

    default:
      {
        printf("Digite um comando válido! \n");
      }
    }
  }
  return 0;
}