#pragma once

#include<vector>
#include "Classes2D/Field2D/Field2D.hpp"

class InitialConditions
{
public:
    static void ApplyInitialCondition(
    Field2D& Field2D_Obj,
    const std::vector<std::vector<double>>& InitialValue2D_);

};
