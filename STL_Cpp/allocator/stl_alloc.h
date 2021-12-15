/*
 * Copyright (c) 1996-1997
 * Silicon Graphics Computer Systems, Inc.
 *
 * Permission to use, copy, modify, distribute and sell this software
 * and its documentation for any purpose is hereby granted without fee,
 * provided that the above copyright notice appear in all copies and
 * that both that copyright notice and this permission notice appear
 * in supporting documentation.  Silicon Graphics makes no
 * representations about the suitability of this software for any
 * purpose.  It is provided "as is" without express or implied warranty.
 */

/* NOTE: This is an internal header file, included by other STL headers.
 *   You should not attempt to use it directly.
 */

#ifndef __SGI_STL_INTERNAL_ALLOC_H 
#define __SGI_STL_INTERNAL_ALLOC_H

#ifdef __SUNPRO_CC
#   define __PRIVATE public 
    //重複のアクセス制限宣言は、不意に何かを私有化することを防止する。
#else
#   define __PRIVATE private
#endif

#ifdef __STL_STATIC_TEMPLATE_MEMBER_BUG
#   define __USE_MALLOC   
#endif

//これは、Standard Node Allocatorを実行する。C++ Draft Standard及びOriginal STLのAllocatorとはお一致しない。
//こちらのAllocatorは、異なるポインタタイプをencapsulateすることではなく、実は、たぶんたった一つのポインタタイプしかない。
//Allocation　primitivesは、個別のオブジェクトに分属され、Original STLのように大きくのスペースを分配されない。

#ifndef __THROW_BAD_ALLOC
#   if defined(__STL_NO_BAD_ALLOC) || !defined(__STL_USE_EXCEPTIONS)
#       include<stdio.h>
#       include<stdlib.h>
#       define __THROW_BAD_ALLOC fprintf(stderr, "out of memory\n"); exit(1)
#   else /* 標準の"out-of-memory"操作に従う */
#       include<new>
#       define __THROW_BAD_ALLOC throw std::bad_alloc()
#   endif 
#endif

#include<stddef.h>
#include<stdlib.h>
#include<string.h>
#include<assert.h>
#ifndef __RESTRICT
#   define __RESTRICT
#endif

#ifdef __STL_THREADS
#   include<stl_threads.h>
#   define __NODE_ALLOCATOR_THREADS true
#   ifdef __STL_SGI_THREADS
    //
    extern "C" {
        extern int __us_rsthread_malloc;
    }
        // 
#   define __NODE_ALLOCATOR_LOCK if (threads && __us_rsthread_malloc) \
                {_S_node_allocator_lock._M_acquire_lock(); }
#   define __NODE_ALLOCATOR_UNLOCK if (threads && __us_rsthread_malloc) \
                {_S_node_allocator_lock._M_release_lock(); }
#else /* !__STL_SGI_THREADS */
#   define __NODE_ALLOCATOR_LOCK \
                { if (threads) _S_node_allocator_lock._M_acquire_lock(); }
#   define __NODE_ALLOCATOR_UNLOCK \
                { if (threads) _S_node_allocator_lock._M_release_lock(); }
#endif
#else 

// スレッドは不安全

#   define __NODE_ALLOCATOR_LOCK
#   define __NODE_ALLOCATOR_UNLOCK
#   define __NODE_ALLOCATOR_THREADS false
#endif

__STL_BEGIN_NAMESPACE

#if defined(__sgi) && !defined(__GNUC__) && (_MIPS_SIM != _MIPS_SIM_ABI32)
#pragma set woff 1174
#endif

//Malloc-based allocator. Typically slower than default alloc below.
//Typically thread-safe and more storage efficient.

#ifdef __STL_STATIC_TEMPLATE_MEMBER_BUG
#   ifdef __DECLARE_GLOBALS_HERE
        void(* __malloc_alloc_oom_handler)() = 0;
        //g++ 2.7.2 does not handle static template date members.
#else
    extern void (* __malloc_alloc_oom_handler)();
#   endif 
#endif

// SGI STL 1 Stage allocator

template <int __inst>
class __malloc_alloc_template {

private:

    // Handle "memory not enough" issue

    static void* _S_oom_malloc(size_t);
    static void* _S_oom_realloc(void*, size_t);

#ifndef __STL_STATIC_TEMPLATE_MEMBER_BUG
    static void (* __malloc_alloc_oom_handler)();
#endif

public:

    //1 Stage allocator directly invokes malloc()
    static void* allocate(size_t __n) {// size_t means sizeof(obj)
        void* __result = malloc(__n);
        if(0 == __result)   __result = _S_oom_malloc(__n);
        return __result;
    }

    //1 Stage allocator directly invokes free()
    static void deallocate(void* __p, size_t /* __n */) {
        free(__p);
    }

    static void* reallocate(void* __p, size_t /* old_sz */, size_t __new_sz) {
        void* __result = realloc(__p, __new_sz);
        if(0 == __result)   __result = _S_oom_realloc(__p, __new_sz);
        return __result;
    }

    

    
}