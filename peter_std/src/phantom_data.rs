use std::marker::PhantomData;

// use phantom data marker to enable lifetime.
// compiler would not check pointer's type safety.

struct Iter<'a, T: 'a> {
    ptr: *const T,
    end: *const T,
    _marker: PhantomData<&'a T>,
}

struct Vec<T> {
    data: *const T, // *const is mutatable.
    len: usize,
    cap: usize,
    _marker: PhantomData<T>,
}
