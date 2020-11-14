#include "stdlib.h"
#include "stats.h"
#include "limits.h"

Table *create_simple_line(int n, double execution_time)
{
    Table *line = (Table *)malloc(sizeof(Table));
    line->n = n;
    line->execution_time = execution_time;
    line->communication_time = (double)INT_MAX;
    line->next = NULL;
    return line;
}

Table *create_line(int n, double execution_time, double communication_time)
{
    Table *line = (Table *)malloc(sizeof(Table));
    line->n = n;
    line->execution_time = execution_time;
    line->communication_time = communication_time;
    line->next = NULL;
    return line;
}

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
