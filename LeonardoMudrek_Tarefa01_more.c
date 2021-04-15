/*******************************************************************************
* Leonardo Mudrek de Almeida
* UTFPR - CT
*
* Descrição: Programa recebe como input na linha de comando o nome do arquivo a
* ser aberto, estando no mesmo diretório e existir, sera exibido o conteúdo e 
* parando a cada 20 leituras de 256 caracteres do arquivo, caso contrário 
* finaliza execução.
*
* License : MIT
*******************************************************************************/

/*******************************************************************************
* INCLUDES
*******************************************************************************/
#include <stdio.h>
#include <stdbool.h>

/*******************************************************************************
* DEFINES E ENUMS
*******************************************************************************/
#define FILE_BUFFER_SIZE 256
#define LINE_COUNT_MAX   20

/*******************************************************************************
* TYPEDEFS
*******************************************************************************/

/*******************************************************************************
* CONSTANTES
*******************************************************************************/

/*******************************************************************************
* VARIÁVEIS
*******************************************************************************/

/*******************************************************************************
* PROTÓTIPOS DE FUNÇÕES
*******************************************************************************/
static void printArgs(int quantity, char * args[]);
static bool readFile(char * fileName);
static void waitForEnter(void);

/*******************************************************************************
* FUNÇÕES EXPORTADAS
*******************************************************************************/

/**
 * Função main
 * 
 * @param parametersQuantity - quantidade parâmetros
 * @param args - argumentos incluídos na execução do arquivo
 */
void main (int parametersQuantity, char * args[]) {

  /* O path do arquivo executado é sempre repassado, ou seja, caso tenha algum
  parâmetros informado na execução, sempre será a quantidade maior que um */
  if (parametersQuantity < 1) {
    printf("Não foram fornecidos parâmetros, finalizando execução!");
    return;
  }

  printArgs(parametersQuantity, args);
}

/*******************************************************************************
* FUNÇÕES LOCAIS
*******************************************************************************/

/**
 * Realiza impressão/leitura de um arquivo e suas linhas
 * 
 * @param quantity - quantidade parâmetros 
 * @param argsPtr - ponteiro para o array de argumentos
 */
static void printArgs(int quantity, char * argsPtr[]) {
  for (int idx = 1; idx < quantity; idx++) {
    bool result = readFile(argsPtr[idx]);

    if (result == false) {
      printf("Falha ao utilizar o parâmetro \"%s\"", argsPtr[idx]);
    }
  }
}

/**
 * Leitura de um arquivo e a impressão de suas linhas
 * 
 * @param fileName - nome/path do arquivo
 * @return true - leitura realizada com sucesso
 * @return false  - falha na leitura
 */
static bool readFile(char * fileName) {
  FILE * filePtr = fopen(fileName, "r");
  char line [FILE_BUFFER_SIZE];

  if (filePtr == NULL) {
    return false;
  }

  int count = 0;

  while (feof(filePtr) == false) {
    if (count == LINE_COUNT_MAX) {
      /* Chegou no máximo para leitura, aguarda ação usuário */
      waitForEnter();
      count = 0;
    }

    fgets(line, FILE_BUFFER_SIZE, filePtr);
    printf("%s", line);
    count ++;
  }

  fclose(filePtr);

  return true;
}

/**
 * Aguarda enter ser pressionado
 */
static void waitForEnter(void) {
  fflush(stdin);
  printf("Aguardando enter");  
  fflush(stdout);
  getchar();
}

/*******************************************************************************
* END OF FILE
*******************************************************************************/
