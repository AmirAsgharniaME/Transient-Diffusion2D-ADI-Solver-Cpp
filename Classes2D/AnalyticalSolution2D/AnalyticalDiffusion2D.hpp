#pragma once

#include <cstddef>
#include <vector>
#include <iostream>
#include <iomanip>
#include "Classes2D/Mesh2D/Mesh2D.hpp"
#include "Classes2D/Boundaries/Boundary.hpp"

class AnalyticalDiffusion2D
{
public:
    explicit AnalyticalDiffusion2D(
        const Mesh2D& Mesh2D_Obj,
        const Geometry& Geometry_Obj,
        const Boundary& TopWall_Obj,
        const Boundary& BottomWall_Obj,
        const Boundary& RightWall_Obj,
        const Boundary& LeftWall_Obj
    );

    void CalculateAnalyticalValues();

    double GetValue(std::size_t j, std::size_t i) const;
    std::size_t GetNrows() const;
    std::size_t GetNcols() const;
    void PrintAnalyticalValues() const;

private:
    double CalculateFourierCoefficient(
        const std::vector<double>& Positions,
        const std::vector<double>& BoundaryValues,
        double Length,
        std::size_t TermNumber
    ) const;

    double CalculateSinhRatio(double NumeratorArgument, double DenominatorArgument) const;

    void AddFourierTerm(std::size_t TermNumber);

    double CalculateMaximumDifference(
        const std::vector<std::vector<double>>& OldValues,
        const std::vector<std::vector<double>>& NewValues
    ) const;

    void ApplyBoundaryValues();

private:
    std::size_t NumOfXNodes;
    std::size_t NumOfYNodes;

    std::vector<std::vector<double>> AnalyticalValues;

    std::vector<double> XPositions;
    std::vector<double> YPositions;

    double Height;
    double Width;

    std::vector<double> TopWallValues;
    std::vector<double> BottomWallValues;
    std::vector<double> RightWallValues;
    std::vector<double> LeftWallValues;

    double Tolerance;
    std::size_t InitialNumberOfTerms;
    std::size_t MaximumNumberOfTerms;
};
