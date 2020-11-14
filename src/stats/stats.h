#ifndef STATS_H_
#define STATS_H_

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
