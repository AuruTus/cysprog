# Lab4 Assignment PartB

**Table of Contents**
- [Exersice 1](#exersice-1)

## Exersice 1

Q: Compile and run the code in the file partB/race_condition.c, compile and run the code. What is your output? What is the expected output? How does this output generated?

A: My output: 

```bash
aurutus@some-desktop:~/workspace/C_DOJO/cysprog/lab4/partB$ ./race_condition.out 
Expected count: 0 (because half increment and half decrement)
Actual count: 0
aurutus@some-desktop:~/workspace/C_DOJO/cysprog/lab4/partB$ ./race_condition.out 
Expected count: 0 (because half increment and half decrement)
Actual count: 14848
aurutus@some-desktop:~/workspace/C_DOJO/cysprog/lab4/partB$ ./race_condition.out 
Expected count: 0 (because half increment and half decrement)
Actual count: -6747
aurutus@some-desktop:~/workspace/C_DOJO/cysprog/lab4/partB$ ./race_condition.out 
Expected count: 0 (because half increment and half decrement)
Actual count: -26785
```

The expected output is 0. But sadly the race happens here, where the critical section has actually three ops in a loop.

```asm
# the x86-64 snippets
        movl    count(%rip), %eax
        addl    $1, %eax
        movl    %eax, count(%rip)
```

The first is reading the old value of read from mem, the second is add 1 to the old value and the final one is storing it
back to that block of mem. As our system supports multi-task things, these apparent sequential codes could be any arrangement on the
 real-world CPU. Maybe `thread A` reads first, and `thread B` then reads it and luckiy enough finishes all the rest works, and finally the `thread A` return back to the CPU to work on the previous dirty value. It's a common incorrect coding case in IO related fields like database. To fix this, we should add some gurantees to made these three ops processed as sequential.

