//
// Created by Joseph Yesselman on 8/27/17.
//

#include <pybind11/pybind11.h>
#include <pybind11/operators.h>

//rnamake headers
#include <base/types.h>
#include <base/file_io.h>
#include <base/simple_string.h>
#include <base/paths.h>
#include <base/json.h>

namespace py = pybind11;
namespace base {

PYBIND11_PLUGIN(base) {
    py::module m("base", "rnamake's most basic classes and functions");

    // SimpleString Class
    py::class_<SimpleString, std::shared_ptr<SimpleString>>(m, "SimpleString")
            .def(py::init<const String &>())
            .def("get_str", &base::SimpleString::get_str)
            .def(py::self == py::self)
            .def(py::self != py::self);

    //             .def("__getitem__", &Basepairs::operator[]);

    py::class_<json::JSON, std::shared_ptr<json::JSON>>(m, "JSON")
            .def("__str__", &json::JSON::dump, py::arg("depth") = 1, py::arg("tab") = String(" "))
            .def("dump_minimized", &json::JSON::dump_minimized, py::arg("depth") = 1, py::arg("tab") = String(" "))
            .def("__setitem__",
                 (std::enable_if<std::is_same<bool,bool>::value, json::JSON&>::type  (json::JSON::*)(bool) ) &json::JSON::operator=)
            .def("__setitem__",
                 (std::enable_if<std::is_integral<int>::value && !std::is_same<int,bool>::value, json::JSON&>::type  (json::JSON::*)(int) ) &json::JSON::operator=)
            .def("__getitem__",
                 (json::JSON & (json::JSON::*)(unsigned int) ) &json::JSON::operator[])
            .def("__getitem__",
                 (json::JSON & (json::JSON::*)(String const &) ) &json::JSON::operator[])
            .def("__setitem__",
                 (json::JSON & (json::JSON::*)(unsigned int) ) &json::JSON::operator[])
            .def("ToBool",  (bool (json::JSON::*)() const ) &json::JSON::ToBool)
            .def("ToString", (String (json::JSON::*)() const ) &json::JSON::ToString)
            .def("append",  (void (json::JSON::*)(bool)) &json::JSON::append )
            .def(py::init<>());

    m.def("Load", &json::JSON::Load);

    // paths
    m.def("base_path", &base_path);
    m.def("resources_path", &resources_path);
    m.def("unittest_resources_path", &unittest_resources_path);

    return m.ptr();

}
}
