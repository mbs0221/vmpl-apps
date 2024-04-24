#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

void *print_message(void *message) {
    printf("%s\n", (char *)message);
    return NULL;
}

int main() {
    pthread_t thread1, thread2;
    const char *message1 = "Thread 1";
    const char *message2 = "Thread 2";

    if (pthread_create(&thread1, NULL, print_message, (void *)message1)) {
        fprintf(stderr, "Error creating thread 1\n");
        return 1;
    }

    if (pthread_create(&thread2, NULL, print_message, (void *)message2)) {
        fprintf(stderr, "Error creating thread 2\n");
        return 1;
    }

    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);

    return 0;
}