# Lab2 Assignment

**Table of Contents**
- [Exersice 1](#exersice-1)
- [Exersice 2](#exersice-2)
- [Exersice 3](#exersice-3)
- [Exersice 4](#exersice-4)
- [Exersice 5](#exersice-5)
- [Exersice 6](#exersice-6)

## Exersice 1

**Q:** If you add a background command after the pipeline command, can the pipeline work?

**A:** There will be an error in bash as `bash: syntax error near unexpected token '&\'`, if we directly add `&` after pipe op `|` like `echo 1 | &`. But if there's another command after `\` op, it could work like this:
```bash
aurutus@some-desktop:~/$ echo 1 | echo &
[1] 16273

aurutus@some-desktop:~$ jobs
[1]+  Done                    echo 1 | echo
```

## Exersice 2

**Q:** What would be the result of the program without this line pid = wait((int *) 0)?

**A:** Parent process may exit first.

with `wait()`
```
parent: child=18160
child: exiting
child 18160 is done
```

without `wait()`, parent exit first.
```
parent: child=18429
child 18429 is done
child: exiting
```

## Exersice 3

**Q:** Can fork and exec be combined in one call?

**A:** Actually I don't get this question too much. But from the view of system design, they shouldn't be combined as one call, because of their different semantics and process controll granularity. Generally speaking, `fork` is uesd to duplicate a process and thus will have two process after its calling, while `exec` only works in the current process, which cannot directly affect another process nor even the parent or child process. If the os designer combine them together, they finally need another more syscall to controll child or parent process, which would make the code a bit redundant to process multi-process logic and hard to read. After all, in such a complex system like os, KISS principle is better.

## Exersice 4

**Q:** How does the above program implement input redirection?

**A:** Closing `stdin` and opening `input.txt` as input source. The previous closed fd 0 make the os choose this lowest one for the newly opened file.

## Exersice 5

**Q:** For the two lines close(0); dup(p[0]); can we reorder them?

**A:** Apparently not. As Exercise 4 said, we need connect the pipe's read end to stdin, so we should close the original stdin to redirect.

## Exersice 6

**Q:** What are the advantages of pipes over temporary files in this situation?

**A:** If the output is big or a stream, we can reuse temp file as a buffer for the late operations (maybe not one) to avoid unneeded re-exec.

## Exercise 8

```bash
css-lab@ cat hello.txt | grep -n hello > c.txt ; ls -l 
enter
seq_left: pipe
pipe_left: cat hello.txt 
; pipe_right: redir: 1
redir_left: grep -n hello 
; redir_right: c.txt 
; seq_right: ls -l 
css-lab@ sleep 10 &
back: sleep 10
```

## Exercise 9

```bash
css-lab@ cat hello.txt > c.txt
css-lab@ cat c.txt
hello
hello;
hello, world!
css-lab@ cat hello.txt | grep -n hello>c.txt; cat c.txt; ls
1:hello
2:hello;
3:hello, world!
alloc.h  ast.h  _hello.txt  lex.yy.c  Makefile      parser.tab.h  README.md  shell
ast.c    c.txt  hello.txt   main.c    parser.tab.c  parser.y      scanner.l  tmp_hello.txt
css-lab@ sleep 1000 &
css-lab@ ps -aux | grep sleep
aurutus   7508  0.0  0.0   8368  1052 ?        Ss   19:35   0:00 sleep 1000
```

```bash
css-lab@ cat < hello.txt
hello
hello;
hello, world!
css-lab@ cat < hello.txt > cc.txt
css-lab@ cat cc.txt
hello
hello;
hello, world!
```