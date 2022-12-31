#pragma once

#include "u_ptr.h"

namespace SmartPtr{

    template<typename T>
    class SharedPtr {
            friend class WeakPtr<T>;
            UPtr<T> *rp;
        public:
            SharedPtr(T *p) : rp(new UPtr<T>(p)) {}
            SharedPtr(const SharedPtr<T>& sp) : rp(sp.rp) {
                ++rp->count;
            }

            void reset() {
                if(--rp->count == 0) {
                    delete rp;
                } 
                rp = new UPtr<T>();
            }

            int use_count() {
               return rp->count; 
            }

            SharedPtr& operator=(const SharedPtr<T>& rhs) {
                ++rhs.rp->count;
                if(--rp->count == 0) 
                    delete rp;

                rp = rhs.rp;
                return *this;
            }

            ~SharedPtr() {
                if(--rp->count == 0) 
                    delete rp;
            }

            T& operator *() {
                return *(rp->data);
            }

            T* operator ->() {
                return rp->data;
            }
    };

}
