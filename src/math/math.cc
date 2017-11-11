#include <pybind11/pybind11.h>
#include <pybind11/operators.h>

#include <math/xyz_vector.h>
#include <math/xyz_matrix.h>
#include <math/numerical.h>

namespace py = pybind11;
namespace math {

PYBIND11_PLUGIN(math_util) {
    py::module m("math_util", "rnamake's math library");

    // Point Class
    py::class_<Point>(m, "Point")
            .def(py::init<const double &, const double &, const double &>())
            .def(py::init<const Point &>())
            .def(py::init<const String &>())
            .def("set_x", &Point::set_x)
            .def("set_y", &Point::set_y)
            .def("set_z", &Point::set_z)
            .def("get_x", &Point::get_x)
            .def("get_y", &Point::get_y)
            .def("get_z", &Point::get_z)
            .def("get_str", &Point::get_str)
            .def("get_length", &Point::get_length)
            .def("get_length_squared", &Point::get_length_squared)
            .def("get_norm", &Point::get_magnitude)
            .def("get_norm_squared", &Point::get_norm_squared)
            .def("get_magnitude", &Point::get_magnitude)
            .def("get_magnitude_squared", &Point::get_magnitude_squared)
            .def("zero", &Point::zero)
            .def("negate", &Point::negate)
            .def("distance", &Point::distance)
            .def("distance_squared", &Point::distance_squared)
            .def("dot", &Point::dot)
            .def("cross", &Point::cross)
            .def("__repr__", &Point::get_str)
                    // overloading operators
            .def(-py::self)
            .def(py::self + py::self)
            .def(py::self - py::self)
            .def(float() + py::self)
            .def(float() * py::self)
            .def(py::self + float())
            .def(py::self * float());

    // Matrix Class
    py::class_<Matrix>(m, "Matrix")
            .def(py::init<const Matrix &>())
            .def(py::init<const String &>())
            .def(py::init<const double &>())
            .def(py::init<const double &, const double &, const double &,
                    const double &, const double &, const double &,
                    const double &, const double &, const double &>())
            .def("__repr__", &Matrix::get_str_readable)
            .def("difference", &Matrix::difference)
            .def("get_xx", &Matrix::get_xx)
            .def("get_xy", &Matrix::get_xy)
            .def("get_xz", &Matrix::get_xz)
            .def("get_yx", &Matrix::get_yx)
            .def("get_yy", &Matrix::get_yy)
            .def("get_yz", &Matrix::get_yz)
            .def("get_zx", &Matrix::get_zx)
            .def("get_zy", &Matrix::get_zy)
            .def("get_zz", &Matrix::get_zz)
            .def("get_str", &Matrix::get_str)
            .def("get_str_readable", &Matrix::get_str_readable)
            .def("get_transpose", &Matrix::get_transposed)
            .def("get_flip_orientation", &Matrix::get_flip_orientation)
            .def("get_unitarize", &Matrix::get_unitarize)
            .def("set_xx", &Matrix::set_xx)
            .def("set_xy", &Matrix::set_xy)
            .def("set_xz", &Matrix::set_xz)
            .def("set_yx", &Matrix::set_yx)
            .def("set_yy", &Matrix::set_yy)
            .def("set_yz", &Matrix::set_yz)
            .def("set_zx", &Matrix::set_zx)
            .def("set_zy", &Matrix::set_zy)
            .def("set_zz", &Matrix::set_zz)
            .def("transpose", &Matrix::transpose)
            .def("unitarize", &Matrix::unitarize)
                    // overloading operators
            .def(py::self - py::self)
            .def(py::self * py::self)
            .def(float() + py::self)
            .def(py::self + float())
            .def(py::self + py::self);

    m.def("dot_vector", (void (*)(Matrix const &, Vector const &, Vector &)) &math::dot_vector);
    m.def("dot_vector",  (Vector (*)(Matrix const &, Vector const &))  &math::dot_vector);


    m.def("are_floats_equal", &math::are_floats_equal, py::arg("a"), py::arg("b"),
          py::arg("tol") = 0.001);

    m.def("are_points_equal", &math::are_points_equal, py::arg("p1"), py::arg("p2"),
          py::arg("tol") = 0.001);

    m.def("are_matrices_equal", &math::are_matrices_equal);

    //m.def("dot_product", &dot_product);
    //m.def("cross_produce", &cross_product);


    return m.ptr();
}
}