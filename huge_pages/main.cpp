#include <cassert>
#include <cstdlib>
#include <random>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/types.h>

#include <benchmark/benchmark.h>

constexpr size_t buf_size = (1024 * 4096) / sizeof(uint64_t);

static void BM_Malloc(benchmark::State& state) {
    uint64_t* data = (uint64_t*)malloc(buf_size * sizeof(uint64_t));
    for (size_t i = 0; i < buf_size; ++i) {
        data[i] = i;
    }

    for (auto _ : state) {
        for (size_t i = 0; i < buf_size; ++i) {
		benchmark::DoNotOptimize(data[i]);
	}
    }
}

static void BM_HugePage(benchmark::State& state) {
    uint64_t* data = (uint64_t*)mmap(
        NULL,
        buf_size * sizeof(uint64_t),
        PROT_READ | PROT_WRITE,
        MAP_PRIVATE | MAP_ANONYMOUS | MAP_HUGETLB | (21 << MAP_HUGE_SHIFT),
        -1,
        0
    );

    if (data == MAP_FAILED) {
        perror("mmap");
        fprintf(stderr, "Error: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    for (size_t i = 0; i < buf_size; ++i) {
        data[i] = i;
    }

    for (auto _ : state) {
        for (size_t i = 0; i < buf_size; ++i) {
		benchmark::DoNotOptimize(data[i]);
	}
    }
}

BENCHMARK(BM_Malloc);
BENCHMARK(BM_HugePage);

BENCHMARK_MAIN();