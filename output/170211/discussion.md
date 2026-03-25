# [flang][OpenMP] Unexpected crashes in _FortranAAssign when offloading on AMD GPUs

**Author:** VeeEM
**URL:** https://github.com/llvm/llvm-project/issues/170211
**Status:** Closed
**Labels:** crash, llvm::Attributor
**Closed Date:** 2025-12-23T09:05:16Z

## Body

I'm seeing some unexpected segfaults in code that calls _FortranAAssign on the GPU. Code that used to work previously. I hit this originally in some code that uses thread private arrays and assigns the values of one array to another, but the problem does not seem to be specific to private arrays.

Here's a minimal reproducer:
```
program a
integer, dimension(5) :: xs

!$omp target map(xs)
xs = 12
!$omp end target

print *, xs
end program
```

Compiled with
`flang -fopenmp --offload-arch=gfx1100 assign.f90`

Produces output like this when executed
```
Display only launched kernel:
Kernel 'omp target in _QQmain @ 4 (__omp_offloading_804_2b87b0e__QQmain_l4)'
OFFLOAD ERROR: memory access fault by GPU 1 (agent 0x563b441c5710) at virtual address (nil). Reasons: Page not present or supervisor privilege
Use 'OFFLOAD_TRACK_ALLOCATION_TRACES=true' to track device allocations
```

