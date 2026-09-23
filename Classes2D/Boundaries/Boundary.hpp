#pragma once

#include <vector>
#include <string>
#include "Classes2D/Mesh2D/Mesh2D.hpp"


enum class BoundaryOrientation
{
    Horizontal,
    Vertical
};

class Boundary
{
public:
    explicit Boundary(
    const Mesh2D& Mesh2D_Obj,
    BoundaryOrientation orientation,
    const double Default_Initial_BoundaryValues_= 0.0);

    
    void SetBoundaryValues(const std::vector<double>& BoundaryValues_);
    const std::vector<double>& ReturnBoundaryValues() const;

private:
    std::vector<double> BoundaryValues;
    std::string BoundaryName;
    
};
