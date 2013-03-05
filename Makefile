# include user specific configurations
include Makefile.conf

# complete set of flags for target release
CXXFLAGS_RELEASE = $(FLAGS_GLOBAL) -O1
# complete set of flags for target debug 
CXXFLAGS_DEBUG   = $(FLAGS_GLOBAL) $(FLAGS_DEBUG) -O0

# define wildcards for files
SRCS = $(wildcard src/*.cpp)
SRCS_TEST = $(wildcard test/*.cpp)
OBJS_RELEASE = $(patsubst src%.cpp, bin/release%.o, $(SRCS))
OBJS_DEBUG   = $(patsubst src%.cpp, bin/debug%.o, $(SRCS))

#
# make all
#
all: clean release

#
# make library and main executable for release
#
release: $(MAIN_LIB).$(LIB_EXT)
	$(CXX) $(CXXFLAGS_RELEASE) $(INCLUDES) $(MAIN).cpp $(LIB) $(MAIN_LIB).$(LIB_EXT) -o $(MAIN)

$(MAIN_LIB).$(LIB_EXT): $(OBJS_RELEASE)
	$(CXX) $(CXXFLAGS_RELEASE) $(INCLUDES) -shared $^ $(LIB) -o $(MAIN_LIB).$(LIB_EXT)

bin/release/%.o: src/%.cpp
	mkdir -p bin/release
	$(CXX) $(CXXFLAGS_RELEASE) $(INCLUDES) -c $^ -o $@

#
# make library and main executable with debug symbols
#
debug: $(MAIN_LIB).dbg.$(LIB_EXT)
	$(CXX) $(CXXFLAGS_DEBUG) $(INCLUDES) $(MAIN).cpp $(LIB) $(MAIN_LIB).dbg.$(LIB_EXT) -o $(MAIN)

$(MAIN_LIB).dbg.$(LIB_EXT): $(OBJS_DEBUG)
	$(CXX) $(CXXFLAGS_DEBUG) $(INCLUDES) -shared $^ $(LIB) -o $(MAIN_LIB).dbg.$(LIB_EXT)

bin/debug/%.o: src/%.cpp
	mkdir -pv bin/debug
	$(CXX) $(CXXFLAGS_DEBUG) $(INCLUDES) -c $^ -o $@

#
# make tests
#


# create documentation
.PHONY: doc
doc:
	mkdir -p doc
	doxygen ./Doxyfile

.PHONY: clean
clean:
	rm -rf $(MAIN) bin/* doc/* $(MAIN_LIB).dbg.$(LIB_EXT) $(MAIN_LIB).$(LIB_EXT): $(OBJS_RELEASE)

