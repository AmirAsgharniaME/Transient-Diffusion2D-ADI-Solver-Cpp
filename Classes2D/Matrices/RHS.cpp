#include "Classes2D/Matrices/RHS.hpp"
#include "EssentialHeaders/SolverOptions.hpp"
//#include <algorithm>



// RHS For Laasonen Method
RHS::RHS(
        const SweepDirection SweepDirection_,
        std::size_t Index_,
        const Mesh2D& Mesh2D_Obj,
        const Field2D& Field2D_Obj,
        const SolutionParameters<double> DiffNumber_Star,
        const double Default_Value_)
    :Direction(SweepDirection_),
     Index(Index_),
     diffNumber_Star(DiffNumber_Star.GetValue())
   {
    if (Direction == SweepDirection::X)
    {
        double diffNumberY_Star = diffNumber_Star;
        J_Index = Index;
        Nx = Mesh2D_Obj.GetNumOfXNodes();
        RHSValues.assign(Nx, Default_Value_);
        CreateRHS(Field2D_Obj,diffNumberY_Star);
    }


    else if (Direction == SweepDirection::Y)
    {   

        double diffNumberX_Star = diffNumber_Star;
        I_Index = Index;
        Ny = Mesh2D_Obj.GetNumOfYNodes();
        RHSValues.assign(Ny, Default_Value_);
        CreateRHS(Field2D_Obj,diffNumberX_Star);
    }


    }
   


/*
     const std::size_t N =(Direction == SweepDirection::X) ? Nx : Ny;
     RHSValues.assign(N, Default_Value_);
     if(Direction == SweepDirection::X)
     {
        std::size_t J = 1;
         for (std::size_t i = 1; i < Nx-1; ++i)
         {
             RHSValues[i] = Field2D_n_Obj.GetValue(J, i);
         }
     }



    //RHS For CrankNicolson Method
     RHS::RHS(
        const Field2D& Field2D_n_Obj,
        const SolutionParameters<double>& DiffNumerX_Obj,
        const SolutionParameters<double>& DiffNumerY_Obj,
        double Default_Initial_Value_     
    )
    :nrows(Field2D_n_Obj.GetNrows()),
     ncols(Field2D_n_Obj.GetNcols()),
     NN(nrows*ncols),
     RHSValues(NN, Default_Initial_Value_),
     d_x(DiffNumerX_Obj.GetValue()),
     d_y(DiffNumerY_Obj.GetValue())

     {
        // ===================
        // CrankNicolson Method
        // ====================

        // LHS (Unknowns at time step n+1):
        // -0.5*d_x * u(j, i-1)^{n+1} - 0.5*d_y * u(j-1, i)^{n+1} + (1 + d_x + d_y) * u(j, i)^{n+1} - 0.5*d_x * u(j, i+1)^{n+1} - 0.5*d_y * u(j+1, i)^{n+1}

        // RHS (Knowns at time step n):
        // 0.5*d_x * u(j, i-1)^n + 0.5*d_y * u(j-1, i)^n + (1 - d_x - d_y) * u(j, i)^n + 0.5*d_x * u(j, i+1)^n + 0.5*d_y * u(j+1, i)^n
        //=============================================================================================================================

        // Create RHS for CrankNicolson Method
           std::size_t p;
           std::size_t Nx = ncols;

            // for internal nodes
            for (std::size_t j = 1; j < nrows-1; j++)
            {
                for (std::size_t i = 1; i < ncols-1; i++)
                { 
                    p = j*Nx + i;

                   // for internal nodes
                   double value = 0.5*d_x * Field2D_n_Obj.GetValue(j, i-1)
                   + 0.5*d_y * Field2D_n_Obj.GetValue(j-1, i) 
                   + (1 - d_x - d_y) * Field2D_n_Obj.GetValue(j, i) 
                   + 0.5*d_x * Field2D_n_Obj.GetValue(j, i+1) 
                   + 0.5*d_y * Field2D_n_Obj.GetValue(j+1, i);
                  
                   RHSValues[p] = value;
                   
                }
                
            }

     }
*/

void RHS::SetValue(std::size_t Index, double Value_)
    {
        RHSValues[Index] = Value_;
    }
    
double RHS::GetValue(std::size_t Index) const
{

    return RHSValues[Index];
}

std::size_t RHS::GetSize() const
{
return RHSValues.size();
}

void RHS::CreateRHS(const Field2D& Field2D_Obj,double diffNumber_Star_)
{
    if (Direction == SweepDirection::X)
    {
            // ================
            // Lassonen
            //SweepDirection::X
            // ================
            //  * RHS= diffNumberY_Star * T(j,i-1)^n
            //  * + (1 - 2*diffNumberY_Star) * T(j,i)^n
            //  * + diffNumberY_Star * T(j,i+1)^n
            for (std::size_t i = 1; i < Nx-1 ; ++i)
            {
                double rhsValue = diffNumber_Star_ * Field2D_Obj.GetValue(J_Index, i-1)
                + (1 - 2*diffNumber_Star_) * Field2D_Obj.GetValue(J_Index, i)
                + diffNumber_Star_ * Field2D_Obj.GetValue(J_Index, i+1);
                RHSValues[i] = rhsValue;
            }
    }

    else if(Direction == SweepDirection::Y)
    {
            // ================
            // Lassonen
            //SweepDirection::Y
            // ================
            // * RHS= diffNumberX_Star * T(j-1,i)^(n+1,*)
            // * + (1 - 2*diffNumberX_Star) * T(j,i)^(n+1,*)
            // * + diffNumberX_Star * T(j+1,i)^(n+1,*)
             for (std::size_t j = 1; j < Ny-1 ; ++j)
            {
                double rhsValue = diffNumber_Star_ * Field2D_Obj.GetValue(j-1, I_Index)
                + (1 - 2*diffNumber_Star_) * Field2D_Obj.GetValue(j, I_Index)
                + diffNumber_Star_ * Field2D_Obj.GetValue(j+1, I_Index);
                RHSValues[j] = rhsValue;
            }



    }


}


void  RHS::SetBoundaryConditions()
{
if (Direction==SweepDirection::X)
{ 
     RHSValues[0] = SolverInputs::LeftWall_Temp_Values[J_Index];
     RHSValues[Nx-1] = SolverInputs::RightWall_temp_Values[J_Index];

}
   
  
else if (Direction==SweepDirection::Y)
{
    RHSValues[0] = SolverInputs::BottomWall_Temp_Values[I_Index];
    RHSValues[Ny-1] = SolverInputs::TopWall_Temp_Values[I_Index];

}    
}

