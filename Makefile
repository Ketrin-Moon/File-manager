SUBDIRS = src lib 
OBJECTS = main.o func.o

vpath %.c src:lib
vpath %.h include
vpath %.o src:lib

CC:=gcc
CFLAGS+=
LDFLAGS+=-lncurses

.PHONY: subdirs $(SUBDIRS)

all: $(OBJECTS)
		$(CC) -o file-manager $^ $(LDFLAGS)

subdirs: $(SUBDIRS)

$(SUBDIRS):
		$(MAKE) -C $@

src: lib

