#pragma once

#include <cstddef>
#include <vector>
#include "Classes2D/Mesh2D/Mesh2D.hpp"
#include "EssentialHeaders/SolverOptions.hpp"

// enum class BoundaryLocation
// {
//     Top,
//     Bottom,
// };


class Field1D
{
public:
    explicit Field1D(
        const SweepDirection SweepDirection_,
        const Mesh2D& Mesh2D_Obj,
        const double InitialValue_ = 0.0);
    
    std::size_t Size() const;

    double GetValue(std::size_t Index_) const;
    void SetValue(std::size_t Index_, double Value_);

    //void SetSideValues(const BoundaryLocation Location ,const double Value_);

    // void SetAllValues(double Value_);

    // void Swap(Field1D& Other) noexcept;
    // void Print() const;

private:
        const SweepDirection Direction;
        std::size_t Nx = 5;
        std::size_t Ny = 5 ;
        std::size_t J_Index;
        std::size_t I_Index;
        std::vector<double> Field1D_Values;

};

    
    

void swap(Field1D& Left, Field1D& Right) noexcept;

