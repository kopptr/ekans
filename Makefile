##############################################################################
#                                                                            #
# This file is part of The Ekans Webserver.                                  #
# Copyright (C) 2010 Samuel C. Payson                                        #
#                                                                            #
# Ekans is free software: you can redistribute it and/or modify it under the #
# terms of the GNU General Public License as published by the Free Software  #
# Foundation, either version 3 of the License, or (at your option) any later #
# version.                                                                   #
#                                                                            #
# Ekans is distributed in the hope that it will be useful, but WITHOUT ANY   #
# WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS  #
# FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more     #
# details.                                                                   #
#                                                                            #
# You should have received a copy of the GNU General Public License along    #
# with Ekans. If not, see <http://www.gnu.org/licenses/>.                    #
#                                                                            #
##############################################################################

CC       = gcc
CFLAGS   = -Wall -g -O0 -Iinclude/
LD       = gcc
LDFLAGS  = -lpthread

MAIN = main

MOD  = http_parse tcp cfg agent rwq http dispatch init html_prefab
TEST = $(patsubst %, test/bin/%.bin, $(MOD))
OBJ  = $(patsubst %, obj/%.o, $(MOD) $(MAIN))

DOC  = spec
TEX  = $(patsubst %, doc/%.tex, $(DOC))
PDF  = $(patsubst %, doc/%.pdf, $(DOC))


obj/%.o: */%.c
	$(CC) -c $(CFLAGS) -o $@ $<

obj/%.o: %.c
	$(CC) -c $(CFLAGS) -o $@ $<

test/bin/%.bin: obj/%.o test/%_test.c
	$(LD) $(CFLAGS) -o $@ $^

ekans: $(OBJ)
	$(LD) $(CFLAGS) -o $@ $(OBJ) $(LDFLAGS)

#test: $(TEST)
#	


clean:
	rm -f *~ */*~ obj/* ekans doc/*~ doc/*.pdf

rebuild: clean ekans
	@echo Whole project rebuilt!