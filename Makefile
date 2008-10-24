# WHAT
NAME          = fminer
LIB1          = lib$(NAME).so
LIB1_SONAME   = $(LIB1).1
LIB1_REALNAME = $(LIB1_SONAME).0.1

# OPTIONS
OBJ           = database.o patterntree.o legoccurrence.o closeleg.o graphstate.o patterngraph.o path.o constraints.o
CC            = g++
CXXFLAGS      = -g -Wall -O3 -I./include/openbabel-2.0/ -fPIC -g
LIBS	      = -lm -ldl -lopenbabel -lgsl -lgslcblas

# TARGETS
.PHONY:
all: $(LIB1_REALNAME) 
$(LIB1_REALNAME): $(OBJ)
	$(CC) -shared -Wl,-soname,$@ -o $@ $(OBJ)
	-ln -sf $@ $(LIB1_SONAME)
	-ln -sf $@ $(LIB1)
.o:
	$(CC) $(CXXFLAGS) $(LIBS) $@


.PHONY:
clean:
	-rm -rf *.o *.so*
