CC := g++-4.8 # This is the main compiler
CFLAGS := -g -std=c++11 -Wall -Wextra -fPIC -fPIE

SRCDIR := src
BUILDDIR := build
TARGET := bin/client
 
SRCEXT := cpp
SOURCES := $(shell find $(SRCDIR) -type f -name *.$(SRCEXT) ! -name client.cpp)
SOURCES_TEST=test/client_test.cpp $(filter-out src/main.cpp, $(SOURCES))
OBJECTS := $(patsubst $(SRCDIR)/%,$(BUILDDIR)/%,$(SOURCES:.$(SRCEXT)=.o))
LIB := -L../initial_project/lib/mbedtls/library -lmbedcrypto -lmbedtls -lmbedx509  
INC := -I. -I../initial_project/lib/mbedtls/include -I../server/include -I/usr/include/qt5

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

.PHONY: clean

