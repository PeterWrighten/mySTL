use std::ptr;


/// A simple Vec implemented in unsafe 
pub struct Vec<T> {
    ptr: *mut T,
    cap: usize,
    len: usize,
}

impl<T> Vec<T> {
    pub push(&mut self, elem: T) {
        if self.len == self.cap {
            self.reallocate();
        }

        unsafe {
            ptr::write(self.ptr.add(self.len), elem);
            self.len += 1;
        }
    }
}
