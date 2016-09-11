T2.o: headerT2.h T2.c main.c
  gcc -c headerT2.h T2.c main.c

T2: T2.o
  gcc T2.o -o T2
