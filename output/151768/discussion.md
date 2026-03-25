# Fatal error regression: Assertion `SubregToRegSrcInsts.empty() && "can this happen?"' fails on neoverse-v2 machines.

**Author:** scamp-nvidia
**URL:** https://github.com/llvm/llvm-project/issues/151768
**Status:** Closed
**Labels:** llvm:codegen, crash
**Closed Date:** 2025-08-05T14:09:26Z

## Body

Recent Flang builds have begun to fail compiling the example code below (reduced and anonymized from a larger code) with an error message: 


test3.f90:
```
      MODULE TEST_MOD
      INTEGER, PARAMETER :: qq =SELECTED_REAL_KIND(10)
      TYPE type1
      REAL(qq),POINTER :: QONE(:,:) => NULL()
      REAL(qq),POINTER :: QTWO(:,:) => NULL()
      INTEGER,POINTER :: IONE(:) => NULL()
      END TYPE type1

      TYPE type2
      REAL(qq)  ,POINTER :: ARRAY1(:,:,:,:)
      END TYPE type2

      type :: subtype1
      integer :: node = 2
      end type subtype1

      type :: type3
      integer :: intone = 10
      integer :: inttwo  = 10
      type(subtype1) :: comm_line
      end type type3

      CONTAINS

      SUBROUTINE DUMMY_SUB(P,WONE,WTWO)
      IMPLICIT COMPLEX(qq) (C)
      IMPLICIT REAL(qq) (A-B,D-H,O-Z)
      TYPE (type2)    WONE
      TYPE (type3)     WTWO
      TYPE (type1), POINTER :: PPTYPE
      COMPLEX(qq) :: CNUM(10,10,10)
      COMPLEX(qq) :: PHASOR(10,4)

      do ii=1,2
      LLL=PPTYPE%IONE(1)
      MAXX=2*LLL+1
      LM=LLL*LLL
      DO ISPIN=0,10
         DO NBVAR=1,WTWO%INTONE
            CNUM(NBVAR,1,1)= WONE%ARRAY1(ISPIN,NBVAR,1,1)*PPTYPE%QTWO(1,1)*(WONE%ARRAY1(1,NBVAR,1,1))
         ENDDO
      ENDDO
      CALL FLOP(CNUM,1,1,WTWO%INTTWO,.FALSE.)
      IF (1.NE.WTWO%COMM_LINE%NODE-1) write(*,*)
            DO M=1,MAXX
               CTEMPO_VAR= WONE%ARRAY1(M,1,1,1)
               PHASOR(M,1)=PHASOR(M,1)+CTEMPO_VAR
            ENDDO
      enddo
      END SUBROUTINE DUMMY_SUB
      END MODULE TEST_MOD
```
Compiling this with a recent Flang build shows that it fails at "-O2" and with "-mcpu=neoverse-v2", but compiles absent either of those. 

```
user:~$ flang test3.f90 -O2 -c -mcpu=neoverse-v2
flang: /proj/build/llvm/Linux_aarch64/llvm/lib/CodeGen/RegisterCoalescer.cpp:2375: bool {anonymous}::RegisterCoalescer::joinCopy(llvm::MachineInstr*, bool&, llvm::SmallPtrSetImpl<llvm::MachineInstr*>&): Assertion `SubregToRegSrcInsts.empty() && "can this happen?"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace.
Stack dump:
0.      Program arguments: flang -fc1 -triple aarch64-unknown-linux-gnu -emit-obj -fcolor-diagnostics -mrelocation-model pic -pic-level 2 -pic-is-pie -target-cpu neoverse-v2 -target-feature +outline-atomics -target-feature +v9a -target-feature +bf16 -target-feature +ccidx -target-feature +complxnum -target-feature +crc -target-feature +dotprod -target-feature +ete -target-feature +fp-armv8 -target-feature +fp16fml -target-feature +fpac -target-feature +fullfp16 -target-feature +i8mm -target-feature +jsconv -target-feature +lse -target-feature +mte -target-feature +neon -target-feature +pauth -target-feature +perfmon -target-feature +rand -target-feature +ras -target-feature +rcpc -target-feature +rdm -target-feature +spe -target-feature +ssbs -target-feature +sve -target-feature +sve-bitperm -target-feature +sve2 -target-feature +trbe -floop-interchange -vectorize-loops -vectorize-slp -resource-dir clang/22 -mframe-pointer=non-leaf -O2 -o test3.o -x f95 test3.f90
1.      Running pass 'Function Pass Manager' on module 'FIRModule'.
2.      Running pass 'Register Coalescer' on function '@_QMtest_modPdummy_sub'
 #0 0x0000000004d209e0 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) 
 #1 0x0000000004d1ddb8 llvm::sys::RunSignalHandlers() 
 #2 0x0000000004d1df3c SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #3 0x0000fffe4ffd08dc (linux-vdso.so.1+0x8dc)
 #4 0x0000fffe4fc4f200 __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #5 0x0000fffe4fc0a67c gsignal ./signal/../sysdeps/posix/raise.c:27:6
 #6 0x0000fffe4fbf7130 abort ./stdlib/abort.c:81:7
 #7 0x0000fffe4fc03fd0 __assert_fail_base ./assert/assert.c:89:7
 #8 0x0000fffe4fc04040 __assert_perror_fail ./assert/assert-perr.c:31:1
 #9 0x000000000792b3b0 (anonymous namespace)::RegisterCoalescer::joinCopy(llvm::MachineInstr*, bool&, llvm::SmallPtrSetImpl<llvm::MachineInstr*>&) (.constprop.0) RegisterCoalescer.cpp:0:0
