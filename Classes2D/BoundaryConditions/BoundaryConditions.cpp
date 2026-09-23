#include "Classes2D/BoundaryConditions/BoundaryConditions.hpp" // class BoundaryConditions;  forward declaration

void BoundaryConditions::ApplyBoundaryCondition(
    Field2D& Field2D_Obj,
    const Boundary& Boundary_Obj, 
    const BoundaryLocation Location)
    {
        if (Location == BoundaryLocation::Top)
        {
           Field2D_Obj.SetSideValues(BoundaryLocation::Top,Boundary_Obj.ReturnBoundaryValues());
        }
        else if (Location == BoundaryLocation::Bottom)
        {
            Field2D_Obj.SetSideValues(BoundaryLocation::Bottom,Boundary_Obj.ReturnBoundaryValues());
        }
        else if (Location == BoundaryLocation::Right)
        {
           Field2D_Obj.SetSideValues(BoundaryLocation::Right,Boundary_Obj.ReturnBoundaryValues());
        }
        else if (Location == BoundaryLocation::Left)
        {
             Field2D_Obj.SetSideValues(BoundaryLocation::Left,Boundary_Obj.ReturnBoundaryValues());
        }
        
        
    }

    