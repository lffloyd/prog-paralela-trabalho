#ifndef STATS_H_
#define STATS_H_

/*
Estrutura que define uma representacao de tabela ou linhas de uma tabela de tempos de experimentos.

Campos:

   - n: numero de linhas/colunas da matriz de adjancencias do cenario experimentado;
   - execution_time: tempo total de execucao no cenario. É uma média;
   - communication_time: tempo total de comunicacao, valido para MPI apenas. É uma média;
   - next: ponteiro para proxima linha na tabela, se existir
*/
typedef struct Table
{
    int n;
    double execution_time;
    double communication_time;
    struct Table *next;
} Table;

Table *create_simple_line(int n, double execution_time);
Table *create_line(int n, double execution_time, double communication_time);
Table *insert_line(Table *line, Table *table);
void free_table(Table *table);

#endif
