#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sched.h>
#include <sys/signal.h>

#include "bench.h"
#include <vmpl/dune.h>

#define THREAD_CORE 10
#define THREAD_2_CORE 20
#define TEST_VECTOR 0xf2

#define NUM_ITERATIONS 1000000

// number of bytes to send to the server.
#define SLEEP_TIME 3

volatile bool t2_ready = false;
volatile bool wait = true;
volatile bool done = false;

void sched_cpuset(int cpu)
{
    pid_t pid = getpid();
    cpu_set_t cpus;
    CPU_ZERO(&cpus);
    CPU_SET(cpu, &cpus);
    sched_setaffinity(pid, sizeof(cpu_set_t), &cpus);
    printf("posted_ipi: process %d running on core %d\n", pid, cpu);
}

static void test_handler(struct dune_tf *tf)
{
    dune_apic_eoi();
    wait = false;
}

void *t2_start(void *arg)
{
    sched_cpuset(THREAD_2_CORE);
    volatile int ret = dune_enter();
    if (ret) {
        printf("posted_ipi: failed to enter dune in thread 2\n");
        return NULL;
    }

    dune_apic_init_rt_entry();
    dune_register_intr_handler(TEST_VECTOR, test_handler);
    asm volatile("mfence" ::: "memory");
    printf("posted_ipi: thread 2 ready\n");
    t2_ready = true;
    while (!done);
    return NULL;
}

void *t1_start(void *arg)
{
    sched_cpuset(THREAD_CORE);
    volatile int ret = dune_enter();
    if (ret) {
        printf("posted_ipi: failed to enter dune in thread 1\n");
        return NULL;
    }

    dune_apic_init_rt_entry();
    asm volatile("mfence" ::: "memory");
    printf("posted_ipi: thread 1 ready\n");
    while (!t2_ready);
    printf("posted_ipi: about to send posted IPI\n");

    unsigned long rdtsc_overhead = measure_tsc_overhead();
    synch_tsc();
    unsigned long start_tick = rdtscll();

    for (int i = 0; i < NUM_ITERATIONS; i++) {
        dune_apic_send_ipi(TEST_VECTOR, dune_apic_id_for_cpu(THREAD_2_CORE, NULL));
        while (wait);
        wait = true;
    }

    unsigned long end_tick = rdtscllp();
    unsigned long latency = (end_tick - start_tick - rdtsc_overhead) / NUM_ITERATIONS;
    printf("Latency: %ld cycles.\n", latency);

    done = true;
    return NULL;
}

int main(int argc, char *argv[]) {
    int ret = EXIT_SUCCESS;

    ret = dune_init(true);
    if (ret) {
        printf("posted_ipi: failed to initialize dune\n");
        return ret;
    }

    pid_t server_pid = fork();
    if (server_pid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    } else if (server_pid == 0) {
        // Child process
        ret = t1_start(1);
    } else {
        // Parent process
        sleep(SLEEP_TIME); // Wait for server to start
        pid_t client_pid = fork();
        if (client_pid == -1) {
            perror("fork");
            kill(server_pid, SIGTERM);
            exit(EXIT_FAILURE);
        } else if (client_pid == 0) {
            // Child process
            ret = t2_start(1);
        } else {
            // Parent process
            int status;
            waitpid(client_pid, &status, 0);
            if (WIFEXITED(status)) {
                printf("Client exited with status %d\n", WEXITSTATUS(status));
            } else {
                printf("Client exited with signal %d\n", WTERMSIG(status));
            }
            waitpid(server_pid, &status, 0);
            if (WIFEXITED(status)) {
                printf("Server exited with status %d\n", WEXITSTATUS(status));
            } else {
                printf("Server exited with signal %d\n", WTERMSIG(status));
            }
        }
    }
    exit(ret);
}