
#File Name: makefile
#Description: Makefile for the Lab0 for ECEN 5033
#Author: Nitik Gupta

DEPS = main.o util.o quicksort.o mergesort.o bucketsort.o locks.o
CC = g++
LIBS = -pthread
CFLAGS = -g -O3

all: mysort

mysort: ${DEPS}
	${CC} ${CFLAGS} ${LIBS} ${DEPS} -o $@ 

*.o: *.cpp *.h
	${CC} -c *.cpp

clean :
	rm mysort *.o