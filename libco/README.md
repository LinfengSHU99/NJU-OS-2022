# M2-libco
## What Is Coroutine?
Coroutine has its own register state and stack, shares the same address space with other coroutines, just like what thread does.  
However, there is a significant difference between coroutine and thread, which is the way of switching.  
The switch of coroutines is all up to their user, through calling yield() function. The user has total control over coroutines.  
In contrast, the OS decides when to switch threads, and that leads to a lot of concurrency problems.  

## My Own Version of Libco
Following jyy's [instruction](http://jyywiki.cn/OS/2022/labs/M2) of building a lightweight coroutine library, I built a ~~buggy but verified by jyy's test cases~~ coroutine library(x64 only currently, not handle main coroutine yet).  
Here are some critical points spending me a lot of time, and I think it's necessary to give them a note here.  
1. Stack Alignment  
    To switch to a new coroutine, we have to pass the **bottom** address of the stack allocated inside `struct co` to $rsp. 
However, x86 requires 16-bytes stack alignment. Here I use `void* get_sp(co *co)` to get proper sp value.  
2. Return from Coroutine  
    When doing some stuff inside a coroutine, we are inside the coroutine's own stack. When the coroutine returns, we have to
recover the original $rsp in order to get right function stack. So, I add a `uintprt_t waiter_sp` field to `struct co`,
and whenever we are about to get into a new coroutine, save the current $rsp `static inline uintptr_t get_rsp()`(`inline` is critical here) into that field. 
    When the coroutine finish, I use `static inline void set_rsp(uintprt_t sp)` to put the value of `uintptr_t waiter_sp` into $rsp.  
3. `co_wait()`  
    `co_wait()` should be called exactly once. So, when we yield to another coroutine which is newly created and not running yet, instead of calling `co_wait()`, we should call `co->entry` directly.  
    All dead coroutines should be recycled when `co_wait()` returns.  
