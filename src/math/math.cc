#include <pybind11/pybind11.h>
#include <pybind11/operators.h>

#include <math/xyz_vector.h>

namespace py = pybind11;

PYBIND11_PLUGIN(math_util) {
    py::module m("math_util", "rnamake's math library");

    py::class_<Point>(m, "Point")
            .def(py::init<const double &, const double &, const double &>())
            .def("set_x", &Point::set_x)
            .def("set_y", &Point::set_y)
            .def("set_z", &Point::set_z)
            .def("get_x", &Point::get_x)
            .def("get_y", &Point::get_y)
            .def("get_z", &Point::get_z)
            .def("zero",  &Point::zero)
            .def("negate", &Point::negate)
            .def("distance", &Point::distance)
            .def("distance_squared", &Point::distance_squared)
            .def("dot", &Point::dot)
            .def("cross", &Point::cross)
            .def("length", &Point::length)
            .def("length_squared", &Point::length_squared)
            .def("__repr__", &Point::to_str)
            // overloading operators
            .def(- py::self)
            .def(py::self + py::self)
            .def(py::self - py::self)
            .def(py::self + float())
            .def(py::self * float());

    //m.def("dot_product", &dot_product);
    //m.def("cross_produce", &cross_product);


    return m.ptr();
}
