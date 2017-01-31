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
           codeRunner:#2> printf("%d\n",a);
           100

   after that you can enter another line of c code
  
           codeRunner:#3> printf("a=%d\n",a);
           a=100

2) if your code failed to compile, such as

           codeRunner:#2> printf("%d\n",b) // b is not declared
           error info output by the compiler: b is not declared

   you can enter again your code

           codeRunner:#2> printf("%d\n",a);
           100

Use your own template
---------------------
The default template provided by codeRunner is rather simple,
and it may missing some headers. So you can use `--template=template.c`
to specify `template.c` as your code template.

			$$yourCode$$

will be replaced by your code typed by codeRunner. Make sure it is 
in a single line.


INSTALL
---------
you should install `gcc` in advance to compile the source code.
execute `make` to generate the program `codeRunner`
execute `make install` and `codeRunner` will be installed under
`/usr/local/bin/`


TODO
-----
[ ] user can choose a custom compiler
[x] user can choose a custom code template
[ ] support other languages such as c++
[ ] support functions


yshen 2017
