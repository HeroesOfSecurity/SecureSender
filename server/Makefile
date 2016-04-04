CC := g++-4.8 # This is the main compiler
CFLAGS := -g -std=c++11 -Wall -Wextra

SRCDIR := src
BUILDDIR := build
TARGET := bin/server
 
SRCEXT := cpp
SOURCES := $(shell find $(SRCDIR) -type f -name *.$(SRCEXT))
SOURCES_TEST=test/server_test.cpp $(SOURCES)
OBJECTS := $(patsubst $(SRCDIR)/%,$(BUILDDIR)/%,$(SOURCES:.$(SRCEXT)=.o))
LIB := -L../initial_project/lib/mbedtls/library -lmbedcrypto -lmbedtls -lmbedx509 -Llib/sqlite-autoconf-3110100/ -lsqlite3  
INC := -Iinclude -I../initial_project/lib/mbedtls/include -Llib/sqlite-autoconf-3110100/

$(TARGET): $(OBJECTS)
	@echo " Linking..."; 
	@echo " $(CC) $^ -o $(TARGET) $(LIB)"; $(CC) $^ -o $(TARGET) $(LIB)

$(BUILDDIR)/%.o: $(SRCDIR)/%.$(SRCEXT)
	@mkdir -p $(BUILDDIR)
	@echo " $(CC) $(CFLAGS) $(INC) -c -o $@ $<"; $(CC) $(CFLAGS) $(INC) -c -o $@ $<

clean:
	@echo " Cleaning..."; 
	@echo " $(RM) -r $(BUILDDIR) $(TARGET)"; $(RM) -r $(BUILDDIR) $(TARGET)

# Tests
tests:
	@mkdir -p $(BUILDDIR)
	@mkdir -p bin
	$(CC) $(CFLAGS) $(SOURCES_TEST) $(INC) $(LIB) -o bin/tests 

# Depends on main-test, runs the test program.
launch-tests: tests	
	./bin/tests
	rm database.db

.PHONY: clean

