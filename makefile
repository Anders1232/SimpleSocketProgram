compilador = gcc
flagsGerais = -std=c99 -Wall -pedantic -g
c99:
	$(compilador) $(flagsGerais)  -pthread -IArgumentAnalizer -IVector Vector/vector.c servidor.c -o servidor.out
	$(compilador) $(flagsGerais)  -pthread -IArgumentAnalizer cliente.c -o cliente.out
vector.o: Vector/vector.c Vector/vector.h
	$(compilador) $(flagsGerais) -c Vector/vector.c -IVector -c -o vector.o
