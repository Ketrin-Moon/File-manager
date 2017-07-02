SUBDIRS = src lib 
OBJECTS = main.o func.o

vpath %.c src:lib
vpath %.h include
vpath %.o src:lib

CC:=gcc
FLAGS+= -O2 -Wall -pedantic -ansi 
LDFLAGS+=-lncurses -std=c99 -lpthread

.PHONY: subdirs $(SUBDIRS)

all: $(OBJECTS)
		$(CC) -o file-manager $^ $(LDFLAGS)

subdirs: $(SUBDIRS)

$(SUBDIRS):
		$(MAKE) -C $@

src: lib

