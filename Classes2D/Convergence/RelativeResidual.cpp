#include "Classes2D/Convergence/RelativeResidual.hpp"
#include <cstddef>
#include <cmath>



/*
Relative L2 Norm of the Solution Change
Relative L2 Residual Based on Successive Iteration Difference
in cfd : Relative residual or Relative residual of velocity based on solution update
ComputeRelativeL2VelocityChange
Residual =
sqrt( Σ (Unew_i - Uold_i)^2 )
/
sqrt( Σ (Unew_i)^2 )

*/

double RelativeResidual::ReturnResidual(const Field2D& Field2D_n_obj,const Field2D& Field2D_nPlus1_obj)

{
    double Sum1 = 0.0; // Σ (Fielnew[j][i] - Field2D_n_obj[j][i])^2
    double Sum2 = 0.0; // Σ (Fielnew[j][i])^2

    const std::size_t nrows = Field2D_n_obj.GetNrows();
    const std::size_t ncols = Field2D_n_obj.GetNcols();

    for (std::size_t j = 0; j < nrows; ++j)
    {
        for (std::size_t i = 0; i < ncols; ++i)
        {
            const double Difference =
                Field2D_nPlus1_obj.GetValue(j, i) - Field2D_n_obj.GetValue(j, i);

            const double Field_nplus1 = Field2D_nPlus1_obj.GetValue(j, i);

            Sum1 += Difference * Difference;
            Sum2 += Field_nplus1 * Field_nplus1;
        }
    }

    return std::sqrt(Sum1) / (std::sqrt(Sum2) + 1.0e-30);
}
