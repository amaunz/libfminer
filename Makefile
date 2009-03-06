# Makefile
# © 2008 by Andreas Maunz, andreas@maunz.de, jun 2008

# This file is part of LibFminer (libfminer).
#
# LibFminer is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.

# LibFminer is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.

# You should have received a copy of the GNU General Public License
# along with LibFminer.  If not, see <http://www.gnu.org/licenses/>.

# WHAT
NAME          = fminer
LIB1          = lib$(NAME).so
LIB1_SONAME   = $(LIB1).1
LIB1_REALNAME = $(LIB1_SONAME).0.1
LIB2          = $(NAME).so

# OPTIONS
INCLUDE       = /usr/local/include/openbabel-2.0/
OBJ           = closeleg.o constraints.o database.o graphstate.o legoccurrence.o path.o patterntree.o fminer.o
CC            = g++
CXXFLAGS      = -O3 -Wall -g -I$(INCLUDE) -fPIC
SWIG          = swig
SWIGFLAGS     = -c++ -ruby
LIBS	      = -lm -ldl -lopenbabel -lgsl -lgslcblas

# PHONY TOP LEVEL TARGETS
.PHONY:
all: $(LIB1_REALNAME) 
.PHONY:
ruby: $(LIB2)

# REAL FILE TARGETS
$(LIB1_REALNAME): $(OBJ)
	$(CC) -shared -Wl,-soname,$@ -o $@ $^
	-ln -sf $@ $(LIB1_SONAME)
	-ln -sf $@ $(LIB1)
.o: .cpp.h
	$(CC) -Wall $(CXXFLAGS) $(LIBS) $@
$(LIB2): $(NAME)_wrap.o $(OBJ)
	$(CC) -shared $(CXXFLAGS) *.o /usr/lib/libopenbabel.so /usr/lib/libgsl.so -o $@

# HELPER TARGETS
$(NAME)_wrap.o: $(NAME)_wrap.cxx
	$(CC) -c $(CXXFLAGS) -I/usr/lib/ruby/1.8/i486-linux/ $^ -o $@
%.cxx: %.i
	$(SWIG) $(SWIGFLAGS) -o $@ $^

.PHONY:
doc: Doxyfile Mainpage.h *.h
	-doxygen $<
.PHONY:
clean:
	-rm -rf *.o *.so* *.cxx
