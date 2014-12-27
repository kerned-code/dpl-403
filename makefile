# Tucker Meyers: CS403 Programming Language Project
# Makefile for Lexical Analysis phase

CC := gcc
CFLAGS := -Wall -g
program_NAME := runner
program_HEADERS := $(wildcard *.h)
program_SOURCES := $(wildcard *.c)
program_OBJECTS := $(program_SOURCES:.c=.o)


.PHONY : build rpn arrays source-arrays conditionals source-conditionals functions source-functions iteration source-iteration lists source-lists objects source-objects recursion source-recursion clean

build : $(program_NAME)

$(program_NAME) : $(program_OBJECTS)
	$(CC) $(CFLAGS) $(program_OBJECTS) -o $(program_NAME)

rpn :
	$(program_NAME) calculator.source

arrays :
	$(program_NAME) test-arrays.source

source-arrays :
	cat test-arrays.source

conditionals :
	$(program_NAME) test-conditionals.source

source-conditionals :
	cat test-conditionals.source

functions :
	$(program_NAME) test-functions.source

source-functions :
	cat test-functions.source

iteration :
	$(program_NAME) test-iteration.source

source-iteration :
	cat test-iteration.source

lists :
	$(program_NAME) test-lists.source

source-lists :
	cat test-lists.source

objects : 
	$(program_NAME) test-objects.source

source-objects : 
	cat test-objects.source

recursion :
	$(program_NAME) test-recursion.source

source-recursion :
	cat test-recursion.source

clean :
	 $(RM) $(program_NAME)
	 $(RM) $(program_OBJECTS)

