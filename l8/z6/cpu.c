#include "cpu.h"

#include <string.h>
#include <time.h>

int read_cpu(cpu_ticks_t *t) {
	char line[512];
	char tag[8];

	FILE *fp = fopen("/proc/stat", "r");
	if (!fp) return -1;

	if (!fgets(line, sizeof(line), fp)) {
		fclose(fp);
		return -1;
	}
	fclose(fp);

	unsigned long long user, nice, system, idle, iowait, irq, softirq, steal;
	int n = sscanf(line, "%7s %llu %llu %llu %llu %llu %llu %llu %llu",
			tag, &t->user, &t->nice, &t->system, &t->idle, &t->iowait, &t->irq,
			&t->softirq, &t->steal);

	if (n != 9 || strcmp(tag, "cpu") != 0) return -1;

	return 0;
}

double cpu_usage_percent(const cpu_ticks_t *prev, const cpu_ticks_t *curr) {
	unsigned long long idle_prev = prev->idle + prev->iowait;
	unsigned long long idle_curr = curr->idle + curr->iowait;

	unsigned long long total_prev = prev->user + prev->nice + prev->system + prev->idle + prev->iowait + prev->irq + prev->softirq + prev->steal;
	unsigned long long total_curr = curr->user + curr->nice + curr->system + curr->idle + curr->iowait + curr->irq + curr->softirq + curr->steal;

	if (total_curr <= total_prev) return 0.0;

	double total_delta = total_curr - total_prev;
	double idle_delta = idle_curr - idle_prev;
	double busy = 100.0 * (total_delta - idle_delta) / total_delta;

	if (busy < 0.0) busy = 0.0;
	if (busy > 100.0) busy = 100.0;
	return busy;
}

void stats_reset(stats_t *s) {
	s->sum = 0.0;
	s->min = 1e9;
	s->max = 0.0;
	s->count = 0;
}

void stats_add(stats_t *s, double v) {
	s->sum += v;
	if (v < s->min) s->min = v;
	if (v > s->max) s->max = v;
	s->count++;
}

void write_summary(FILE *log, const stats_t *s) {
	if (s->count == 0) return;

	time_t now = time(NULL);
	struct tm tm;
	localtime_r(&now, &tm);

	char timestamp[32];
	strftime(timestamp, sizeof(timestamp), "%Y-%m-%dT%H:%M:%S", &tm);

	fprintf(log, "%s %.2f %.2f %.2f\n",
			timestamp,
			s->sum / (double)s->count,
			s->max,
			s->min);
	fflush(log);
}
