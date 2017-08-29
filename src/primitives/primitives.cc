//
// Created by Joseph Yesselman on 8/27/17.
//

#include <pybind11/pybind11.h>
#include <pybind11/operators.h>
#include <pybind11/stl.h>

//rnamake headers
#include <primitives/residue.h>
#include <primitives/chain.h>

namespace primitives {

namespace py = pybind11;

//PYBIND11_MAKE_OPAQUE(std::vector<Residue>);

PYBIND11_PLUGIN(primitives) {
    py::module m("primitives", "rnamake's primitive classes");

    // Uuid Class
    py::class_<Residue, std::shared_ptr<Residue>>(m, "Residue")
            .def("get_chain_id", &Residue::get_chain_id)
            .def("get_name", &Residue::get_name)
            .def("get_num", &Residue::get_num)
            .def("get_i_code", &Residue::get_i_code)
            .def("get_uuid", &Residue::get_uuid)
            .def("get_str", &Residue::get_str)
            .def(py::init<const char &, const int &, const char &, const char &, const Uuid &>());

    py::class_<Chain<Residue>, std::shared_ptr<Chain<Residue>>>(m, "Chain")
            .def(py::init<const ResidueOPs &>())
            .def("__iter__", [](const Chain<Residue> & c) { return py::make_iterator(c.begin(), c.end()); },
            py::keep_alive<0, 1>())
            .def("__len__", &Chain<Residue>::get_length)
            .def("get_first", &Chain<Residue>::get_first)
            .def("get_last",  &Chain<Residue>::get_last)
            .def("get_residue", &Chain<Residue>::get_residue)
            .def("get_length", &Chain<Residue>::get_length);

    py::register_exception<ResidueException>(m, "ResidueException");
    py::register_exception<ChainException>(m, "ChainException");

    return m.ptr();
}

}