#include "Classes2D/InitialConditions/InitialConditions.hpp"

void InitialConditions::ApplyInitialCondition(    
    Field2D& Field2D_Obj,
    const std::vector<std::vector<double>>& InitialValue2D_)
{
    if (Field2D_Obj.GetNrows() == 0 || Field2D_Obj.GetNrows()==0 )
    {
        return;
    }

    Field2D_Obj.SetAll2DValue(InitialValue2D_);

}
