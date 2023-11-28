#pragma once

#include <cstdint>
#include <cstring>
#include <cassert>
#include <unistd.h>
#include <sys/syscall.h>
#include <sys/mman.h>
#include <sys/types.h>

class MirroredCircularBuffer {

public:
    using size_type = size_t;

    const size_type capacity;

private:
    uint8_t* m_buffer;
    size_type m_size;
    size_type m_front;
    size_type m_back;

private:
    void* safe_mmap(void* addr, size_t len, int prot, int flags, int fd, off_t offset) {
        void* ret = mmap(addr, len, prot, flags, fd, offset);
        assert((ret != MAP_FAILED) && "mmap failed");
        return ret;
    }

    int safe_memfd_create(const char *name, unsigned int flags) {
        int ret = syscall(__NR_memfd_create, name, flags);
        assert((ret != -1) && "memfd_create failed");
        return ret;
    }

    void safe_ftruncate(int fd, off_t length) {
        int ret = ftruncate(fd, length);
        assert((ret != -1) && "ftruncate failed");
    }

public:
    MirroredCircularBuffer(size_type capacity) 
        : capacity(capacity), m_buffer(nullptr), m_size(0), m_front(0), m_back(0) {
        
        // Create a virtual file.
        int fd = safe_memfd_create("", 0);
        safe_ftruncate(fd, capacity);

        // Create and mirror a virtual buffer.
        m_buffer = (uint8_t*)safe_mmap(NULL, capacity * 2, PROT_NONE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
        safe_mmap(m_buffer, capacity, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_FIXED, fd, 0);
        safe_mmap(m_buffer + capacity, capacity, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_FIXED, fd, 0);
    }

    void push(uint8_t* data, size_type size) {
        assert(capacity - m_size >= size);

        std::memcpy(m_buffer + m_back, data, size);

        m_back += size;
        m_size += size;
    }

    void pop(uint8_t* data, size_type size) {
        assert(m_size >= size);

        std::memcpy(data, m_buffer + m_front, size);

        m_front += size;
        if (m_front > capacity) {
            m_back -= capacity;
            m_front -= capacity;
        }
        m_size -= size;
    }

    size_type size() const {
        return m_size;
    }
};