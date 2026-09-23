# ==============================================================================
# Compiler Configuration
# ==============================================================================
CXX      = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -g -I.

# Output binary target
TARGET = bin/main


# ==============================================================================
# Project Structure Breakdown
# ==============================================================================

# 1. Essential Headers:
#    Contains standalone, header-only declarations and utilities (.hpp only)
ESSENTIAL_HEADERS = $(wildcard EssentialHeaders/*.hpp)


# 2. 2D Simulation Classes:
#    Modular object-oriented components. Each class consists of:
#    - Declaration / Interface : (.hpp)
#    - Implementation / Source : (.cpp)

# Class Headers (.hpp)
CLASS_HEADERS = \
    Classes2D/Geometry/Geometry.hpp \
    Classes2D/Thermophysical_Properties/ThermophysicalProperties.hpp \
    Classes2D/SolverSettings/SolutionParameters.hpp \
    Classes2D/Mesh2D/Mesh2D.hpp \
    Classes2D/Field2D/Field2D.hpp \
    Classes2D/InitialConditions/InitialConditions.hpp \
    Classes2D/Boundaries/Boundary.hpp \
    Classes2D/BoundaryConditions/BoundaryConditions.hpp \
    Classes2D/AnalyticalSolution2D/AnalyticalDiffusion2D.hpp \
    Classes2D/FileWriter2D/FileWriter2D.hpp \
    Classes2D/Plotter/ResidualPlotter.hpp \
    Classes2D/Plotter/OutputPlotter2D.hpp \
    Classes2D/Convergence/RelativeResidual.hpp \
    Classes2D/Printer/StatusPrinter.hpp \
    Classes2D/Matrices/TridiagonalMatrix.hpp \
    Classes2D/Field1D/Field1D.hpp \
    Classes2D/Matrices/RHS.hpp \
    Classes2D/LinearSolvers/ThomasAlgorithm.hpp
    

# Class Implementations (.cpp)
CLASS_SRCS = \
    Classes2D/Geometry/Geometry.cpp \
    Classes2D/Thermophysical_Properties/ThermophysicalProperties.cpp \
    Classes2D/SolverSettings/SolutionParameters.cpp \
    Classes2D/Mesh2D/Mesh2D.cpp \
    Classes2D/Field2D/Field2D.cpp \
    Classes2D/InitialConditions/InitialConditions.cpp \
    Classes2D/Boundaries/Boundary.cpp \
    Classes2D/BoundaryConditions/BoundaryConditions.cpp \
    Classes2D/AnalyticalSolution2D/AnalyticalDiffusion2D.cpp \
    Classes2D/FileWriter2D/FileWriter2D.cpp \
    Classes2D/Plotter/ResidualPlotter.cpp \
    Classes2D/Plotter/OutputPlotter2D.cpp \
    Classes2D/Convergence/RelativeResidual.cpp \
    Classes2D/Printer/StatusPrinter.cpp \
    Classes2D/Matrices/TridiagonalMatrix.cpp \
    Classes2D/Field1D/Field1D.cpp \
    Classes2D/Matrices/RHS.cpp \
    Classes2D/LinearSolvers/ThomasAlgorithm.cpp
   


# 3. Application Entry Point:
#    The driver routine that links all classes and headers together
MAIN_SRC = main/main.cpp


# ==============================================================================
# Aggregated Dependencies
# ==============================================================================
ALL_SRCS    = $(MAIN_SRC) $(CLASS_SRCS)
ALL_HEADERS = $(ESSENTIAL_HEADERS) $(CLASS_HEADERS)


# ==============================================================================
# Build Rules
# ==============================================================================
.PHONY: all clean

all: $(TARGET)

$(TARGET): $(ALL_SRCS) $(ALL_HEADERS)
	mkdir -p bin
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(ALL_SRCS)

clean:
	rm -rf bin/*
