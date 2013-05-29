# include user specific configurations
OS = $(shell uname -s 2>/dev/null | tr [:upper:] [:lower:])
-include Makefile.$(OS)
-include Makefile.conf

ifeq ($(LIB_EXT),)
  $(error Makfile not properly configured)
endif

#commandline options for makefile
verbose ?= 0

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

### misc
ifeq ($(verbose), 1)
  ECHO :=
else
  ECHO := @
endif

ifeq ($(OS), darwin)
  ECHO_FLAGS :=
else
  ECHO_FLAGS := -e
endif

#
# make all
#
all: release

#
# make library and main executable for release
#
release: $(MAIN_LIB).$(LIB_EXT)
	@echo $(ECHO_FLAGS) " [CXX+LNK]\t$@"
	-$(ECHO) $(CXX) $(CXXFLAGS_RELEASE) $(INCLUDES) $(PLYGRND).cpp $(MAIN_LIB).$(LIB_EXT) $(LIB) -o $(PLYGRND)

$(MAIN_LIB).$(LIB_EXT): $(OBJS_RELEASE)
	$(ECHO) mkdir -p $(MAIN_LIB_DIR)
	@echo $(ECHO_FLAGS) " [LNK]  \t$@"
	$(ECHO) $(CXX) $(CXXFLAGS_RELEASE) $(LIB) -shared $^ -o $(MAIN_LIB).$(LIB_EXT)

obj/release/%.o: src/%.cpp
	$(ECHO)mkdir -p obj/release
	@echo $(ECHO_FLAGS) " [CXX]  \t$@"
	$(ECHO) $(CXX) $(CXXFLAGS_RELEASE) $(INCLUDES) -c $^ -o $@

#
# make library and main executable with debug symbols
#
debug: $(MAIN_LIB).dbg.$(LIB_EXT)
	@echo $(ECHO_FLAGS) " [CXX+LNK] {D} \t$@"
	-$(ECHO) $(CXX) $(CXXFLAGS_DEBUG) $(INCLUDES) $(PLYGRND).cpp $(MAIN_LIB).dbg.$(LIB_EXT) $(LIB) -o $(PLYGRND)

$(MAIN_LIB).dbg.$(LIB_EXT): $(OBJS_DEBUG)
	$(ECHO)mkdir -p $(MAIN_LIB_DIR)
	@echo $(ECHO_FLAGS) " [LNK] {D} \t$@"
	$(ECHO) $(CXX) $(CXXFLAGS_DEBUG) $(LIB) -shared $^ -o $(MAIN_LIB).dbg.$(LIB_EXT)

obj/debug/%.o: src/%.cpp
	$(ECHO)mkdir -pv obj/debug
	@echo $(ECHO_FLAGS) " [CXX] {D} \t$@"
	$(ECHO) $(CXX) $(CXXFLAGS_DEBUG) $(INCLUDES) -c $^ -o $@

#
# build tests
# all tests are currently build with the debug target
#
test: debug $(EXEC_TEST)

test/bin%: test/src%.cpp
	$(ECHO) mkdir -p test/bin
	@echo $(ECHO_FLAGS) " [LNK]\t$@"
	$(ECHO) $(CXX) $(CXXFLAGS_DEBUG) $(INCLUDES) $^ $(MAIN_LIB).dbg.$(LIB_EXT) $(LIB) -o $@

#
# run tests
#
#    make checkTestName -> starts the test with the executable test/bin/TestName
#    make check         -> runs TestAll binary which includes all default tests
#
check%: test/bin/%
	$^

check: test
	@echo $(ECHO_FLAGS) " [TESTING] "
	./test/bin/TestAll



# create documentation
ifeq ($(verbose), 1)
  OUT_REDIR :=
else
  OUT_REDIR := > /dev/null
endif

.PHONY: doc
doc:
	$(ECHO) mkdir -p doc
	@echo $(ECHO_FLAGS) "[DOC]"
	$(ECHO)doxygen ./Doxyfile $(OUT_REDIR)

.PHONY: clean
clean:
	rm -vrf $(PLYGRND) obj/* doc/* test/bin/* $(MAIN_LIB).dbg.$(LIB_EXT) $(MAIN_LIB).$(LIB_EXT)
