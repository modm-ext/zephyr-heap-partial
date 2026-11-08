# Zephyr sys_heap partial tree

https://github.com/zephyrproject-rtos/zephyr

This is just a partial copy of the Zephyr heap allocator (`sys_heap`) to reduce
repository size. The following paths are extracted:

- `LICENSE`
- `lib/heap/heap.c` and `lib/heap/heap.h` to `heap/`
- `include/zephyr/sys/sys_heap.h` to `heap/`

`update.py` replaces the `<zephyr/...>` includes with `heap/zephyr_port.h`,
which is maintained in this repository and defines the few Zephyr macros and
`CONFIG_SYS_HEAP_*` options the allocator needs. Every option can be overridden
by defining it before or on the command line.

This repository is updated periodically by GitHub Actions.
