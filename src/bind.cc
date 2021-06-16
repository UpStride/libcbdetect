#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/numpy.h>
#include "libcbdetect/boards_from_corners.h"
#include "libcbdetect/config.h"
#include "libcbdetect/find_corners.h"
#include <opencv2/opencv.hpp>
#include <vector>

namespace py = pybind11;


std::vector<std::vector<std::vector<std::vector<double>>>> detect(const char* filename) {
    std::vector<std::vector<std::vector<std::vector<double>>>> checkerboards;
    std::vector<std::vector<std::vector<double>>> checkerboard;
    std::vector<std::vector<double>> checkerboard_row;

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
        // loop board rows
        for (const auto& row: board.idx) {
            // loop row indices
            for (const int i: row) {
                if (i < 0) {
                    continue;
                }
                const cv::Point2d corner(corners.p[i]);
                checkerboard_row.push_back({corner.x, corner.y});
            }
            if (checkerboard_row.size() != 0) {
                checkerboard.push_back(checkerboard_row);
                checkerboard_row.clear();
            }
        }
        checkerboards.push_back(checkerboard);
        checkerboard.clear();
    }
    return checkerboards;
}


PYBIND11_MODULE(cbdetect_python, m) {
    m.doc() = "libcbdetect Python binding";

    m.def("detect", &detect, pybind11::return_value_policy::take_ownership);
}
