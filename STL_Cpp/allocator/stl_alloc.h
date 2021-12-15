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

