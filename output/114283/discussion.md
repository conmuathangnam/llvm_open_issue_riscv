# Unaligned accesses on MIPS

**Author:** purplesyringa
**URL:** https://github.com/llvm/llvm-project/issues/114283
**Status:** Closed
**Labels:** backend:MIPS, miscompilation
**Closed Date:** 2025-02-11T07:18:13Z

## Body

LLVM occasionally emits unaligned `lw` calls on 32-bit MIPS. This leads to `SIGBUS` in all sorts of programs. Unaligned access is sometimes emulated by the Linux kernel, but it's opt-in and inefficient, so better not do that.

I'm having trouble reproducing this in C, because this bug only seems to be triggered on complex code. I have a Rust reproducer though:

```rust
use core::mem::ManuallyDrop;

fn noop() {}

fn meow_new<E>(cause: E) -> ManuallyDrop<E> {
    noop();
    ManuallyDrop::new(cause)
}

#[repr(packed)]
struct Unaligned(i32);

fn main() {
    meow_new(Unaligned(123i32));
}
```

```
$ rustc -Vv
rustc 1.84.0-nightly (439284741 2024-10-21)
binary: rustc
commit-hash: 4392847410ddd67f6734dd9845f9742ff9e85c83
commit-date: 2024-10-21
host: x86_64-unknown-linux-gnu
release: 1.84.0-nightly
LLVM version: 19.1.1

$ cross run --target mips-unknown-linux-gnu -Zbuild-std
```

...and the relevant LLVM IR:

