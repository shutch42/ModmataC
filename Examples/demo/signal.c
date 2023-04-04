#include "signal.h"

static int running = 0;
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

void run() {
	pthread_mutex_lock(&lock);
	running = 1;
	pthread_mutex_unlock(&lock);
}

int is_running() {
	return running;
}

void stop() {
	pthread_mutex_lock(&lock);
	running = 0;
	pthread_mutex_unlock(&lock);
}
