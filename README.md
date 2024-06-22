Lux SP - Stream Processing for Modern C++
===

[![Build](https://github.com/rng-dynamics/lux-sp/actions/workflows/build.yml/badge.svg)](https://github.com/rng-dynamics/lux-sp/actions/workflows/build.yml?query=branch%3Amain)
[![Unit Test](https://github.com/rng-dynamics/lux-sp/actions/workflows/unit-test.yml/badge.svg)](https://github.com/rng-dynamics/lux-sp/actions/workflows/unit-test.yml?query=branch%3Amain)
[![Codecov](https://codecov.io/gh/rng-dynamics/lux-sp/graph/badge.svg?)](https://codecov.io/gh/rng-dynamics/lux-sp)
[![Static analysis](https://github.com/rng-dynamics/lux-sp/actions/workflows/code-checker.yml/badge.svg)](https://github.com/rng-dynamics/lux-sp/actions/workflows/code-checker.yml?query=branch%3Amain+)
[![Coverity Scan Build Status](https://scan.coverity.com/projects/30306/badge.svg)](https://scan.coverity.com/projects/rng-dynamics-lux-sp)
[![GitHub license](https://img.shields.io/badge/license-MIT-blue.svg)](https://raw.githubusercontent.com/rng-dynamics/lux-sp/master/LICENSE.md)

Build
---

```
$ mkdir build && cd build
$ cmake .. && make
```

Content
---

- [Concurrency support](lux_sp/src/lux_sp/concurrency_support.h)
- [Memory pool](lux_sp/src/lux_sp/memory_pool.h)
- [Lock-fee queue](lux_sp/src/lux_sp/spsc/lock_free_queue.h)
