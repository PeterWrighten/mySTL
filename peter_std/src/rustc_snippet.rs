/// rustc implement Pointee for both types
pub fn ptr_meta_ty(&'tcx self, tcx: TyCtxt<'tcx>) -> Ty<'tcx> {
    // FIXME: should this normalize?
    let tail = tcx.struct_tail_without_normalization(self);
    match tail.kind() {
        ty::Infer(ty::IntVar(_) | ty::FloatVar(_))
            | ty::Unit(_)
            | ty::Int(_)
            | ty::Bool
            | ty::Float(_)
            | ty::FnDef(..)
            | ty::FnPtr(_)
            | ty::RawPtr(..)
            | ty::Char
            | ty::Ref(..)
            | ty::Generator(..)
            | ty::GeneratorWitness(..)
            | ty::Array(..)
            | ty::Closure(..)
            | ty::Never
            | ty::Error(_)
            | ty::Foreign(..)
            | ty::Adt(..)
            | ty::Tuple(..) => tcx.types.unit, // tcx.types.unit means `()`

            ty::Str | ty::Slice(_) => tcx.types.usize,

            ty::Dynamic(..) => {
                let dyn_metadata = tcx.lang_items().dyn_metadata()
                    .unwrap();
                tcx.type_of(dyn_metadata).subst(tcx, &[tail.into()])
            },

            ty::Projection(_)
                | ty::Param(_)
                | ty::Opaque(..)
                | ty::Infer(ty::TyVar(_))
                | ty::Bound(..)
                | ty::Placeholder(..)
                | ty::Infer(ty::FreshTy(_) | ty::FreshIntTy(_) 
                | ty::FreshFloaty(_)) => {
                  bug!("`ptr_metadata_ty` applied to unexpected type: {:?}", tail)
            } 
    }


pub struct DynMetadata<Dyn: ?Sized> {
    // Reference variable in VTTable
    vtable_ptr: &'static VTable,
    // Indicate struct's ownership of Dyn
    phantom: crate::marker::PhantomData<Dyn>,
}

struct VTable {
    // drop method's pointer for `trait` object.
    drop_in_place: fn(*mut ()),
    size_of: usize,
    align_of: usize,
}
