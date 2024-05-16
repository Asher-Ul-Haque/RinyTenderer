SYSCONF_LINK = g++
CPPFLAGS     = -O3
LDFLAGS      = -O3
LIBS         = -lm

DESTDIR = ./
OBJDIR = ./build/
TARGET  = $(DESTDIR)RinyTenderer

# List of source files
SOURCES := $(wildcard *.cpp)

# Generate object filenames from source filenames
OBJECTS := $(patsubst %.cpp,$(OBJDIR)%.o,$(SOURCES))

# Default target
all: $(TARGET)

# Rule to link the objects into the final executable
$(TARGET): $(OBJECTS) | $(DESTDIR)
	$(SYSCONF_LINK) $(LDFLAGS) -o $(TARGET) $(OBJECTS) $(LIBS)

# Rule to compile source files into object files
$(OBJDIR)%.o: %.cpp | $(OBJDIR)
	$(SYSCONF_LINK) $(CPPFLAGS) -c $< -o $@

# Create the build directory if it doesn't exist
$(OBJDIR):
	mkdir -p $(OBJDIR)

# Create the destination directory if it doesn't exist
$(DESTDIR):
	mkdir -p $(DESTDIR)

clean:
	rm -rf $(OBJDIR)
	rm -f $(TARGET)
	rm -f *.tga
	rm -f *.o
	rm -f RinyTenderer
