# Allocator

2-stage Allocation.

- If Chunk > 128 bytes, allocate to 1 stage(```malloc()```);
- If Chunk < 128 bytes, allocate to 2 stage(Memory Pool);