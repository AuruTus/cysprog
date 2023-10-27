all: arp udp tcp

.PHONY: arp
arp:
	gcc -o arp_forge.out ARP-forge.c

.PHONY: tcp
tcp:
	@echo TODO

.PHONY: udp
udp:
	gcc -o udp_forge.out UDP-forge.c

.PHONY: clean
clean:
	rm -rf *.out