#10 0x000000000792b794 (anonymous namespace)::RegisterCoalescer::copyCoalesceWorkList(llvm::MutableArrayRef<llvm::MachineInstr*>) RegisterCoalescer.cpp:0:0
#11 0x000000000792bb20 (anonymous namespace)::RegisterCoalescer::copyCoalesceInMBB(llvm::MachineBasicBlock*) RegisterCoalescer.cpp:0:0
#12 0x000000000792ee80 (anonymous namespace)::RegisterCoalescer::run(llvm::MachineFunction&) RegisterCoalescer.cpp:0:0
#13 0x000000000792ff6c (anonymous namespace)::RegisterCoalescerLegacy::runOnMachineFunction(llvm::MachineFunction&) RegisterCoalescer.cpp:0:0
#14 0x0000000007733c7c llvm::MachineFunctionPass::runOnFunction(llvm::Function&) (.part.0) MachineFunctionPass.cpp:0:0
#15 0x000000000a25feb8 llvm::FPPassManager::runOnFunction(llvm::Function&) 
#16 0x000000000a260178 llvm::FPPassManager::runOnModule(llvm::Module&) 
#17 0x000000000a260a0c llvm::legacy::PassManagerImpl::run(llvm::Module&) 
#18 0x0000000004d72698 Fortran::frontend::CodeGenAction::executeAction() 
#19 0x0000000004d58e24 Fortran::frontend::FrontendAction::execute() 
#20 0x0000000004d431d4 Fortran::frontend::CompilerInstance::executeAction(Fortran::frontend::FrontendAction&) 
#21 0x0000000004d60378 Fortran::frontend::executeCompilerInvocation(Fortran::frontend::CompilerInstance*) 
#22 0x00000000047190ac fc1_main(llvm::ArrayRef<char const*>, char const*)
#23 0x0000000004718c54 main
#24 0x0000fffe4fbf73fc __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:74:3
#25 0x0000fffe4fbf74cc call_init ./csu/../csu/libc-start.c:128:20
#26 0x0000fffe4fbf74cc __libc_start_main ./csu/../csu/libc-start.c:379:5
#27 0x000000000471736c _start 
flang-22: error: unable to execute command: Aborted
flang-22: error: flang frontend command failed due to signal (use -v to see invocation)
flang version 22.0.0git (https://github.com/llvm/llvm-project b63a9b7a3cdc1e41545df595215077e50bfd04af)
Target: aarch64-unknown-linux-gnu
Thread model: posix
Build config: +assertions
flang-22: note: diagnostic msg:
********************
:~$ flang test3.f90 -O2 -c
:~$ flang test3.f90 -c -mcpu=neoverse-v2
:~$
```
It would seem that the similar "`SubregToRegSrcInsts.empty() && "can this happen?"'" signatures with #151592 implies it's likely related to the same problem. 

## Comments

### Comment 1 - scamp-nvidia

I believe #134408 may be responsible here. Can you please take a look @sdesmalen-arm? Thanks! 

---

### Comment 2 - llvmbot


@llvm/issue-subscribers-backend-aarch64

Author: None (scamp-nvidia)

<details>
Recent Flang builds have begun to fail compiling the example code below (reduced and anonymized from a larger code) with an error message: 


test3.f90:
```
      MODULE TEST_MOD
      INTEGER, PARAMETER :: qq =SELECTED_REAL_KIND(10)
      TYPE type1
      REAL(qq),POINTER :: QONE(:,:) =&gt; NULL()
      REAL(qq),POINTER :: QTWO(:,:) =&gt; NULL()
      INTEGER,POINTER :: IONE(:) =&gt; NULL()
      END TYPE type1

      TYPE type2
      REAL(qq)  ,POINTER :: ARRAY1(:,:,:,:)
      END TYPE type2

      type :: subtype1
      integer :: node = 2
      end type subtype1

      type :: type3
      integer :: intone = 10
      integer :: inttwo  = 10
      type(subtype1) :: comm_line
      end type type3

      CONTAINS

      SUBROUTINE DUMMY_SUB(P,WONE,WTWO)
      IMPLICIT COMPLEX(qq) (C)
      IMPLICIT REAL(qq) (A-B,D-H,O-Z)
      TYPE (type2)    WONE
      TYPE (type3)     WTWO
      TYPE (type1), POINTER :: PPTYPE
      COMPLEX(qq) :: CNUM(10,10,10)
      COMPLEX(qq) :: PHASOR(10,4)

      do ii=1,2
      LLL=PPTYPE%IONE(1)
      MAXX=2*LLL+1
      LM=LLL*LLL
      DO ISPIN=0,10
         DO NBVAR=1,WTWO%INTONE
            CNUM(NBVAR,1,1)= WONE%ARRAY1(ISPIN,NBVAR,1,1)*PPTYPE%QTWO(1,1)*(WONE%ARRAY1(1,NBVAR,1,1))
         ENDDO
      ENDDO
      CALL FLOP(CNUM,1,1,WTWO%INTTWO,.FALSE.)
      IF (1.NE.WTWO%COMM_LINE%NODE-1) write(*,*)
            DO M=1,MAXX
               CTEMPO_VAR= WONE%ARRAY1(M,1,1,1)
               PHASOR(M,1)=PHASOR(M,1)+CTEMPO_VAR
            ENDDO
      enddo
      END SUBROUTINE DUMMY_SUB
      END MODULE TEST_MOD
```
Compiling this with a recent Flang build shows that it fails at "-O2" and with "-mcpu=neoverse-v2", but compiles absent either of those. 

```
scamp@<!-- -->grace2:~$ flang test3.f90 -O2 -c -mcpu=neoverse-v2
flang: /proj/build/llvm/Linux_aarch64/llvm/lib/CodeGen/RegisterCoalescer.cpp:2375: bool {anonymous}::RegisterCoalescer::joinCopy(llvm::MachineInstr*, bool&amp;, llvm::SmallPtrSetImpl&lt;llvm::MachineInstr*&gt;&amp;): Assertion `SubregToRegSrcInsts.empty() &amp;&amp; "can this happen?"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace.
Stack dump:
0.      Program arguments: /proj/nv/llvm/Linux_aarch64/llvm-3590/bin/flang -fc1 -triple aarch64-unknown-linux-gnu -emit-obj -fcolor-diagnostics -mrelocation-model pic -pic-level 2 -pic-is-pie -target-cpu neoverse-v2 -target-feature +outline-atomics -target-feature +v9a -target-feature +bf16 -target-feature +ccidx -target-feature +complxnum -target-feature +crc -target-feature +dotprod -target-feature +ete -target-feature +fp-armv8 -target-feature +fp16fml -target-feature +fpac -target-feature +fullfp16 -target-feature +i8mm -target-feature +jsconv -target-feature +lse -target-feature +mte -target-feature +neon -target-feature +pauth -target-feature +perfmon -target-feature +rand -target-feature +ras -target-feature +rcpc -target-feature +rdm -target-feature +spe -target-feature +ssbs -target-feature +sve -target-feature +sve-bitperm -target-feature +sve2 -target-feature +trbe -floop-interchange -vectorize-loops -vectorize-slp -resource-dir /proj/nv/llvm/Linux_aarch64/llvm-3590/lib/clang/22 -mframe-pointer=non-leaf -O2 -o test3.o -x f95 test3.f90
1.      Running pass 'Function Pass Manager' on module 'FIRModule'.
2.      Running pass 'Register Coalescer' on function '@<!-- -->_QMtest_modPdummy_sub'
 #<!-- -->0 0x0000000004d209e0 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/proj/nv/llvm/Linux_aarch64/llvm-3590/bin/flang+0x4d209e0)
 #<!-- -->1 0x0000000004d1ddb8 llvm::sys::RunSignalHandlers() (/proj/nv/llvm/Linux_aarch64/llvm-3590/bin/flang+0x4d1ddb8)
 #<!-- -->2 0x0000000004d1df3c SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #<!-- -->3 0x0000fffe4ffd08dc (linux-vdso.so.1+0x8dc)
 #<!-- -->4 0x0000fffe4fc4f200 __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #<!-- -->5 0x0000fffe4fc0a67c gsignal ./signal/../sysdeps/posix/raise.c:27:6
 #<!-- -->6 0x0000fffe4fbf7130 abort ./stdlib/abort.c:81:7
 #<!-- -->7 0x0000fffe4fc03fd0 __assert_fail_base ./assert/assert.c:89:7
 #<!-- -->8 0x0000fffe4fc04040 __assert_perror_fail ./assert/assert-perr.c:31:1
 #<!-- -->9 0x000000000792b3b0 (anonymous namespace)::RegisterCoalescer::joinCopy(llvm::MachineInstr*, bool&amp;, llvm::SmallPtrSetImpl&lt;llvm::MachineInstr*&gt;&amp;) (.constprop.0) RegisterCoalescer.cpp:0:0
#<!-- -->10 0x000000000792b794 (anonymous namespace)::RegisterCoalescer::copyCoalesceWorkList(llvm::MutableArrayRef&lt;llvm::MachineInstr*&gt;) RegisterCoalescer.cpp:0:0
#<!-- -->11 0x000000000792bb20 (anonymous namespace)::RegisterCoalescer::copyCoalesceInMBB(llvm::MachineBasicBlock*) RegisterCoalescer.cpp:0:0
#<!-- -->12 0x000000000792ee80 (anonymous namespace)::RegisterCoalescer::run(llvm::MachineFunction&amp;) RegisterCoalescer.cpp:0:0
#<!-- -->13 0x000000000792ff6c (anonymous namespace)::RegisterCoalescerLegacy::runOnMachineFunction(llvm::MachineFunction&amp;) RegisterCoalescer.cpp:0:0
#<!-- -->14 0x0000000007733c7c llvm::MachineFunctionPass::runOnFunction(llvm::Function&amp;) (.part.0) MachineFunctionPass.cpp:0:0
#<!-- -->15 0x000000000a25feb8 llvm::FPPassManager::runOnFunction(llvm::Function&amp;) (/proj/nv/llvm/Linux_aarch64/llvm-3590/bin/flang+0xa25feb8)
#<!-- -->16 0x000000000a260178 llvm::FPPassManager::runOnModule(llvm::Module&amp;) (/proj/nv/llvm/Linux_aarch64/llvm-3590/bin/flang+0xa260178)
#<!-- -->17 0x000000000a260a0c llvm::legacy::PassManagerImpl::run(llvm::Module&amp;) (/proj/nv/llvm/Linux_aarch64/llvm-3590/bin/flang+0xa260a0c)
#<!-- -->18 0x0000000004d72698 Fortran::frontend::CodeGenAction::executeAction() (/proj/nv/llvm/Linux_aarch64/llvm-3590/bin/flang+0x4d72698)
#<!-- -->19 0x0000000004d58e24 Fortran::frontend::FrontendAction::execute() (/proj/nv/llvm/Linux_aarch64/llvm-3590/bin/flang+0x4d58e24)
#<!-- -->20 0x0000000004d431d4 Fortran::frontend::CompilerInstance::executeAction(Fortran::frontend::FrontendAction&amp;) (/proj/nv/llvm/Linux_aarch64/llvm-3590/bin/flang+0x4d431d4)
#<!-- -->21 0x0000000004d60378 Fortran::frontend::executeCompilerInvocation(Fortran::frontend::CompilerInstance*) (/proj/nv/llvm/Linux_aarch64/llvm-3590/bin/flang+0x4d60378)
#<!-- -->22 0x00000000047190ac fc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*) (/proj/nv/llvm/Linux_aarch64/llvm-3590/bin/flang+0x47190ac)
#<!-- -->23 0x0000000004718c54 main (/proj/nv/llvm/Linux_aarch64/llvm-3590/bin/flang+0x4718c54)
#<!-- -->24 0x0000fffe4fbf73fc __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:74:3
#<!-- -->25 0x0000fffe4fbf74cc call_init ./csu/../csu/libc-start.c:128:20
#<!-- -->26 0x0000fffe4fbf74cc __libc_start_main ./csu/../csu/libc-start.c:379:5
#<!-- -->27 0x000000000471736c _start (/proj/nv/llvm/Linux_aarch64/llvm-3590/bin/flang+0x471736c)
flang-22: error: unable to execute command: Aborted
flang-22: error: flang frontend command failed due to signal (use -v to see invocation)
flang version 22.0.0git (https://github.com/llvm/llvm-project b63a9b7a3cdc1e41545df595215077e50bfd04af)
Target: aarch64-unknown-linux-gnu
Thread model: posix
InstalledDir: /proj/nv/llvm/Linux_aarch64/llvm-3590/bin
Build config: +assertions
flang-22: note: diagnostic msg:
********************
scamp@<!-- -->grace2:~$ flang test3.f90 -O2 -c
scamp@<!-- -->grace2:~$ flang test3.f90 -c -mcpu=neoverse-v2
scamp@<!-- -->grace2:~$
```
It would seem that the similar "`SubregToRegSrcInsts.empty() &amp;&amp; "can this happen?"'" signatures with #<!-- -->151592 implies it's likely related to the same problem. 
</details>


---

### Comment 3 - scamp-nvidia

#134408 resolved this issue. 

---

