/*===========
C++ Libraries
=============*/ 
#include <iostream>
#include <vector>
#include <cmath> 
#include <iomanip>
#include <thread> // Added for optional small delays


/*======
Headers
========*/

//Stops The Loop Solver Iterations by Pressing ESC
#include "EssentialHeaders/KeyboardHandler.hpp"

//Inputs
#include "EssentialHeaders/SolverInputs.hpp"

#include "EssentialHeaders/SolverOptions.hpp"

/*=======
Classes
========*/

//Geometry
#include "Classes2D/Geometry/Geometry.hpp"

//Thermophysical_Properties
#include "Classes2D/Thermophysical_Properties/ThermophysicalProperties.hpp"

//SolverSettings
#include "Classes2D/SolverSettings/SolutionParameters.hpp"

//Mesh
#include "Classes2D/Mesh2D/Mesh2D.hpp"

//Field2D
#include "Classes2D/Field2D/Field2D.hpp"

//Field1D
#include "Classes2D/Field1D/Field1D.hpp"

//InitialConditions
#include "Classes2D/InitialConditions/InitialConditions.hpp"

//Boundaries
#include "Classes2D/Boundaries/Boundary.hpp"

//BoundaryConditions
#include "Classes2D/BoundaryConditions/BoundaryConditions.hpp"

//AnalyticalSolution2D
#include "Classes2D/AnalyticalSolution2D/AnalyticalDiffusion2D.hpp"

//FileWriter2D
#include "Classes2D/FileWriter2D/FileWriter2D.hpp"

//Plotter
#include "Classes2D/Plotter/ResidualPlotter.hpp"
#include "Classes2D/Plotter/OutputPlotter2D.hpp"


//Convergence
#include "Classes2D/Convergence/RelativeResidual.hpp"


//Printer
#include "Classes2D/Printer/StatusPrinter.hpp"

//Matrix
#include "Classes2D/Matrices/TridiagonalMatrix.hpp"
#include "Classes2D/Matrices/RHS.hpp"

//Linear Solvers
#include "Classes2D/LinearSolvers/ThomasAlgorithm.hpp"


// =============================================================================
// Solver Selection: Choose scheme directly here in the editor
// =============================================================================
enum class SolverScheme 
{
    Lassonen,
    CrankNicolson
};

constexpr SolverScheme ActiveScheme = SolverScheme::Lassonen;
//constexpr SolverScheme ActiveScheme = SolverScheme::CrankNicolson;

