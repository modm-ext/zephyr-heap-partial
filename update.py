#!/usr/bin/env python3
# Copies the Zephyr sys_heap allocator and replaces the Zephyr includes with a
# small stand-alone port header, so that it compiles without the Zephyr tree.

import re
import sys
import subprocess
from pathlib import Path

# wget -qL https://raw.githubusercontent.com/modm-ext/partial/main/partial.py
import partial
partial.keepalive()

repo = "zephyrproject-rtos/zephyr"
src = Path("zephyr_src")
tag = sys.argv[1] if len(sys.argv) > 1 and not sys.argv[1].startswith("-") else partial.latest_release_tag(repo)

if "--fast" not in sys.argv:
    # The Zephyr repository is huge, only check out the few files we need
    subprocess.run(["rm", "-rf", str(src)], check=True)
    subprocess.check_call(["git", "-c", "advice.detachedHead=false", "clone", "--depth=1", "--filter=blob:none",
                           "--sparse", "--branch", tag, f"https://github.com/{repo}.git", str(src)])
    subprocess.check_call(["git", "-C", str(src), "sparse-checkout", "set", "--no-cone",
                           "/LICENSE", "/lib/heap/heap.c", "/lib/heap/heap.h", "/include/zephyr/sys/sys_heap.h"])

def localize(line):
    # sys_heap.h is copied next to heap.c, all other Zephyr headers are replaced by the port header
    line = line.replace("#include <zephyr/sys/sys_heap.h>", '#include "sys_heap.h"')
    return re.sub(r'#include <zephyr/.*?>', '#include "zephyr_port.h"', line)

files = partial.copy_files(src, ["LICENSE"])
# delete=False: heap/ also holds the port header of this repository
files += partial.copy_files(src / "lib/heap", ["heap.c", "heap.h"], dest=Path("heap"), delete=False, modifier=localize)
files += partial.copy_files(src / "include/zephyr/sys", ["sys_heap.h"], dest=Path("heap"), delete=False, modifier=localize)
# the port header is maintained in this repository
files += [Path("heap/zephyr_port.h")]
assert files[-1].exists()

# Fail if a Zephyr include survived, the port header is missing something then
leftover = [f for f in files if f.suffix in (".c", ".h") and f.name != "zephyr_port.h" and "#include <zephyr/" in f.read_text()]
assert not leftover, f"Zephyr includes left in {leftover}"

partial.commit(files, tag)
