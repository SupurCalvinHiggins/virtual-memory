#include <assert.h>
#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/types.h>

void* safe_mmap(void* addr, size_t len, int prot, int flags, int fd, off_t offset) {
    void* ret = mmap(addr, len, prot, flags, fd, offset);
    assert((ret != MAP_FAILED) && "mmap failed");
    return ret;
}

int main() {
    
    // Allocate 16GB of virtual memory.
    const size_t GB = 1024 * 1024 * 1024;
    uint8_t* data = (uint8_t*)safe_mmap(
        NULL, 
        16 * GB, 
        PROT_READ | PROT_WRITE, 
        MAP_PRIVATE | MAP_ANONYMOUS | MAP_NORESERVE, 
        -1, 
        0
    );

    // Read every page in the allocated memory.
    uint64_t sum = 0;
    for (size_t i = 0; i < 16 * GB; i += 4096) {
        sum += data[i];
    }

    // Display the sum.
    printf("sum = %lu\n", sum);
}