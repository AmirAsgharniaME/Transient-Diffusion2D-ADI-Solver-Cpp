#include "Classes2D/Plotter/OutputPlotter2D.hpp"

#include <stdexcept>

OutputPlotter2D::OutputPlotter2D(const Mesh2D& Mesh2DObj, const std::string& title)
    : gnuplotPipe(nullptr),
      plotTitle(title),
      numXNodes(Mesh2DObj.GetNumOfXNodes()),
      numYNodes(Mesh2DObj.GetNumOfYNodes()) {
    gnuplotPipe = popen("gnuplot -persist", "w");

    if (gnuplotPipe == nullptr) {
        throw std::runtime_error("Failed to open Gnuplot.");
    }

    configurePlot();
}

OutputPlotter2D::~OutputPlotter2D() {
    if (gnuplotPipe != nullptr) {
        fprintf(gnuplotPipe, "exit\n");
        pclose(gnuplotPipe);
        gnuplotPipe = nullptr;
    }
}

void OutputPlotter2D::configurePlot() const {
    fprintf(gnuplotPipe, "set terminal qt size 1500,600\n");
    fprintf(gnuplotPipe, "set palette defined (0 'blue', 1 'cyan', 2 'yellow', 3 'red')\n");
    fprintf(gnuplotPipe, "set view map\n");
    fprintf(gnuplotPipe, "set dgrid3d %zu,%zu\n", numXNodes, numYNodes);
    fprintf(gnuplotPipe, "set pm3d map\n");
    fprintf(gnuplotPipe, "set multiplot layout 1,3 title '%s'\n", plotTitle.c_str());
    fprintf(gnuplotPipe, "set xlabel 'x'\n");
    fprintf(gnuplotPipe, "set ylabel 'y'\n");
    fprintf(gnuplotPipe, "set cblabel 'Temperature'\n");
    fprintf(gnuplotPipe, "set cbrange [0:100]\n");
    fflush(gnuplotPipe);
}

void OutputPlotter2D::plotComparison(
    const std::string& file1,
    const std::string& label1,
    const std::string& file2,
    const std::string& label2,
    const std::string& file3,
    const std::string& label3
) const {
    fprintf(gnuplotPipe, "set title '%s'\n", label1.c_str());
    fprintf(gnuplotPipe, "splot '%s' using 1:2:3 with pm3d notitle\n", file1.c_str());

    fprintf(gnuplotPipe, "set title '%s'\n", label2.c_str());
    fprintf(gnuplotPipe, "splot '%s' using 1:2:3 with pm3d notitle\n", file2.c_str());

    fprintf(gnuplotPipe, "set title '%s'\n", label3.c_str());
    fprintf(gnuplotPipe, "splot '%s' using 1:2:3 with pm3d notitle\n", file3.c_str());

    fprintf(gnuplotPipe, "unset multiplot\n");
    fflush(gnuplotPipe);
}


