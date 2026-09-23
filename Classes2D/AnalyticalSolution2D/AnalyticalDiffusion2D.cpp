#include "Classes2D/AnalyticalSolution2D/AnalyticalDiffusion2D.hpp"
#include <iostream>
#include <iomanip>
#include <cmath>
#include <algorithm>
#include <stdexcept>

namespace
{
constexpr double Pi = 3.14159265358979323846;
}

AnalyticalDiffusion2D::AnalyticalDiffusion2D(
    const Mesh2D& Mesh2D_Obj,
    const Geometry& Geometry_Obj,
    const Boundary& TopWall_Obj,
    const Boundary& BottomWall_Obj,
    const Boundary& RightWall_Obj,
    const Boundary& LeftWall_Obj
)
    : NumOfXNodes(Mesh2D_Obj.GetNumOfXNodes()),
      NumOfYNodes(Mesh2D_Obj.GetNumOfYNodes()),
      AnalyticalValues(NumOfYNodes, std::vector<double>(NumOfXNodes, 0.0)),
      XPositions(Mesh2D_Obj.GetXPositionVector()),
      YPositions(Mesh2D_Obj.GetYPositionVector()),
      Height(Geometry_Obj.GetHeight()),
      Width(Geometry_Obj.GetWidth()),
      TopWallValues(TopWall_Obj.ReturnBoundaryValues()),
      BottomWallValues(BottomWall_Obj.ReturnBoundaryValues()),
      RightWallValues(RightWall_Obj.ReturnBoundaryValues()),
      LeftWallValues(LeftWall_Obj.ReturnBoundaryValues()),
      Tolerance(1.0e-8),
      InitialNumberOfTerms(10),
      MaximumNumberOfTerms(2000)
{
    CalculateAnalyticalValues();
}

void AnalyticalDiffusion2D::CalculateAnalyticalValues()
{
    if (NumOfXNodes < 2 || NumOfYNodes < 2)
    {
        throw std::runtime_error("AnalyticalDiffusion2D needs at least 2 nodes in each direction.");
    }
    if (Width <= 0.0 || Height <= 0.0)
    {
        throw std::runtime_error("AnalyticalDiffusion2D needs positive Width and Height.");
    }
    if (XPositions.size() != NumOfXNodes || YPositions.size() != NumOfYNodes)
    {
        throw std::runtime_error("Position vector size is not consistent with mesh node numbers.");
    }
    if (TopWallValues.size() != NumOfXNodes ||
        BottomWallValues.size() != NumOfXNodes ||
        RightWallValues.size() != NumOfYNodes ||
        LeftWallValues.size() != NumOfYNodes)
    {
        throw std::runtime_error("Boundary value vector size is not consistent with mesh node numbers.");
    }

    std::size_t PreviousNumberOfTerms = 0;
    std::size_t CurrentNumberOfTerms = InitialNumberOfTerms;

    while (CurrentNumberOfTerms <= MaximumNumberOfTerms)
    {
        std::vector<std::vector<double>> PreviousValues = AnalyticalValues;

        const std::size_t LastTerm =
            std::min(CurrentNumberOfTerms, MaximumNumberOfTerms);

        for (std::size_t n = PreviousNumberOfTerms + 1; n <= LastTerm; ++n)
        {
            AddFourierTerm(n);
        }

        PreviousNumberOfTerms = LastTerm;

        ApplyBoundaryValues();

        const double MaximumDifference =
            CalculateMaximumDifference(PreviousValues, AnalyticalValues);

        if (MaximumDifference < Tolerance)
        {
            break;
        }

        if (CurrentNumberOfTerms == MaximumNumberOfTerms)
        {
            break;
        }

        CurrentNumberOfTerms *= 2;
    }
}

double AnalyticalDiffusion2D::CalculateFourierCoefficient(
    const std::vector<double>& Positions,
    const std::vector<double>& BoundaryValues,
    double Length,
    std::size_t TermNumber
) const
{
    double Integral = 0.0;
    const double n = static_cast<double>(TermNumber);

    for (std::size_t k = 0; k + 1 < Positions.size(); ++k)
    {
        const double x0 = Positions[k];
        const double x1 = Positions[k + 1];

        const double f0 =
            BoundaryValues[k] *
            std::sin(n * Pi * x0 / Length);

        const double f1 =
            BoundaryValues[k + 1] *
            std::sin(n * Pi * x1 / Length);

        Integral += 0.5 * (f0 + f1) * (x1 - x0);
    }

    return (2.0 / Length) * Integral;
}

double AnalyticalDiffusion2D::CalculateSinhRatio(
    double NumeratorArgument,
    double DenominatorArgument
) const
{
    if (DenominatorArgument <= 0.0)
    {
        return 0.0;
    }

    const double ExpPart = std::exp(NumeratorArgument - DenominatorArgument);
    const double NumeratorCorrection = 1.0 - std::exp(-2.0 * NumeratorArgument);
    const double DenominatorCorrection = 1.0 - std::exp(-2.0 * DenominatorArgument);

    if (std::abs(DenominatorCorrection) < 1.0e-14)
    {
        return 0.0;
    }

    return ExpPart * (NumeratorCorrection / DenominatorCorrection);
}

