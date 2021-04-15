/*******************************************************************************
* Leonardo Mudrek de Almeida
* UTFPR - CT
*
* Descrição: Arquivo realiza a busca binária ou sequêncial dependendo do parâmetro
* de input da execução, ao qual também fornece o tamanho do buffer a ser realizada
* a busca. O buffer é preenchido com valores inteiros e sequenciais
*
* License : MIT
*******************************************************************************/

/*******************************************************************************
* INCLUDES
*******************************************************************************/
#include <stdio.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/*******************************************************************************
* DEFINES E ENUMS
*******************************************************************************/
/* Tamanho máximo do buffer de dados a serem analisados */
#define BUFFER_SIZE UINT32_MAX

/* Tamanho minimo de parâmetros a serem informados na execução do código */
#define NUM_MIN_INPUTS  3

typedef enum positionInputs_t {
  ARCHIVE_NAME_INPUT = 0x00,
  SEARCH_TYPE_INPUT  = 0x01,
  NUMBER_ELEMENTS_INPUT = 0x02,
  NUM_INPUTS,
} positionInputs_t;

/* Tipos de buscaas disponíveis */
typedef enum searchType_t
{
  BINARY_SEARCH = 0x00,
  SEQUENTIAL_SEARCH = 0x01,
  NULL_SEARCH = 0x02,
  NUM_SEARCH,
} searchType_t;

/*******************************************************************************
* TYPEDEFS
*******************************************************************************/
/* Callback utilizado para interagir com o algoritimo de busca */
typedef int64_t (*algorithmCallback_t)(uint32_t *bufferPtr, uint32_t size, uint32_t key);

/* Estrutura de uma busca */
typedef struct search_t
{
  /* Tipo da busca */
  searchType_t type;
  /* Callback de chamada do algoritimo */
  algorithmCallback_t callback;
  /* Chave de input da main.c */
  char key;
  /* Header do log para identificação */
  char *log;
  /* Tamanho da quantidade de dados */
  uint32_t size;
} search_t;

/*******************************************************************************
* PROTÓTIPOS DE FUNÇÕES
*******************************************************************************/
static void getSearchType(const uint32_t parametersQuantity, 
                          const char *argsPtr[], 
                          search_t * searchPtr);
static void writeValues(uint32_t *arrayPtr, uint32_t size);
static void benchmarking(search_t searchRequest, uint32_t *bufferPtr);
static int64_t sequenceSearch(uint32_t *bufferPtr, uint32_t size, uint32_t key);
static int64_t binarySearch(uint32_t *bufferPtr, uint32_t size, uint32_t key);

/*******************************************************************************
* CONSTANTES
*******************************************************************************/

/* Tabela de configuração de um algoritimo de busca */
static const search_t search[NUM_SEARCH] = {
    /* Busca binária e sua configuração */
    {
      .type = BINARY_SEARCH,
      .callback = binarySearch,
      .key = 'b',
      .log = "Binaria",
    },
    /* Busca sequencial e sua configuração */
    {
      .type = SEQUENTIAL_SEARCH,
      .callback = sequenceSearch,
      .key = 's',
      .log = "Sequencial"
    },
    /* Busca nula, fim da tabela */
    {
        .type = NULL_SEARCH,
        .callback = NULL,
    },
};

/*******************************************************************************
* VARIÁVEIS
*******************************************************************************/
/* Tamanho do buffer a ser analisado */
static uint32_t dataset[BUFFER_SIZE];

/*******************************************************************************
* FUNÇÕES EXPORTADAS
*******************************************************************************/

/**
 * Função main
 * 
 * @param parametersQuantity - quantidade parâmetros
 * @param args - argumentos incluídos na execução do arquivo
 */
