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

void map_null(void) {
    const int page_size = getpagesize();
    safe_mmap(
        NULL, 
        page_size,
        PROT_EXEC | PROT_READ | PROT_WRITE,
        MAP_PRIVATE | MAP_ANONYMOUS | MAP_FIXED, 
        -1,
        0
    );
}

int main() {
    map_null();

    // Write to NULL.
    *(uint64_t*)NULL = 42;
    printf("*NULL = %lu\n", *(uint64_t*)NULL);

    // Make NULL into printf.
    *(uintptr_t*)NULL = (uintptr_t)&printf;
    (*((int (**)(const char*, ...))NULL))("Hello, World!\n");
}