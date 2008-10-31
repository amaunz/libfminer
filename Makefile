# WHAT
NAME          = fminer
LIB1          = lib$(NAME).so
LIB1_SONAME   = $(LIB1).1
LIB1_REALNAME = $(LIB1_SONAME).0.1

# OPTIONS
INCLUDE       = /usr/local/include/openbabel-2.0/
OBJ           = database.o patterntree.o legoccurrence.o closeleg.o graphstate.o path.o constraints.o fminer.o
CC            = g++
CXXFLAGS      = -g -Wall -O3 -I$(INCLUDE) -fPIC
LIBS	      = -lm -ldl -lopenbabel -lgsl -lgslcblas

# TARGETS
.PHONY:
all: $(LIB1_REALNAME) 
$(LIB1_REALNAME): $(OBJ)
	$(CC) -shared -Wl,-soname,$@ -o $@ $(OBJ)
	-ln -sf $@ $(LIB1_SONAME)
	-ln -sf $@ $(LIB1)
.o: .c.h
	$(CC) $(CXXFLAGS) $(LIBS) $@

.PHONY:
doc: Doxyfile Mainpage.h
	-doxygen Doxyfile

.PHONY:
clean:
	-rm -rf *.o *.so*
