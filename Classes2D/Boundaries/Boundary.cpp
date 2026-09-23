#include "Classes2D/Boundaries/Boundary.hpp"

Boundary::Boundary(
    const Mesh2D& Mesh2D_Obj,
    BoundaryOrientation orientation,
    const double Default_Initial_BoundaryValues_)
{
    if (orientation == BoundaryOrientation::Horizontal)
    {
        BoundaryValues.assign(Mesh2D_Obj.GetNumOfXNodes(), Default_Initial_BoundaryValues_);
    }
    else
    {
        BoundaryValues.assign(Mesh2D_Obj.GetNumOfYNodes(), Default_Initial_BoundaryValues_);
    }
}




void Boundary::SetBoundaryValues(const std::vector<double>& BoundaryValues_)
{
 for (size_t i = 0; i < BoundaryValues.size(); i++)
 {
    BoundaryValues[i]= BoundaryValues_[i];
 }
    
}

const std::vector<double>& Boundary::ReturnBoundaryValues() const
{
   return BoundaryValues;
}