int main() 
{
const std::string schemeName = (ActiveScheme == SolverScheme::Lassonen) 
                                  ? "Lassonen" 
                                  : "CrankNicolson";

std::cout << "Selected Scheme: " << schemeName << std::endl;

// =========================================================================
// Setup (Geometry, Properties, Mesh, Conditions)
// =========================================================================

//===========
//Geometry 2D
//===========

Geometry Plate;
Plate.SetHeight(SolverInputs::Height_Value);
Plate.SetWidth(SolverInputs::Width_Value);


//======================
//ThermophysicalProperties
//========================
ThermophysicalProperties alpha;    //ThermalDiffusivity
alpha.SetValue(SolverInputs::alpha_Value);


//============
//Solver Settings
//===============
SolutionParameters<std::size_t> NumTimeLevels;
SolutionParameters<double> dt;
SolutionParameters<double> DiffNumberX;
SolutionParameters<double> DiffNumberX_Star;
SolutionParameters<double> DiffNumberY;
SolutionParameters<double> DiffNumberY_Star;
SolutionParameters<double> Tolerance;


NumTimeLevels.SetValue(SolverInputs::NumTimeStep_Value);
dt.SetValue(SolverInputs::dt_Value);



Tolerance.SetValue(SolverInputs::Tolerance_Value);

//========
//Mesh2D
//========

  Mesh2D mesh2D(Plate);
  mesh2D.SetNumOfXNodes(SolverInputs::NumOfXNodes_Value);
  mesh2D.SetNumOfYNodes(SolverInputs::NumOfYNodes_Value);
  //mesh2D.Print();

//=================
//Diffusion Nymber
//=================
DiffNumberX.SetValue((alpha.GetValue() * dt.GetValue())/(mesh2D.GetdeltaX()*mesh2D.GetdeltaX()));
DiffNumberY.SetValue((alpha.GetValue() * dt.GetValue())/(mesh2D.GetdeltaY()*mesh2D.GetdeltaY()));
DiffNumberX_Star.SetValue(DiffNumberX.GetValue()/2);
DiffNumberY_Star.SetValue(DiffNumberY.GetValue()/2);

//========
//Field1D
//========
Field1D T_nPlus1_Star_Row(SweepDirection::X, mesh2D);
Field1D T_nPlus1_Col(SweepDirection::Y, mesh2D);


//========
//Field2D
//========
Field2D T_0(mesh2D);
Field2D T_1(mesh2D);
Field2D T_n(mesh2D);
Field2D T_nPlus1_Star(mesh2D);
Field2D T_nPlus1(mesh2D);



//=====================================
//Apply Initial Conditions To Field2D T_0
//======================================
InitialConditions::ApplyInitialCondition(T_0, SolverInputs::Initial_Plate_Temp);



//======================
//Boundaries For Field2D
//======================
Boundary TopWall(mesh2D,BoundaryOrientation::Horizontal);
Boundary BottomWall(mesh2D, BoundaryOrientation::Horizontal);
Boundary RightWall(mesh2D, BoundaryOrientation::Vertical);
Boundary LeftWall(mesh2D, BoundaryOrientation::Vertical);

//Boundary Setters
TopWall.SetBoundaryValues(SolverInputs::TopWall_Temp_Values);
BottomWall.SetBoundaryValues(SolverInputs::BottomWall_Temp_Values);
RightWall.SetBoundaryValues(SolverInputs::RightWall_temp_Values);
LeftWall.SetBoundaryValues(SolverInputs::LeftWall_Temp_Values);


//=======================================
//Apply Boundary Conditions To Field2D T_0
//=========================================
BoundaryConditions::ApplyBoundaryCondition(T_0,TopWall,BoundaryLocation::Top);
BoundaryConditions::ApplyBoundaryCondition(T_0,BottomWall,BoundaryLocation::Bottom);
BoundaryConditions::ApplyBoundaryCondition(T_0,RightWall,BoundaryLocation::Right);
BoundaryConditions::ApplyBoundaryCondition(T_0,LeftWall,BoundaryLocation::Left);

//T_0.Print(mesh2D);


AnalyticalDiffusion2D U_Analytical_2D(mesh2D, Plate, TopWall, BottomWall, RightWall, LeftWall);


std::string RelativePath = "OutPutData/" + schemeName;

FileWriter2D::WriteField2D(T_0, mesh2D,FieldType::Initial_Numerical, RelativePath);
FileWriter2D::WriteField2D(U_Analytical_2D, mesh2D,FieldType::Steady_State_Analytical, RelativePath);


ResidualPlotter plotter("CFD Convergence: Diffusion 2D (" + schemeName + ")");
std::vector<double> stepHistory;
std::vector<double> residualHistory;

//=========================================================
// Create Tridiagonal  Matrix For ADI Method
// Tridiagonal Matrix Holds three 1D Vectors:
//  1.LowerDiagonal;
//  2.MainDiagonal;
//  3.UpperDiagonal;
// =============================================================
TridiagonalMatrix A3X(SweepDirection::X, mesh2D, DiffNumberX, DiffNumberX_Star);
TridiagonalMatrix A3Y(SweepDirection::Y, mesh2D, DiffNumberY, DiffNumberY_Star);

A3X.SetBoundaryConditions();
A3Y.SetBoundaryConditions();


//=======
//StartUp
//=======

T_n.Swap(T_0);


// ============
// Solver Loop 
// ============
std::size_t TimeLevel_start = 1;
std::size_t TimeLevel_n = TimeLevel_start;

std::size_t TimeLevel_Total = NumTimeLevels.GetValue();
std::size_t Nx = mesh2D.GetNumOfXNodes();
std::size_t Ny = mesh2D.GetNumOfYNodes();


for (; TimeLevel_n <= TimeLevel_Total; TimeLevel_n++)
{

//This Condition Stops The Loop Solver Iterations by Pressing ESC
if (isEscPressed())
{
    std::cout << "\nESC pressed. Exiting program now..." << std::endl;
    return 0;
}

// =====================
//ADI Method
// ====================
// SweepDirection::X
for (std::size_t J = 1; J < Ny- 1; ++J)
{
TridiagonalMatrix A3X_Copy = A3X;
RHS RHS1D_J(SweepDirection::X, J, mesh2D, T_n, DiffNumberY_Star);
RHS1D_J.SetBoundaryConditions();

ThomasAlgorithm::Solve(A3X_Copy, RHS1D_J, T_nPlus1_Star_Row);

T_nPlus1_Star.SetRowValues(J, T_nPlus1_Star_Row);
}


//Apply Boundary Conditions To Field2ِ T_nPlus1_Star 
BoundaryConditions::ApplyBoundaryCondition(T_nPlus1_Star,TopWall,BoundaryLocation::Top);
BoundaryConditions::ApplyBoundaryCondition(T_nPlus1_Star,BottomWall,BoundaryLocation::Bottom);
// BoundaryConditions::ApplyBoundaryCondition(T_nPlus1_Star,RightWall,BoundaryLocation::Right);
// BoundaryConditions::ApplyBoundaryCondition(T_nPlus1_Star,LeftWall,BoundaryLocation::Left);

//SweepDirection::Y
for (std::size_t I = 1; I < Nx - 1; ++I)
{
TridiagonalMatrix A3Y_Copy = A3Y;
RHS RHS1D_I(SweepDirection::Y, I, mesh2D, T_nPlus1_Star, DiffNumberX_Star);
RHS1D_I.SetBoundaryConditions();
ThomasAlgorithm::Solve(A3Y_Copy, RHS1D_I, T_nPlus1_Col);
T_nPlus1.SetColValues(I, T_nPlus1_Col);
}

//Apply Boundary Conditions To New Field
BoundaryConditions::ApplyBoundaryCondition(T_nPlus1,TopWall,BoundaryLocation::Top);
BoundaryConditions::ApplyBoundaryCondition(T_nPlus1,BottomWall,BoundaryLocation::Bottom);
BoundaryConditions::ApplyBoundaryCondition(T_nPlus1,RightWall,BoundaryLocation::Right);
BoundaryConditions::ApplyBoundaryCondition(T_nPlus1,LeftWall,BoundaryLocation::Left);

//Claculate The Relative Residual
double Residual = RelativeResidual::ReturnResidual(T_n,T_nPlus1);

// Print Step Status such as Time // Step // Relative Residual
StatusPrinter::PrintStepStatus(TimeLevel_n,Residual,dt);


//T_n <--- U_nPlus1 : T=Tnew Copy The Values But Swap does not Copy
T_n.Swap(T_nPlus1);

// Record data for plotting
stepHistory.push_back(static_cast<double>(TimeLevel_n));
residualHistory.push_back(Residual);

// Update plot every 20 steps
if (TimeLevel_n % 20 == 0) 
{
    plotter.updatePlot(stepHistory, residualHistory);
}


//Codition For Convergence To stady state Solution
if(Residual < Tolerance.GetValue())
{
StatusPrinter::PrintConvergenceStatus(TimeLevel_n,Residual,dt);
FileWriter2D::WriteField2D(T_n,mesh2D,FieldType::Steady_State_Numerical,RelativePath);
break;
}

}
//End Of Loop Solver*************


// =========================================================================
// 3. Common Post-Processing
// =========================================================================
OutputPlotter2D outputPlotter(mesh2D, "Solution Comparison (" + schemeName + ")");
outputPlotter.plotComparison(
    RelativePath + "/Initial_Numerical2D.dat", "Initial Condition",
    RelativePath + "/Steady_State_Numerical2D.dat", "Steady State Numerical Solution",
    RelativePath + "/Steady_State_Analytical2D.dat", "Steady State Analytical Solution"
);


    std::cout << "Calculations Completed Successfully for " << schemeName << std::endl;
    std::cin.get();
    return 0;
}