I did some debugging on this. Here's a stacktrace from rocgdb:
```
Thread 6 "a.out" received signal SIGSEGV, Segmentation fault.
Warning: precise memory violation signal reporting is not enabled, reported
location may not be accurate.  See "show amdgpu precise-memory".
[Switching to thread 6, lane 0 (AMDGPU Lane 1:1:1:8/0 (0,0,0)[224,0,0])]
0x00007ffee189d014 in Fortran::runtime::Descriptor::InlineElements (this=generic#0xe8) at /home/acc/llvm-project/flang-rt/include/flang-rt/runtime/descriptor.h:396
396	    if (n == 0) {
(gdb) bt
#0  0x00007ffee189d014 in Fortran::runtime::Descriptor::InlineElements (this=generic#0xe8) at /home/acc/llvm-project/flang-rt/include/flang-rt/runtime/descriptor.h:396
#1  Fortran::runtime::Componentwise::Componentwise (derived=..., this=<optimized out>) at /home/acc/llvm-project/flang-rt/include/flang-rt/runtime/work-queue.h:138
#2  Fortran::runtime::ComponentsOverElements::ComponentsOverElements (derived=..., from=generic#0x0, this=<optimized out>, instance=...) at /home/acc/llvm-project/flang-rt/include/flang-rt/runtime/work-queue.h:181
#3  Fortran::runtime::FinalizeTicket::FinalizeTicket (derived=..., this=<optimized out>, instance=...) at /home/acc/llvm-project/flang-rt/include/flang-rt/runtime/work-queue.h:288
#4  std::__detail::__variant::_Uninitialized<Fortran::runtime::FinalizeTicket, true>::_Uninitialized<Fortran::runtime::Descriptor const&, Fortran::runtime::typeInfo::DerivedType const&> (__args=..., this=<optimized out>, __args=...)
    at /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/variant:224
#5  std::_Construct<std::__detail::__variant::_Uninitialized<Fortran::runtime::FinalizeTicket, true>, std::in_place_index_t<0ul> const&, Fortran::runtime::Descriptor const&, Fortran::runtime::typeInfo::DerivedType const&> (__args=..., 
    __p=<optimized out>, __args=..., __args=...) at /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/bits/stl_construct.h:119
#6  std::__detail::__variant::__emplace<3ul, true, Fortran::runtime::NullTicket, Fortran::runtime::InitializeTicket, Fortran::runtime::InitializeCloneTicket, Fortran::runtime::FinalizeTicket, Fortran::runtime::DestroyTicket, Fortran::runtime::AssignTicket, Fortran::runtime::DerivedAssignTicket<false>, Fortran::runtime::DerivedAssignTicket<true>, Fortran::runtime::io::descr::DescriptorIoTicket<(Fortran::runtime::io::Direction)0>, Fortran::runtime::io::descr::DescriptorIoTicket<(Fortran::runtime::io::Direction)1>, Fortran::runtime::io::descr::DerivedIoTicket<(Fortran::runtime::io::Direction)0>, Fortran::runtime::io::descr::DerivedIoTicket<(Fortran::runtime::io::Direction)1>, Fortran::runtime::Descriptor const&, Fortran::runtime::typeInfo::DerivedType const&> (__args=..., __v=..., __args=...) at /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/variant:567
#7  std::variant<Fortran::runtime::NullTicket, Fortran::runtime::InitializeTicket, Fortran::runtime::InitializeCloneTicket, Fortran::runtime::FinalizeTicket, Fortran::runtime::DestroyTicket, Fortran::runtime::AssignTicket, Fortran::runtime::DerivedAssignTicket<false>, Fortran::runtime::DerivedAssignTicket<true>, Fortran::runtime::io::descr::DescriptorIoTicket<(Fortran::runtime::io::Direction)0>, Fortran::runtime::io::descr::DescriptorIoTicket<(Fortran::runtime::io::Direction)1>, Fortran::runtime::io::descr::DerivedIoTicket<(Fortran::runtime::io::Direction)0>, Fortran::runtime::io::descr::DerivedIoTicket<(Fortran::runtime::io::Direction)1> >::emplace<3ul, Fortran::runtime::Descriptor const&, Fortran::runtime::typeInfo::DerivedType const&> (__args=..., this=<optimized out>, __args=...) at /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/variant:1570
#8  std::variant<Fortran::runtime::NullTicket, Fortran::runtime::InitializeTicket, Fortran::runtime::InitializeCloneTicket, Fortran::runtime::FinalizeTicket, Fortran::runtime::DestroyTicket, Fortran::runtime::AssignTicket, Fortran::runtime::DerivedAssignTicket<false>, Fortran::runtime::DerivedAssignTicket<true>, Fortran::runtime::io::descr::DescriptorIoTicket<(Fortran::runtime::io::Direction)0>, Fortran::runtime::io::descr::DescriptorIoTicket<(Fortran::runtime::io::Direction)1>, Fortran::runtime::io::descr::DerivedIoTicket<(Fortran::runtime::io::Direction)0>, Fortran::runtime::io::descr::DerivedIoTicket<(Fortran::runtime::io::Direction)1> >::emplace<Fortran::runtime::FinalizeTicket, Fortran::runtime::Descriptor const&, Fortran::runtime::typeInfo::DerivedType const&> (__args=..., this=<optimized out>, __args=...) at /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/variant:1522
#9  Fortran::runtime::WorkQueue::BeginFinalize (derived=..., this=<optimized out>, descriptor=...) at /home/acc/llvm-project/flang-rt/include/flang-rt/runtime/work-queue.h:477
#10 Fortran::runtime::AssignTicket::Begin (this=<optimized out>, workQueue=...) at /home/acc/llvm-project/flang-rt/lib/runtime/assign.cpp:377
#11 Fortran::runtime::Ticket::Continue(Fortran::runtime::WorkQueue&)::$_0::operator()<Fortran::runtime::AssignTicket>(Fortran::runtime::AssignTicket&) const (this=<optimized out>, specificTicket=...)
    at /home/acc/llvm-project/flang-rt/lib/runtime/work-queue.cpp:29
#12 Fortran::common::log2visit::Log2VisitHelper<0ul, 5ul, int, Fortran::runtime::Ticket::Continue(Fortran::runtime::WorkQueue&)::$_0, std::variant<Fortran::runtime::NullTicket, Fortran::runtime::InitializeTicket, Fortran::runtime::InitializeCloneTicket, Fortran::runtime::FinalizeTicket, Fortran::runtime::DestroyTicket, Fortran::runtime::AssignTicket, Fortran::runtime::DerivedAssignTicket<false>, Fortran::runtime::DerivedAssignTicket<true>, Fortran::runtime::io::descr::DescriptorIoTicket<(Fortran::runtime::io::Direction)0>, Fortran::runtime::io::descr::DescriptorIoTicket<(Fortran::runtime::io::Direction)1>, Fortran::runtime::io::descr::DerivedIoTicket<(Fortran::runtime::io::Direction)0>, Fortran::runtime::io::descr::DerivedIoTicket<(Fortran::runtime::io::Direction)1> >&>(Fortran::runtime::Ticket::Continue(Fortran::runtime::WorkQueue&)::$_0&&, unsigned long, std::variant<Fortran::runtime::NullTicket, Fortran::runtime::InitializeTicket, Fortran::runtime::InitializeCloneTicket, Fortran::runtime::FinalizeTicket, Fortran::runtime::DestroyTicket, Fortran::runtime::AssignTicket, Fortran::runtime::DerivedAssignTicket<false>, Fortran::runtime::DerivedAssignTicket<true>, Fortran::runtime::io::descr::DescriptorIoTicket<(Fortran::runtime::io::Direction)0>, Fortran::runtime::io::descr::DescriptorIoTicket<(Fortran::runtime::io::Direction)1>, Fortran::runtime::io::descr::DerivedIoTicket<(Fortran::runtime::io::Direction)0>, Fortran::runtime::io::descr::DerivedIoTicket<(Fortran::runtime::io::Direction)1> >&) (visitor=..., which=<optimized out>, u=...) at /home/acc/llvm-project/flang-rt/../flang/include/flang/Common/visit.h:50
#13 Fortran::common::log2visit::Log2VisitHelper<0ul, 11ul, int, Fortran::runtime::Ticket::Continue(Fortran::runtime::WorkQueue&)::$_0, std::variant<Fortran::runtime::NullTicket, Fortran::runtime::InitializeTicket, Fortran::runtime::InitializeCloneTicket, Fortran::runtime::FinalizeTicket, Fortran::runtime::DestroyTicket, Fortran::runtime::AssignTicket, Fortran::runtime::DerivedAssignTicket<false>, Fortran::runtime::DerivedAssignTicket<true>, Fortran::runtime::io::descr::DescriptorIoTicket<(Fortran::runtime::io::Direction)0>, Fortran::runtime::io::descr::DescriptorIoTicket<(Fortran::runtime::io::Direction)1>, Fortran::runtime::io::descr::DerivedIoTicket<(Fortran::runtime::io::Direction)0>, Fortran::runtime::io::descr::DerivedIoTicket<(Fortran::runtime::io::Direction)1> >&>(Fortran::runtime::Ticket::Continue(Fortran::runtime::WorkQueue&)::$_0&&, unsigned long, std::variant<Fortran::runtime::NullTicket, Fortran::runtime::InitializeTicket, Fortran::runtime::InitializeCloneTicket, Fortran::runtime::FinalizeTicket, Fortran::runtime::DestroyTicket, Fortran::runtime::AssignTicket, Fortran::runtime::DerivedAssignTicket<false>, Fortran::runtime::DerivedAssignTicket<true>, Fortran::runtime::io::descr::DescriptorIoTicket<(Fortran::runtime::io::Direction)0>, Fortran::runtime::io::descr::DescriptorIoTicket<(Fortran::runtime::io::Direction)1>, Fortran::runtime::io::descr::DerivedIoTicket<(Fortran::runtime::io::Direction)0>, Fortran::runtime::io::descr::DerivedIoTicket<(Fortran::runtime::io::Direction)1> >&) (which=5, visitor=..., u=...) at /home/acc/llvm-project/flang-rt/../flang/include/flang/Common/visit.h:61
#14 Fortran::common::log2visit::visit<Fortran::runtime::Ticket::Continue(Fortran::runtime::WorkQueue&)::$_0, std::variant<Fortran::runtime::NullTicket, Fortran::runtime::InitializeTicket, Fortran::runtime::InitializeCloneTicket, Fortran::runtime::FinalizeTicket, Fortran::runtime::DestroyTicket, Fortran::runtime::AssignTicket, Fortran::runtime::DerivedAssignTicket<false>, Fortran::runtime::DerivedAssignTicket<true>, Fortran::runtime::io::descr::DescriptorIoTicket<(Fortran::runtime::io::Direction)0>, Fortran::runtime::io::descr::DescriptorIoTicket<(Fortran::runtime::io::Direction)1>, Fortran::runtime::io::descr::DerivedIoTicket<(Fortran::runtime::io::Direction)0>, Fortran::runtime::io::descr::DerivedIoTicket<(Fortran::runtime::io::Direction)1> >&>(Fortran::runtime::Ticket::Continue(Fortran::runtime::WorkQueue&)::$_0&&, std::variant<Fortran::runtime::NullTicket, Fortran::runtime::InitializeTicket, Fortran::runtime::InitializeCloneTicket, Fortran::runtime::FinalizeTicket, Fortran::runtime::DestroyTicket, Fortran::runtime::AssignTicket, Fortran::runtime::DerivedAssignTicket<false>, Fortran::runtime::DerivedAssignTicket<true>, Fortran::runtime::io::descr::DescriptorIoTicket<(Fortran::runtime::io::Direction)0>, Fortran::runtime::io::descr::DescriptorIoTicket<(Fortran::runtime::io::Direction)1>, Fortran::runtime::io::descr::DerivedIoTicket<(Fortran::runtime::io::Direction)0>, Fortran::runtime::io::descr::DerivedIoTicket<(Fortran::runtime::io::Direction)1> >&) (
    visitor=..., u=...) at /home/acc/llvm-project/flang-rt/../flang/include/flang/Common/visit.h:78
#15 Fortran::runtime::Ticket::Continue (this=<optimized out>, workQueue=...) at /home/acc/llvm-project/flang-rt/lib/runtime/work-queue.cpp:27
#16 Fortran::runtime::WorkQueue::Run (this=<optimized out>) at /home/acc/llvm-project/flang-rt/lib/runtime/work-queue.cpp:104
#17 0x00007ffee18ae2bc in Fortran::runtime::Assign (to=..., from=..., terminator=..., flags=11, memmoveFct=<optimized out>) at llvm-project/flang-rt/lib/runtime/assign.cpp:275
#18 _FortranAAssign (to=..., from=..., sourceFile=<optimized out>, sourceLine=<optimized out>) at llvm-project/flang-rt/lib/runtime/assign.cpp:796
#19 __omp_offloading_805_34fe22__QQmain_l4 () at assign.f90:5
```

