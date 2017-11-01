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
    typedef Graph<DataType> ResidueGraph;
    py::class_<ResidueGraph, std::shared_ptr<ResidueGraph> >(m, "ResidueGraph")
            .def(py::init<>())
            .def("__len__", &ResidueGraph::get_num_nodes)
            .def("__iter__", [](ResidueGraph const & g) { return py::make_iterator(g.begin(), g.end()); },
                 py::keep_alive<0, 1>())
            .def("change_transverse_start", &ResidueGraph::change_transverse_start)
            .def("get_edges", &ResidueGraph::get_edges)
            .def("get_num_edges", &ResidueGraph::get_num_edges)
            .def("get_num_nodes", &ResidueGraph::get_num_nodes)
            .def("are_nodes_connected", &ResidueGraph::are_nodes_connected)
            .def("get_node", &ResidueGraph::get_node)
            .def("add_node", (int (ResidueGraph::*)(DataTypeOP)) &ResidueGraph::add_node)
            .def("add_node", (int (ResidueGraph::*)(DataTypeOP, Index)) &ResidueGraph::add_node);

    py::class_<ResidueGraph::Node, std::shared_ptr<ResidueGraph::Node> >(m, "ResidueGraphNode")
            .def(py::init<Index, DataType const *>())
            .def_readonly("index", &ResidueGraph::Node::index)
            .def_readonly("data", &ResidueGraph::Node::data);

    /*typedef primitives::PrimitiveChain DataType1;
    typedef std::shared_ptr<DataType1> DataTypeOP1;
    typedef Graph<DataType1> GraphType1;
    py::class_<GraphType1, std::shared_ptr<GraphType1> >(m, "Graph")
            .def(py::init<>())
            .def("__len__", &GraphType1::get_num_nodes);*/

    // StaticEdgedGraph Class
    /*typedef StaticEdgedGraph<DataType> StaticEdgedGraphType;
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
    */

    return m.ptr();

}
}

