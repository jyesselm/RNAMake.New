//
// Created by Joseph Yesselman on 10/22/17.
//

#include <pybind11/pybind11.h>
#include <pybind11/operators.h>

//rnamake headers
#include <base/types.h>
#include <data_structures/graph.h>

#include <primitives/residue.h>
#include <primitives/chain.h>

namespace py = pybind11;
namespace data_structures {

PYBIND11_PLUGIN(data_structures) {
    py::module m("data_structures", "basic organization classes");
    // Edge Class
    py::class_<Edge, std::shared_ptr<Edge>>(m, "Edge")
            .def(py::init<Index, Index, Index, Index>())
            .def_readonly("node_i", &Edge::node_i)
            .def_readonly("node_j", &Edge::node_j)
            .def_readonly("edge_i", &Edge::edge_i)
            .def_readonly("edge_j", &Edge::edge_j);

    // Graph Class
    typedef primitives::Residue DataType;
    typedef std::shared_ptr<DataType> DataTypeOP;
    typedef Graph<DataType> GraphType;
    py::class_<GraphType, std::shared_ptr<GraphType> >(m, "Graph")
            .def(py::init<>())
            .def("__len__", &GraphType::get_num_nodes)
            .def("__iter__", [](GraphType const & g) { return py::make_iterator(g.begin(), g.end()); },
                 py::keep_alive<0, 1>())
            .def("change_transverse_start", &GraphType::change_transverse_start)
            .def("get_edges", &GraphType::get_edges)
            .def("get_num_edges", &GraphType::get_num_edges)
            .def("get_num_nodes", &GraphType::get_num_nodes)
            .def("are_nodes_connected", &GraphType::are_nodes_connected)
            .def("get_node", &GraphType::get_node)
            .def("add_node", (int (GraphType::*)(DataTypeOP)) &GraphType::add_node)
            .def("add_node", (int (GraphType::*)(DataTypeOP, Index)) &GraphType::add_node);

    // StaticEdgedGraph Class
    typedef StaticEdgedGraph<DataType> StaticEdgedGraphType;
    py::class_<StaticEdgedGraphType, std::shared_ptr<StaticEdgedGraphType> >(m, "StaticEdgedGraph")
            .def(py::init<>())
            .def("__len__", &StaticEdgedGraphType::get_num_nodes)
            .def("__iter__", [](StaticEdgedGraphType const & g) { return py::make_iterator(g.begin(), g.end()); },
                 py::keep_alive<0, 1>())
            //.def("change_transverse_start", &StaticEdgedGraphType::change_transverse_start)
            .def("get_edges", &StaticEdgedGraphType::get_edges)
            .def("get_num_edges", &StaticEdgedGraphType::get_num_edges)
            .def("get_num_nodes", &StaticEdgedGraphType::get_num_nodes)
            .def("are_nodes_connected", &StaticEdgedGraphType::are_nodes_connected)
            .def("get_node", &StaticEdgedGraphType::get_node)
            .def("add_node", (int (StaticEdgedGraphType::*)(DataTypeOP)) &StaticEdgedGraphType::add_node)
            .def("add_node", (int (StaticEdgedGraphType::*)(DataTypeOP, Index)) &StaticEdgedGraphType::add_node);


    return m.ptr();

}
}

