

# Output
EXECUTABLE = rt
BUILD_PATH := build

# Sources
SRC_PATH := src
SRC  	:= $(wildcard $(SRC_PATH)/*.cpp)
HEADERS := $(wildcard $(SRC_PATH)/*.h)
OBJECTS := $(patsubst %.cpp, $(BUILD_PATH)/%.o, $(SRC))
DEP     := $(SOURCES:.cpp=.d)

# Tools
LDFLAGS := $(shell pkg-config --libs lua libpng) -lpthread
CPPFLAGS := $(shell pkg-config --cflags lua libpng)
CXXFLAGS := $(CPPFLAGS) -W -Wall -g -Wno-unused-parameter -Wno-unused-variable -Wno-deprecated-declarations
CXX = clang++

all: $(EXECUTABLE)

# Setup auto-dependency generation inclusion
ifneq ($(MAKECMDGOALS),clean)
ifneq ($(MAKECMDGOALS),purge)
-include $(DEP)
endif
endif

# build paths as necessary
$(BUILD_PATH):
	@mkdir -p $(BUILD_PATH)
	@mkdir -p $(BUILD_PATH)/src

# main executable compile
$(EXECUTABLE): $(BUILD_PATH) $(DEP) $(OBJECTS)
	@echo Creating $@...
	@$(CXX) -o $@ $(OBJECTS) $(LDFLAGS)

# compile obj files from cpp with directory change
$(BUILD_PATH)/%.o: %.cpp
	@echo Compiling $<...
	@$(CXX) -o $@ -c $(CXXFLAGS) $<

# build dependencies
%.d: %.cpp
	@echo Building $@...
	@set -e; $(CC) -M $(CPPFLAGS) $< \
                | sed 's/\($*\)\.o[ :]*/\1.o $@ : /g' > $@; \
                [ -s $@ ] || rm -f $@
clean:
	rm -rf $(BUILD_PATH) $(EXECUTABLE)

render:
	@echo Compiling All Images
	@LUA_PATH="sample/?.lua" ./rt sample/nonhier.lua 8 
	@LUA_PATH="sample/?.lua" ./rt sample/hier.lua 8
	@LUA_PATH="sample/?.lua" ./rt sample/simple-cows.lua 8
	@LUA_PATH="sample/?.lua" ./rt sample/macho-cows.lua 8

.PHONY: all clean render
