#include <stdio.h>
#include <sched.h>
#include <vmpl/vmpl.h>

#define TEST_VECTOR 0xF2

static void self_ipi_handler(struct dune_tf *tf)
{
    printf("ipi_handler_self: received IPI on core %d\n", sched_getcpu());
    apic_eoi();
    printf("ipi_handler_self: EOI sent\n");
}

int main(int argc, char *argv[])
{
    VMPL_ENTER;
    printf("Hello from main thread\n");

    // APIC init for VMPL mode
    apic_init_rt_entry();

    // register IPI handler
    dune_register_intr_handler(TEST_VECTOR, self_ipi_handler);

    // Send IPI to self
    apic_send_ipi(TEST_VECTOR, apic_get_id());

    printf("Sent IPI to self\n");

    return 0;
}
