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

ifeq ($(DEBUG), true)
  CXXFLAGS = $(FLAGS_GLOBAL) $(FLAGS_DEBUG) -O0
else
  CXXFLAGS = $(FLAGS_GLOBAL) -O2
endif

# define source and object files for the project
# SRCS = $(wildcard src/*.cpp)
SRCS = src/Util.cpp \
	src/DataType.cpp \
	src/DataSet.cpp \
	src/NDBuffer.cpp \
	src/Group.cpp \
	src/PandoraEntity.cpp \
	src/NamedEntity.cpp \
	src/File.cpp \
	src/Block.cpp

OBJS = $(patsubst src%.cpp, obj%.o, $(SRCS))

# define source and executable files for tests
# SRCS_TEST = $(wildcard test/src/*.cpp)

SRCS_TEST = test/src/TestFile.cpp

EXEC_TEST = $(patsubst test/src%.cpp, test/bin%, $(SRCS_TEST))

### misc
ifeq ($(verbose), 1)
  ECHO :=
else
  ECHO := @
endif

#
# make all
#
all: lib $(ALL_EXTRA_TARGETS)

#
# make library
#
lib: $(MAIN_LIB).$(LIB_EXT)

$(MAIN_LIB).$(LIB_EXT): $(OBJS)
	$(ECHO)mkdir -p $(MAIN_LIB_DIR)
	@echo " [LNK]  \t$@"
	$(ECHO)$(CXX) $(CXXFLAGS) $(LIB) -shared $^ -o $(MAIN_LIB).$(LIB_EXT)
	
obj/%.o: src/%.cpp
	$(ECHO)mkdir -p obj/release
	@echo " [CXX]  \t$@"
	$(ECHO)$(CXX) $(CXXFLAGS) $(INCLUDES) -c $^ -o $@
	
#
# make playground
#
playgrnd: $(PLYGRND)

$(PLYGRND): lib $(PLYGRND).cpp
	@echo " [CXX+LNK]\t$@"
	$(ECHO)$(CXX) $(CXXFLAGS) $(INCLUDES) $(PLYGRND).cpp $(MAIN_LIB).$(LIB_EXT) $(LIB) -o $(PLYGRND)

#
# build tests
# all tests are currently build with the debug target
#
test: lib $(EXEC_TEST)

test/bin%: test/src%.cpp
	$(ECHO)mkdir -p test/bin
	@echo " [LNK]\t$@"
	$(ECHO)$(CXX) $(CXXFLAGS) $(INCLUDES) $^ $(MAIN_LIB).$(LIB_EXT) $(LIB) -o $@

#
# run tests
#
#    make checkTestName -> starts the test with the executable test/bin/TestName
#    make check         -> runs TestAll binary which includes all default tests
#
check%: test/bin/%
	$^

check: test
	@echo " [TESTING] "
	./test/bin/TestAll



# create documentation
ifeq ($(verbose), 1)
  OUT_REDIR :=
else
  OUT_REDIR := > /dev/null
endif

.PHONY: doc
doc:
	$(ECHO)mkdir -p doc
	@echo "[DOC]"
	$(ECHO)doxygen ./Doxyfile $(OUT_REDIR)

.PHONY: clean
clean:
	$(ECHO)rm -rf $(PLYGRND) obj/* doc/* test/bin/* $(MAIN_LIB).$(LIB_EXT)
