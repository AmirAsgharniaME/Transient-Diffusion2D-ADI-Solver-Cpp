#pragma once


#include "Classes2D/Mesh2D/Mesh2D.hpp"
#include <string>
#include <vector>
#include <cstddef>
#include "Classes2D/SolverSettings/SolutionParameters.hpp"
#include "EssentialHeaders/SolverOptions.hpp"

class TridiagonalMatrix
{
public:
    TridiagonalMatrix(
        SweepDirection SweepDirection_,
        const Mesh2D& Mesh2D_Obj,
        SolutionParameters<double> DiffNumber_StarX,
        SolutionParameters<double> DiffNumber_StarY,
        double Default_Value_ = 0.0);

    std::size_t SizeM() const;
    std::size_t SizeU() const;
    std::size_t SizeL() const;

    double GetMidValue(std::size_t Index_) const;
    double GetUValue(std::size_t Index_) const;
    double GetLValue(std::size_t Index_) const;

    void SetMValue(
        std::size_t Index_,
        double Value_);

    void SetUValue(
        std::size_t Index_,
        double Value_);

    void SetLValue(
        std::size_t Index_,
        double Value_);

    void SetBoundaryConditions();

private:
    const SweepDirection Direction;
    const std::size_t Nx;
    const std::size_t Ny;

    std::vector<double> LowerDiagonalValues;
    std::vector<double> MainDiagonalValues;
    std::vector<double> UpperDiagonalValues;

    double diffNumberX_Star;
    double diffNumberY_Star;
};