I think these lines are interesting
```
#9  Fortran::runtime::WorkQueue::BeginFinalize (derived=..., this=<optimized out>, descriptor=...) at /home/acc/llvm-project/flang-rt/include/flang-rt/runtime/work-queue.h:477
#10 Fortran::runtime::AssignTicket::Begin (this=<optimized out>, workQueue=...) at /home/acc/llvm-project/flang-rt/lib/runtime/assign.cpp:377
```
It looks like a branch is being taken that should be impossible. The call to WorkQueue::BeginFinalize is guarded by a null check on toDerived_, but it seems to be taken even though toDerived_ is null.

https://github.com/llvm/llvm-project/blob/main/flang-rt/lib/runtime/assign.cpp#L373-L380

I did check this by adding some debug prints to write out the value of toDerived_ after it has been set at the beginning of the function, and the printed value is 0, but the stack trace on crash still shows the branch behind the null check having been taken.

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-flang-runtime

Author: Ville-Markus Yli-Suutala (VeeEM)

<details>
I'm seeing some unexpected segfaults in code that calls _FortranAAssign on the GPU. Code that used to work previously. I hit this originally in some code that uses thread private arrays and assigns the values of one array to another, but the problem does not seem to be specific to private arrays.

Here's a minimal reproducer:
```
program a
integer, dimension(5) :: xs

!$omp target map(xs)
xs = 12
!$omp end target

print *, xs
end program
```

