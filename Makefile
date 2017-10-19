# Author : Suraj Joseph
# Available for public use (but not misuse !) 

# Header files
INCDIR = include

# Source files
SRCDIR = src

# Output files 
OUTDIR = build

# Object files
OBJDIR = build/obj

# Compiler 
CXX = g++

# Compile options
CFLAGS = -c -Wall -I$(INCDIR) -fdiagnostics-color=always

# Linking options
LDFLAGS = -lncurses

# Executables 
TARGET = gun_hunt

#-- Do not edit below this line, except change .cpp to .c, if C is being used instead of C++ --

SOURCES := $(wildcard $(SRCDIR)/*.cpp)
INCLUDES := $(wildcard $(INCDIR)/*.h)
OBJECTS := $(patsubst $(SRCDIR)%, $(OBJDIR)%, $(patsubst %.cpp, %.o, $(SOURCES)))

all: $(OUTDIR)/$(TARGET) 

$(OUTDIR)/$(TARGET): $(OBJECTS)
	@echo "Building" $(TARGET) "..."
	$(BLDCMD)

BLDCMD = $(CXX) -o $(OUTDIR)/$(TARGET) $(OBJECTS) $(LDFLAGS)

$(OBJDIR)/%.o : $(SRCDIR)/%.cpp $(INCLUDES)
	-@mkdir -p $(OBJDIR)	
	@echo "Compiling" $< "..."  
	$(CXX) $(CFLAGS) -c $< -o $@ 

.PHONY: clean

clean: 
	@echo Cleaning $(TARGET) "..."
	-@rm -f $(OUTDIR)/$(TARGET)
	-@rm -f $(OBJDIR)/*.o
	-@rm -rf $(OBJDIR)
	-@rm -f *.bin


