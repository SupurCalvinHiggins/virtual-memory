#pragma once

#include <cstdint>
#include <cstring>
#include <cassert>
#include <algorithm>

class CircularBuffer {

public:
    using size_type = size_t;

    const size_type capacity;

private:
    uint8_t* m_buffer;
    size_type m_size;
    size_type m_front;
    size_type m_back;

public:

    CircularBuffer(size_type capacity) : 
        capacity(capacity), m_buffer(new uint8_t[capacity]), m_size(0), m_front(0), m_back(0) { }

    void push(uint8_t* data, size_type size) {
        assert(capacity - m_size >= size);

        const size_type back_size = std::min(size, capacity - m_back);
        const size_type front_size = size - back_size;

        std::memcpy(m_buffer + m_back, data, back_size);
        std::memcpy(m_buffer, data + back_size, front_size);

        m_back += size;
        if (m_back > capacity) {
            m_back -= capacity;
        }
        m_size += size;
    }

    void pop(uint8_t* data, size_type size) {
        assert(m_size >= size);

        const size_type front_size = std::min(size, capacity - m_front);
        const size_type back_size = size - front_size;

        std::memcpy(data, m_buffer + m_front, front_size);
        std::memcpy(data + front_size, m_buffer, back_size);

        m_front += size;
        if (m_front > capacity) {
            m_front -= capacity;
        }
        m_size -= size;
    }

    size_type size() const {
        return m_size;
    }
};