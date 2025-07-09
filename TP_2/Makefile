# Assumindo que seu Makefile atual se parece com algo assim:
# Variáveis
CXX = g++
CXXFLAGS = -Wall -Iinclude -std=c++11
SRCDIR = src
OBJDIR = obj
BINDIR = bin
INCDIR = include

SOURCES = $(wildcard $(SRCDIR)/*.cpp)
OBJECTS = $(SOURCES:$(SRCDIR)/%.cpp=$(OBJDIR)/%.o)
TARGET = $(BINDIR)/tp2.out

all: $(TARGET)

$(TARGET): $(OBJECTS) | $(BINDIR)
	$(CXX) $(CXXFLAGS) $(OBJECTS) -o $@

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp | $(OBJDIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJDIR):
	mkdir -p $(OBJDIR)

$(BINDIR):
	mkdir -p $(BINDIR)

clean:
	rm -f obj/*.o
	rm -f bin/*.out
	rm -f bin/tp2.out

.PHONY: all clean