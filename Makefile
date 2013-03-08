# include user specific configurations
OS = $(shell uname -s 2>/dev/null | tr [:upper:] [:lower:])
-include Makefile.$(OS)
-include Makefile.conf

ifeq ($(LIB_EXT),)
  $(error Makfile not properly configured)
endif

# libs
MAIN_LIB_DIR  = lib
MAIN_LIB_NAME = libpandora
MAIN_LIB = $(MAIN_LIB_DIR)/$(MAIN_LIB_NAME)

LIB = $(LIB_PATS) $(LIB_NAMES)

# complete set of flags for target release
CXXFLAGS_RELEASE = $(FLAGS_GLOBAL) -O1
# complete set of flags for target debug 
CXXFLAGS_DEBUG   = $(FLAGS_GLOBAL) $(FLAGS_DEBUG) -O0

# define source and object files for the project
SRCS = $(wildcard src/*.cpp)
OBJS_RELEASE = $(patsubst src%.cpp, obj/release%.o, $(SRCS))
OBJS_DEBUG   = $(patsubst src%.cpp, obj/debug%.o, $(SRCS))

# define source and executable files for tests
SRCS_TEST = $(wildcard test/src/*.cpp)
EXEC_TEST = $(patsubst test/src%.cpp, test/bin%, $(SRCS_TEST))

#
# make all
#
all: clean release

#
# make library and main executable for release
#
release: $(MAIN_LIB).$(LIB_EXT)
	$(CXX) $(CXXFLAGS_RELEASE) $(INCLUDES) $(MAIN).cpp $(MAIN_LIB).$(LIB_EXT) $(LIB) -o $(MAIN)

$(MAIN_LIB).$(LIB_EXT): $(OBJS_RELEASE)
	mkdir -p $(MAIN_LIB_DIR)
	$(CXX) $(CXXFLAGS_RELEASE) $(LIB) -shared $^ -o $(MAIN_LIB).$(LIB_EXT)

obj/release/%.o: src/%.cpp
	mkdir -p obj/release
	$(CXX) $(CXXFLAGS_RELEASE) $(INCLUDES) -c $^ -o $@

#
# make library and main executable with debug symbols
#
debug: $(MAIN_LIB).dbg.$(LIB_EXT)
	$(CXX) $(CXXFLAGS_DEBUG) $(INCLUDES) $(MAIN).cpp $(MAIN_LIB).dbg.$(LIB_EXT) $(LIB) -o $(MAIN)

$(MAIN_LIB).dbg.$(LIB_EXT): $(OBJS_DEBUG)
	mkdir -p $(MAIN_LIB_DIR)
	$(CXX) $(CXXFLAGS_DEBUG) $(LIB) -shared $^ -o $(MAIN_LIB).dbg.$(LIB_EXT)

obj/debug/%.o: src/%.cpp
	mkdir -pv obj/debug
	$(CXX) $(CXXFLAGS_DEBUG) $(INCLUDES) -c $^ -o $@

#
# build tests
# all tests are currently build with the debug target
#
test: debug $(EXEC_TEST)

test/bin%: test/src%.cpp
	mkdir -p test/bin
	$(CXX) $(CXXFLAGS_DEBUG) $(INCLUDES) $^ $(MAIN_LIB).dbg.$(LIB_EXT) $(LIB) -o $@

#
# run tests
# 
#    make checkTestName -> starts the test with the executable test/bin/TestName
#    make check         -> runs all tests in test/bin
#
check%: test/bin/%
	$^

check: test
	$(foreach var,$(EXEC_TEST), $(var))

# create documentation
.PHONY: doc
doc:
	mkdir -p doc
	doxygen ./Doxyfile

.PHONY: clean
clean:
	rm -vrf $(MAIN) obj/* doc/* test/bin/* $(MAIN_LIB).dbg.$(LIB_EXT) $(MAIN_LIB).$(LIB_EXT)
