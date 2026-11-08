/*
 * Copyright (c) 2026, Niklas Hauser
 * SPDX-License-Identifier: Apache-2.0
 *
 * Stand-alone replacement for the Zephyr headers used by lib/heap/heap.c and
 * include/zephyr/sys/sys_heap.h. update.py replaces all <zephyr/...> includes
 * with this file. Every CONFIG_ option can be overridden by defining it first.
 */
#ifndef ZEPHYR_HEAP_PORT_H
#define ZEPHYR_HEAP_PORT_H

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <errno.h>

/* Kconfig options of lib/heap with their Zephyr defaults */
#ifndef CONFIG_SYS_HEAP_ALLOC_LOOPS
#define CONFIG_SYS_HEAP_ALLOC_LOOPS 3
#endif
#ifndef CONFIG_SYS_HEAP_HARDENING_LEVEL
#define CONFIG_SYS_HEAP_HARDENING_LEVEL 1
#endif
#ifndef CONFIG_SYS_HEAP_LOG_LEVEL
#define CONFIG_SYS_HEAP_LOG_LEVEL 0
#endif

/* sys/util.h */
#ifndef BIT
#define BIT(n) (1UL << (n))
#endif
#ifndef BIT_MASK
#define BIT_MASK(n) (BIT(n) - 1UL)
#endif
#ifndef ROUND_UP
#define ROUND_UP(x, a) ((((uintptr_t)(x) + ((uintptr_t)(a) - 1)) / (uintptr_t)(a)) * (uintptr_t)(a))
#endif
#ifndef ROUND_DOWN
#define ROUND_DOWN(x, a) (((uintptr_t)(x) / (uintptr_t)(a)) * (uintptr_t)(a))
#endif
#ifndef ARG_UNUSED
#define ARG_UNUSED(x) (void)(x)
#endif

/* IS_ENABLED(CONFIG_X) is 1 if CONFIG_X is defined to 1, else 0 */
#define Z_IS_ENABLED_1 0,
#define Z_IS_ENABLED_TAKE2(a, b, ...) b
#define Z_IS_ENABLED_HELPER(x) Z_IS_ENABLED_TAKE2(x 1, 0, 0)
#define Z_IS_ENABLED_PASTE(x) Z_IS_ENABLED_HELPER(Z_IS_ENABLED_##x)
#define IS_ENABLED(x) Z_IS_ENABLED_PASTE(x)
/* None of the optional features (stats, listeners, sanitizers) are supported */
#define IF_ENABLED(cfg, code)

/* sys/minmax.h, not in C++ where they clash with std::min/max */
#ifndef __cplusplus
#ifndef min
#define min(a, b) (((a) < (b)) ? (a) : (b))
#endif
#ifndef max
#define max(a, b) (((a) > (b)) ? (a) : (b))
#endif
#endif

/* toolchain.h */
#ifndef ALWAYS_INLINE
#define ALWAYS_INLINE inline __attribute__((always_inline))
#endif
#ifndef __aligned
#define __aligned(x) __attribute__((aligned(x)))
#endif
#ifndef __syscall
#define __syscall
#endif
#ifndef __deprecated
#define __deprecated __attribute__((deprecated))
#endif
#ifndef likely
#define likely(x) __builtin_expect(!!(x), 1)
#endif
#ifndef unlikely
#define unlikely(x) __builtin_expect(!!(x), 0)
#endif

/* sys/__assert.h and logging/log.h: no output, define __ASSERT to check */
#ifndef __ASSERT
#define __ASSERT(x, ...) do { } while (0)
#endif
#ifndef __ASSERT_NO_MSG
#define __ASSERT_NO_MSG(x) __ASSERT(x, "")
#endif
#define LOG_MODULE_REGISTER(...)
#ifndef LOG_ERR
#define LOG_ERR(...) do { } while (0)
#endif

/* kernel.h: heap corruption found by the hardening checks */
#ifndef k_panic
#define k_panic() __builtin_trap()
#endif

/* sys/mem_stats.h */
struct sys_memory_stats {
	size_t free_bytes;
	size_t allocated_bytes;
	size_t max_allocated_bytes;
};

#endif /* ZEPHYR_HEAP_PORT_H */
