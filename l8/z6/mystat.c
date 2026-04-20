#include "system.h"
#include "cpu.h"

#include <errno.h>
#include <getopt.h>
#include <stdlib.h>

static unsigned long parse_ul(const char *s, const char *what) {
	char *end = NULL;
	errno = 0;
	unsigned long v = strtoul(s, &end, 10);
	if (errno != 0 || end == s || *end != '\0' || v == 0) {
		fprintf(stderr, "invalid %s: %s\n", what, s);
		exit(EXIT_FAILURE);
	}
	return v;
}


int main(int argc, char **argv) {
	unsigned long period = 1;
	unsigned long interval = 60;
	const char *logfile = "/var/log/mystat.log";

	static const struct option longopts[] = {
		{ "period",   required_argument, NULL, 'p' },
		{ "interval", required_argument, NULL, 'i' },
		{ "logfile",  required_argument, NULL, 'f' },
		{ "help",     no_argument,       NULL, 'h' },
		{ NULL, 0, NULL, 0 }
	};

	for (int opt; (opt = getopt_long(argc, argv, "p:i:f:h", longopts, NULL)) != -1;) {
		switch (opt) {
			case 'p':
				period = parse_ul(optarg, "period");
				break;
			case 'i':
				interval = parse_ul(optarg, "interval");
				break;
			case 'f':
				logfile = optarg;
				break;
			case 'h':
				printf("usage: %s [-p SEC] [-i SEC] [-f FILE]\n", argv[0]);
				return 0;
			default:
				return 1;
		}
	}

	install_handlers();

	FILE *log = fopen(logfile, "a");
	if (!log) {
		perror(logfile);
		return 1;
	}

	cpu_ticks_t prev, curr;
	if (read_cpu(&prev) != 0) {
		perror("/proc/stat");
		fclose(log);
		return 1;
	}

	stats_t stats;
	stats_reset(&stats);

	unsigned long elapsed = 0;

	while (!stopped()) {
		sleep_seconds(period);
		if (stopped()) break;

		if (read_cpu(&curr) == 0) {
			double busy = cpu_usage_percent(&prev, &curr);
			stats_add(&stats, busy);
			prev = curr;
			elapsed += period;
		}

		if (elapsed >= interval) {
			write_summary(log, &stats);
			stats_reset(&stats);
			elapsed = 0;
		}
	}

	if (stats.count > 0) {
		write_summary(log, &stats);
	}

	fclose(log);
	return 0;
}
