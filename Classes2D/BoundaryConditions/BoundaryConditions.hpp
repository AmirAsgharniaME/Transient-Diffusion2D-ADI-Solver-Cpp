#pragma once


#include "Classes2D/Boundaries/Boundary.hpp"
#include "Classes2D/Field2D/Field2D.hpp" // and enum class BoundaryLocation

class BoundaryConditions
{
public:
    static void ApplyBoundaryCondition(
    Field2D& Field2D_Obj,
    const Boundary& Boundary_Obj, 
    const BoundaryLocation Location);

};


