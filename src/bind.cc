#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/numpy.h>
#include "libcbdetect/boards_from_corners.h"
#include "libcbdetect/config.h"
#include "libcbdetect/find_corners.h"
#include <opencv2/opencv.hpp>
#include <vector>

namespace py = pybind11;


std::vector<py::array_t<double>> detect(const py::array_t<uint8_t>& img_py) {
    std::vector<py::array_t<double>> output;
    py::array_t<double> board_py_array;

    cbdetect::Corner corners;
    std::vector<cbdetect::Board> boards;  
    cbdetect::Params params;

    params.corner_type = cbdetect::SaddlePoint;
    params.show_processing = false;

    // cast image and verify its correctness
    auto rows = img_py.shape(0);
    auto cols = img_py.shape(1);
    auto type = CV_8UC3;
    cv::Mat img(rows, cols, type, (unsigned char*)img_py.data());
    if (img.empty())
        throw std::runtime_error("The image is empty.");

    // detect corners and checkerboards
    cbdetect::find_corners(img, corners, params);
    cbdetect::boards_from_corners(img, corners, boards, params);

    // loop boards
    for (const auto& board: boards) {
        int j = 0;
        size_t h = board.idx.size() - 2;
        size_t w = board.idx[0].size() - 2;
        size_t size = h*w*2;
        double *points = new double[size];

        // loop board rows
        for (const auto& row: board.idx) {

            // loop row indices
            for (const int i: row) {
                if (i < 0) {
                    continue;
                }
                const cv::Point2d corner(corners.p[i]);
                points[j*2] = (double) corner.x;
                points[j*2 + 1] = (double) corner.y;
                j++;
            }
        }

        // Create a Python object that will free the allocated memory when destroyed
        py::capsule free_when_done(points, [](void *f) {
            double *points = reinterpret_cast<double *>(f);
            delete[] points;
        });

        // Create a Numpy array
        board_py_array = py::array_t<double>(
            {(long int)h, (long int)w, (long int)2},                // shape
            {w*2*sizeof(double), 2*sizeof(double), sizeof(double)}, // C-style contiguous strides for double
            points,                                                 // the data pointer
            free_when_done);                                        // numpy array references this parent
        output.push_back(board_py_array);
    }
    return output;
}


PYBIND11_MODULE(cbdetect_python, m) {
    m.doc() = "libcbdetect Python binding";

    m.def("detect", &detect, "Returns a list with N 3D Numpy arrays, where N is the number of\
checkerboards detected. The two outermost dimensions of the array correspond to the height and width\
indexes of the checkerboard intersection and the innermost dimension is its pixel coordinates.");
}
