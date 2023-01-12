SRCDIR =../src
INCLUDE =../include
VPATH=$(SRCDIR):$(INCLUDE)
CFLAGS = -I $(INCLUDE)
TARGET = prog
OBJ = main.o 
.PHONY: clean test
$(TARGET): $(OBJ)
	gcc -o $@ $^
$(OBJ): 

clean:
	rm -f $(TARGET) $(OBJ)

