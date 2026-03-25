# After 0e46b49de433, Assertion failed: ((PDiffI->getUnitInc() >= 0) == (PNew >= POld) && "PSet overflow/underflow"), function getUpwardPressureDelta, file llvm/lib/CodeGen/RegisterPressure.cpp, line 1180.

**Author:** DimitryAndric
**URL:** https://github.com/llvm/llvm-project/issues/76416
**Status:** Open
**Labels:** llvm:codegen, llvm:regalloc, crash

## Body

After 0e46b49de43349f8cbb2a7d4c6badef6d16e31ae ("Reapply "RegisterCoalescer: Add implicit-def of super register when coalescing SUBREG_TO_REG") I am seeing an assertion failure when building https://github.com/freebsd/freebsd-src/blob/main/sys/dev/fb/vga.c:

```
Assertion failed: ((PDiffI->getUnitInc() >= 0) == (PNew >= POld) && "PSet overflow/underflow"), function getUpwardPressureDelta, file /share/dim/src/llvm/llvm-project/llvm/lib/CodeGen/RegisterPressure.cpp, line 1180.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.      Program arguments: /home/dim/obj/llvmorg-18-init-15692-g007ed0dccd6a-freebsd15-amd64-ninja-clang-rel-1/bin/clang -cc1 -triple x86_64-unknown-freebsd15.0 -S -disable-free -clear-ast-before-backend -mrelocation-model static -mframe-pointer=all -relaxed-aliasing -ffp-contract=on -fno-rounding-math -mconstructor-aliases -ffreestanding -mcmodel=kernel -target-cpu x86-64 -target-feature -mmx -target-feature -sse -target-feature -aes -target-feature -avx -disable-red-zone -no-implicit-float -tune-cpu generic -O2 -Wall -Wstrict-prototypes -Wmissing-prototypes -Wpointer-arith -Wcast-qual -Wundef -Wmissing-include-dirs -Werror -std=gnu99 -fwrapv -stack-protector 1 -fgnuc-version=4.2.1 -vectorize-loops -vectorize-slp -faddrsig vga-0959df.i
1.      <eof> parser at end of file
2.      Code generation
3.      Running pass 'Function Pass Manager' on module 'vga-0959df.c'.
4.      Running pass 'Machine Instruction Scheduler' on function '@vga_save_state'
 #0 0x0000000004248fd8 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/home/dim/obj/llvmorg-18-init-15692-g007ed0dccd6a-freebsd15-amd64-ninja-clang-rel-1/bin/clang+0x4248fd8)
 #1 0x0000000004246d19 llvm::sys::RunSignalHandlers() (/home/dim/obj/llvmorg-18-init-15692-g007ed0dccd6a-freebsd15-amd64-ninja-clang-rel-1/bin/clang+0x4246d19)
 #2 0x0000000004249796 SignalHandler(int) Signals.cpp:0:0
 #3 0x0000000829aa050f handle_signal /usr/src/lib/libthr/thread/thr_sig.c:0:3
 #4 0x0000000829a9facb thr_sighandler /usr/src/lib/libthr/thread/thr_sig.c:245:1
 #5 0x0000000828adb2d3 ([vdso]+0x2d3)
 #6 0x000000082f52a05a _thr_kill /usr/obj/usr/src/amd64.amd64/lib/libc/thr_kill.S:4:0
 #7 0x000000082f4a3b34 __raise /usr/src/lib/libc/gen/raise.c:0:10
 #8 0x000000082f5558f9 abort /usr/src/lib/libc/stdlib/abort.c:67:17
 #9 0x000000082f486e01 (/lib/libc.so.7+0x98e01)
#10 0x00000000039cbcdb (/home/dim/obj/llvmorg-18-init-15692-g007ed0dccd6a-freebsd15-amd64-ninja-clang-rel-1/bin/clang+0x39cbcdb)
#11 0x000000000386f4e2 llvm::GenericScheduler::initCandidate(llvm::GenericSchedulerBase::SchedCandidate&, llvm::SUnit*, bool, llvm::RegPressureTracker const&, llvm::RegPressureTracker&) (/home/dim/obj/llvmorg-18-init-15692-g007ed0dccd6a-freebsd15-amd64-ninja-clang-rel-1/bin/clang+0x386f4e2)
#12 0x000000000386fc61 llvm::GenericScheduler::pickNodeFromQueue(llvm::SchedBoundary&, llvm::GenericSchedulerBase::CandPolicy const&, llvm::RegPressureTracker const&, llvm::GenericSchedulerBase::SchedCandidate&) (/home/dim/obj/llvmorg-18-init-15692-g007ed0dccd6a-freebsd15-amd64-ninja-clang-rel-1/bin/clang+0x386fc61)
#13 0x000000000387070d llvm::GenericScheduler::pickNode(bool&) (/home/dim/obj/llvmorg-18-init-15692-g007ed0dccd6a-freebsd15-amd64-ninja-clang-rel-1/bin/clang+0x387070d)
#14 0x0000000003866b11 llvm::ScheduleDAGMILive::schedule() (/home/dim/obj/llvmorg-18-init-15692-g007ed0dccd6a-freebsd15-amd64-ninja-clang-rel-1/bin/clang+0x3866b11)
#15 0x0000000003873157 (anonymous namespace)::MachineSchedulerBase::scheduleRegions(llvm::ScheduleDAGInstrs&, bool) MachineScheduler.cpp:0:0
#16 0x00000000038729cf (anonymous namespace)::MachineScheduler::runOnMachineFunction(llvm::MachineFunction&) MachineScheduler.cpp:0:0
#17 0x00000000037c9018 llvm::MachineFunctionPass::runOnFunction(llvm::Function&) (/home/dim/obj/llvmorg-18-init-15692-g007ed0dccd6a-freebsd15-amd64-ninja-clang-rel-1/bin/clang+0x37c9018)
#18 0x0000000003d70405 llvm::FPPassManager::runOnFunction(llvm::Function&) (/home/dim/obj/llvmorg-18-init-15692-g007ed0dccd6a-freebsd15-amd64-ninja-clang-rel-1/bin/clang+0x3d70405)
#19 0x0000000003d789e4 llvm::FPPassManager::runOnModule(llvm::Module&) (/home/dim/obj/llvmorg-18-init-15692-g007ed0dccd6a-freebsd15-amd64-ninja-clang-rel-1/bin/clang+0x3d789e4)
#20 0x0000000003d70ff2 llvm::legacy::PassManagerImpl::run(llvm::Module&) (/home/dim/obj/llvmorg-18-init-15692-g007ed0dccd6a-freebsd15-amd64-ninja-clang-rel-1/bin/clang+0x3d70ff2)
#21 0x00000000049af1c1 clang::EmitBackendOutput(clang::DiagnosticsEngine&, clang::HeaderSearchOptions const&, clang::CodeGenOptions const&, clang::TargetOptions const&, clang::LangOptions const&, llvm::StringRef, llvm::Module*, clang::BackendAction, llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>, std::__1::unique_ptr<llvm::raw_pwrite_stream, std::__1::default_delete<llvm::raw_pwrite_stream>>, clang::BackendConsumer*) (/home/dim/obj/llvmorg-18-init-15692-g007ed0dccd6a-freebsd15-amd64-ninja-clang-rel-1/bin/clang+0x49af1c1)
#22 0x00000000049cf912 clang::BackendConsumer::HandleTranslationUnit(clang::ASTContext&) (/home/dim/obj/llvmorg-18-init-15692-g007ed0dccd6a-freebsd15-amd64-ninja-clang-rel-1/bin/clang+0x49cf912)
#23 0x000000000664bdb6 clang::ParseAST(clang::Sema&, bool, bool) (/home/dim/obj/llvmorg-18-init-15692-g007ed0dccd6a-freebsd15-amd64-ninja-clang-rel-1/bin/clang+0x664bdb6)
#24 0x0000000004d95803 clang::FrontendAction::Execute() (/home/dim/obj/llvmorg-18-init-15692-g007ed0dccd6a-freebsd15-amd64-ninja-clang-rel-1/bin/clang+0x4d95803)
#25 0x0000000004d0b3ad clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/home/dim/obj/llvmorg-18-init-15692-g007ed0dccd6a-freebsd15-amd64-ninja-clang-rel-1/bin/clang+0x4d0b3ad)
#26 0x0000000004e6e35c clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/home/dim/obj/llvmorg-18-init-15692-g007ed0dccd6a-freebsd15-amd64-ninja-clang-rel-1/bin/clang+0x4e6e35c)
#27 0x00000000026d7e1a cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/home/dim/obj/llvmorg-18-init-15692-g007ed0dccd6a-freebsd15-amd64-ninja-clang-rel-1/bin/clang+0x26d7e1a)
#28 0x00000000026d5073 ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&) driver.cpp:0:0
#29 0x00000000026d406a clang_main(int, char**, llvm::ToolContext const&) (/home/dim/obj/llvmorg-18-init-15692-g007ed0dccd6a-freebsd15-amd64-ninja-clang-rel-1/bin/clang+0x26d406a)
#30 0x00000000026e2d64 main (/home/dim/obj/llvmorg-18-init-15692-g007ed0dccd6a-freebsd15-amd64-ninja-clang-rel-1/bin/clang+0x26e2d64)
#31 0x000000082f4785ca __libc_start1 /usr/src/lib/libc/csu/libc_start1.c:157:2
Abort trap
```

Reduced test case (C):

```c
// clang -cc1 -triple x86_64-- -S -tune-cpu generic -O2 -std=gnu99 vga-min.c
int outb_port, biosadapters, probe_adapters___trans_tmp_1;
typedef int vi_probe_t(int, int **, void *, int);
typedef int vi_init_t(int, int *, int);
typedef int vi_show_font_t(int *, int);
typedef int vi_save_palette_t(int *, unsigned char *);
typedef int vi_load_palette_t(int *, unsigned char *);
typedef int vi_set_border_t(int *, int);
typedef int vi_save_state_t(int *, void *, unsigned long);
typedef int vi_set_win_org_t(int *, long);
typedef int vi_read_hw_cursor_t(int *, int *, int *);
typedef int vi_set_hw_cursor_t(int *, int, int);
typedef int vi_set_hw_cursor_shape_t(int *, int, int, int, int);
typedef int vi_blank_display_t(int *, int);
typedef int vi_mmap_t(int *, unsigned long, unsigned long *, int, char *);
typedef int vi_fill_rect_t(int *, int, int, int, int, int);
typedef int vi_bitblt_t(int *, ...);
typedef struct {
  vi_init_t *init;
  vi_show_font_t *show_font;
  vi_save_palette_t *save_palette;
  vi_load_palette_t *load_palette;
  vi_set_border_t *set_border;
  vi_save_state_t *save_state;
  vi_set_win_org_t *set_win_org;
  vi_read_hw_cursor_t *read_hw_cursor;
  vi_set_hw_cursor_t *set_hw_cursor;
  vi_set_hw_cursor_shape_t *set_hw_cursor_shape;
  vi_blank_display_t *blank_display;
  vi_mmap_t *mmap;
  vi_fill_rect_t *fill_rect;
  vi_bitblt_t *bitblt;
  int (*reserved2)(void);
} video_switch_t;
struct video_driver {
  int (*configure)(int);
};
struct {
  char regs[4];
} *vga_load_state_p;
char vga_load_state_data;
static int vga_configure(int flags) { return biosadapters; }
static int vga_error(void) { return 9; }
vi_probe_t vga_probe;
static int vga_init(int unit, int *adp, int flags) { return 0; }
static int vga_show_font(int *adp, int page) { return 0; }
static int vga_save_palette(int *adp, unsigned char *palette) { return 0; }
static int vga_load_palette(int *adp, unsigned char *palette) { return 0; }
static int vga_set_border(int *adp, int color) { return 0; }
static int vga_save_state(int *adp, void *p, unsigned long size) { return 0; }
static int vga_load_state(void) {
  int i;
  for (i = 0; i < 4; ++i) {
    char data = 0;
    __asm("" : : "a"(data));
  }
  for (i = 0; i < 20; ++i) {
    __asm("" : : "Nd"(outb_port));
    vga_load_state_data = vga_load_state_p->regs[i];
    __asm("");
  }
  return 0;
}
static int vga_set_origin(int *adp, long offset) { return 9; }
static int vga_read_hw_cursor(int *adp, int *col, int *row) { return 0; }
static int vga_set_hw_cursor(int *adp, int col, int row) { return 0; }
static int vga_set_hw_cursor_shape(int *adp, int base, int height, int celsize,
                                   int blink) {
  return 0;
}
static int vga_blank_display(int *adp, int mode) { return 0; }
static int vga_mmap_buf(int *adp, unsigned long offset, unsigned long *paddr,
                        int prot, char *memattr) {
  return 0;
}
static int vga_fill_rect(int *adp, int val, int x, int y, int cx, int cy) {
  return 9;
}
static int vga_bitblt(int *adp, ...) { return 9; }
video_switch_t vgavidsw = {vga_init,          vga_show_font,
                           vga_save_palette,  vga_load_palette,
                           vga_set_border,    vga_save_state,
                           vga_set_origin,    vga_read_hw_cursor,
                           vga_set_hw_cursor, vga_set_hw_cursor_shape,
                           vga_blank_display, vga_mmap_buf,
                           vga_fill_rect,     vga_bitblt,
                           vga_error};
struct video_driver vga_driver = {vga_configure};
static void clear_mode_map(void) {}
static int map_bios_mode_num(void) { return 1; }
static int probe_adapters(void) {
  map_bios_mode_num();
  switch (probe_adapters___trans_tmp_1)
    clear_mode_map;
  return biosadapters;
}
static void fill(void) {}
static void filll_io(void) {}
int vga_probe(int unit, int **adpp, void *arg, int flags) {
  probe_adapters();
  vga_load_state();
  return 0;
  fill;
  filll_io;
}
```

Further reduced with bugpoint:

```ll
; ModuleID = 'bugpoint-reduced-simplified.bc'
source_filename = "vga-min.c"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-freebsd15.0"

%struct.video_switch_t.3.26.76.97.147.117 = type { ptr, ptr, ptr, ptr, ptr, ptr, ptr, ptr, ptr, ptr, ptr, ptr, ptr, ptr, ptr }
%struct.video_driver.4.27.77.98.148.118 = type { ptr }
%struct.anon.5.28.78.99.149.119 = type { [4 x i8] }

@vgavidsw = external dso_local global %struct.video_switch_t.3.26.76.97.147.117, align 8
@vga_driver = external dso_local global %struct.video_driver.4.27.77.98.148.118, align 8
@outb_port = external dso_local global i32, align 4
@biosadapters = external dso_local global i32, align 4
@probe_adapters___trans_tmp_1 = external dso_local global i32, align 4
@vga_load_state_p = external dso_local global ptr, align 8
@vga_load_state_data = external dso_local global i8, align 1

declare dso_local void @vga_init() #0

declare dso_local void @vga_show_font() #0

declare dso_local void @vga_save_palette() #0

declare dso_local void @vga_load_palette() #0

declare dso_local void @vga_set_border() #0

declare dso_local void @vga_save_state() #0

declare dso_local void @vga_set_origin() #0

declare dso_local void @vga_read_hw_cursor() #0

declare dso_local void @vga_set_hw_cursor() #0

declare dso_local void @vga_set_hw_cursor_shape() #0

declare dso_local void @vga_blank_display() #0

declare dso_local void @vga_mmap_buf() #0

declare dso_local void @vga_fill_rect() #0

declare dso_local void @vga_bitblt(ptr, ...) #0

declare dso_local void @vga_error() #0

declare dso_local void @vga_configure() #0

declare dso_local void @vga_probe() #0

declare dso_local void @probe_adapters() #0

define dso_local void @vga_load_state() #0 {
entry:
  %i = alloca i32, align 4
  store i32 0, ptr %i, align 4
  br label %for.cond

for.cond:                                         ; preds = %for.body, %entry
  %0 = load i32, ptr %i, align 4
  %cmp = icmp slt i32 %0, 4
  br i1 %cmp, label %for.body, label %for.end

for.body:                                         ; preds = %for.cond
  call void asm sideeffect "", "{ax},~{dirflag},~{fpsr},~{flags}"(i8 0) #2
  %1 = load i32, ptr %i, align 4
  %inc = add nsw i32 %1, 1
  store i32 %inc, ptr %i, align 4
  br label %for.cond

for.end:                                          ; preds = %for.cond
  store i32 0, ptr %i, align 4
  br label %for.cond1

for.cond1:                                        ; preds = %for.cond1, %for.end
  call void asm sideeffect "", "N{dx},~{dirflag},~{fpsr},~{flags}"(i32 poison) #2
  %2 = load ptr, ptr @vga_load_state_p, align 8
  %regs = getelementptr inbounds %struct.anon.5.28.78.99.149.119, ptr %2, i32 0, i32 0
  %3 = load i32, ptr %i, align 4
  %idxprom = sext i32 %3 to i64
  %arrayidx = getelementptr inbounds [4 x i8], ptr %regs, i64 0, i64 %idxprom
  %4 = load i8, ptr %arrayidx, align 1
  store i8 %4, ptr @vga_load_state_data, align 1
  %5 = load i32, ptr %i, align 4
  %inc5 = add nsw i32 %5, 1
  store i32 %inc5, ptr %i, align 4
  br label %for.cond1, !llvm.loop !1
}

declare dso_local void @map_bios_mode_num() #0

; Function Attrs: nocallback nofree nosync nounwind willreturn memory(argmem: readwrite)
declare void @llvm.lifetime.start.p0(i64 immarg, ptr nocapture) #1

; Function Attrs: nocallback nofree nosync nounwind willreturn memory(argmem: readwrite)
declare void @llvm.lifetime.end.p0(i64 immarg, ptr nocapture) #1

attributes #0 = { "tune-cpu"="generic" }
attributes #1 = { nocallback nofree nosync nounwind willreturn memory(argmem: readwrite) }
attributes #2 = { nounwind }

!llvm.ident = !{!0}

!0 = !{!"clang version 18.0.0git (https://github.com/llvm/llvm-project.git 007ed0dccd6a3d19f331eb7cd91438d792754439)"}
!1 = distinct !{!1, !2}
!2 = !{!"llvm.loop.mustprogress"}
```



## Comments

### Comment 1 - arsenm

Reduced MIR test:

```
 # RUN: llc -mtriple=x86_64-unknown-unknown -run-pass=machine-scheduler -verify-misched -o - %s

---
name:            vga_load_state
tracksRegLiveness: true
body:             |
  bb.0:
    liveins: $rdi

    %0:gr64 = COPY $rdi
    undef %1.sub_32bit:gr64_nosp = MOV32r0 implicit-def dead $eflags
    dead $eax = MOV32r0 implicit-def dead $eflags, implicit-def $rax
    INLINEASM &"", 1 /* sideeffect attdialect */
    dead $eax = MOV32r0 implicit-def dead $eflags
    INLINEASM &"", 1 /* sideeffect attdialect */
    dead $eax = MOV32r0 implicit-def dead $eflags
    dead $eax = MOV32r0 implicit-def dead $eflags, implicit-def $rax

...
```


---

### Comment 2 - DimitryAndric

So maybe for the time being, we should revert "RegisterCoalescer: Add implicit-def of super register when coalescing SUBREG_TO_REG" yet again? Or is that now too much hassle? It seems to have gone in and out several times now... :)


