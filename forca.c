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
}dados;
dados data;

FILE * p;
char tmp[60];
int chutesdados = 0;
int numerosGerados = 0;
int numeroAleatorio;
int numeroDeDicas = 0;
int numeroDicas = 0;
int mostraDica = 0;

int jachutou(char letra) {
  int achou = 0;
  for (int j = 0; j < chutesdados; j++) {
    if (data.chutes[j] == letra) {
      achou = 1;
      break;
    }
  }
  return achou;
}

void dicas (){
	if(numeroDicas > 0){
		printf("Você acabou de utilizar uma dica: -1\n");
		numeroDicas--;
		mostraDica = 1;
	}
	else{
		printf("Você não possui dicas para utilizar...\n");
	}
}

int letraexiste(char letra) {
	if(letra == '0'){return 1;}

  for (int j = 0; j < strlen(data.palavra); j++) {
    if (letra == data.palavra[j]) {
      return 1;
    }
  }
  return 0;
}

int chuteserrados() {
  int erros = 0;
  for (int i = 0; i < chutesdados; i++) {
    if (!letraexiste(data.chutes[i])) {
      erros++;
    }
  }
  return erros;
}

void zerarPartida() {
  chutesdados = 0;
	mostraDica = 0;
  for (int i = 0; i < strlen(data.chutes); i++) {
    data.chutes[i] = '\0';
  }
}

void desenhaforca() {
  system("clear");

  int erros = chuteserrados();

  printf("  _______       \n");
  printf(" |/      |      \n");
  printf(" |      %c%c%c  \n", (erros >= 1 ? '(' : ' '), (erros >= 1 ? '_' : ' '), (erros >= 1 ? ')' : ' '));
  printf(" |      %c%c%c  \n", (erros >= 3 ? '\\' : ' '), (erros >= 2 ? '|' : ' '), (erros >= 3 ? '/' : ' '));
  printf(" |       %c     \n", (erros >= 2 ? '|' : ' '));
  printf(" |      %c %c   \n", (erros >= 4 ? '/' : ' '), (erros >= 4 ? '\\' : ' '));
  printf(" |              \n");
  printf("_|___           \n");
  printf("\n\n");
  printf("%d palavra de %d\n", numerosGerados, data.quantidadePalavras);
	printf("Você possui %d dica(s)\n", numeroDicas);
	if(mostraDica == 1){printf("A dica é %s\n", data.dica);}
  for (int i = 0; i < strlen(data.palavra); i++) {
    if (jachutou(data.palavra[i])) {
      printf("%c ", data.palavra[i]);
    } else {
      printf("_ ");
    }
  }
  printf("\n");
}

void chuta() {
  char chute;
	printf("Para utilizar uma dica digite 0...\n");
  printf("Qual letra? ");
  scanf(" %c", & chute);
	if(chute == '0'){
		dicas();
	}
  chute = toupper(chute);

  if (letraexiste(chute)) {
    printf("Você acertou: a palavra tem a letra %c\n\n", chute);
  } else {
    printf("\nVocê errou: a palavra NÃO tem a letra %c\n\n", chute);
  }
  data.chutes[chutesdados] = chute;
  chutesdados++;
}

void novaPalavra() {
  p = fopen(data.filename, "r+");

  printf("Digite a nova palavra: ");
  scanf("%[^\n]s", data.palavra);
  getchar();
  printf("Digite a dica para essa palavra: ");
  scanf("%[^\n]s", data.dica);
  getchar();

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

  if (numerosGerados == data.quantidadePalavras) {
    char temp;
    printf("Todas palavras foram usadas!\n");
    printf("Deseja adicionar uma nova palavra: S/N");
    getchar();
    scanf("%c", & temp);
    if (toupper(temp) == 'S') {
      novaPalavra();
    } else {
      exit(0);
    }
  }

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
  printf("%s\n", data.palavra);
  strcpy(data.dica, strtok(NULL, ";"));
  printf("%s\n", data.dica);

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
  return chuteserrados() >= 5;
}

int acertouPalavra() {
  for (int i = 0; i < strlen(data.palavra); i++) {
    if (!jachutou(data.palavra[i])) {
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

void criaArquivo (){
	p = fopen(data.filename, "w");

char palavras[][50] = {
    "CARRO",
    "BANANA",
    "UNIVATES",
    "ACADEMIA"
  };
  char dicas[][50] = {
    "AUTOMOVEL",
    "FRUTA",
    "UNIVESIDADE",
    "LOCAL"
  };

	for (int i = 0; i < 4; i++) {
    fprintf(p, "%s", palavras[i]);
    fprintf(p, ";%s\n", dicas[i]);
  }

	data.quantidadePalavras = 4;
	fclose(p);
}

void verificaArquivo(){
	p = fopen(data.filename, "r");
	if(!p){
		criaArquivo();
	}
	else if(p){
		fclose(p);
		contarPalavras();
	}
}

int main(int argc, char *argv[]) {
	
	if(argc != 2){
		strcpy(data.filename, "bd.csv");
	}
	if(argc == 2){
		strcpy(data.filename, argv[0]); 
	}
	
		
  int select = 1;
  verificaArquivo();

  while (select != 0) {
    menu();
    scanf("%d", & select);

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
            desenhaforca();
            chuta();
          }
          printf("A palavra era: %s \n", data.palavra);
          if (enforcou()) {
            printf("Você perdeu, pressione enter para voltar ao menu inicial! \n");
            getchar();
            getchar();
            zerarPartida();
            numerosGerados = 0;
            break;
          }
          if (ganhou()) {
            printf("Parabéns, você acabou de acertar todas as palavras!\n");
            printf("Pressione ENTER para voltar ao menu inicial...\n");
            getchar();
            getchar();
            numerosGerados = 0;
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