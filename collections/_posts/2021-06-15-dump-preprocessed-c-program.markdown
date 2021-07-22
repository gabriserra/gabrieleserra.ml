---
layout: post
title:  "Dump preprocessed C program"
date:   2020-06-30 11:30:15 +0200
categories: c macro
style: blog
---

As a C programmer, lot of times I find myself to reason wheter I can simplify the code I'm writing using some *macros*. If you are into C and GNU stuff you can find macros really everywhere.

Common scenario: let's suppose you are working on an existent C program and you find some macros you can't really figure out what they are doing.
Or, badly, you are writing a macro and you can't undestand why it is not working.

For instance, you are working on macro *stringizing* and you have the following code

```c
#define xstr(s) str(s)
#define str(s) #s
#define foo 4
```

I know it's a really simple snippet, but it's there just to understand the context.
So what you expect from the expansion of `str(foo)` or from `xstr(foo)`?

You can let GCC do the work for you passing the `-E` option! We can write again our program, save it as `bar.c` and test macro expansion.

```c
#define xstr(s) str(s)
#define str(s) #s
#define foo 4

"str(foo)" -> str(foo) 
"xstr(foo)" -> xstr(foo)
```

The nice thing here is that, with the `-E` option, GCC does not perform any compilation stage nor error checking. Hence it is easy to debug a source that is not working.

```bash
$ gcc -E bar.c
"str(foo)" -> "foo"
"xstr(foo)" -> "4"
```

And that's it! The output is print over `stdout`, however you can redirect onto a file if you want.

```bash
$ gcc -E bar.c > dump.c
```

The dump generated is a valid `C` file (obv. if the C file was valid before) so you can preprocess and then compile the result obtained.
