# cpp-systems-lab

Small, production-style **systems primitives** in modern C++ (C++20):
- LRU cache (O(1) get/put) with eviction
- Token-bucket rate limiter (coming)
- SPSC ring buffer (coming)

## Build
`cmake -S . -B build` 

`cmake --build build`

## Tests (GoogleTest)
`ctest --test-dir build --output-on-failure`

## Why "systems"?
These are examples of atomic units of system design: caching, rate control, and lock-free queues under resource constraints.
