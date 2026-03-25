# Compiler crash Exception Code: 0xC0000005

**Author:** johnmgreenwood2-a11y
**URL:** https://github.com/llvm/llvm-project/issues/164846

## Body

Hello 

Trying to compile an old application and getting this: 

[mplab crash.txt](https://github.com/user-attachments/files/23103108/mplab.crash.txt)

it seems to be complaining about the definition of _Bool in stdbool.h - but I suspect this is a red herring... 

I'm using 

Product Version: MPLAB X IDE v6.25
Java: 1.8.0_422; OpenJDK 64-Bit Server VM 25.422-b05
Runtime: OpenJDK Runtime Environment 1.8.0_422-b05
System: Windows 11 version 10.0 running on amd64; Cp1252; en_GB (mplab)


This used to work with a much older version of the compiler. 

I've tried running System File Checker without success. 

Any help greatly appreciated. 


john 

## Comments

### Comment 1 - AaronBallman

Thanks for the report! Can you attach source code which reproduces the issue and which version of Clang you were using?

---

### Comment 2 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: None (johnmgreenwood2-a11y)

<details>
Hello 

Trying to compile an old application and getting this: 

[mplab crash.txt](https://github.com/user-attachments/files/23103108/mplab.crash.txt)

it seems to be complaining about the definition of _Bool in stdbool.h - but I suspect this is a red herring... 

I'm using 

Product Version: MPLAB X IDE v6.25
Java: 1.8.0_422; OpenJDK 64-Bit Server VM 25.422-b05
Runtime: OpenJDK Runtime Environment 1.8.0_422-b05
System: Windows 11 version 10.0 running on amd64; Cp1252; en_GB (mplab)


This used to work with a much older version of the compiler. 

I've tried running System File Checker without success. 

Any help greatly appreciated. 


john 
</details>


---

### Comment 3 - johnmgreenwood2-a11y

Hi Aaron, 

Thanks for the quick response. 

Firstly clang: 

C:\Program Files\Microchip\xc8\v3.10\pic\bin>clang -v

clang version 19.1.7 (https://bitbucket.microchip.com/scm/XC8/llvm-project.git 8d97188e64dd9e189caa0b1a8ff43e27cdd90b0d)

The project is quite large using lots of MCC generated files so i've zipped it up 

thanks for your help 

John 

[WorkshopClockRebuild.X.zip](https://github.com/user-attachments/files/23122234/WorkshopClockRebuild.X.zip)

---

### Comment 4 - AaronBallman

Thank you! I'm not certain if it's feasible, but if you can try the code on the latest Clang (Clang 21 or even trunk) that would be helpful; it's possible the issue has already been fixed. In the meantime, I added the needs-reduction label in case anyone wants to come along and reduce the example to a more manageable amount of code.

---

### Comment 5 - johnmgreenwood2-a11y

previous compiler version PIC18F-K_DFP 1.14.303 XC8 (2.5) works. 

Thank 

---

