codeRunner: run code as an interpreted language such as python...
====================================================================
> codeRunner can save your trouble to write/compile the c program on your own.
Simply type the code line by line and codeRunner will execute it for you.
It is good for education/testing use. At present, codeRunner only support
the C language. However, codeRunner is designed not only for the C language.
In the future, other languages will be supported.

A quick guide
-------------
1) enter c code and codeRunner will compile and run your code

           codeRunner:#1> int a = 100;
   whenever this line of code has output, it will print on the screen

           codeRunner:#2> printf("%d\n",a++);
           100

   after that you can enter another line of c code
  
           codeRunner:#3> printf("%d\n",a);
           a=101

2) if your code failed to compile, such as

           codeRunner:#2> printf("%d\n",b) // b is not declared
           error info output by the compiler: b is not declared

   you can enter again your code

           codeRunner:#2> printf("%d\n",a);
           101

Examples
----------
### one line code
![](http://shenyu.wiki/uploads/codeRunner-eg1.gif)

### multi line code
![](http://shenyu.wiki/uploads/codeRunner-eg2.gif)

### a complex example
![](http://shenyu.wiki/uploads/codeRunner-eg3.gif)

### error hint
![](http://shenyu.wiki/uploads/codeRunner-eg4.gif)

Use your own template
---------------------
The default template provided by codeRunner is rather simple,
and it may missing some headers. So you can use `--template=template.c`
to specify `template.c` as your code template.

```c
/* c code template for codeRunner */
#include<stdio.h>
#include<stdlib.h>

int main()
{
        $$yourCode$$
        return 0;
}
```

will be replaced by your code typed by codeRunner. Make sure it is 
in a single line.


INSTALL
---------
### requirements
1. you should have `gcc` installed to compile the source code.
```
sudo apt install gcc
```
2. you should have `libreadline` installed to do auto-complete
```
sudo apt install libreadline6-dev
```

### compile from source
1. execute `make` to generate the program `codeRunner`
if you donot have readline installed use `make no-readline` instead

2. execute `make install` and `codeRunner` will be installed under
`/usr/local/bin/` by default.


TODO
-----
[x] use libreadline to make input more convienent
[ ] user can choose a custom compiler
[x] user can choose a custom code template
[ ] support other languages such as c++
[ ] support functions


report bugs to shenyufly@163.com
yshen 2017
