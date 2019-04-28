# Copyleft  by Potrepalov I.S.

CC=gcc
#CC=cl

#CFLAGS=-falign-functions -fomit-frame-pointer -O2 -pedantic
CFLAGS=-fomit-frame-pointer -O2 -pedantic

forth : forth.c block.c core.c double.c except.c file.c loop.c \
 memory.c search.c string.c words.inc \
 Makefile
	${CC} ${CFLAGS} forth.c -o forth
