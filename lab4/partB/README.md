# Lab4 Assignment PartB

**Table of Contents**
- [Exersice 1](#exersice-1)
- [Exersice 4](#exersice-4)

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

The first is reading the old value of read from mem, the second is add 1 to the old value and the final one is storing it back to that block of mem. As our system supports multi-task things, these apparent sequential codes could be any arrangement on the real-world CPU. Maybe `thread A` reads first, and `thread B` then reads it and luckiy enough finishes all the rest works, and finally the `thread A` returns back to the CPU to work on the previous dirty value. It's a common incorrect coding case in IO related fields like database. To fix this, we should add some gurantees to made these three ops processed as sequential.

## Exercise 4

Q: In protecting shared data structures, when should we use a mutex versus a spinlock? There might be no simple answer to this question. Herein, we will try to answer this question according to one simple criteria---the execution time. Please try to measure the execution time for the programs in the above exercise 2 and 3, respectively. Which one runs more efficiently?

Hint: you might need not write any new code, try to use the time command:
```bash
css-lab@tiger:~$ time
```

A: Here are the results:

```bash
aurutus@DESKTOP-25JCVH5:~/workspace/C_DOJO/cysprog/lab4/partB$ time ./mutex.out 
Expected count: 0 (because half increment and half decrement)
Actual count: 0

real    0m0.014s
user    0m0.018s
sys     0m0.052s
```

```bash
aurutus@DESKTOP-25JCVH5:~/workspace/C_DOJO/cysprog/lab4/partB$ time ./spinlock.out 
Expected count: 0 (because half increment and half decrement)
Actual count: 0

real    0m0.011s
user    0m0.018s
sys     0m0.000s
```

I have thought to test it with iteration like some benchmarks, but the `time` on my distro cannot accept options so I just give it up. And we can see the real time is almost the same, which means actually in this scenario, they have almost the same time on solving this problem. But the `mutex` version has taken much more `system` time. It's definitly used in the kernel mode for blocked `mutex` queueing logic. In this scenario, the `spinlock` is more efficient, for the lock's small enough granularity and simple task makes the `spinlock`'s busy loop not wasting CPU resources. But if the critical section is longer and the task is more CPU intensive, the context-switch and preemption of `mutex` can handle it more efficiently. As a result, in our most application layer developments (, in which the thread or process can sleep), `spinlock` is used for short and simpler code while `mutex` is used for longer and more complex code. And if there're threads much more than CPUs, I may prefer using mutex in case of starvation made by spinlock's busy awaiting. However, if we cannot let the thread sleep, spinlock is our only choice.
