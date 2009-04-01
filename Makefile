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

# OPTIONS
CC            = g++
INCLUDE       = -I/local/include/openbabel-2.0 -I/gnuwin/include # set -I as appropriate
LDFLAGS       = -L/local/bin -L/gnuwin/bin # set -L as appropriate
CXXFLAGS      = -O3 -g $(INCLUDE)
OBJ           = closeleg.o constraints.o database.o graphstate.o legoccurrence.o path.o patterntree.o fminer.o
SWIG          = swig
SWIGFLAGS     = -c++ -ruby

# WHAT
NAME          = fminer

ifeq ($(OS), Windows_NT) # assume MinGW/Windows
LIBS	      = -lm -llibopenbabel-3 -llibgsl -llibgslcblas
LIB1          = lib$(NAME).dll
.PHONY:
all: $(LIB1)
$(LIB1): $(OBJ)
	$(CC) $(LDFLAGS) $(LIBS) -shared -o $@ $^

else                     # assume GNU/Linux
CXXFLAGS      = $(CXXFLAGS) -fPIC
LIBS	      = -ldl -lm -lopenbabel -lgsl -lgslcblas
LIB1          = lib$(NAME).so
LIB1_SONAME   = $(LIB1).1
LIB1_REALNAME = $(LIB1_SONAME).0.1
LIB2          = $(NAME).so
.PHONY:
all: $(LIB1_REALNAME) 
.PHONY:
ruby: $(LIB2)
$(LIB1_REALNAME): $(OBJ)
	$(CC) $(LDFLAGS) $(LIBS) -shared -Wl,-soname,$@ -o $@ $^
	-ln -sf $@ $(LIB1_SONAME)
	-ln -sf $@ $(LIB1)
$(LIB2): $(NAME)_wrap.o $(OBJ)
	$(CC) $(LDFLAGS) -shared $(CXXFLAGS) *.o /usr/lib/libopenbabel.so /usr/lib/libgsl.so -o $@
$(NAME)_wrap.o: $(NAME)_wrap.cxx
	$(CC) -c $(CXXFLAGS) -I/usr/lib/ruby/1.8/i486-linux/ $^ -o $@
%.cxx: %.i
	$(SWIG) $(SWIGFLAGS) -o $@ $^

endif


# FILE TARGETS
.o: .cpp.h
	$(CC) -Wall $(CXXFLAGS) $(LIBS) $@

# HELPER TARGETS
.PHONY:
doc: Doxyfile Mainpage.h *.h
	-doxygen $<
.PHONY:
clean:
	-rm -rf *.o *.so* *.cxx
