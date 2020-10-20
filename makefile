
#File Name: makefile
#Description: Makefile for the Lab0 for ECEN 5033
#Author: Nitik Gupta

DEPS1 = main.o util.o mergesort.o bucketsort.o locks.o
DEPS2 = counter.o locks.o util.o
CC = g++
LIBS = -pthread
CFLAGS = -g -O3

all: mysort counter

mysort: ${DEPS1}
	${CC} ${CFLAGS} ${LIBS} ${DEPS1} -o $@ 

counter: ${DEPS2}
	${CC} ${CFLAGS} ${LIBS} ${DEPS2} -o $@

*.o: *.cpp *.h
	${CC} -c *.cpp

clean :
	rm mysort counter *.o