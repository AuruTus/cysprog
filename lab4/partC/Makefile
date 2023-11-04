all:
	# empty

atomic:
	gcc atomic.c -o atomic.out
	./atomic.out

spinlock:
	gcc spinlock.c -o spinlock.out
	./spinlock.out

semlock:
	gcc sema.c -o semlock.out
	./semlock.out

clean:
	rm -rf *.a *.out *.o *.s