---

### Comment 3 - DimitryAndric

(Still asserts with llvmorg-18-init-15938-g c1764a7842ac)

---

### Comment 4 - thevinster

I'm also seeing a similar issue of this and it fails similarly when cross-compiling for windows assert for release/15.x and release/16.x.  Reverting 0e46b49de433 seems to solve the issue for me as well. Thanks for the revert!

---

### Comment 5 - DimitryAndric

I haven't yet reverted it on main, just on my FreeBSD branch. If Matt currently doesn't have time to fix the underlying problem, I will revert it later today.


---

### Comment 6 - thevinster

@arsenm Are there any updates on this? If this is taking a while to fix, could we please revert the patch so that downstream builds could continue to build? 

---

### Comment 7 - arsenm

> @arsenm Are there any updates on this? If this is taking a while to fix, could we please revert the patch so that downstream builds could continue to build?

I've been debugging it for a few days; it's a pre-existing RegisterPressure/Scheduler bug but I'm not really getting anywhere with it. The symptoms disappear with -precompute-phys-liveness. The pathway that doesn't use accurate liveness information seems to not work correctly with multiple defs of overlapping registers 

---

### Comment 8 - arsenm

This was reverted in c4146121e940b6b853148c780568dee38b97382f. Another reproducer was posted at https://github.com/llvm/llvm-project/commit/0e46b49de43349f8cbb2a7d4c6badef6d16e31ae

