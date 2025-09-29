# Compiler settings - Can be customized.
CC = g++
CXXFLAGS = -std=c++17 -Wall -fPIC
LDFLAGS = 

# Makefile settings - Can be customized.
APPNAME = main
EXT = .cpp
SRCDIR = src
OBJDIR = obj

TESTAPPNAME = test_app
TESTSRCDIR = test
TESTOBJDIR = testobj


############## Do not change anything from here downwards! #############
SRC = $(wildcard $(SRCDIR)/*$(EXT))
OBJ = $(SRC:$(SRCDIR)/%$(EXT)=$(OBJDIR)/%.o)
DEP = $(OBJ:$(OBJDIR)/%.o=%.d)

TESTSRC = $(wildcard $(TESTSRCDIR)/*$(EXT))
TESTOBJ = $(TESTSRC:$(TESTSRCDIR)/%$(EXT)=$(TESTOBJDIR)/%.o)
TESTDEP = $(TESTOBJ:$(TESTOBJDIR)/%.o=%.d)
# UNIX-based OS variables & settings
RM = rm
DELOBJ = $(OBJ) $(TESTOBJ)
# Windows OS variables & settings
DEL = del
EXE = .exe
WDELOBJ = $(SRC:$(SRCDIR)/%$(EXT)=$(OBJDIR)\\%.o)
WDELTESTOBJ = $(TESTSRC:$(TESTSRCDIR)/%$(EXT)=$(TESTOBJDIR)\\%.o)

########################################################################
####################### Targets beginning here #########################
########################################################################

# Builds the app
$(APPNAME): $(OBJ) main.cpp
	$(CC) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

$(TESTAPPNAME): $(OBJ) $(TESTOBJ)
	$(CC) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

# Dependency generation (for each source file)
%.d: $(SRCDIR)/%$(EXT)
	@$(CC) $(CXXFLAGS) -MM $< -MT $(OBJDIR)/$*.o > $@

# Includes all .h files
-include $(DEP)
-include $(TESTDEP)

# Building rule for .o files from src
$(OBJDIR)/%.o: $(SRCDIR)/%$(EXT)
	@mkdir -p $(OBJDIR)
	$(CC) $(CXXFLAGS) -o $@ -c $<

$(TESTOBJDIR)/%.o: $(TESTSRCDIR)/%$(EXT)
	@mkdir -p $(TESTOBJDIR)
	$(CC) $(CXXFLAGS) -o $@ -c $<  -Isrc/

SHAREDDIR = shortest_path
SRCBASE = $(notdir $(basename $(SRC)))
SHAREDLIBS = $(addprefix $(SHAREDDIR)/, $(addsuffix .so, $(SRCBASE)))

# Rule to build all shared libraries
sharedlibs: $(SHAREDLIBS)

# Pattern rule to build each shared library from its object file
$(SHAREDDIR)/%.so: $(OBJ)
	@mkdir -p $(SHAREDDIR)
	$(CC) -shared -fPIC -o $@ $^

# Update 'all' target to build shared libs too
all: $(APPNAME) $(TESTAPPNAME) sharedlibs

# Benchmark settings
BENCHDIR = benchmark
BENCHBIN_DIR = benchbin
BENCHSRC = $(wildcard $(BENCHDIR)/*.cpp)
BENCHBINARIES = $(patsubst $(BENCHDIR)/%.cpp,$(BENCHBIN_DIR)/%,$(BENCHSRC))

.PHONY: bench run-benches

# Build all benchmark executables
bench: $(BENCHBINARIES)

# Pattern rule: build single benchmark exe from its .cpp and link project objects
# Adjust linking flags if your benchmarks don't need project objects -- remove $(OBJ) in that case.
$(BENCHBIN_DIR)/%: $(BENCHDIR)/%.cpp $(OBJ)
	@mkdir -p $(BENCHBIN_DIR)
	$(CC) $(CXXFLAGS) -Isrc -o $@ $< $(OBJ) $(LDFLAGS) -lbenchmark -lpthread

# Run all benchmarks sequentially (use --benchmark_filter or run a single exe directly for selective runs)
run-benches: bench
	@for f in $(BENCHBINARIES); do \
		echo "==== Running $$f ===="; \
		"./$$f"; \
	done

################### Cleaning rules for Unix-based OS ###################
# Cleans complete project
.PHONY: clean
clean:
	$(RM) $(DELOBJ) $(DELTESTOBJ)  $(DEP) $(TESTDEP) $(APPNAME)

# Cleans only all files with the extension .d
.PHONY: cleandep
cleandep:
	$(RM) $(DEP) $(TESTDEP)

#################### Cleaning rules for Windows OS #####################
# Cleans complete project
.PHONY: cleanw
cleanw:
	$(DEL) $(WDELOBJ) $(WDELTESTOBJ) $(DEP) $(TESTDEP) $(APPNAME)$(EXE)

# Cleans only all files with the extension .d
.PHONY: cleandepw
cleandepw:
	$(DEL) $(DEP) $(TESTDEP)