# ASTImporter going into an infinite loop and consuming increasing amount of memory, when Importing a CallExpr, CompoundStmt

**Author:** YuvrajTalukdar
**URL:** https://github.com/llvm/llvm-project/issues/126042

## Body

I was trying to import a CallExpr using clang::ASTImporter, and realized the program was going into an infinite loop while consuming more and more memory. I had to forcefully kill the program since the memory consumption was continuously increasing.

On performing some further test, I realized the problem also occurs when importing a CompoundStmt, . Here is a few example where the problem occurs.
Example1


```
void foo1()
{
   printf("hello");
}

void foo2()
{
   foo1();// s1
}
```

Example2

```
void foo1()
{
   {// s2
       printf("hello");
       printf("world");
    }
}
```

In example 1 the problem occurs when importing statement s1 and in example 2 the problem occurs when importing CompoundStmt s2.

Can any one else confirm this issue, and is there any solution to this.

I tested this on clang version 19.1.7 and ASTImporter was initialized like this:-
`ASTImporter Importer(astContext1, astContext1.getSourceManager().getFileManager(),astContext2, astContext2.getSourceManager().getFileManager(),false,nullptr);`



On running the same inside gdb the infinite loop looks like this

```
#22 0x00007ffff4da869c in clang::ASTImporter::ImportImpl(clang::Decl*) () from /usr/lib/libclang-cpp.so.19.1
#23 0x00007ffff4d7ab93 in clang::ASTImporter::Import(clang::Decl*) () from /usr/lib/libclang-cpp.so.19.1
#24 0x00007ffff73f1316 in ?? () from /usr/lib/libclang-cpp.so.19.1
#25 0x00007ffff4da8245 in clang::ASTNodeImporter::VisitFunctionDecl(clang::FunctionDecl*) ()
   from /usr/lib/libclang-cpp.so.19.1
#26 0x00007ffff4da869c in clang::ASTImporter::ImportImpl(clang::Decl*) () from /usr/lib/libclang-cpp.so.19.1
#27 0x00007ffff4d7ab93 in clang::ASTImporter::Import(clang::Decl*) () from /usr/lib/libclang-cpp.so.19.1
#28 0x00007ffff73f1316 in ?? () from /usr/lib/libclang-cpp.so.19.1
#29 0x00007ffff4da8245 in clang::ASTNodeImporter::VisitFunctionDecl(clang::FunctionDecl*) ()
   from /usr/lib/libclang-cpp.so.19.1
#30 0x00007ffff4da869c in clang::ASTImporter::ImportImpl(clang::Decl*) () from /usr/lib/libclang-cpp.so.19.1
#31 0x00007ffff4d7ab93 in clang::ASTImporter::Import(clang::Decl*) () from /usr/lib/libclang-cpp.so.19.1
#32 0x00007ffff73f1316 in ?? () from /usr/lib/libclang-cpp.so.19.1
#33 0x00007ffff4da8245 in clang::ASTNodeImporter::VisitFunctionDecl(clang::FunctionDecl*) ()
   from /usr/lib/libclang-cpp.so.19.1
```


## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: Yuvraj Talukdar (YuvrajTalukdar)

<details>
I was trying to import a CallExpr using clang::ASTImporter, and realized the program was going into an infinite loop while consuming more and more memory. I had to forcefully kill the program since the memory consumption was continuously increasing.

On performing some further test, I realized the problem also occurs when importing a CompoundStmt, . Here is a few example where the problem occurs.
Example1


```
void foo1()
{
   printf("hello");
}

void foo2()
{
   foo1();// s1
}
```

Example2

```
void foo1()
{
   {// s2
       printf("hello");
       printf("world");
    }
}
```

In example 1 the problem occurs when importing statement s1 and in example 2 the problem occurs when importing CompoundStmt s2.

Can any one else confirm this issue, and is there any solution to this.

I tested this on clang version 19.1.7 and ASTImporter was initialized like this:-
`ASTImporter Importer(astContext1, astContext1.getSourceManager().getFileManager(),astContext2, astContext2.getSourceManager().getFileManager(),false,nullptr);`



On running the same inside gdb the infinite loop looks like this

```
#<!-- -->22 0x00007ffff4da869c in clang::ASTImporter::ImportImpl(clang::Decl*) () from /usr/lib/libclang-cpp.so.19.1
#<!-- -->23 0x00007ffff4d7ab93 in clang::ASTImporter::Import(clang::Decl*) () from /usr/lib/libclang-cpp.so.19.1
#<!-- -->24 0x00007ffff73f1316 in ?? () from /usr/lib/libclang-cpp.so.19.1
#<!-- -->25 0x00007ffff4da8245 in clang::ASTNodeImporter::VisitFunctionDecl(clang::FunctionDecl*) ()
   from /usr/lib/libclang-cpp.so.19.1
#<!-- -->26 0x00007ffff4da869c in clang::ASTImporter::ImportImpl(clang::Decl*) () from /usr/lib/libclang-cpp.so.19.1
#<!-- -->27 0x00007ffff4d7ab93 in clang::ASTImporter::Import(clang::Decl*) () from /usr/lib/libclang-cpp.so.19.1
#<!-- -->28 0x00007ffff73f1316 in ?? () from /usr/lib/libclang-cpp.so.19.1
#<!-- -->29 0x00007ffff4da8245 in clang::ASTNodeImporter::VisitFunctionDecl(clang::FunctionDecl*) ()
   from /usr/lib/libclang-cpp.so.19.1
#<!-- -->30 0x00007ffff4da869c in clang::ASTImporter::ImportImpl(clang::Decl*) () from /usr/lib/libclang-cpp.so.19.1
#<!-- -->31 0x00007ffff4d7ab93 in clang::ASTImporter::Import(clang::Decl*) () from /usr/lib/libclang-cpp.so.19.1
#<!-- -->32 0x00007ffff73f1316 in ?? () from /usr/lib/libclang-cpp.so.19.1
#<!-- -->33 0x00007ffff4da8245 in clang::ASTNodeImporter::VisitFunctionDecl(clang::FunctionDecl*) ()
   from /usr/lib/libclang-cpp.so.19.1
```

</details>


---

### Comment 2 - shafik

CC @balazske 

---

