#pragma once

enum class SweepDirection
{
    X,
    Y
};

enum class TimeScheme
{
    Explicit,
    Implicit
};

enum class BoundaryCondition
{
    Dirichlet,
    Neumann
};