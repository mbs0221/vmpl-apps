#include <stdio.h>

unsigned int get_cpu_id() {
    unsigned int cpu_id;
    asm volatile (
        "mov $1, %%eax\n\t"   // CPUID 功能号 1
        "cpuid\n\t"
        "mov %%ebx, %0\n\t"   // 将 ebx 寄存器中的值存入 cpu_id 变量
        : "=r" (cpu_id)        // 输出：cpu_id 变量
        :                      // 输入：无
        : "%eax", "%ecx", "%edx", "memory"  // 临时寄存器和内存
    );
    return (cpu_id >> 24) & 0xFF;  // 获取高 8 位，即 CPU 编号
}

unsigned int cpuid_vendor(char *vendor) {
    unsigned int eax, ebx, ecx, edx;
    asm volatile (
        "cpuid\n\t"
        : "=a" (eax), "=b" (ebx), "=c" (ecx), "=d" (edx)
        : "a" (0)
    );
    vendor[0] = ebx & 0xFF;
    vendor[1] = (ebx >> 8) & 0xFF;
    vendor[2] = (ebx >> 16) & 0xFF;
    vendor[3] = (ebx >> 24) & 0xFF;
    vendor[4] = edx & 0xFF;
    vendor[5] = (edx >> 8) & 0xFF;
    vendor[6] = (edx >> 16) & 0xFF;
    vendor[7] = (edx >> 24) & 0xFF;
    vendor[8] = ecx & 0xFF;
    vendor[9] = (ecx >> 8) & 0xFF;
    vendor[10] = (ecx >> 16) & 0xFF;
    vendor[11] = (ecx >> 24) & 0xFF;
    vendor[12] = '\0';
    return eax;
}

int main() {
    unsigned int cpu_id = get_cpu_id();
    printf("Current CPU ID: %u\n", cpu_id);
    char vendor[13];
    unsigned int eax = cpuid_vendor(vendor);
    if (eax == 0) {
        printf("CPUID not supported\n");
        return 1;
    }
    printf("CPU Vendor: %s\n", vendor);
    return 0;
}
