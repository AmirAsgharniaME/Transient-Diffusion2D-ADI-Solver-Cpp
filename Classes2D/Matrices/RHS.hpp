#pragma once

#include <iostream>
#include <iomanip>
#include <cstddef>
#include <vector>

#include "Classes2D/Field2D/Field2D.hpp"
#include "Classes2D/Mesh2D/Mesh2D.hpp"
#include "Classes2D/SolverSettings/SolutionParameters.hpp"
#include "EssentialHeaders/SolverInputs.hpp"
#include "EssentialHeaders/SolverOptions.hpp"

class RHS
{


public:
    

    //RHS For Lassonen 
    //For X-Direction
    explicit RHS(
        const SweepDirection SweepDirection_,
        std::size_t Index_,
        const Mesh2D& Mesh2D_Obj,
        const Field2D& Field2D_Obj,
        const SolutionParameters<double> DiffNumber_Star,
        const double Default_Value_ = 0.0);

    
    //RHS For Lassonen 
    //For Y-Direction
    explicit RHS(
        const Field2D& Field2D_n_Obj,
        const SolutionParameters<double>& DiffNumerX_Obj,
        const SolutionParameters<double>& DiffNumerY_Obj,
        double Default_Initial_Value_ = 0.0      
    );


   //Public Setter
    void SetValue(std::size_t Index, double Value_);
    void SetAllSingleValue(double Value_);

    //Public Getters
     std::size_t GetSize() const;
     double GetValue(std::size_t Index) const;
     void SetBoundaryConditions();

private:
    //const SweepDirection Direction;
    const SweepDirection Direction;
    std::size_t J_Index =1 ;
    std::size_t I_Index =1 ;
    std::size_t Index;
    std::size_t Nx =5;
    std::size_t Ny =5;
    double diffNumber_Star; //x Difussion Number OR y Difussion Number
    std::vector<double> RHSValues;

    void CreateRHS(const Field2D& Field2D_Obj,double diffNumber_Star_);
};

void swap(Field2D& Left, Field2D& Right) noexcept;

