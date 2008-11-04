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
CXXFLAGS      = -g -Wall -O3 -I$(INCLUDE) -fPIC
SWIG          = swig
SWIGFLAGS     = -Wall -c++ -ruby
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
	$(CC) $(CXXFLAGS) $(LIBS) $@
$(LIB2): $(NAME)_wrap.o $(OBJ)
	g++ -shared -fPIC *.o /usr/local/lib/libopenbabel.so /usr/lib/libgsl.so -o $@

# HELPER TARGETS
$(NAME)_wrap.cxx: $(NAME).i
	$(SWIG) $(SWIGFLAGS) $^
$(NAME)_wrap.o: $(NAME)_wrap.cxx
	g++ -c -g -Wall -I$(INCLUDE) -I/usr/local/lib/ruby/1.8/i686-linux/ $^
.PHONY:
doc: Doxyfile Mainpage.h
	-doxygen $<
.PHONY:
clean:
	-rm -rf *.o *.so* *wrap*
