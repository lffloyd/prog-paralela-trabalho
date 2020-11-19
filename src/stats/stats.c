#include <stdlib.h>
#include <limits.h>
#include "stats.h"

/*
Cria uma linha na tabela de tempos apenas com tempo de execucao

Recebe:

   - n: numero de linhas/colunas da matriz do cenario testado;
   - execution_time: tempo total médio de execucao do cenario testado;

Retorna:

    - linha da tabela preenchida com os valores passados
*/
Table *create_simple_line(int n, double execution_time)
{
    Table *line = (Table *)malloc(sizeof(Table));
    line->n = n;
    line->execution_time = execution_time;
    line->communication_time = (double)INT_MAX;
    line->next = NULL;
    return line;
}

/*
Cria uma linha na tabela de tempos apenas com tempos de execucao e comunicacao

Recebe:

   - n: numero de linhas/colunas da matriz do cenario testado;
   - execution_time: tempo total médio de execucao do cenario testado;
   - communication_time: tempo total médio de comunicacao do cenario testado

Retorna:

    - linha da tabela preenchida com os valores passados
*/
Table *create_line(int n, double execution_time, double communication_time)
{
    Table *line = (Table *)malloc(sizeof(Table));
    line->n = n;
    line->execution_time = execution_time;
    line->communication_time = communication_time;
    line->next = NULL;
    return line;
}

/*
Insere uma nova linha na tabela. Insercoes sao feitas ao fim da tabela

Recebe:

   - line: nova linha a ser inserida;
   - table: tabela a ser modificada

Retorna:

    - ponteiro para inicio da tabela atualizada
*/
Table *insert_line(Table *line, Table *table)
{
    if (table == NULL)
        return line;

    Table *ptr = table;
    while (ptr->next != NULL)
        ptr = ptr->next;
    ptr->next = line;
    return table;
}


/*
Libera tabela

Recebe:

   - table: tabela (ou linha) a ser liberada
*/
void free_table(Table *table)
{
    Table *ptr = table;
    while (ptr != NULL)
    {
        Table *temp = ptr->next;
        free(ptr);
        ptr = temp;
    }
}
