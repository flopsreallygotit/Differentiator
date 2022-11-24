# Configure compiler. (I use .cpp extension for files, so it must me g++)
CXX = g++

# Configure compile flags.
CXXFLAGS ?= -Wall -Wextra -ggdb3

#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

# Configure sources you need to compile.
SOURCES = Universal/universalUtils.cpp Tree/treeUtils.cpp Differentiator/differentiatorUtils.cpp Differentiator/main.cpp

# Configure directory flags.
DIRFLAGS = -ITree -IUniversal -IDifferentiator

# Configure dependence headers.
HEADERS  = Tree/treeUtils.h Tree/treeConfig.h Universal/universalUtils.h Differentiator/differentiatorUtils.h

#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

# Pick executable name.
EXECUTABLE ?= main

#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

# WARNING: Don't change values and commands below.

# Making objects names from sources names.
OBJECTS = $(SOURCES:.cpp=.o)

# Adding directory flags to compiler flags.
CXXFLAGS += $(DIRFLAGS)

#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

.PHONY: all
all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS) $(HEADERS)
	@echo "Making executable from objects;"
	@$(CXX) $(CXXFLAGS) $(OBJECTS) -o $@
	@echo "Done;"

.PHONY: .cpp.o
.cpp.o:
	@echo "Making object file from $<;"
	@$(CXX) -c $(CXXFLAGS) $< -o $@

#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

.PHONY: clear
clear:
	@echo "Clearing object files and executable;";
	@rm -rf $(OBJECTS) $(DEPENDS) $(EXECUTABLE)

#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~