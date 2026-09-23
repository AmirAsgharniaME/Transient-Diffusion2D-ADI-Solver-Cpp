#pragma once

#include <vector>
#include <cstddef>

namespace SolverInputs
{   /*******************Geometry Inputs*****************************/
    inline constexpr double Height_Value = 3.5; //[ft]
    inline constexpr double Width_Value = 3.5; //[ft]

    /*****************Thermophysical_Properties Inputs************************/
    inline constexpr double alpha_Value = 0.645;  //// [ft^2/hr]

    /****************Solver Settings Inputs**************************/
    inline constexpr std::size_t NumTimeStep_Value = 20000;
    inline constexpr double dt_Value = 0.001;
    inline constexpr double Tolerance_Value = 1e-7;
    /**********************Mesh2D*****************************/
    inline constexpr std::size_t NumOfXNodes_Value =40;
    inline constexpr std::size_t NumOfYNodes_Value =40;

   /***********Initial Condition Values****************/
   inline const std::vector<std::vector<double>> Initial_Plate_Temp(NumOfYNodes_Value,std::vector<double>(NumOfXNodes_Value,0.0));

   /***************Boundary Codition Values*****************/

   inline const std::vector<double> TopWall_Temp_Values(NumOfXNodes_Value,0.0);
   inline const std::vector<double> BottomWall_Temp_Values(NumOfXNodes_Value,200.0);
   inline const std::vector<double> RightWall_temp_Values(NumOfYNodes_Value,0.0);
   inline const std::vector<double> LeftWall_Temp_Values(NumOfYNodes_Value,200.0);
}
