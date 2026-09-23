#include "Classes2D/Printer/StatusPrinter.hpp"
#include <iomanip>
#include <iostream>
void StatusPrinter::PrintStepStatus(const std::size_t TimeLevel,const double Residual,const SolutionParameters<double>& dt_Obj)
{

const double Time =static_cast<double>(TimeLevel) * dt_Obj.GetValue();
std::cout << "Step: " << std::setw(6) << TimeLevel
          << " | Time: " << std::fixed << std::setprecision(2)
          << std::setw(10) << Time << " s"
          << " | Relative residual: " << std::scientific
          << std::setprecision(2) << Residual
          << std::defaultfloat << '\n';
}


void StatusPrinter::PrintConvergenceStatus(const std::size_t TimeLevel,const double Residual,const SolutionParameters<double>& dt_Obj)
{
const double Time =static_cast<double>(TimeLevel) * dt_Obj.GetValue();
std::cout << "\n"
          << "========================================\n"
          << "Convergence achieved\n"
          << "Step   : " << std::setw(6) << TimeLevel << '\n'
          << "Time   : " << std::fixed << std::setprecision(2)
          << std::setw(10) << Time << " s\n"
          << "Residual: " << std::scientific << std::setprecision(2) << Residual << '\n'
          << "========================================\n"
          << std::defaultfloat;


}

