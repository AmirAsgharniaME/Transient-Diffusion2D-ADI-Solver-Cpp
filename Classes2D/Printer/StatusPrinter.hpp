#pragma once


#include <cstddef>
#include "Classes2D/SolverSettings/SolutionParameters.hpp"

class StatusPrinter
{
public:
    StatusPrinter() =delete;
    
   static void PrintStepStatus(const std::size_t TimeLevel,const double Residual,const SolutionParameters<double>& dt_Obj);
   static void PrintConvergenceStatus(const std::size_t TimeLevel,const double Residual,const SolutionParameters<double>& dt_Obj);

};

