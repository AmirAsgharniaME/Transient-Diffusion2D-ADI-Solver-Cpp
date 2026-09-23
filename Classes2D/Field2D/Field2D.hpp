#pragma once

#include <iostream>
#include <iomanip>
#include <cstddef>
#include <vector>
#include "Classes2D/Mesh2D/Mesh2D.hpp"
#include "Classes2D/Field1D/Field1D.hpp"

enum class BoundaryLocation
{
    Top,
    Bottom,
    Right,
    Left
};

class Field2D
{


private:
    std::size_t nrows;
    std::size_t ncols;
    std::vector<std::vector<double>> FieldValues;


public:
    explicit Field2D(
        const Mesh2D& Mesh2D_Obj,
        double Default_Initial_Value_ = 0.0);

    
    /*****Public Setter*****/ 
    void SetValue(std::size_t jIndex, std::size_t iIndex, double Value_);
    void SetAllSingleValue(double Value_);
    void SetAll2DValue(const std::vector<std::vector<double>>& Value2D_);
    void SetSideValues(const BoundaryLocation Location ,const std::vector<double>& BoundaryValues_);
    void SetRowValues(std::size_t Row_Index,Field1D& Field1D_Obj);
    void SetColValues(std::size_t Col_Index,Field1D& Field1D_Obj);

    /*****Public Getters*****/ 
    std::size_t GetNrows() const;
    std::size_t GetNcols() const;
    double GetValue(std::size_t jIndex_, std::size_t iIndex_) const;

    /*****Public Method*****/ 
    void Print(const Mesh2D& Mesh2D_Obj);
    void Swap(Field2D& Other) noexcept;
    



};

void swap(Field2D& Left, Field2D& Right) noexcept;

