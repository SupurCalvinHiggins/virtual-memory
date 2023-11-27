#include <sys/mman.h>
#include <unistd.h>
#include <stdint.h>
#include <assert.h>
#include <stdio.h>

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
    *(uintptr_t*)NULL = (uintptr_t)&printf;
    (*((int (**)(const char*, ...))NULL))("Hello, World!\n");
}