---

### Comment 9 - arsenm

More reduced test case:

```
# RUN: llc -mtriple=x86_64-unknown-unknown -run-pass=machine-scheduler -misched-bottomup -verify-misched -o - %s
# xUN: llc -mtriple=x86_64-unknown-unknown -run-pass=machine-scheduler -misched-topdown -verify-misched -o - %s
# xUN: llc -mtriple=x86_64-unknown-unknown -run-pass=machine-scheduler -misched-topdown -o - %s

---
name:            vga_load_state
tracksRegLiveness: true
body:             |
  bb.0:
    liveins: $rdi

    INLINEASM &"", 1 /* sideeffect attdialect */
    INLINEASM &"", 1 /* sideeffect attdialect */
    INLINEASM &"", 1 /* sideeffect attdialect */
    INLINEASM &"", 1 /* sideeffect attdialect */
    $ebx = MOV32r0 implicit-def $eflags
    dead $ecx = MOV32r0 implicit-def $eflags
    $eax = MOV32r0 implicit-def $eflags
    $eax = MOV32r0 implicit-def $eflags

...

```

---

### Comment 10 - arsenm

Adding a real use of $eax makes this no longer sensitive to having cached liveness info

```
---
name:            dead_register_def_pressure_tracking_underflow_assert
tracksRegLiveness: true
body:             |
  bb.0:
    INLINEASM &"", 1 /* sideeffect attdialect */
    INLINEASM &"", 1 /* sideeffect attdialect */
    INLINEASM &"", 1 /* sideeffect attdialect */
    INLINEASM &"", 1 /* sideeffect attdialect */
    $ebx = MOV32r0 implicit-def $eflags
    dead $ecx = MOV32r0 implicit-def $eflags
    $eax = MOV32r1 implicit-def $eflags
    $eax = MOV32r0 implicit-def $eflags

  bb.1:
    liveins: $eax
    INLINEASM &"", 1, implicit $eax
...
```

