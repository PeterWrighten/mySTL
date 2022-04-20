# Standard Library of Rust

## `mem` module

 **Variable in memory's perspective**

 - `_start`
 - `size of mem`
 - `byte alignment`
 - `sequence of member memory` 
  > In Rust, sequence of menmber memory is job of compiler. But Rust is able to transform a chunk of memory into a `typed var`.

C could arbitrarily modify data-layout of typed var, which is unsafe. Rust could also do it in unsafe part, but type checker still works. GC programming languages degrade performance to ensure safety.

**Point**

- Intrinstic Function for memory modification
- Insection of memory chunk and type system: raw pointer `*const T` / `*mut T`
- Raw pointer's wrapper: `NonNull<T> / Unique<T>`
- Uninitialized modification of memory chunk: `MaybeUninit<T>`/ `ManuallyDrop<T>`
- `alloc/dealloc` about heap memory

### Raw pointer

Raw ptr connect memory and type system, `*const T` indicates `_start`, `size`and `alignment` of memory chunk. Not promise effectivity and safety.

`&T`/`&mut T` ensures safety and effectivity.

**How Raw ptr work**

- cast `usize` into `*const T / *mut T` mandatorily, memory chunk initialized by the numerical would be casted into corresponded type.

- cast between different raw ptr types, which essentialy finished memory-chunk-binded type casting, it is unsafe.

- `*const u8` binds var as heap memory.

- modificate values in memory chunk

- copy memory chunk explicitly.

- use ptr offset calculation to retain new memory chunk.

- FFi

**Implementation**

Raw ptr is actually a data struct constructed by memory address and meta data.

[raw ptr](./src/raw_ptr.rs)

**Metadata Rule**

- For ptr implemented `Sized` trait, which defined as *thin pointer*
    - Metadata: 0
    - Type: `()`
    - Instance: `[T; usize]`
- For ptr with dynamic size, which called as *fat pointer*
    - as for `struct`, if final member is dynamic sized type, metadata is dynamic sized type.
    - as for `str`, metadata is byte-length, metadata type is `usize`.