void main(uint32_t parametersQuantity, const char *argsPtr[])
{
  search_t searchRequest;
  getSearchType(parametersQuantity, argsPtr, &searchRequest);

  if (searchRequest.type == NULL_SEARCH)
  {
    printf("Falha tipo de busca, terminando execução!\n");
    return;
  }

  /* Escreve valores sequenciais no buffer, de acordo com o tamanho informado */
  writeValues(dataset, searchRequest.size);

  /* Realiza execução do algotimo de busca */
  benchmarking(searchRequest, dataset);
}

/*******************************************************************************
* FUNÇÕES LOCAIS
*******************************************************************************/

/**
 * Get do tipo de pesquisa a ser realizada conforme input
 * 
 * @param parametersQuantity - quantidade de itens repassados
 * @param argsPtr - ponteiro para o buffer recebido
 * @param searchPtr - objeto a ser escrito
 */
static void getSearchType(const uint32_t parametersQuantity, 
                          const char *argsPtr[], 
                          search_t * searchPtr)
{

  *searchPtr = search[NULL_SEARCH];

  if (parametersQuantity < NUM_INPUTS)
  {
    return;
  }

  for (uint32_t index = 0UL; index < NUM_SEARCH; index++)
  {
    if (search[index].key == *argsPtr[SEARCH_TYPE_INPUT])
    {
      *searchPtr = search[index];

      /* Transforma string numérica em uint */
      searchPtr->size = strtoul(argsPtr[NUMBER_ELEMENTS_INPUT], NULL, 10);
      break;
    }
  }
}

/**
 * Escreve valores no array a ser analisado
 * A função não testa se o tamanho passado corresponde a quantidade alocada de
 * memoria alocada ao buffer
 * 
 * @param arrayPtr Ponteiro para o array a ser preenchido
 * @param size Tamanho da escrita
 */
static void writeValues(uint32_t *arrayPtr, uint32_t size)
{
  for (uint32_t index = 0UL; index < size; index++)
  {
    arrayPtr[index] = index;
  }
}

static void benchmarking(search_t searchRequest, uint32_t *bufferPtr)
{
  if (searchRequest.callback == NULL)
  {
    printf("Callback nulo, terminando execução!");
    return;
  }

  int64_t indexFound = (*searchRequest.callback)(bufferPtr, searchRequest.size, searchRequest.size - 2UL);

  if (indexFound == -1LL)
  {
    printf("%s - Falha na busca\n", searchRequest.log);
    return;
  }

  printf("%s - Valor encontrado = %u\n", searchRequest.log, bufferPtr[indexFound]);
}

/**
 * Implementação algorítimo de busca sequêncial
 * 
 * @param bufferPtr - ponteiro para buffer a ser procurado
 * @param size - tamanho da busca
 * @param key - item a ser encontrado
 * @return int64_t - Caso sucesso, retorna o inteiro positivo correspondente
 * caso contrário -1
 */
static int64_t sequenceSearch(uint32_t *bufferPtr, uint32_t size, uint32_t key)
{
  for (uint32_t index = 0UL; index < size; index++)
  {
    if (bufferPtr[index] == key)
    {
      /* Valor encontrado */
      return index;
    }
  }

  return -1LL;
}

/**
 * Implementação algorítimo de busca binária
 * 
 * @param bufferPtr - ponteiro para buffer a ser procurado
 * @param size - tamanho da busca
 * @param key - item a ser encontrado
 * @return int64_t - Caso sucesso, retorna o inteiro positivo correspondente
 * caso contrário -1
 */
static int64_t binarySearch(uint32_t *bufferPtr, uint32_t size, uint32_t key)
{
  uint32_t low = 0UL;
  uint32_t high = size - 1UL;

  while (low <= high)
  {
    uint32_t mid = (low + (high - 1UL)) / 2UL;

    if (bufferPtr[mid] == key)
    {
      /* Valor encontrado */
      return mid;
    }

    /* Testa exclusão limites inferior ou superior */
    low = bufferPtr[mid] < key ? low = mid + 1UL : mid - 1UL;
  }

  return -1LL;
}

/*******************************************************************************
* END OF FILE
*******************************************************************************/