---

### Comment 11 - arsenm

Even simpler: 
```
---
name:            dead_register_def_pressure_tracking_underflow_assert_onereg
tracksRegLiveness: true
body:             |
 bb.0:
   INLINEASM &"", 1 /* sideeffect attdialect */
   INLINEASM &"", 1 /* sideeffect attdialect */
   INLINEASM &"", 1 /* sideeffect attdialect */
   INLINEASM &"", 1 /* sideeffect attdialect */
   INLINEASM &"", 1 /* sideeffect attdialect */
   INLINEASM &"", 1 /* sideeffect attdialect */
   $eax = MOV32r1 implicit-def $eflags
   $eax = MOV32r0 implicit-def $eflags

 bb.1:
   liveins: $eax
   INLINEASM &"", 1, implicit $eax
...

```

This passes when the dead flag is applied to the first $eax def. (actually it passes with a dead on the second one too, where we have a missing verifier error). The pressure diff is negative without the dead flag for the SU for the first mov. There's no pressure diff with the dead flag.

I've stared at this for an extremely long time, and I'm stuck. It seems to me the logic of upward tracking is essentially faulty. It's implicitly relying on accurate dead flags. It assumes if it sees a live def, it will be used before a redef. There is detectDeadDefs, but it isn't always used. It also tries to use cached regunit results, so you cannot rely on that either.  I'm also not sure why this is using LiveIntervals. If it's stepping backwards over every instruction, it should have exact liveness information.