[llvm-ir.txt](https://github.com/user-attachments/files/17577199/llvm-ir.txt)

The function `meow_new` is compiled to:

<details><summary>Disassembly</summary>

```asm
000a7fb8 <_ZN7garbage8meow_new17he7e271863f5ba90fE>:
   a7fb8:       3c020023        lui     v0,0x23
   a7fbc:       24421098        addiu   v0,v0,4248
   a7fc0:       27bdffb0        addiu   sp,sp,-80
   a7fc4:       afbf004c        sw      ra,76(sp)
   a7fc8:       00590821        addu    at,v0,t9
   a7fcc:       afa10018        sw      at,24(sp)
   a7fd0:       afa40020        sw      a0,32(sp)
   a7fd4:       8fa20020        lw      v0,32(sp)
   a7fd8:       afa2001c        sw      v0,28(sp)
   a7fdc:       24020000        li      v0,0
   a7fe0:       a3a0002f        sb      zero,47(sp)
   a7fe4:       24020001        li      v0,1
   a7fe8:       a3a2002f        sb      v0,47(sp)
   a7fec:       8c218038        lw      at,-32712(at)
   a7ff0:       24397fb0        addiu   t9,at,32688
   a7ff4:       0411ffee        bal     a7fb0 <_ZN7garbage4noop17h890d2a6d84bbb6fdE>
   a7ff8:       00000000        nop
   a7ffc:       1000000b        b       a802c <_ZN7garbage8meow_new17he7e271863f5ba90fE+0x74>
   a8000:       00000000        nop
   a8004:       93a1002f        lbu     at,47(sp)
   a8008:       30210001        andi    at,at,0x1
   a800c:       1c20003c        bgtz    at,a8100 <_ZN7garbage8meow_new17he7e271863f5ba90fE+0x148>
   a8010:       00000000        nop
   a8014:       10000035        b       a80ec <_ZN7garbage8meow_new17he7e271863f5ba90fE+0x134>
   a8018:       00000000        nop
   a801c:       afa40030        sw      a0,48(sp)
   a8020:       afa50034        sw      a1,52(sp)
   a8024:       1000fff7        b       a8004 <_ZN7garbage8meow_new17he7e271863f5ba90fE+0x4c>
   a8028:       00000000        nop
   a802c:       8fa10018        lw      at,24(sp)
   a8030:       24020000        li      v0,0
   a8034:       a3a0002f        sb      zero,47(sp)
   a8038:       27a4002b        addiu   a0,sp,43
   a803c:       27a5001c        addiu   a1,sp,28
   a8040:       24060004        li      a2,4
   a8044:       8c39dd78        lw      t9,-8840(at)
   a8048:       0320f809        jalr    t9
   a804c:       00000000        nop
   a8050:       8fa10018        lw      at,24(sp)
   a8054:       27a40038        addiu   a0,sp,56
   a8058:       27a5002b        addiu   a1,sp,43
   a805c:       24060004        li      a2,4
   a8060:       8c39dd78        lw      t9,-8840(at)
   a8064:       0320f809        jalr    t9
   a8068:       00000000        nop
   a806c:       8fa10018        lw      at,24(sp)
   a8070:       8fa20038        lw      v0,56(sp)
   a8074:       afa20044        sw      v0,68(sp)
   a8078:       27a40040        addiu   a0,sp,64
   a807c:       27a50044        addiu   a1,sp,68
   a8080:       24060004        li      a2,4
   a8084:       8c39dd78        lw      t9,-8840(at)
   a8088:       0320f809        jalr    t9
   a808c:       00000000        nop
   a8090:       8fa10018        lw      at,24(sp)
   a8094:       27a40048        addiu   a0,sp,72
   a8098:       27a50040        addiu   a1,sp,64
   a809c:       24060004        li      a2,4
   a80a0:       8c39dd78        lw      t9,-8840(at)
   a80a4:       0320f809        jalr    t9
   a80a8:       00000000        nop
   a80ac:       8fa10048        lw      at,72(sp)
   a80b0:       afa10014        sw      at,20(sp)
   a80b4:       8fa10018        lw      at,24(sp)
   a80b8:       8fa20014        lw      v0,20(sp)
   a80bc:       afa2003c        sw      v0,60(sp)
   a80c0:       27a40027        addiu   a0,sp,39
   a80c4:       27a5003c        addiu   a1,sp,60
   a80c8:       24060004        li      a2,4
   a80cc:       8c39dd78        lw      t9,-8840(at)
   a80d0:       0320f809        jalr    t9
   a80d4:       00000000        nop
   a80d8:       8fa20027        lw      v0,39(sp)
   a80dc:       8fbf004c        lw      ra,76(sp)
   a80e0:       27bd0050        addiu   sp,sp,80
   a80e4:       03e00008        jr      ra
   a80e8:       00000000        nop
   a80ec:       8fbc0018        lw      gp,24(sp)
   a80f0:       8fa40030        lw      a0,48(sp)
   a80f4:       8f99dd20        lw      t9,-8928(gp)
   a80f8:       0320f809        jalr    t9
   a80fc:       00000000        nop
   a8100:       1000fffa        b       a80ec <_ZN7garbage8meow_new17he7e271863f5ba90fE+0x134>
   a8104:       00000000        nop
```

</details>

The relevant part is this instruction:

```asm
   a80d8:       8fa20027        lw      v0,39(sp)
```

`sp` is aligned at this point, so this access triggers an unaligned read.

I understand this isn't a lot of helpful information, but I'm not sure what else I can provide on this bug, so please ask if you need anything for debugging.

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-backend-mips

Author: Alisa Sireneva (purplesyringa)

<details>
LLVM occasionally emits unaligned `lw` calls on 32-bit MIPS. This leads to `SIGBUS` in all sorts of programs. Unaligned access is sometimes emulated by the Linux kernel, but it's opt-in and inefficient, so better not do that.

I'm having trouble reproducing this in C, because this bug only seems to be triggered on complex code. I have a Rust reproducer though:

```rust
use core::mem::ManuallyDrop;

fn noop() {}

fn meow_new&lt;E&gt;(cause: E) -&gt; ManuallyDrop&lt;E&gt; {
    noop();
    ManuallyDrop::new(cause)
}

#[repr(packed)]
struct Unaligned(i32);

fn main() {
    meow_new(Unaligned(123i32));
}
```

```
$ rustc -Vv
rustc 1.84.0-nightly (439284741 2024-10-21)
binary: rustc
commit-hash: 4392847410ddd67f6734dd9845f9742ff9e85c83
commit-date: 2024-10-21
host: x86_64-unknown-linux-gnu
release: 1.84.0-nightly
LLVM version: 19.1.1

$ cross run --target mips-unknown-linux-gnu -Zbuild-std
```

...and the relevant LLVM IR:

[llvm-ir.txt](https://github.com/user-attachments/files/17577199/llvm-ir.txt)

The function `meow_new` is compiled to:

&lt;details&gt;&lt;summary&gt;Disassembly&lt;/summary&gt;

```asm
000a7fb8 &lt;_ZN7garbage8meow_new17he7e271863f5ba90fE&gt;:
   a7fb8:       3c020023        lui     v0,0x23
   a7fbc:       24421098        addiu   v0,v0,4248
   a7fc0:       27bdffb0        addiu   sp,sp,-80
   a7fc4:       afbf004c        sw      ra,76(sp)
   a7fc8:       00590821        addu    at,v0,t9
   a7fcc:       afa10018        sw      at,24(sp)
   a7fd0:       afa40020        sw      a0,32(sp)
   a7fd4:       8fa20020        lw      v0,32(sp)
   a7fd8:       afa2001c        sw      v0,28(sp)
   a7fdc:       24020000        li      v0,0
   a7fe0:       a3a0002f        sb      zero,47(sp)
   a7fe4:       24020001        li      v0,1
   a7fe8:       a3a2002f        sb      v0,47(sp)
   a7fec:       8c218038        lw      at,-32712(at)
   a7ff0:       24397fb0        addiu   t9,at,32688
   a7ff4:       0411ffee        bal     a7fb0 &lt;_ZN7garbage4noop17h890d2a6d84bbb6fdE&gt;
   a7ff8:       00000000        nop
   a7ffc:       1000000b        b       a802c &lt;_ZN7garbage8meow_new17he7e271863f5ba90fE+0x74&gt;
   a8000:       00000000        nop
   a8004:       93a1002f        lbu     at,47(sp)
   a8008:       30210001        andi    at,at,0x1
   a800c:       1c20003c        bgtz    at,a8100 &lt;_ZN7garbage8meow_new17he7e271863f5ba90fE+0x148&gt;
   a8010:       00000000        nop
   a8014:       10000035        b       a80ec &lt;_ZN7garbage8meow_new17he7e271863f5ba90fE+0x134&gt;
   a8018:       00000000        nop
   a801c:       afa40030        sw      a0,48(sp)
   a8020:       afa50034        sw      a1,52(sp)
   a8024:       1000fff7        b       a8004 &lt;_ZN7garbage8meow_new17he7e271863f5ba90fE+0x4c&gt;
   a8028:       00000000        nop
   a802c:       8fa10018        lw      at,24(sp)
   a8030:       24020000        li      v0,0
   a8034:       a3a0002f        sb      zero,47(sp)
   a8038:       27a4002b        addiu   a0,sp,43
   a803c:       27a5001c        addiu   a1,sp,28
   a8040:       24060004        li      a2,4
   a8044:       8c39dd78        lw      t9,-8840(at)
   a8048:       0320f809        jalr    t9
   a804c:       00000000        nop
   a8050:       8fa10018        lw      at,24(sp)
   a8054:       27a40038        addiu   a0,sp,56
   a8058:       27a5002b        addiu   a1,sp,43
   a805c:       24060004        li      a2,4
   a8060:       8c39dd78        lw      t9,-8840(at)
   a8064:       0320f809        jalr    t9
   a8068:       00000000        nop
   a806c:       8fa10018        lw      at,24(sp)
   a8070:       8fa20038        lw      v0,56(sp)
   a8074:       afa20044        sw      v0,68(sp)
   a8078:       27a40040        addiu   a0,sp,64
   a807c:       27a50044        addiu   a1,sp,68
   a8080:       24060004        li      a2,4
   a8084:       8c39dd78        lw      t9,-8840(at)
   a8088:       0320f809        jalr    t9
   a808c:       00000000        nop
   a8090:       8fa10018        lw      at,24(sp)
   a8094:       27a40048        addiu   a0,sp,72
   a8098:       27a50040        addiu   a1,sp,64
   a809c:       24060004        li      a2,4
   a80a0:       8c39dd78        lw      t9,-8840(at)
   a80a4:       0320f809        jalr    t9
   a80a8:       00000000        nop
   a80ac:       8fa10048        lw      at,72(sp)
   a80b0:       afa10014        sw      at,20(sp)
   a80b4:       8fa10018        lw      at,24(sp)
   a80b8:       8fa20014        lw      v0,20(sp)
   a80bc:       afa2003c        sw      v0,60(sp)
   a80c0:       27a40027        addiu   a0,sp,39
   a80c4:       27a5003c        addiu   a1,sp,60
   a80c8:       24060004        li      a2,4
   a80cc:       8c39dd78        lw      t9,-8840(at)
   a80d0:       0320f809        jalr    t9
   a80d4:       00000000        nop
   a80d8:       8fa20027        lw      v0,39(sp)
   a80dc:       8fbf004c        lw      ra,76(sp)
   a80e0:       27bd0050        addiu   sp,sp,80
   a80e4:       03e00008        jr      ra
   a80e8:       00000000        nop
   a80ec:       8fbc0018        lw      gp,24(sp)
   a80f0:       8fa40030        lw      a0,48(sp)
   a80f4:       8f99dd20        lw      t9,-8928(gp)
   a80f8:       0320f809        jalr    t9
   a80fc:       00000000        nop
   a8100:       1000fffa        b       a80ec &lt;_ZN7garbage8meow_new17he7e271863f5ba90fE+0x134&gt;
   a8104:       00000000        nop
```

&lt;/details&gt;

The relevant part is this instruction:

```asm
   a80d8:       8fa20027        lw      v0,39(sp)
```

`sp` is aligned at this point, so this access triggers an unaligned read.

I understand this isn't a lot of helpful information, but I'm not sure what else I can provide on this bug, so please ask if you need anything for debugging.
</details>


---

### Comment 2 - purplesyringa

For what it's worth, here's another user reporting ostensibly the same bug in C code, so it can't just be a Rust frontend issue:

https://forums.sgi.sh/index.php?threads/llvm-progress-update.601/#post-3669

---

### Comment 3 - efriedma-quic

Something that reproduces the issue without installing Rust would help... I briefly tried just compiling the IR you gave with llc, but I see a different result.

If you can't figure it out, maybe try opening a bug in the Rust repo.

---

### Comment 4 - purplesyringa

There's already a downstream issue. I'll see if I can find a simpler reproducer.

---

### Comment 5 - brad0

@purplesyringa Any update on this?

@yingopq @wzssyqa 

---

### Comment 6 - purplesyringa

Nope, not really. I've tried to reproduce this by passing the IR to `llc`, but I haven't managed to make it output the same binary as rustc. *Something* in the configuration must be different, but I have no idea what.

---

### Comment 7 - purplesyringa

Reproduced it with `-fPIC`.

[ir.ll.txt](https://github.com/user-attachments/files/18551103/ir.ll.txt)

```shell
$ llc --relocation-model=pic -O0 ir.ll -o example.s
$ grep '39($sp)' example.s
	lw	$2, 39($sp)
```



---

### Comment 8 - nikic

I'm seeing the `lw	$2, 39($sp)` on LLVM 19, but on trunk there is only `lwr     $2, 39($sp)` (https://llvm.godbolt.org/z/G3bcro8q7). So I think this has been fixed already?

---

### Comment 9 - purplesyringa

I wouldn't be so sure. This bug is incredibly sensitive to source code changes. You can see that the codegen around the `lw` line on LLVM 19 is very different compared to the code around `lwr` on trunk, so it's very possible that an unrelated change on truck simply made this bug unreproducible on this particular IR. If at all possible, I think finding why this causes a problem on LLVM 19 would be valuable.

---

### Comment 10 - wzssyqa

With bisect, I find that this problem was resolved by

https://github.com/llvm/llvm-project/pull/106231

So I think that we should back port it to llvm19.

---

### Comment 11 - brad0

> So I think that we should back port it to llvm19.

From the looks of it 19 will not have any further releases.

---

### Comment 12 - purplesyringa

Yay, thanks a lot for bisecting! Glad to hear this is resolved.

---

