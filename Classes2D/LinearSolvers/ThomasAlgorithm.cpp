#include "Classes2D/LinearSolvers/ThomasAlgorithm.hpp"

#include <cmath>
#include <cstddef>
#include <stdexcept>
/*
 * Performs the forward elimination stage of the Thomas algorithm.
 *
 * The lower-diagonal entries are eliminated mathematically without being
 * explicitly set to zero. The main diagonal and right-hand-side vector
 * are modified in place, while the upper diagonal remains unchanged.
 *
 *
 * The Thomas algorithm is a specialized form of Gaussian elimination
 * designed exclusively for tridiagonal linear systems. Unlike standard
 * Gaussian elimination, it eliminates only the lower-diagonal entry
 * directly below each pivot because all other entries below the main
 * diagonal are already zero.
 *
 * Therefore, no full row operations are required. For a system with N
 * equations, the Thomas algorithm has O(N) computational complexity and
 * O(N) storage requirements, whereas standard Gaussian elimination for
 * a dense system generally requires O(N^3) operations and O(N^2) storage.
 *
 * During this stage, the lower-diagonal entries are eliminated
 * mathematically without being explicitly set to zero. The main diagonal
 * and the right-hand-side vector are modified in place, while the upper
 * diagonal remains unchanged.
 *
 * Unlike Gaussian elimination with partial pivoting, the standard Thomas
 * algorithm does not perform row exchanges. Therefore, a zero or near-zero
 * pivot causes a runtime error and may indicate that the method is not
 * numerically suitable for the given system.
 * 
 * A zero or near-zero pivot does not necessarily mean that the
 * tridiagonal system has no unique solution. It only means that
 * the standard Thomas algorithm without pivoting cannot proceed
 * reliably with the current ordering of equations.
 * 
 * TODO: Add the Pivoted Thomas Algorithm (Tridiagonal Gaussian Elimination
 * with Partial Pivoting) as a separate solver to improve numerical robustness
 * and handle tridiagonal systems in which the standard Thomas algorithm fails
 * because of a zero or near-zero pivot, even when a unique solution may exist.
 * Unlike the general partial row pivoting implemented in Gaussian elimination,
 * which searches all remaining rows in the current column for the largest
 * available pivot, tridiagonal partial pivoting compares only the current
 * diagonal entry with the adjacent subdiagonal entry and, when necessary,
 * interchanges two adjacent rows. This restricted pivoting preserves the
 * banded structure as much as possible, introduces at most a second
 * superdiagonal, and retains O(N) computational complexity.

*/

void ThomasAlgorithm::Solve(
        TridiagonalMatrix& A3,
        RHS& RHS1D_Obj,
        Field1D& Solution)
    {
    ForwardElimination(A3 , RHS1D_Obj);
    BackSubstitution(A3 , RHS1D_Obj, Solution);
    }

void ThomasAlgorithm::ForwardElimination(
        TridiagonalMatrix& TridiagonalMatrix , RHS& RHS_Obj)
{
    const std::size_t NumRows = TridiagonalMatrix.SizeM();

    if (NumRows == 0)
    {
        throw std::invalid_argument(
            "ForwardElimination: diagonal vectors must not be empty.");
    }

    if (TridiagonalMatrix.SizeL() != NumRows|| TridiagonalMatrix.SizeU() != NumRows || RHS_Obj.GetSize() != NumRows)
    {
        throw std::invalid_argument(
            "ForwardElimination: all diagonal vectors and RHS must have the same size.");
    }


for (std::size_t RowIndex = 1; RowIndex < NumRows; ++RowIndex)
    {
    const double OriginalDiagonalValue = TridiagonalMatrix.GetMidValue(RowIndex - 1);

    if (std::fabs(OriginalDiagonalValue) < 1.0e-14)
    {
        throw std::runtime_error(
            "ForwardElimination: zero or nearly zero diagonal entry detected.");
    }

    const double Factor =TridiagonalMatrix.GetLValue(RowIndex)/ OriginalDiagonalValue;

    const double NewDiagonal =TridiagonalMatrix.GetMidValue(RowIndex)- Factor * TridiagonalMatrix.GetUValue(RowIndex - 1);

    TridiagonalMatrix.SetMValue(RowIndex,NewDiagonal);

    const double NewRHS =RHS_Obj.GetValue(RowIndex)- Factor * RHS_Obj.GetValue(RowIndex - 1);

    RHS_Obj.SetValue(RowIndex,NewRHS);

    TridiagonalMatrix.SetLValue(RowIndex,0.0);

    }

}


void ThomasAlgorithm::BackSubstitution(
    const TridiagonalMatrix& TridiagonalMatrix,
    const RHS& RHS_Obj,
    Field1D& Solution)
{
    const std::size_t NumRows = TridiagonalMatrix.SizeM();

    if (NumRows == 0)
    {
        throw std::invalid_argument(
            "ThomasAlgorithm::BackSubstitution: "
            "diagonal vectors must not be empty.");
    }

    if (TridiagonalMatrix.SizeU() != NumRows
        || RHS_Obj.GetSize() != NumRows
        || Solution.Size() != NumRows)
    {
        throw std::invalid_argument(
            "ThomasAlgorithm::BackSubstitution: "
            "all vectors must have the same size.");
    }

    const double LastDiagonalValue = TridiagonalMatrix.GetMidValue(NumRows - 1);

    if (std::fabs(LastDiagonalValue) < 1.0e-14)
    {
        throw std::runtime_error(
            "ThomasAlgorithm::BackSubstitution: "
            "zero or near-zero pivot encountered.");
    }

    Solution.SetValue(NumRows - 1,RHS_Obj.GetValue(NumRows - 1) / LastDiagonalValue);

    for (std::ptrdiff_t RowIndex = static_cast<std::ptrdiff_t>(NumRows) - 2; RowIndex >= 0; --RowIndex)
    {
        
        const std::size_t i = static_cast<std::size_t>(RowIndex);

        const double DiagonalValue = TridiagonalMatrix.GetMidValue(i);

        if (std::fabs(DiagonalValue) < 1.0e-14)
        {
            throw std::runtime_error(
                "ThomasAlgorithm::BackSubstitution: "
                "zero or near-zero pivot encountered.");
        }

        const double NewSolution =
            (RHS_Obj.GetValue(i)
             - TridiagonalMatrix.GetUValue(i)
               * Solution.GetValue(i + 1))
            / DiagonalValue;

        Solution.SetValue(i, NewSolution);
    }
}

