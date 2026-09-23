#include "Classes2D/Matrices/TridiagonalMatrix.hpp"

TridiagonalMatrix::TridiagonalMatrix( 
        SweepDirection SweepDirection_,
        const Mesh2D& Mesh2D_Obj,
        SolutionParameters<double> DiffNumber_StarX,
        SolutionParameters<double> DiffNumber_StarY,
        double Default_Value_)
    : Direction(SweepDirection_),
      Nx(Mesh2D_Obj.GetNumOfXNodes()),
      Ny(Mesh2D_Obj.GetNumOfYNodes()),
      diffNumberX_Star(DiffNumber_StarX.GetValue()),
      diffNumberY_Star(DiffNumber_StarY.GetValue())
{
        const std::size_t N =(Direction == SweepDirection::X) ? Nx : Ny;

        LowerDiagonalValues.assign(N, Default_Value_);
        MainDiagonalValues.assign(N, Default_Value_);
        UpperDiagonalValues.assign(N, Default_Value_);

        // ================
        // Lassonen Method
        //SweepDirection::X
        // ================
        //  * First half-step of the Laasonen directional-splitting scheme.
        //  *
        //  * The solution is advanced from T^n to the intermediate field T^(n+1,*).
        //  * The x-direction is implicit at time level n+1,*,
        //  * while the y-direction is explicit at time level n.
        //  *
        //  * For interior nodes:
        //  *
        //  * [T(j,i)^(n+1,*) - T(j,i)^n] / (dt/2)
        //  *
        //  * = alpha * [
        //  *     (T(j-1,i)^(n+1,*) - 2*T(j,i)^(n+1,*) + T(j+1,i)^(n+1,*)) / dx^2
        //  *     + (T(j,i-1)^n - 2*T(j,i)^n + T(j,i+1)^n) / dy^2
        //  *   ]
        //  *
        //  * Define:
        //  *
        //  * diffNumberX_Star = alpha * (dt/2) / dx^2
        //  * diffNumberY_Star = alpha * (dt/2) / dy^2
        //  *
        //  * Rearranged form:
        //  *
        //  * -diffNumberX_Star * T(j-1,i)^(n+1,*)
        //  * + (1 + 2*diffNumberX_Star) * T(j,i)^(n+1,*)
        //  * - diffNumberX_Star * T(j+1,i)^(n+1,*) = RHS
        //  *
        //  * RHS= diffNumberY_Star * T(j,i-1)^n
        //  * + (1 - 2*diffNumberY_Star) * T(j,i)^n
        //  * + diffNumberY_Star * T(j,i+1)^n
        //  *
        //  * Valid for:
        //  *
        //  * i = 1, ..., Nx - 1
        //  * j = 1, ..., Ny - 1
        //  *
        //  * For each fixed j, this equation forms a tridiagonal this equation forms a tridiagonal system
        //  * along the x-direction for then+1,*).
 

        // Create : 
        // *LowerDiagonalValues 
        // *MainDiagonalValues
        // *UpperDiagonalValues
        // ========================================

        if (Direction == SweepDirection::X)
        {
        // Iterate through the interior nodes
        //For Uniform Grid the Tridiagonal Matrix are the same For each fixed j
        for (std::size_t i = 1; i + 1 < Nx; ++i)
        {
            LowerDiagonalValues[i] = -diffNumberX_Star;

            MainDiagonalValues[i] = 1.0 + 2.0 * diffNumberX_Star;

            UpperDiagonalValues[i] = -diffNumberX_Star; 
        }

        }

        // ================
        // Lassonen Method
        // SweepDirection::Y
        // ================

        // * Second half-step of the Laasonen directional-splitting scheme.
        // *
        // * The intermediate field T^(n+1,*) is advanced to T^(n+1).
        // * The x-direction is explicit at time level n+1,*,
        // * while the y-direction is implicit at time level n+1.
        // *
        // * For interior nodes:
        // *
        // * [T(j,i)^(n+1) - T(j,i)^(n+1,*)] / (dt/2)
        // *
        // * = alpha * [
        // *     (T(j-1,i)^(n+1,*) - 2*T(j,i)^(n+1,*) + T(j+1,i)^(n+1,*)) / dx^2
        // *     + (T(j,i-1)^(n+1) - 2*T(j,i)^(n+1) + T(j,i+1)^(n+1)) / dy^2
        // *   ]
        // *
        // * Define:
        // *
        // * diffNumberX_Star = alpha * (dt/2) / dx^2
        // * diffNumberY_Star = alpha * (dt/2) / dy^2
        // *
        // * Rearranged form:
        // *
        // * -diffNumberY_Star * T(j,i-1)^(n+1)
        // * + (1 + 2*diffNumberY_Star) * T(j,i)^(n+1)
        // * - diffNumberY_Star * T(j,i+1)^(n+1) = RHS
        // *
        // * RHS= diffNumberX_Star * T(j-1,i)^(n+1,*)
        // * + (1 - 2*diffNumberX_Star) * T(j,i)^(n+1,*)
        // * + diffNumberX_Star * T(j+1,i)^(n+1,*)
        // *
        // * Valid for:
        // *
        // * i = 1, ..., Nx - 1
        // * j = 1, ..., Ny - 1
        // *
        // * For each fixed i, this equation forms a tridiagonal system
        // * along the y-direction for the unknowns T(j,i)^(n+1).
 


        else if (Direction == SweepDirection::Y)
        {
        for (std::size_t j = 1; j + 1 < Ny; ++j)
        {
            LowerDiagonalValues[j] = -diffNumberY_Star;

            MainDiagonalValues[j] = 1.0 + 2.0 * diffNumberY_Star;

            UpperDiagonalValues[j] = -diffNumberY_Star; 
        }

        }
}


