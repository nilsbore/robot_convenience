#ifndef SIMPLE_GRAPH_GRAPHVIZ_H
#define SIMPLE_GRAPH_GRAPHVIZ_H

#include <simple_graph/simple_graph.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <cstdlib>

namespace simple_graph {

//ostream& operator<<(ostream& os, const Date& dt)
//{
//    os << dt.mo << '/' << dt.da << '/' << dt.yr;
//    return os;
//}

/*
std::ofstream& operator<<(std::ofstream& os, const graph<std::pair<double, std::string>, double>::node& node)
{
    os << "[label=\"" << node.value.first << "\"]" << "[image=\"" << node.value.second << "\"]";
    return os;
}

std::ofstream& operator<<(std::ofstream& os, const graph<std::pair<double, std::string>, double>::neighbor& edge)
{
    os << "[label=\"" << edge.value << "]";
    return os;
}
*/

std::string graph_serialize(const graph<std::pair<std::string, std::string>, double>::node& node)
{
    return std::string("[label=\"") + node.value.first + "\"]" + "[image=\"" + node.value.second + "\"]";
}

/*
std::string graph_serialize(const graph<std::pair<double, std::string>, double>::node& node)
{
    return std::string("[label=\"") + std::to_string(node.value.first) + "\"]" + "[image=\"" + node.value.second + "\"]";
}
*/

std::string graph_serialize(const graph<std::pair<std::string, std::string>, double>::neighbor& edge)
{
    return std::string("[label=\"") + std::to_string(edge.value) + "\"]";
}

template <typename NodeT, typename EdgeT>
std::ofstream& operator<<(std::ofstream& os, graph<NodeT, EdgeT>& g)
{
    using node = typename graph<NodeT, EdgeT>::node;
    using edge_iterator = typename graph<NodeT, EdgeT>::edge_iterator;

    os << "graph G {\n";
    size_t counter = 0;
    for (const node& n : g.nodes) {
        os << counter << "[shape=\"box\"]" << graph_serialize(n) << ";\n";
        ++counter;
    }
    for (edge_iterator e = g.edges.begin(); e != g.edges.end(); ++e) {
        size_t i = std::distance(g.nodes.begin(), e.node_iterator);
        size_t j = (*e.neighbor_iterator).index;
        os << i << "--" << j << "[style=\"dashed\"]" << graph_serialize(*e.neighbor_iterator) << ";\n";
    }
    os << "}";
    return os;
}

cv::Mat graph_to_image(graph<std::pair<std::string, std::string>, double>& g, const std::string& filename)
{
    std::string imagefile = "graph.png";
    std::ofstream file;
    file.open(filename);
    file << g;
    file.close();
    std::string command = std::string("dot -Tpng ") + filename + " > " + imagefile;
    int res = system(command.c_str());
    cv::Mat image = cv::imread(imagefile);
    return image;
}

} // namespace simple_graph

#endif // SIMPLE_GRAPH_GRAPHVIZ_H