---

### Comment 12 - arsenm

I wanted to look into whether more reliable dead flag preservation would help the original testcase, but I can't reproduce with the patch reapplied anymore 

---

### Comment 13 - jrbyrnes

Can we actually rely on accurate PDiffs for PhysRegs? https://github.com/llvm/llvm-project/blob/7a28192ce1c1d9d0398348eabc46c94eadb317d8/llvm/lib/CodeGen/MachineScheduler.cpp#L1364

---

### Comment 14 - jrbyrnes

Can hack things to repro the PDiff crash for AMDGPU by using standard scheduler


```
ScheduleDAGInstrs *MachineScheduler::createMachineScheduler() {
  /*
  // Select the scheduler, or set the default.
  MachineSchedRegistry::ScheduleDAGCtor Ctor = MachineSchedOpt;
  if (Ctor != useDefaultMachineSched)
    return Ctor(this);

  // Get the default scheduler set by the target for this function.
  ScheduleDAGInstrs *Scheduler = PassConfig->createMachineScheduler(this);
  if (Scheduler)
    return Scheduler;

  // Default to GenericScheduler.
  */
  return createGenericSchedLive(this);
}
```


```
# RUN: llc -mtriple=amdgcn -run-pass=machine-scheduler -misched-bottomup -verify-misched -o - %s

---
name:            pdiff
tracksRegLiveness: true
body:             |
  bb.0:
    INLINEASM &"", 1 /* sideeffect attdialect */
    INLINEASM &"", 1 /* sideeffect attdialect */
    $vgpr1 = V_MOV_B32_e32 0, implicit-def $vgpr2, implicit $exec
    $vgpr1 = V_MOV_B32_e32 0, implicit-def $vgpr2, implicit $exec

...
```

