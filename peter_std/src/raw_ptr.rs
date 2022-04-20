//! Raw ptr is actually `PtrComponents<T>`

use core::hash::Hash;
use core::marker::Unpin;

pub(crate) union PtrRepr<T: ?Sized> {
    pub(crate) const_ptr: *const T,
    pub(crate) mut_ptr: *mut T,
    pub(crate) components: PtrComponents<T>,
}

pub(crate) struct PtrComponents<T: ?Sized> {
    /// `*const ()` ensures metadata part is null.
    pub(crate) data_address: *const(),
    /// metadata of diffrent types
    pub(crate) metadata: <T as Pointee>::Metadata,
}

// Rust technique: Use `trait` bound to constrain associate type.
// `Pointee` trait is used to constrain `Matadata` type.
pub trait Pointee {
    /// The type for metadata in pointers and references to `Self`
    type Metadata: Copy + Send + Sync + Ord + Hash + Unpin; 
}

pub trait Thin = Pointee<Metadata = ()>; 