#pragma once

#include "Classes2D/Field2D/Field2D.hpp"

class RelativeResidual
{
public:
    RelativeResidual() = delete;
    static double ReturnResidual(const Field2D& Field2D_n_obj,const Field2D& Field2D_nPlus1_obj);

private:
    
};

