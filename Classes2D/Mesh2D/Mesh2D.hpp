#pragma once

#include <cstddef>
#include <vector>
#include <algorithm> // std::find
#include "Classes2D/Geometry/Geometry.hpp"


class Mesh2D
{

    private:

    std::size_t NumOfXNodes;
    std::size_t NumOfYNodes;

    std::size_t NumOfXIntervals;
    std::size_t NumOfYIntervals;

    std::size_t nrows;
    std::size_t ncols;

    double Height;
    double  Width;

    double deltaX;
    double deltaY;
    
    std::vector<double> XPositions;
    std::vector<double> YPositions;

public:
    explicit Mesh2D(
        const Geometry& Geometry_Obj,
        std::size_t Default_NumOfXNodes_ = 10,
        std::size_t Default_NumOfYNodes_ = 10
    );


   //Public Getters 
    std::size_t GetNumOfXNodes() const;
    std::size_t GetNumOfYNodes() const;

    std::size_t GetNumOfXIntervals() const;
    std::size_t GetNumOfYIntervals() const;

    std::size_t GetNrows() const;
    std::size_t GetNcols() const;

    double GetdeltaX() const;
    double GetdeltaY() const;

    double GetXPositions(std::size_t Index) const;
    double GetYPositions(std::size_t Index) const;
    const std::vector<double>& GetXPositionVector() const;
    const std::vector<double>& GetYPositionVector() const;



    //Public Setters
    void SetNumOfXNodes(std::size_t NumOfXNodes_);
    void SetNumOfYNodes(std::size_t NumOfYNodes_);

    //Public Print Values
    void PrintXPositions() const;
    void PrintYPositions() const;
    void Print() const;




    
    
    //Private Methods
    void DiscretizeXDirection();
    void DiscretizeYDirection();
};

