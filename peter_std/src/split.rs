
use std::slice::from_raw_parts_mut;

// assume there is a slice

struct Aslice<'a, T>(&'a mut [T]);

impl<'a, T> Aslice<'a, T> {
    pub fn new(slice: &'a mut [T]) -> Aslice<'a, T> {
        Self(slice)
    }
    pub fn split_at_mut(&'a mut self, mid: usize) -> (&'a mut [T], &'a mut [T]) {
        let len = self.0.len();
        let ptr = self.0.as_mut_ptr();

        unsafe {
            assert!(mid <= len);
            (from_raw_parts_mut(ptr, mid),
             from_raw_parts_mut(ptr.add(mid), len - mid))
        }
    }
}

fn main() {
    let mut d = [1, 3, 5, 8];
    let mut demo: Aslice<u8> = Aslice::new(&mut d);
    let (l, r) = demo.split_at_mut(2);
    assert_eq!(l, [1, 3]);
    assert_eq!(r, [5, 8]);
    println!("{:?}", l);
    println!("{:?}", r);
}
