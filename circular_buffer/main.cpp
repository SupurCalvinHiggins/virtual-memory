#include <cassert>

#include <benchmark/benchmark.h>

#include "circular_buffer.hpp"
#include "mirrored_circular_buffer.hpp"

constexpr size_t page_size = 4096;
constexpr size_t buf_size = page_size;
constexpr size_t data_size = buf_size - 1;

static void BM_CircularBuffer(benchmark::State& state) {
    uint8_t data[buf_size];
    for (size_t i = 0; i < buf_size; ++i) {
        data[i] = i;
    }

    CircularBuffer buf(buf_size);
    for (auto _ : state) {
        for (int i = 0; i < state.range(0); ++i) {
            buf.push(data, data_size); 
            buf.pop(data, data_size);
        }   
    }

    state.SetComplexityN(state.range(0));
}

static void BM_MirroredCircularBuffer(benchmark::State& state) {
    uint8_t data[buf_size];
    for (size_t i = 0; i < buf_size; ++i) {
        data[i] = i;
    }

    MirroredCircularBuffer buf(buf_size);
    for (auto _ : state) {
        for (int i = 0; i < state.range(0); ++i) {
            buf.push(data, data_size); 
            buf.pop(data, data_size);
        }
    }

    state.SetComplexityN(state.range(0));
}

BENCHMARK(BM_CircularBuffer)->Range(8, 8 << 12)->Complexity();
BENCHMARK(BM_MirroredCircularBuffer)->Range(8, 8 << 12)->Complexity();

BENCHMARK_MAIN();
