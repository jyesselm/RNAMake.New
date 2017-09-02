#include <pybind11/pybind11.h>
#include <pybind11/operators.h>

#include <math/xyz_vector.h>
#include <math/xyz_math::Matrix.h>
#include <math/numerical.h>

namespace py = pybind11;

PYBIND11_PLUGIN(math_util) {
    py::module m("math_util", "rnamake's math library");
    
    // math::Point Class
    py::class_<math::Point>(m, "math::Point")
            .def(py::init<const double &, const double &, const double &>())
            .def(py::init<const math::Point &>())
            .def(py::init<const String &>())
            .def("set_x", &math::Point::set_x)
            .def("set_y", &math::Point::set_y)
            .def("set_z", &math::Point::set_z)
            .def("get_x", &math::Point::get_x)
            .def("get_y", &math::Point::get_y)
            .def("get_z", &math::Point::get_z)
            .def("get_str", &math::Point::get_str)
            .def("get_length", &math::Point::get_length)
            .def("get_length_squared", &math::Point::get_length_squared)
            .def("get_norm", &math::Point::get_magnitude)
            .def("get_norm_squared", &math::Point::get_norm_squared)
            .def("get_magnitude", &math::Point::get_magnitude)
            .def("get_magnitude_squared", &math::Point::get_magnitude_squared)
            .def("zero", &math::Point::zero)
            .def("negate", &math::Point::negate)
            .def("distance", &math::Point::distance)
            .def("distance_squared", &math::Point::distance_squared)
            .def("dot", &math::Point::dot)
            .def("cross", &math::Point::cross)
            .def("__repr__", &math::Point::get_str)
                    // overloading operators
            .def(-py::self)
            .def(py::self + py::self)
            .def(py::self - py::self)
            .def(float() + py::self)
            .def(float() * py::self)
            .def(py::self + float())
            .def(py::self * float());

    // math::Matrix Class
    py::class_<math::Matrix>(m, "math::Matrix")
            .def(py::init<const math::Matrix &>())
            .def(py::init<const String &>())
            .def(py::init<const double &>())
            .def(py::init<const double &, const double &, const double &,
                    const double &, const double &, const double &,
                    const double &, const double &, const double &>())
            .def("__repr__", &math::Matrix::get_str_readable)
            .def("difference", &math::Matrix::difference)
            .def("get_xx", &math::Matrix::get_xx)
            .def("get_xy", &math::Matrix::get_xy)
            .def("get_xz", &math::Matrix::get_xz)
            .def("get_yx", &math::Matrix::get_yx)
            .def("get_yy", &math::Matrix::get_yy)
            .def("get_yz", &math::Matrix::get_yz)
            .def("get_zx", &math::Matrix::get_zx)
            .def("get_zy", &math::Matrix::get_zy)
            .def("get_zz", &math::Matrix::get_zz)
            .def("get_str", &math::Matrix::get_str)
            .def("get_str_readable", &math::Matrix::get_str_readable)
            .def("get_transpose", &math::Matrix::get_transposed)
            .def("get_flip_orientation", &math::Matrix::get_flip_orientation)
            .def("get_unitarize", &math::Matrix::get_unitarize)
            .def("set_xx", &math::Matrix::set_xx)
            .def("set_xy", &math::Matrix::set_xy)
            .def("set_xz", &math::Matrix::set_xz)
            .def("set_yx", &math::Matrix::set_yx)
            .def("set_yy", &math::Matrix::set_yy)
            .def("set_yz", &math::Matrix::set_yz)
            .def("set_zx", &math::Matrix::set_zx)
            .def("set_zy", &math::Matrix::set_zy)
            .def("set_zz", &math::Matrix::set_zz)
            .def("transpose", &math::Matrix::transpose)
            .def("unitarize", &math::Matrix::unitarize)
                    // overloading operators
            .def(py::self - py::self)
            .def(py::self * py::self)
            .def(float() + py::self)
            .def(py::self + float())
            .def(py::self + py::self);

    m.def("dot_vector", &math::dot_vector);


    m.def("are_floats_equal", &math::are_floats_equal, py::arg("a"), py::arg("b"),
          py::arg("tol") = 0.001);

    m.def("are_math::Points_equal", &math::are_math::Points_equal, py::arg("p1"), py::arg("p2"),
          py::arg("tol") = 0.001);

    m.def("are_matrices_equal", &math::are_matrices_equal);

    //m.def("dot_product", &dot_product);
    //m.def("cross_produce", &cross_product);


    return m.ptr();
}
