	# // sniffing requires super user privilege
all:
	gcc promiscuous_main.c packet-analyze.c -o main.out
	sudo ./main.out ${dev}

sniff:
	gcc -o analysis.out filter.c main.c packet-process.c
	sudo ./analysis.out ${dev}

clean:
	rm -rf *.out

