include_directories(${PROJECT_SOURCE_DIR}/include)
add_executable(example example.cc)
target_link_libraries(example PUBLIC cbdetect ${OpenCV_LIBS})

# Add python binding
pybind11_add_module(cbdetect_python SHARED bind.cc)
target_link_libraries(cbdetect_python PUBLIC cbdetect ${OpenCV_LIBS})