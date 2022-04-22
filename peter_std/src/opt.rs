use std::mem::size_of;

enum MyOption<T> {
    Some(T),
    None,
}

#[repr(u8)]
enum MyReprOption<T> {
    Some(T),
    None,
}

#[cfg(test)]
mod tests {
    use super::*;
    
    #[test]
    fn test_repr() {
        assert_eq!(8, size_of::<MyOption<&u16>>());
        assert_eq!(16, size_of::<MyReprOption<&u16>>());
    }
}
