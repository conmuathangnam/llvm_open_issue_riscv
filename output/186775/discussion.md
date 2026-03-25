# [lldb][Linux][MacOS] Watchpoint dump core if set on same variable after rerun

**Author:** DhruvSrivastavaX
**URL:** https://github.com/llvm/llvm-project/issues/186775
**Status:** Open
**Labels:** lldb, platform:linux, platform:macos, crash, platform:aix

## Body

After setting watchpoint on a variable, if we rerun without exiting lldb, and set watchpoint on the same variable again, lldb coredumps after the first hit on re-run. 
Verified this issue on multiple platforms, including MacOS arm64, Linux x86_64, Linux ppc64 and AIX ppc64. 
Pasting an example of an x86_64 debugging session:
```
(lldb) c
Process 2832575 resuming
Counter after iteration 3: 40

Watchpoint 1 hit:
old value: 40
new value: 50
Process 2832575 stopped
* thread #1, name = 'watchpoint', stop reason = watchpoint 1
    frame #0: 0x0000000000401177 watchpoint`main at watchpoint.c:12:46
   9            // Modify the variable multiple times
   10           for (int i = 0; i < 5; i++) {
   11                   counter += 10;
-> 12                   printf("Counter after iteration %d: %d\n", i, counter);
   13           }
   14  
   15           printf("Final counter: %d\n", counter);
