# clang::ParseAST compile error

**Author:** PhilipDeegan
**URL:** https://github.com/llvm/llvm-project/issues/143307

## Body

Hi, 

I'm running into a pretty gnarly issue locally and on github actions with clang for the hpc code that I work on.

I've tried using both creduce and cvise with the output script/source from the clang dump but I just don't see how to get it to work sorry.

You can see this report here 
https://github.com/PhilipDeegan/PHARE/actions/runs/15468570591/job/43546672556

I've added `.txt` to the end of the filenames to get them to pass the upload filter.

you should be able to reproduce this yourselves if you try out the clang action here on this specific branch 
https://github.com/PhilipDeegan/PHARE/blob/mkn/.github/workflows/cmake_ubuntu.yml

[test_fields_schedules-71bb52.cpp.txt](https://github.com/user-attachments/files/20644417/test_fields_schedules-71bb52.cpp.txt)
[test_fields_schedules-71bb52.sh.txt](https://github.com/user-attachments/files/20644416/test_fields_schedules-71bb52.sh.txt)


[In this gist](https://gist.github.com/PhilipDeegan/59520807ac7326360a8e707a532782f5)

you can see the stack from github actions

and in the first comment, the result from me running the attached script locally


## Comments

### Comment 1 - PhilipDeegan

I should add this persists with `-emit-llvm -Xclang -disable-llvm-passes` as described [in the doc](https://llvm.org/docs/HowToSubmitABug.html)

---

### Comment 2 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: ♦♣♠♥ (PhilipDeegan)

<details>
Hi, 

I'm running into a pretty gnarly issue locally and on github actions with clang for the hpc code that I work on.

I've tried using both creduce and cvise with the output script/source from the clang dump but I just don't see how to get it to work sorry.

You can see this report here 
https://github.com/PhilipDeegan/PHARE/actions/runs/15468570591/job/43546672556

I've added `.txt` to the end of the filenames to get them to pass the upload filter.

you should be able to reproduce this yourselves if you try out the clang action here on this specific branch 
https://github.com/PhilipDeegan/PHARE/blob/mkn/.github/workflows/cmake_ubuntu.yml

[test_fields_schedules-71bb52.cpp.txt](https://github.com/user-attachments/files/20644417/test_fields_schedules-71bb52.cpp.txt)
[test_fields_schedules-71bb52.sh.txt](https://github.com/user-attachments/files/20644416/test_fields_schedules-71bb52.sh.txt)


[In this gist](https://gist.github.com/PhilipDeegan/59520807ac7326360a8e707a532782f5)

you can see the stack from github actions

and in the first comment, the result from me running the attached script locally

</details>


---

### Comment 3 - EugeneZelenko

Could you please try 20 or `main` branch?

---

### Comment 4 - PhilipDeegan

I'll try 20 from `http://apt.llvm.org/buster` and if that doesn't work I'll build main from source

---

### Comment 5 - PhilipDeegan

it persists on 20 and main (clang version 21.0.0git (https://github.com/llvm/llvm-project 07a86a525ea2f85ade9e224c200f0311fec9e433))

---

### Comment 6 - PhilipDeegan

this appears to have been resolved.

apple clang was reporting a ` redeclaration of using-enum declaration`  and after resolving that general clang seems to build 

---

