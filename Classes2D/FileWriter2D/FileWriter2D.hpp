#pragma once

#include "Classes2D/Field2D/Field2D.hpp"
#include "Classes2D/AnalyticalSolution2D/AnalyticalDiffusion2D.hpp"
#include "Classes2D/Mesh2D/Mesh2D.hpp"
#include <fstream>
#include <iomanip>
#include <filesystem>
#include <stdexcept>
#include <iostream>

#include <vector>
#include <string>

enum class FieldType
{
    Initial_Numerical,
    Steady_State_Numerical,
    Steady_State_Analytical,
};

class FileWriter2D
{
public:
    FileWriter2D() =delete;

   static void WriteField2D(
    Field2D& Field2D_Obj, 
    Mesh2D& Mesh2D_Obj,
    FieldType FieldType_,
    std::string RelativePath);

   static void WriteField2D(
    AnalyticalDiffusion2D& Analytical2D_Obj,
    Mesh2D& Mesh2D_Obj ,
     [[maybe_unused]] FieldType FieldType_,
    std::string RelativePath);


};