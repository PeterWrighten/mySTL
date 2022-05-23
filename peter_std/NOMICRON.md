# Peter's Outline about NOMICRON

## Unsafe

## Memory Data-layout

**`repre(Rust)`**

1.Alignment(Because of Program Counter Register)

- `structs`: named product types.
- `enum`: named sum types.
- `union`: untagged union.
- `array`: homogeneous product type.
- `tuple`: anonymous product type.

Rust data-layout could have multiple alignment:

```rust 
struct A {
    a: u8,
    _pad1: [u8; 3],
    b: u32,
    c: u16,
    _pad2: [u8; 2],
}
```

or just like:

```rust
struct A {
    b: u32,
    c: u16,
    a: u8,
    _pad: u8,
}
```

Rust ensures the same data-layout of different instances. 

So you could see that different alignment decides meomory usage.

For `enum`, it is more complicated:

```rust
enum Foo {
    A(u32),
    B(u64),
    C(u8),
}
```

the above `enum` may be layouted as:

```rust
struct FooRepr {
    data: u64, // based on tag, would be u64, u32, u8
    tag: u8, // 0 = A, 1 = B, 2 = C
}
```

But this layout is inefficient. just like `Nullptr`:

```rust
size_of::<Option<&T>>() == size_of::<&T>() // None == Null, that's inefficient
```

**Unusual Sized Type**

- Dynamically Sized Types(DSTs)
    - Stored as a `Fat ptr`
    - Types: trait objects: `dyn MyTrait`, slices: `[T]`, `str`
    ```rust
    // Technique: Build an DST struct

    // Bad Case
    struct MySuperSlice {
        info: u32,
        data: [u8], // unsized, compile error
    }

    // Good Case
    struct MySuperSliceable<T: ?Sized> { 
    // create a dst generics, and derive unsizing coercion.
        info: u32,
        data: T,
    }
    ```
- ZSTs
    ```rust
    struct Nothing;

    struct LotsOfNothing {
        qux: (), // ZSTs
        foo: Nothing, // ZST
        baz: [u8; 0], //ZSTs
    }
    ```
    - In Rust, Any acts to Genreate or Store could be seemed as no-op.
    - Ex: As for any `Map<Key, Value>`, could implement a `Set<Key> = Map<Key, ()>`.

- Void Type
    - Rust allowed to declare some type which could not be instanced. These type could merely mention sth in perspective of typesystem but value.
    - `enum Void {}`
    - Ex: `Result<T, Void>` could merely indicate `T`.
    - Could generate a rawptr pointed to `Void`, but not deref it.
    - `*const ()` == `void*`, `*const Void` != `void*`

**Customed Data Layout**
 - `repr(C)`
    - As for ZST, `repr(C)` means 1 byte `Null`.
    - DST(Fat pointer) and Tuple do not exist in C
    - `enum` with section, but have defined FFi [RFCs](https://github.com/rust-lang/rfcs/blob/master/text/2195-really-tagged-unions.md)

- `repr(transparent)`
    - Only usable for struct which only has single non-zero sized section.
    - `UnsafeCell`: let coercion between section and struct available.

- `repr(u*), repr(i*)`
    - use for non-section `enum`
    - unavailable for `struct`
    ```rust
    enum MyOption<T> {
        Some(T),
        None,
    }

    // FFi version
    /// use `repr(u*)` could prevent Nullptr Optimization.
    #[repr(u8)]
    enum MyReprOption<T> {
        Some(T),
        None,
    }
    ```

- `repr(packed)`
    - enforce Rustc to eliminateany paddles. align into 1 byte.
    - `repr(packed)` is unsafe.

- `repr(align(n))`: n is powers of 2.
    - enforce type align at least based on n. it is a modified version of `repr(C)` and `repr(Rust)` and not compatible with `repr(packed)`.

## Ownership and lifetime

**Reference**

- alias
    - `fn compute(input: &u32, output: &mut u32)`, like this, we would know input and output would never be alia of each other because of ownership system.
    - in Rust, `&mut` could not be allowed to own alias.

**Lifetime**
- T: 'static and &'static T
- HRTBs(Higher-Rank Trait Bounds)
    - in case to bound function pointer:
    
rust
    impl<F> Closure<F> 
        where for<'a> F: Fn(&'a (u8, u16)) -> &'a u8,
    
    or like the following:
    
rust
    where F: for<'a> Fn(&'a (u8, u16)) -> &'a u8,
    
    - for<'a> means 'as for any 'a lifetime bound'
- inheritance, subtyping and polymorphism
    - covariant: `F<Sub> -> F<Super>`
    - contravariant `F<Super> -> F<Sub>`
    - invariant