std::size_t TridiagonalMatrix::SizeM() const
{
    return MainDiagonalValues.size();
}

std::size_t TridiagonalMatrix::SizeU() const
{
    return UpperDiagonalValues.size();
}

std::size_t TridiagonalMatrix::SizeL() const
{
    return LowerDiagonalValues.size();
}


double TridiagonalMatrix::GetMidValue(std::size_t Index_) const
{
    return MainDiagonalValues.at(Index_);
}

double TridiagonalMatrix::GetUValue(std::size_t Index_) const
{
    return UpperDiagonalValues.at(Index_);
}

double TridiagonalMatrix::GetLValue(std::size_t Index_) const
{
    return LowerDiagonalValues.at(Index_);
}


void TridiagonalMatrix::SetMValue(
    std::size_t Index_,
    double Value_)
{
    MainDiagonalValues.at(Index_) = Value_;
}

void TridiagonalMatrix::SetUValue(
    std::size_t Index_,
    double Value_)
{
    UpperDiagonalValues.at(Index_) = Value_;
}

void TridiagonalMatrix::SetLValue(
    std::size_t Index_,
    double Value_)
{
    LowerDiagonalValues.at(Index_) = Value_;
}

void TridiagonalMatrix::SetBoundaryConditions()
{
    const std::size_t N =(Direction == SweepDirection::X) ? Nx : Ny;

    if (N == 0 || N==1)
    {
        return;
    }

    //Apply Dirichlet boundary condition To MainDiagonal Values
    MainDiagonalValues[0] = 1.0;
    MainDiagonalValues[N - 1] = 1.0;
   
    //Apply Dirichlet boundary condition To UpperDiagonal Values
    UpperDiagonalValues[0]= 0.0; //for Diriclet BC
    UpperDiagonalValues[ N -1 ] = 0.0 ; // unused: just for Even index
    
    LowerDiagonalValues[0] = 0.0 ; // unused: just for Even index
    LowerDiagonalValues[N -1] = 0.0 ; //for Diriclet BC
}
