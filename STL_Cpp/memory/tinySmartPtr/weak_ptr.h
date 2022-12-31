#pragma once

#include "u_ptr.h"

namespace SmartPtr {
    template <typename T>
    class WeakPtr {
        UPtr<T>* rp;
        friend class SharedPtr<T>;
    public:
        WeakPtr(const SharedPtr<T>& rhs) : rp(rhs.rp) {}
        WeakPtr(const SharedPtr<T>&& rhs) : rp(rhs.rp) {}
        WeakPtr(T* ptr=nullptr): rp(new UPtr<T>(ptr)) {}
        WeakPtr(const WeakPtr<T>& sp) : rp(sp.rp) {}
        
        WeakPtr& operator=(SharedPtr<T>& rhs) {
            rp = rhs.rp;
            return *this;
        }

        int use_count() {
           return rp->count; 
        }

        T operator *() {
            return *(rp->data); 
        }

        ~WeakPtr() {
            // nauce bug
            if(rp->count == 0) {
                delete rp;
            }
        }
    };
}