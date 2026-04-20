#ifndef CPU
#define CPU

#include <stdio.h>

typedef struct {
	unsigned long long user, nice, system, idle, iowait, irq, softirq, steal;
} cpu_ticks_t;

typedef struct {
	double sum;
	double min;
	double max;
	unsigned long count;
} stats_t;

int read_cpu(cpu_ticks_t *t);
double cpu_usage_percent(const cpu_ticks_t *a, const cpu_ticks_t *b);

void stats_reset(stats_t *s);
void stats_add(stats_t *s, double v);

void write_summary(FILE *log, const stats_t *s);

#endif
