# Error on generic lambda with trailing return type within lambda within template

**Author:** Eczbek
**URL:** https://github.com/llvm/llvm-project/issues/125715

## Body

https://godbolt.org/z/E5nrss4df

```cpp
template<class T>constexpr int x=([]{[](auto)->void{}(0);}(),0);
int main(){x<int>;}
```

```
<source>:1:32: error: constexpr variable 'x<int>' must be initialized by a constant expression
    1 | template<class T>constexpr int x=([]{[](auto)->void{}(0);}(),0);
      |                                ^ ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
<source>:2:12: note: in instantiation of variable template specialization 'x<int>' requested here
    2 | int main(){x<int>;}
      |            ^
<source>:1:38: note: undefined function 'operator()<int>' cannot be used in a constant expression
    1 | template<class T>constexpr int x=([]{[](auto)->void{}(0);}(),0);
      |                                      ^
<source>:1:35: note: in call to '[] {
    [](auto) -> void {
    }(0);
}.operator()()'
    1 | template<class T>constexpr int x=([]{[](auto)->void{}(0);}(),0);
      |                                   ^~~~~~~~~~~~~~~~~~~~~~~~~~
<source>:1:38: note: declared here
    1 | template<class T>constexpr int x=([]{[](auto)->void{}(0);}(),0);
      |                                      ^
<source>:2:12: warning: expression result unused [-Wunused-value]
    2 | int main(){x<int>;}
      |            ^~~~~~
1 warning and 1 error generated.
Compiler returned: 1
```


## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: None (Eczbek)

<details>
https://godbolt.org/z/E5nrss4df

```cpp
template&lt;class T&gt;constexpr int x=([]{[](auto)-&gt;void{}(0);}(),0);
int main(){x&lt;int&gt;;}
```

```
&lt;source&gt;:1:32: error: constexpr variable 'x&lt;int&gt;' must be initialized by a constant expression
    1 | template&lt;class T&gt;constexpr int x=([]{[](auto)-&gt;void{}(0);}(),0);
      |                                ^ ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
&lt;source&gt;:2:12: note: in instantiation of variable template specialization 'x&lt;int&gt;' requested here
    2 | int main(){x&lt;int&gt;;}
      |            ^
&lt;source&gt;:1:38: note: undefined function 'operator()&lt;int&gt;' cannot be used in a constant expression
    1 | template&lt;class T&gt;constexpr int x=([]{[](auto)-&gt;void{}(0);}(),0);
      |                                      ^
&lt;source&gt;:1:35: note: in call to '[] {
    [](auto) -&gt; void {
    }(0);
}.operator()()'
    1 | template&lt;class T&gt;constexpr int x=([]{[](auto)-&gt;void{}(0);}(),0);
      |                                   ^~~~~~~~~~~~~~~~~~~~~~~~~~
&lt;source&gt;:1:38: note: declared here
    1 | template&lt;class T&gt;constexpr int x=([]{[](auto)-&gt;void{}(0);}(),0);
      |                                      ^
&lt;source&gt;:2:12: warning: expression result unused [-Wunused-value]
    2 | int main(){x&lt;int&gt;;}
      |            ^~~~~~
1 warning and 1 error generated.
Compiler returned: 1
```

</details>


---

### Comment 2 - shafik

edg also accepts: https://godbolt.org/z/4MjYbj7xo

---

### Comment 3 - shafik

CC @cor3ntin 

---

