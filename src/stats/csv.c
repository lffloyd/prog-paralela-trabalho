#include <stdlib.h>
#include <stdio.h>
#include "csv.h"

/*
Escreve arquivo CSV a partir da tabela de tempos de execuca

Recebe:

   - table: tabela de tempos de execucao;
   - filename: nome do arquivo CSV a ser salvo

Retorna:

    - inteiro indicando se operacao falhou (1) ou ocorreu com sucesso (0)
*/
int write_csv(Table *table, char *filename) {
    FILE *fp;

    if ((fp = fopen(filename, "w")) == NULL)
    {
        printf("Cannot open file.\n");
        return 1;
    }

    fprintf(fp, "n,Tempo de Execução,\n");

    Table *ptr = table;
    while (ptr != NULL) {
        fprintf(fp, "%d,%g,\n", ptr->n, ptr->execution_time);
        ptr = ptr->next;
    }

    fclose(fp);

    return 0;
}

/*
Escreve arquivo CSV a partir da tabela de tempos de execucao da versao MPI do Prim

Recebe:

   - table: tabela de tempos de execucao da versao MPI;
   - filename: nome do arquivo CSV a ser salvo

Retorna:

    - inteiro indicando se operacao falhou (1) ou ocorreu com sucesso (0)
*/
int write_mpi_prim_csv(Table *table, char *filename) {
    FILE *fp;

    if ((fp = fopen(filename, "w")) == NULL)
    {
        printf("Cannot open file.\n");
        return 1;
    }

    fprintf(fp, "n,Tempo de Execução,Tempo de Comunicação,\n");

    Table *ptr = table;
    while (ptr != NULL) {
        fprintf(fp, "%d,%g,%g,\n", ptr->n, ptr->execution_time, ptr->communication_time);
        ptr = ptr->next;
    }

    fclose(fp);

    return 0;
}
