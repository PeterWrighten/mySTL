#pragma once

namespace SmartPtr{
    template <typename T>
    class SharedPtr;

    template <typename T>
    class WeakPtr;

    template <typename T>
    class UPtr {
        private:
            friend class SharedPtr<T>;
            friend class WeakPtr<T>;

            T* data;
            int count; 
            UPtr(T* ptr = nullptr): data(ptr), count(1) {}
            ~UPtr() {
                delete data;
            }
    };
}


