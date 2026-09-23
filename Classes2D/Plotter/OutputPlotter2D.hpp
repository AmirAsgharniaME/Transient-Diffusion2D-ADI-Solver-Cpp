#ifndef OUTPUTPLOTTER_HPP
#define OUTPUTPLOTTER_HPP

#include <cstdio>
#include <cstddef>
#include <string>
#include "Classes2D/Mesh2D/Mesh2D.hpp"

class OutputPlotter2D {
private:
    FILE* gnuplotPipe;
    std::string plotTitle;
    std::size_t numXNodes;
    std::size_t numYNodes;

    void configurePlot() const;

public:
    OutputPlotter2D(const Mesh2D& Mesh2DObj, const std::string& title = "Solution Comparison");
    ~OutputPlotter2D();

    void plotComparison(
        const std::string& file1,
        const std::string& label1,
        const std::string& file2,
        const std::string& label2,
        const std::string& file3,
        const std::string& label3
    ) const;
};

#endif
