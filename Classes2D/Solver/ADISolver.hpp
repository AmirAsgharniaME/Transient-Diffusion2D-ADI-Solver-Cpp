#pragma once

#include <vector>
#include "Classes2D/Field2D/Field2D.hpp"
#include "Classes2D/Mesh2D/Mesh2D.hpp"
#include "Classes2D/Thermophysical_Properties/ThermophysicalProperties.hpp"
#include "Classes2D/SolverSettings/SolutionParameters.hpp"
#include "Classes2D/LinearSolvers/ThomasAlgorithm.hpp"


class Field2D;
class ThomasAlgorithm;

class ADISolver
{
public:
    ADISolver(
        Field2D& Field2D_Obj,
        Mesh2D& Mesh2D_Obj,
        ThermophysicalProperties alpha_Obj,
        SolutionParameters<double>& dt_Obj,
    );

    void advanceOneTimeStep();

private:

    double alpha_;
    double deltaX_;
    double deltaY_;
    double deltaT_;

    std::vector<std::vector<double>> intermediateField_;

    void solveFirstHalfStep();
    void solveSecondHalfStep();

    void solveRow(int rowIndex);
    void solveColumn(int columnIndex);

    void applyBoundaryConditions();
};
