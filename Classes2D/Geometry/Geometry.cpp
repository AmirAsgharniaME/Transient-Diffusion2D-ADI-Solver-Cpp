#include "Classes2D/Geometry/Geometry.hpp"
#include <stdexcept>


Geometry::Geometry(double DefaultValue_)
:Height(DefaultValue_),
 Width(DefaultValue_)
{}

double Geometry::GetHeight() const 
{return Height;}
double Geometry::GetWidth() const 
{return Width;}

void Geometry::SetHeight(double Value_)
{ 

    if (Value_<= 0.0)
    {
    throw std::invalid_argument("Height must be greater than zero.");
    }
    else
    {
       Height = Value_;
    }
    
}

void Geometry::SetWidth(double Value_)
{ 

    if (Value_<= 0.0)
    {
    throw std::invalid_argument("Width must be greater than zero.");
    }
    else
    {
       Width = Value_;
    }
    
}