---

### Comment 15 - sdesmalen-arm

From reading this conversation, my understanding is that 0e46b49de43349f8cbb2a7d4c6badef6d16e31ae triggered a bug in the scheduler/regpressure-tracking, which requires accurate `dead` flags on register operands and/or an accurate Liveinterval cache for all used physregs.

I tried to reproduce the issue and found that:
* The scheduler issue still exists, because passing in the reduced MIR from [above](https://github.com/llvm/llvm-project/issues/76416#issuecomment-2032074817) directly to the scheduler still fails the assert `(PDiffI->getUnitInc() >= 0) == (PNew >= POld) && "PSet overflow/underflow"`.
* The original patch (0e46b49de43349f8cbb2a7d4c6badef6d16e31ae) no longer triggers this issue, because re-applying the patch to `main` and compiling the reproducers mentioned [here](https://github.com/llvm/llvm-project/issues/76416#issue-2056763707) and [here](https://github.com/llvm/llvm-project/commit/0e46b49de43349f8cbb2a7d4c6badef6d16e31ae#commitcomment-136147998) no longer fail.
* The reason they no longer fail is because of this patch: #121734.

If I understand things correctly then:
* 0e46b49de43349f8cbb2a7d4c6badef6d16e31ae adds the implicit-def of the super-register. When the register is a physreg, then the Coalescer should make sure LiveIntervals tracks the (previously unconsidered) regunit.
* #121734 does this by fluke as a side-effect of calling [`LIS->getRegUnit(Unit)`](https://github.com/llvm/llvm-project/pull/121734/files#diff-b3dcaed3424cb564d701200d64cf65f81db57987b01746ff75bb33eb4d1f0094R1357) when trying to determine if an instruction is rematerializable.
* Alternatively, compiling with `-precompute-phys-liveness` populates live-ranges for all regunits from phys-regs, but I'd still expect the coalescer must do something to ensure their liveranges are updated. Also, there is probably a reason this option is not enabled by default?

I think we should be able to reland a modified version of 0e46b49de43349f8cbb2a7d4c6badef6d16e31ae even if there is a latent bug in the scheduler, as long as the MIR has the correct dead flags and regunits are correctly updated in the cache. Is that a fair assessment?

Just a bit of context of why I'm looking into this. I am trying to enable subreg-liveness tracking (SRLT) for AArch64 and found that a missing `implicit-def <superreg>` after coalescing a SUBREG_TO_REG results in broken MIR. The implicit-def of the super-reg is needed to represent the zeroing of the top lanes done by the MOV (as described the SUBREG_TO_REG). But if this information is lost by coalescing, then the machine constant-propagation pass thinks it can remove the MOV, which means the top bits of the register are no longer zeroed. It seems this patch tried to fix that issue, although the patch does not yet work when SRLT is enabled (there is a FIXME+assert to say that the lanemasks need updating).

@arsenm, what is the best way forward here?  I'd be happy to help out with re-landing this if you can use the help.

---

### Comment 16 - arsenm

> Alternatively, compiling with -precompute-phys-liveness populates live-ranges for all regunits from phys-regs, but I'd still expect the coalescer must do something to ensure their liveranges are updated. 

Yes, I remember this property. It's part of why it was a pain to debug

> Also, there is probably a reason this option is not enabled by default? 

Compile time. Most of the time you shouldn't need to know the liveness for physical registers until later in the process

> I think we should be able to reland a modified version of https://github.com/llvm/llvm-project/commit/0e46b49de43349f8cbb2a7d4c6badef6d16e31ae even if there is a latent bug in the scheduler, as long as the MIR has the correct dead flags and regunits are correctly updated in the cache. Is that a fair assessment?

More or less, I don't have any particular interest in the scheduler bug beyond surviving reverts. I've failed enough times at debugging it to leave it for someone who know the scheduler better (we should probably commit an xfailed version of the MIR test for tracking purposes). It happens to compute the needed liveness for this one register, but it's still possible to lose that liveness information later. In any case, this is still another form of getting lucky to avoid breaking the test. 


> @arsenm, what is the best way forward here? I'd be happy to help out with re-landing this if you can use the help.

If we get can get this relanded, that would be great. I had other work on top of this that I've forgotten about. 

I also still think we should get rid of SUBREG_TO_REG. It's structurally broken and requires this hackery, but it's a big project.


---