Compiled with
`flang -fopenmp --offload-arch=gfx1100 assign.f90`

Produces output like this when executed
```
Display only launched kernel:
Kernel 'omp target in _QQmain @ 4 (__omp_offloading_804_2b87b0e__QQmain_l4)'
OFFLOAD ERROR: memory access fault by GPU 1 (agent 0x563b441c5710) at virtual address (nil). Reasons: Page not present or supervisor privilege
Use 'OFFLOAD_TRACK_ALLOCATION_TRACES=true' to track device allocations
```

I did some debugging on this. Here's a stacktrace from rocgdb:
```
Thread 6 "a.out" received signal SIGSEGV, Segmentation fault.
Warning: precise memory violation signal reporting is not enabled, reported
location may not be accurate.  See "show amdgpu precise-memory".
[Switching to thread 6, lane 0 (AMDGPU Lane 1:1:1:8/0 (0,0,0)[224,0,0])]
0x00007ffee189d014 in Fortran::runtime::Descriptor::InlineElements (this=generic#<!-- -->0xe8) at /home/acc/llvm-project/flang-rt/include/flang-rt/runtime/descriptor.h:396
396	    if (n == 0) {
(gdb) bt
#<!-- -->0  0x00007ffee189d014 in Fortran::runtime::Descriptor::InlineElements (this=generic#<!-- -->0xe8) at /home/acc/llvm-project/flang-rt/include/flang-rt/runtime/descriptor.h:396
#<!-- -->1  Fortran::runtime::Componentwise::Componentwise (derived=..., this=&lt;optimized out&gt;) at /home/acc/llvm-project/flang-rt/include/flang-rt/runtime/work-queue.h:138
#<!-- -->2  Fortran::runtime::ComponentsOverElements::ComponentsOverElements (derived=..., from=generic#<!-- -->0x0, this=&lt;optimized out&gt;, instance=...) at /home/acc/llvm-project/flang-rt/include/flang-rt/runtime/work-queue.h:181
#<!-- -->3  Fortran::runtime::FinalizeTicket::FinalizeTicket (derived=..., this=&lt;optimized out&gt;, instance=...) at /home/acc/llvm-project/flang-rt/include/flang-rt/runtime/work-queue.h:288
#<!-- -->4  std::__detail::__variant::_Uninitialized&lt;Fortran::runtime::FinalizeTicket, true&gt;::_Uninitialized&lt;Fortran::runtime::Descriptor const&amp;, Fortran::runtime::typeInfo::DerivedType const&amp;&gt; (__args=..., this=&lt;optimized out&gt;, __args=...)
    at /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/variant:224
#<!-- -->5  std::_Construct&lt;std::__detail::__variant::_Uninitialized&lt;Fortran::runtime::FinalizeTicket, true&gt;, std::in_place_index_t&lt;0ul&gt; const&amp;, Fortran::runtime::Descriptor const&amp;, Fortran::runtime::typeInfo::DerivedType const&amp;&gt; (__args=..., 
    __p=&lt;optimized out&gt;, __args=..., __args=...) at /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/bits/stl_construct.h:119
#<!-- -->6  std::__detail::__variant::__emplace&lt;3ul, true, Fortran::runtime::NullTicket, Fortran::runtime::InitializeTicket, Fortran::runtime::InitializeCloneTicket, Fortran::runtime::FinalizeTicket, Fortran::runtime::DestroyTicket, Fortran::runtime::AssignTicket, Fortran::runtime::DerivedAssignTicket&lt;false&gt;, Fortran::runtime::DerivedAssignTicket&lt;true&gt;, Fortran::runtime::io::descr::DescriptorIoTicket&lt;(Fortran::runtime::io::Direction)0&gt;, Fortran::runtime::io::descr::DescriptorIoTicket&lt;(Fortran::runtime::io::Direction)1&gt;, Fortran::runtime::io::descr::DerivedIoTicket&lt;(Fortran::runtime::io::Direction)0&gt;, Fortran::runtime::io::descr::DerivedIoTicket&lt;(Fortran::runtime::io::Direction)1&gt;, Fortran::runtime::Descriptor const&amp;, Fortran::runtime::typeInfo::DerivedType const&amp;&gt; (__args=..., __v=..., __args=...) at /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/variant:567
#<!-- -->7  std::variant&lt;Fortran::runtime::NullTicket, Fortran::runtime::InitializeTicket, Fortran::runtime::InitializeCloneTicket, Fortran::runtime::FinalizeTicket, Fortran::runtime::DestroyTicket, Fortran::runtime::AssignTicket, Fortran::runtime::DerivedAssignTicket&lt;false&gt;, Fortran::runtime::DerivedAssignTicket&lt;true&gt;, Fortran::runtime::io::descr::DescriptorIoTicket&lt;(Fortran::runtime::io::Direction)0&gt;, Fortran::runtime::io::descr::DescriptorIoTicket&lt;(Fortran::runtime::io::Direction)1&gt;, Fortran::runtime::io::descr::DerivedIoTicket&lt;(Fortran::runtime::io::Direction)0&gt;, Fortran::runtime::io::descr::DerivedIoTicket&lt;(Fortran::runtime::io::Direction)1&gt; &gt;::emplace&lt;3ul, Fortran::runtime::Descriptor const&amp;, Fortran::runtime::typeInfo::DerivedType const&amp;&gt; (__args=..., this=&lt;optimized out&gt;, __args=...) at /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/variant:1570
#<!-- -->8  std::variant&lt;Fortran::runtime::NullTicket, Fortran::runtime::InitializeTicket, Fortran::runtime::InitializeCloneTicket, Fortran::runtime::FinalizeTicket, Fortran::runtime::DestroyTicket, Fortran::runtime::AssignTicket, Fortran::runtime::DerivedAssignTicket&lt;false&gt;, Fortran::runtime::DerivedAssignTicket&lt;true&gt;, Fortran::runtime::io::descr::DescriptorIoTicket&lt;(Fortran::runtime::io::Direction)0&gt;, Fortran::runtime::io::descr::DescriptorIoTicket&lt;(Fortran::runtime::io::Direction)1&gt;, Fortran::runtime::io::descr::DerivedIoTicket&lt;(Fortran::runtime::io::Direction)0&gt;, Fortran::runtime::io::descr::DerivedIoTicket&lt;(Fortran::runtime::io::Direction)1&gt; &gt;::emplace&lt;Fortran::runtime::FinalizeTicket, Fortran::runtime::Descriptor const&amp;, Fortran::runtime::typeInfo::DerivedType const&amp;&gt; (__args=..., this=&lt;optimized out&gt;, __args=...) at /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/variant:1522
#<!-- -->9  Fortran::runtime::WorkQueue::BeginFinalize (derived=..., this=&lt;optimized out&gt;, descriptor=...) at /home/acc/llvm-project/flang-rt/include/flang-rt/runtime/work-queue.h:477
#<!-- -->10 Fortran::runtime::AssignTicket::Begin (this=&lt;optimized out&gt;, workQueue=...) at /home/acc/llvm-project/flang-rt/lib/runtime/assign.cpp:377
#<!-- -->11 Fortran::runtime::Ticket::Continue(Fortran::runtime::WorkQueue&amp;)::$_0::operator()&lt;Fortran::runtime::AssignTicket&gt;(Fortran::runtime::AssignTicket&amp;) const (this=&lt;optimized out&gt;, specificTicket=...)
    at /home/acc/llvm-project/flang-rt/lib/runtime/work-queue.cpp:29
#<!-- -->12 Fortran::common::log2visit::Log2VisitHelper&lt;0ul, 5ul, int, Fortran::runtime::Ticket::Continue(Fortran::runtime::WorkQueue&amp;)::$_0, std::variant&lt;Fortran::runtime::NullTicket, Fortran::runtime::InitializeTicket, Fortran::runtime::InitializeCloneTicket, Fortran::runtime::FinalizeTicket, Fortran::runtime::DestroyTicket, Fortran::runtime::AssignTicket, Fortran::runtime::DerivedAssignTicket&lt;false&gt;, Fortran::runtime::DerivedAssignTicket&lt;true&gt;, Fortran::runtime::io::descr::DescriptorIoTicket&lt;(Fortran::runtime::io::Direction)0&gt;, Fortran::runtime::io::descr::DescriptorIoTicket&lt;(Fortran::runtime::io::Direction)1&gt;, Fortran::runtime::io::descr::DerivedIoTicket&lt;(Fortran::runtime::io::Direction)0&gt;, Fortran::runtime::io::descr::DerivedIoTicket&lt;(Fortran::runtime::io::Direction)1&gt; &gt;&amp;&gt;(Fortran::runtime::Ticket::Continue(Fortran::runtime::WorkQueue&amp;)::$_0&amp;&amp;, unsigned long, std::variant&lt;Fortran::runtime::NullTicket, Fortran::runtime::InitializeTicket, Fortran::runtime::InitializeCloneTicket, Fortran::runtime::FinalizeTicket, Fortran::runtime::DestroyTicket, Fortran::runtime::AssignTicket, Fortran::runtime::DerivedAssignTicket&lt;false&gt;, Fortran::runtime::DerivedAssignTicket&lt;true&gt;, Fortran::runtime::io::descr::DescriptorIoTicket&lt;(Fortran::runtime::io::Direction)0&gt;, Fortran::runtime::io::descr::DescriptorIoTicket&lt;(Fortran::runtime::io::Direction)1&gt;, Fortran::runtime::io::descr::DerivedIoTicket&lt;(Fortran::runtime::io::Direction)0&gt;, Fortran::runtime::io::descr::DerivedIoTicket&lt;(Fortran::runtime::io::Direction)1&gt; &gt;&amp;) (visitor=..., which=&lt;optimized out&gt;, u=...) at /home/acc/llvm-project/flang-rt/../flang/include/flang/Common/visit.h:50
#<!-- -->13 Fortran::common::log2visit::Log2VisitHelper&lt;0ul, 11ul, int, Fortran::runtime::Ticket::Continue(Fortran::runtime::WorkQueue&amp;)::$_0, std::variant&lt;Fortran::runtime::NullTicket, Fortran::runtime::InitializeTicket, Fortran::runtime::InitializeCloneTicket, Fortran::runtime::FinalizeTicket, Fortran::runtime::DestroyTicket, Fortran::runtime::AssignTicket, Fortran::runtime::DerivedAssignTicket&lt;false&gt;, Fortran::runtime::DerivedAssignTicket&lt;true&gt;, Fortran::runtime::io::descr::DescriptorIoTicket&lt;(Fortran::runtime::io::Direction)0&gt;, Fortran::runtime::io::descr::DescriptorIoTicket&lt;(Fortran::runtime::io::Direction)1&gt;, Fortran::runtime::io::descr::DerivedIoTicket&lt;(Fortran::runtime::io::Direction)0&gt;, Fortran::runtime::io::descr::DerivedIoTicket&lt;(Fortran::runtime::io::Direction)1&gt; &gt;&amp;&gt;(Fortran::runtime::Ticket::Continue(Fortran::runtime::WorkQueue&amp;)::$_0&amp;&amp;, unsigned long, std::variant&lt;Fortran::runtime::NullTicket, Fortran::runtime::InitializeTicket, Fortran::runtime::InitializeCloneTicket, Fortran::runtime::FinalizeTicket, Fortran::runtime::DestroyTicket, Fortran::runtime::AssignTicket, Fortran::runtime::DerivedAssignTicket&lt;false&gt;, Fortran::runtime::DerivedAssignTicket&lt;true&gt;, Fortran::runtime::io::descr::DescriptorIoTicket&lt;(Fortran::runtime::io::Direction)0&gt;, Fortran::runtime::io::descr::DescriptorIoTicket&lt;(Fortran::runtime::io::Direction)1&gt;, Fortran::runtime::io::descr::DerivedIoTicket&lt;(Fortran::runtime::io::Direction)0&gt;, Fortran::runtime::io::descr::DerivedIoTicket&lt;(Fortran::runtime::io::Direction)1&gt; &gt;&amp;) (which=5, visitor=..., u=...) at /home/acc/llvm-project/flang-rt/../flang/include/flang/Common/visit.h:61
#<!-- -->14 Fortran::common::log2visit::visit&lt;Fortran::runtime::Ticket::Continue(Fortran::runtime::WorkQueue&amp;)::$_0, std::variant&lt;Fortran::runtime::NullTicket, Fortran::runtime::InitializeTicket, Fortran::runtime::InitializeCloneTicket, Fortran::runtime::FinalizeTicket, Fortran::runtime::DestroyTicket, Fortran::runtime::AssignTicket, Fortran::runtime::DerivedAssignTicket&lt;false&gt;, Fortran::runtime::DerivedAssignTicket&lt;true&gt;, Fortran::runtime::io::descr::DescriptorIoTicket&lt;(Fortran::runtime::io::Direction)0&gt;, Fortran::runtime::io::descr::DescriptorIoTicket&lt;(Fortran::runtime::io::Direction)1&gt;, Fortran::runtime::io::descr::DerivedIoTicket&lt;(Fortran::runtime::io::Direction)0&gt;, Fortran::runtime::io::descr::DerivedIoTicket&lt;(Fortran::runtime::io::Direction)1&gt; &gt;&amp;&gt;(Fortran::runtime::Ticket::Continue(Fortran::runtime::WorkQueue&amp;)::$_0&amp;&amp;, std::variant&lt;Fortran::runtime::NullTicket, Fortran::runtime::InitializeTicket, Fortran::runtime::InitializeCloneTicket, Fortran::runtime::FinalizeTicket, Fortran::runtime::DestroyTicket, Fortran::runtime::AssignTicket, Fortran::runtime::DerivedAssignTicket&lt;false&gt;, Fortran::runtime::DerivedAssignTicket&lt;true&gt;, Fortran::runtime::io::descr::DescriptorIoTicket&lt;(Fortran::runtime::io::Direction)0&gt;, Fortran::runtime::io::descr::DescriptorIoTicket&lt;(Fortran::runtime::io::Direction)1&gt;, Fortran::runtime::io::descr::DerivedIoTicket&lt;(Fortran::runtime::io::Direction)0&gt;, Fortran::runtime::io::descr::DerivedIoTicket&lt;(Fortran::runtime::io::Direction)1&gt; &gt;&amp;) (
    visitor=..., u=...) at /home/acc/llvm-project/flang-rt/../flang/include/flang/Common/visit.h:78
#<!-- -->15 Fortran::runtime::Ticket::Continue (this=&lt;optimized out&gt;, workQueue=...) at /home/acc/llvm-project/flang-rt/lib/runtime/work-queue.cpp:27
#<!-- -->16 Fortran::runtime::WorkQueue::Run (this=&lt;optimized out&gt;) at /home/acc/llvm-project/flang-rt/lib/runtime/work-queue.cpp:104
#<!-- -->17 0x00007ffee18ae2bc in Fortran::runtime::Assign (to=..., from=..., terminator=..., flags=11, memmoveFct=&lt;optimized out&gt;) at llvm-project/flang-rt/lib/runtime/assign.cpp:275
#<!-- -->18 _FortranAAssign (to=..., from=..., sourceFile=&lt;optimized out&gt;, sourceLine=&lt;optimized out&gt;) at llvm-project/flang-rt/lib/runtime/assign.cpp:796
#<!-- -->19 __omp_offloading_805_34fe22__QQmain_l4 () at assign.f90:5
```

I think these lines are interesting
```
#<!-- -->9  Fortran::runtime::WorkQueue::BeginFinalize (derived=..., this=&lt;optimized out&gt;, descriptor=...) at /home/acc/llvm-project/flang-rt/include/flang-rt/runtime/work-queue.h:477
#<!-- -->10 Fortran::runtime::AssignTicket::Begin (this=&lt;optimized out&gt;, workQueue=...) at /home/acc/llvm-project/flang-rt/lib/runtime/assign.cpp:377
```
It looks like a branch is being taken that should be impossible. The call to WorkQueue::BeginFinalize is guarded by a null check on toDerived_, but it seems to be taken even though toDerived_ is null.

https://github.com/llvm/llvm-project/blob/main/flang-rt/lib/runtime/assign.cpp#L373-L380

I did check this by adding some debug prints to write out the value of toDerived_ after it has been set at the beginning of the function, and the printed value is 0, but the stack trace on crash still shows the branch behind the null check having been taken.
</details>


---

### Comment 2 - VeeEM

This was fixed when the patch that caused it was reverted https://github.com/llvm/llvm-project/pull/170257.

---

