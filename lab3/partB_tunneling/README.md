# Lab3 Assignment Part B

**Table of Contents**
- [Exersice 1](#exersice-1)

## Exersice 1

Q: Do we need calculate the checksum field for the ICMP header? If so, what algorithm we should use?


A: Sure, we need do this. As cited from `RFC 792`

```text
 The 16 bit one's complement of the one's complement sum of all 16
 bit words in the header.  For computing the checksum, the checksum
 field should be zero.  This checksum may be replaced in the
 future.
```

We can just add sum up all 16 bits in the header by hand, and then get the completemented sum (, and maybe deal with some corner cases).
