#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/numpy.h>
#include "libcbdetect/boards_from_corners.h"
#include "libcbdetect/config.h"
#include "libcbdetect/find_corners.h"
#include <opencv2/opencv.hpp>
#include <vector>

namespace py = pybind11;


std::vector<py::array_t<double>> detect(const char* filename) {
    std::vector<py::array_t<double>> output;
    py::array_t<double> boardPyArray;

    cbdetect::Corner corners;
    std::vector<cbdetect::Board> boards;  
    cbdetect::Params params;

    params.corner_type = cbdetect::SaddlePoint;
    params.show_processing = false;

    // load image
    cv::Mat img = cv::imread(filename, cv::IMREAD_COLOR);
    if (img.empty()) {
        std::cout << "Image not found!" << std::endl;
        exit(123);
    }

    // detect corners and checkerboards
    cbdetect::find_corners(img, corners, params);
    cbdetect::boards_from_corners(img, corners, boards, params);

    // loop boards
    for (const auto& board: boards) {
        int j = -1;
        size_t h = board.idx.size() - 2;
        size_t w = board.idx[0].size() - 2;
        size_t size = h*w*2;
        double *points = new double[size];

        // loop board rows
        for (const auto& row: board.idx) {
            int k = 0;

            // loop row indices
            for (const int i: row) {
                if (i < 0) {
                    continue;
                }
                const cv::Point2d corner(corners.p[i]);
                points[j*2*w + 2*k] = (double) corner.x;
                points[j*2*w + 2*k + 1] = (double) corner.y;
                k++;
            }
            j++;
        }

        // Create a Python object that will free the allocated
        // memory when destroyed:
        py::capsule free_when_done(points, [](void *f) {
            double *points = reinterpret_cast<double *>(f);
            delete[] points;
        });

        // Cast to py:array so that is interpreted as Numpy array in Python. Explanation:
        // https://github.com/pybind/pybind11/issues/1299#issuecomment-368815504
        boardPyArray = py::array_t<double>(std::vector<ptrdiff_t>{(long int)h, (long int)w, 2},
                                                 &points[0]);
        output.push_back(boardPyArray);
    }
    return output;
}


PYBIND11_MODULE(cbdetect_python, m) {
    m.doc() = "libcbdetect Python binding";

    m.def("detect", &detect, "Returns a list with N 3D Numpy arrays, where N is the number of\
checkerboards detected. The two outermost dimensions of the array correspond to the height and width\
indexes of the checkerboard intersection and the innermost dimension is its pixel coordinates.");
}
