#pragma once


#include "Classes2D/Field2D/Field2D.hpp"
#include  "Classes2D/Matrices/TridiagonalMatrix.hpp"
#include  "Classes2D/Matrices/RHS.hpp"
#include  "Classes2D/Field1D/Field1D.hpp"
 
#include <cstddef>

class ThomasAlgorithm
{
public:
    // (Stateless Class)
    ThomasAlgorithm() = delete;


    static void Solve(
        TridiagonalMatrix& A3,
        RHS& RHS1D_Obj,
        Field1D& Solution);

private:
    // (Forward Sweep)
    static void ForwardElimination(
        TridiagonalMatrix& TridiagonalMatrix,
        RHS& RHS_Obj);

    // (Back Substitution)
    static void BackSubstitution(
        const TridiagonalMatrix& TridiagonalMatrix,
        const RHS& RHS_Obj,
        Field1D& Solution);
};
