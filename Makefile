#########################################################
# Ekans Server Makefile                                 #
#                                                       #
# ekans   -- compiles main binary                       #
# clean   -- cleans directories                         #
# rebuild -- rebuilds whole project                     #
#                                                       #
# Created by Samuel C. Payson on November 18, 2010      #
#                                                       #
#########################################################

CC       = gcc
CFLAGS   = -Wall -g -O3 -Iinclude/
LD       = gcc
LDFLAGS  = -lpthread

MAIN = main

MOD  = slist

TEST = $(patsubst %, test/bin/%.bin, $(MOD))

OBJ  = $(patsubst %, obj/%.o, $(MOD) $(MAIN))

obj/%.o: */%.c
	$(CC) -c $(CFLAGS) -o $@ $<

obj/%.o: %.c
	$(CC) -c $(CFLAGS) -o $@ $<

test/bin/%.bin: obj/%.o test/%_test.c
	$(LD) $(CFLAGS) -o $@ $^

ekans: $(OBJ)
	$(LD) $(CFLAGS) -o $@ $(OBJ) $(LDFLAGS)

test: $(TEST)
	

clean:
	rm -f *~ */*~ obj/* ekans

rebuild: clean ekans
	@echo Whole project rebuilt!