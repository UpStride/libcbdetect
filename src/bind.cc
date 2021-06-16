#include <pybind11/pybind11.h>
// #include "libcbdetect/boards_from_corners.h"
// #include "libcbdetect/config.h"
#include "libcbdetect/find_corners.h"
// #include "libcbdetect/plot_boards.h"
// #include "libcbdetect/plot_corners.h"
// #include <chrono>
// #include <opencv2/opencv.hpp>
// #include <vector>

// using namespace std::chrono;


PYBIND11_MODULE(cbdetect_python, m) {
    m.doc() = "libcbdetect Python binding";

    m.def("find_corners", &cbdetect::find_corners, "");
}
