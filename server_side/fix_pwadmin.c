#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>

__attribute__((constructor))
void init_pwadmin_patch(void) {
    long page_size = sysconf(_SC_PAGESIZE);
    void *addr = (void *)0x080ab000;
    if (mprotect(addr, page_size, PROT_READ | PROT_WRITE | PROT_EXEC) == 0) {
        unsigned char *p = (unsigned char *)0x080ab851;
        if (p[0] == 0x74 && p[1] == 0x15) {
            p[0] = 0xeb; // unconditional jump to accept password
        }
    }
}
