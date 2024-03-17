#include <stdio.h>
#include <stdint.h>

// Function to read the APIC ID
uint32_t read_apic_id() {
    uint32_t apic_id;
    
    // Read the APIC ID using the appropriate CPU instruction
    asm volatile("movl $1, %%eax\n\t"
                 "cpuid\n\t"
                 "movl %%ebx, %0\n\t"
                 : "=r" (apic_id)
                 :
                 : "%eax", "%ebx", "%ecx", "%edx");
    
    return apic_id;
}

int main() {
    // Read and print the APIC ID
    uint32_t apic_id = read_apic_id();
    printf("APIC ID: %u\n", apic_id);
    
    return 0;
}