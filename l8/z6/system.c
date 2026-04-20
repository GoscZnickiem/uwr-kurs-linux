#include "system.h"

#include <errno.h>
#include <signal.h>
#include <string.h>
#include <time.h>

static volatile sig_atomic_t stop = 0;

static void on_signal(int sig) {
	stop = 1;
}

void install_handlers(void) {
	struct sigaction sa;
	memset(&sa, 0, sizeof(sa));
	sa.sa_handler = on_signal;
	sigemptyset(&sa.sa_mask);

	sigaction(SIGINT, &sa, NULL);
	sigaction(SIGTERM, &sa, NULL);
}

void sleep_seconds(unsigned long sec) {
	struct timespec req = { .tv_sec = (time_t)sec, .tv_nsec = 0 };
	while (nanosleep(&req, &req) == -1 && errno == EINTR && !stop);
}

int stopped() {
	return stop;
}