void AnalyticalDiffusion2D::AddFourierTerm(std::size_t TermNumber)
{
    const double n = static_cast<double>(TermNumber);

    const double BottomCoefficient =
        CalculateFourierCoefficient(
            XPositions,
            BottomWallValues,
            Width,
            TermNumber
        );

    const double TopCoefficient =
        CalculateFourierCoefficient(
            XPositions,
            TopWallValues,
            Width,
            TermNumber
        );

    const double LeftCoefficient =
        CalculateFourierCoefficient(
            YPositions,
            LeftWallValues,
            Height,
            TermNumber
        );

    const double RightCoefficient =
        CalculateFourierCoefficient(
            YPositions,
            RightWallValues,
            Height,
            TermNumber
        );

    const double VerticalDenominatorArgument = n * Pi * Height / Width;
    const double HorizontalDenominatorArgument = n * Pi * Width / Height;

    for (std::size_t j = 0; j < NumOfYNodes; ++j)
    {
        const double y = YPositions[j];

        const double BottomYFactor =
            CalculateSinhRatio(
                n * Pi * (Height - y) / Width,
                VerticalDenominatorArgument
            );

        const double TopYFactor =
            CalculateSinhRatio(
                n * Pi * y / Width,
                VerticalDenominatorArgument
            );

        const double YSine =
            std::sin(n * Pi * y / Height);

        for (std::size_t i = 0; i < NumOfXNodes; ++i)
        {
            const double x = XPositions[i];

            const double XSine =
                std::sin(n * Pi * x / Width);

            const double LeftXFactor =
                CalculateSinhRatio(
                    n * Pi * (Width - x) / Height,
                    HorizontalDenominatorArgument
                );

            const double RightXFactor =
                CalculateSinhRatio(
                    n * Pi * x / Height,
                    HorizontalDenominatorArgument
                );

            AnalyticalValues[j][i] +=
                BottomCoefficient * BottomYFactor * XSine +
                TopCoefficient * TopYFactor * XSine +
                LeftCoefficient * LeftXFactor * YSine +
                RightCoefficient * RightXFactor * YSine;
        }
    }
}

double AnalyticalDiffusion2D::CalculateMaximumDifference(
    const std::vector<std::vector<double>>& OldValues,
    const std::vector<std::vector<double>>& NewValues
) const
{
    double MaximumDifference = 0.0;

    for (std::size_t j = 0; j < NumOfYNodes; ++j)
    {
        for (std::size_t i = 0; i < NumOfXNodes; ++i)
        {
            const double Difference = std::abs(NewValues[j][i] - OldValues[j][i]);
            if (Difference > MaximumDifference)
            {
                MaximumDifference = Difference;
            }
        }
    }

    return MaximumDifference;
}

void AnalyticalDiffusion2D::ApplyBoundaryValues()
{
    // اعمال مرز پایین: j = 0 (دیواره BottomWall)
    for (std::size_t i = 0; i < NumOfXNodes; ++i)
    {
        AnalyticalValues[0][i] = BottomWallValues[i];
    }

    // اعمال مرز بالا: j = NumOfYNodes - 1 (دیواره TopWall)
    for (std::size_t i = 0; i < NumOfXNodes; ++i)
    {
        AnalyticalValues[NumOfYNodes - 1][i] = TopWallValues[i];
    }

    // اعمال مرز چپ: i = 0 (دیواره LeftWall)
    for (std::size_t j = 0; j < NumOfYNodes; ++j)
    {
        AnalyticalValues[j][0] = LeftWallValues[j];
    }

    // اعمال مرز راست: i = NumOfXNodes - 1 (دیواره RightWall)
    for (std::size_t j = 0; j < NumOfYNodes; ++j)
    {
        AnalyticalValues[j][NumOfXNodes - 1] = RightWallValues[j];
    }
}

double AnalyticalDiffusion2D::GetValue(std::size_t j, std::size_t i) const
{
    return AnalyticalValues[j][i];
}

std::size_t AnalyticalDiffusion2D::GetNrows() const
{
    return NumOfYNodes;
}

std::size_t AnalyticalDiffusion2D::GetNcols() const
{
    return NumOfXNodes;
}

void AnalyticalDiffusion2D::PrintAnalyticalValues() const
{
    for (std::size_t j = 0; j < NumOfYNodes; ++j)
    {
        for (std::size_t i = 0; i < NumOfXNodes; ++i)
        {
            std::cout << std::scientific << std::setprecision(4) << std::setw(15) << AnalyticalValues[j][i];
        }
        std::cout << '\n';
    }
}