(lldb) c
Process 2832575 resuming
Counter after iteration 4: 50
Final counter: 50
Process 2832575 exited with status = 10 (0x0000000a) 
(lldb) r
Process 2832695 launched: '/root/dhruv/tests/watchpoint' (x86_64)
Process 2832695 stopped
* thread #1, name = 'watchpoint', stop reason = breakpoint 1.1
    frame #0: 0x000000000040113f watchpoint`main at watchpoint.c:5:6
   2   
   3    int main() {
   4   
-> 5            int a = 10;
   6            int counter = 0; 
   7            printf("Initial counter: %d\n", counter);
   8   
(lldb) watchpoint set variable counter
Watchpoint created: Watchpoint 1: addr = 0x7fffffffdd04 size = 4 state = enabled type = m
    declare @ '/root/dhruv/tests/watchpoint.c:6'
    watchpoint spec = 'counter'
    watchpoint resources:
       #0: addr = 0x7fffffffdd04 size = 4
Watchpoint 1 hit:
    
(lldb) c
Process 2832695 resuming
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace.
 #0 0x00007fee97926b5a llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/lib64/libLLVM.so.20.1+0x926b5a)
 #1 0x00007fee97923b54 llvm::sys::RunSignalHandlers() (/lib64/libLLVM.so.20.1+0x923b54)
 #2 0x00007fee979272cb (/lib64/libLLVM.so.20.1+0x9272cb)
 #3 0x00007fee9683ebf0 __restore_rt (/lib64/libc.so.6+0x3ebf0)
 #4 0x00007fee9f16d210 (/lib64/liblldb.so.20.1+0x56d210)
 #5 0x00007fee9f3c7bd9 (/lib64/liblldb.so.20.1+0x7c7bd9)
 #6 0x00007fee9f381c62 (/lib64/liblldb.so.20.1+0x781c62)
 #7 0x00007fee9f382327 (/lib64/liblldb.so.20.1+0x782327)
 #8 0x00007fee9f4892dd (/lib64/liblldb.so.20.1+0x8892dd)
 #9 0x00007fee9f490644 (/lib64/liblldb.so.20.1+0x890644)
#10 0x00007fee9f490c4a (/lib64/liblldb.so.20.1+0x890c4a)
#11 0x00007fee9f18733f (/lib64/liblldb.so.20.1+0x58733f)
#12 0x00007fee9f18d03d (/lib64/liblldb.so.20.1+0x58d03d)
#13 0x00007fee9f25e0c8 (/lib64/liblldb.so.20.1+0x65e0c8)
#14 0x00007fee9688a19a start_thread (/lib64/libc.so.6+0x8a19a)
#15 0x00007fee9690f240 __GI___clone3 (/lib64/libc.so.6+0x10f240)
LLDB diagnostics will be written to /tmp/diagnostics-611756
Please include the directory content when filing a bug report
Segmentation fault (core dumped)
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-lldb

Author: Dhruv Srivastava (DhruvSrivastavaX)

<details>
After setting watchpoint on a variable and letting the execution complete, if we rerun without exiting lldb, and set watchpoint on the same variable again, lldb coredumps after the first hit on re-run. 
Verified this issue on multiple platforms, including MacOS arm64, Linux x86_64, Linux ppc64 and AIX ppc64. 
Pasting an example of an x86_64 debugging session:
```
(lldb) c
Process 2832575 resuming
Counter after iteration 3: 40

Watchpoint 1 hit:
old value: 40
new value: 50
Process 2832575 stopped
* thread #<!-- -->1, name = 'watchpoint', stop reason = watchpoint 1
    frame #<!-- -->0: 0x0000000000401177 watchpoint`main at watchpoint.c:12:46
   9            // Modify the variable multiple times
   10           for (int i = 0; i &lt; 5; i++) {
   11                   counter += 10;
-&gt; 12                   printf("Counter after iteration %d: %d\n", i, counter);
   13           }
   14  
   15           printf("Final counter: %d\n", counter);
(lldb) c
Process 2832575 resuming
Counter after iteration 4: 50
Final counter: 50
Process 2832575 exited with status = 10 (0x0000000a) 
(lldb) r
Process 2832695 launched: '/root/dhruv/tests/watchpoint' (x86_64)
Process 2832695 stopped
* thread #<!-- -->1, name = 'watchpoint', stop reason = breakpoint 1.1
    frame #<!-- -->0: 0x000000000040113f watchpoint`main at watchpoint.c:5:6
   2   
   3    int main() {
   4   
-&gt; 5            int a = 10;
   6            int counter = 0; 
   7            printf("Initial counter: %d\n", counter);
   8   
(lldb) watchpoint set variable counter
Watchpoint created: Watchpoint 1: addr = 0x7fffffffdd04 size = 4 state = enabled type = m
    declare @ '/root/dhruv/tests/watchpoint.c:6'
    watchpoint spec = 'counter'
    watchpoint resources:
       #<!-- -->0: addr = 0x7fffffffdd04 size = 4
Watchpoint 1 hit:
    
(lldb) c
Process 2832695 resuming
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace.
 #<!-- -->0 0x00007fee97926b5a llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/lib64/libLLVM.so.20.1+0x926b5a)
 #<!-- -->1 0x00007fee97923b54 llvm::sys::RunSignalHandlers() (/lib64/libLLVM.so.20.1+0x923b54)
 #<!-- -->2 0x00007fee979272cb (/lib64/libLLVM.so.20.1+0x9272cb)
 #<!-- -->3 0x00007fee9683ebf0 __restore_rt (/lib64/libc.so.6+0x3ebf0)
 #<!-- -->4 0x00007fee9f16d210 (/lib64/liblldb.so.20.1+0x56d210)
 #<!-- -->5 0x00007fee9f3c7bd9 (/lib64/liblldb.so.20.1+0x7c7bd9)
 #<!-- -->6 0x00007fee9f381c62 (/lib64/liblldb.so.20.1+0x781c62)
 #<!-- -->7 0x00007fee9f382327 (/lib64/liblldb.so.20.1+0x782327)
 #<!-- -->8 0x00007fee9f4892dd (/lib64/liblldb.so.20.1+0x8892dd)
 #<!-- -->9 0x00007fee9f490644 (/lib64/liblldb.so.20.1+0x890644)
#<!-- -->10 0x00007fee9f490c4a (/lib64/liblldb.so.20.1+0x890c4a)
#<!-- -->11 0x00007fee9f18733f (/lib64/liblldb.so.20.1+0x58733f)
#<!-- -->12 0x00007fee9f18d03d (/lib64/liblldb.so.20.1+0x58d03d)
#<!-- -->13 0x00007fee9f25e0c8 (/lib64/liblldb.so.20.1+0x65e0c8)
#<!-- -->14 0x00007fee9688a19a start_thread (/lib64/libc.so.6+0x8a19a)
#<!-- -->15 0x00007fee9690f240 __GI___clone3 (/lib64/libc.so.6+0x10f240)
LLDB diagnostics will be written to /tmp/diagnostics-611756
Please include the directory content when filing a bug report
Segmentation fault (core dumped)
```
</details>


---

### Comment 2 - DhruvSrivastavaX

I see an existing issue for the same but the PR was never merged:
https://github.com/llvm/llvm-project/issues/135590
Any particular reason for that?

---

### Comment 3 - zeokin

Hi @DhruvSrivastavaX @EugeneZelenko  Can I contribute to this issue? If possible, could you please assign this to me?

---

### Comment 4 - EugeneZelenko

@zeokin: Just prepare pull request and mention it here.

---

### Comment 5 - zeokin

https://github.com/llvm/llvm-project/issues/186775#issuecomment-4066988526
@EugeneZelenko Could you let me know this is already solved or not?

---

### Comment 6 - DhruvSrivastavaX

@jasonmolenda might be able to tell more on this
FYI @hapeeeeee 

---

### Comment 7 - DhruvSrivastavaX

@zeokin As per my last build 4 days ago, and check on the mentioned platforms, it has not been resolved.
you may take a reference of the above mentioned issue and continue.

---

