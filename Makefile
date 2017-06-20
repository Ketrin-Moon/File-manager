SUBDIRS = src lib 
OBJECTS = main.o func.o

vpath %.c src:lib
vpath %.h include
vpath %.o src:lib

CC:=gcc
CFLAGS+= -O2 -Wall -pedantic -ansi
LDFLAGS+=-lncurses

.PHONY: subdirs $(SUBDIRS)

all: $(OBJECTS)
		$(CC) -o file-manager $^ $(LDFLAGS)

subdirs: $(SUBDIRS)

$(SUBDIRS):
		$(MAKE) -C $@

src: lib

