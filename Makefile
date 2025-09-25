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