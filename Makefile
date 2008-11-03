# WHAT
NAME          = fminer
LIB1          = lib$(NAME).so
LIB1_SONAME   = $(LIB1).1
LIB1_REALNAME = $(LIB1_SONAME).0.1
LIB2          = $(NAME).so

# OPTIONS
INCLUDE       = /usr/local/include/openbabel-2.0/
OBJ           = database.o patterntree.o legoccurrence.o closeleg.o graphstate.o path.o constraints.o fminer.o
CC            = g++
CXXFLAGS      = -g -Wall -O3 -I$(INCLUDE) -fPIC
LIBS	      = -lm -ldl -lopenbabel -lgsl -lgslcblas

# PHONY TOP LEVEL TARGETS
.PHONY:
all: $(LIB1_REALNAME) 
.PHONY:
ruby: $(LIB2)

# REAL FILE TARGETS
$(LIB1_REALNAME): $(OBJ)
	$(CC) -shared -Wl,-soname,$@ -o $@ $(OBJ)
	-ln -sf $@ $(LIB1_SONAME)
	-ln -sf $@ $(LIB1)
.o: .cpp.h
	$(CC) $(CXXFLAGS) $(LIBS) $@
$(LIB2): fminer_wrap.o $(OBJ)
	g++ -shared /usr/local/lib/libopenbabel.so *.o -o $@

# HELPER TARGETS
$(NAME)_wrap.cpp: $(NAME).i
	swig -c++ -ruby -Wall -o $@ $<
$(NAME)_wrap.o: $(NAME)_wrap.cpp
	g++ -c -g -Wall -I$(INCLUDE) -I/usr/local/lib/ruby/1.8/i686-linux/ $<
.PHONY:
doc: Doxyfile Mainpage.h
	-doxygen $<
.PHONY:
clean:
	-rm -rf *.o *.so* doc *wrap*
