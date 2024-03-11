#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sched.h>

#include "bench.h"
#include <vmpl/dune.h>

#define THREAD_CORE 10
#define THREAD_2_CORE 20
#define TEST_VECTOR 0xf2

#define NUM_ITERATIONS 1000000

volatile bool t2_ready = false;
volatile bool wait = true;
volatile bool done = false;

void sched_cpuset(int cpu)
{
    cpu_set_t cpus;
    CPU_ZERO(&cpus);
    CPU_SET(cpu, &cpus);
    pthread_setaffinity_np(pthread_self(), sizeof(cpu_set_t), &cpus);
    pid_t tid = gettid();
    printf("posted_ipi: thread %d running on core %d\n", tid, cpu);
}

static void test_handler(struct dune_tf *tf)
{
    printf("posted_ipi: received IPI on core %d\n", sched_getcpu());
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

int main(int argc, char *argv[])
{
    volatile int ret;
    pthread_t t1, t2;
    pthread_attr_t attr;

    printf("posted_ipi: not running dune yet\n");

    ret = dune_init(true);
    if (ret) {
        printf("failed to initialize dune\n");
        return ret;
    }
    printf("posted_ipi: now printing from dune mode\n");

    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
    pthread_create(&t2, &attr, t2_start, NULL);
    pthread_create(&t1, &attr, t1_start, NULL);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    return 0;
}
