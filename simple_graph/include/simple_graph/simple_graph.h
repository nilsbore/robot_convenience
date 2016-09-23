#ifndef SIMPLE_GRAPH_H
#define SIMPLE_GRAPH_H

#include <eigen3/Eigen/Dense>

namespace simple_graph {

template <typename NodeT, typename EdgeT> struct _node;

template <typename NodeT, typename EdgeT>
struct _neighbor {

    using node = _node<NodeT, EdgeT>;

    std::vector<node>& nodes;
    size_t index;
    EdgeT value;

    node& operator* ()
    {
        return nodes.at(index);
    }
    const node& operator* () const
    {
        return nodes.at(index);
    }
    template <class Archive>
    void serialize(Archive& archive)
    {
        archive(index, value);
    }

    //_neighbor(_neighbor& other) : nodes (other.nodes), index(other.index), value(other.value) {}
};

template <typename NodeT, typename EdgeT>
struct _node {

    using neighbor = _neighbor<NodeT, EdgeT>;

    NodeT value;
    std::vector<neighbor> neighbors;

    template <class Archive>
    void serialize(Archive& archive)
    {
        archive(value, neighbors);
    }
};

template <typename NodeT, typename EdgeT>
struct _edge_iterator {

    using neighbor = _neighbor<NodeT, EdgeT>;
    using node = _node<NodeT, EdgeT>;

    std::vector<node>& nodes;
    typename std::vector<node>::iterator node_iterator;
    typename std::vector<neighbor>::iterator neighbor_iterator;

    _edge_iterator& operator++ ()
    {
        if (node_iterator == nodes.end()) {
            return *this;
        }
        else if (neighbor_iterator == node_iterator->neighbors.end()) {
            ++node_iterator;
            if (node_iterator != nodes.end()) {
                neighbor_iterator = node_iterator->neighbors.begin();
                if (neighbor_iterator == node_iterator->neighbors.end()) {
                    operator++();
                }
            }
        }
        else {
            ++neighbor_iterator;
            if (neighbor_iterator == node_iterator->neighbors.end()) {
                operator++();
            }
        }
        // here, we are guaranteed to either have a value or having reached the end
        if (node_iterator != nodes.end()) {
            // just return nodes with i > j
            if (std::distance(nodes.begin(), node_iterator) < (*neighbor_iterator).index) {
                operator++();
            }
        }
        return *this;
    }
    std::pair<node*, node*> operator* ()
    {
        return std::make_pair(&*node_iterator, &**neighbor_iterator);
    }

    bool operator== (const _edge_iterator& other) const
    {
        return node_iterator == other.node_iterator && neighbor_iterator == other.neighbor_iterator;
    }

    bool operator!= (const _edge_iterator& other) const
    {
        return node_iterator != other.node_iterator || neighbor_iterator != other.neighbor_iterator;
    }
};

template <typename NodeT, typename EdgeT>
struct _edges_type
{

    using node = _node<NodeT, EdgeT>;
    using iterator = _edge_iterator<NodeT, EdgeT>;
    using neighbor = _neighbor<NodeT, EdgeT>;

    std::vector<node>& nodes;

    iterator end()
    {
        return iterator {nodes, nodes.end(), nodes.back().neighbors.end()};
    }

    iterator begin()
    {
        iterator iter = iterator {nodes, nodes.begin(), nodes.at(0).neighbors.begin()};
        if (iter.node_iterator == nodes.end() || iter.neighbor_iterator == (*iter.node_iterator).neighbors.end() ||
                std::distance(nodes.begin(), iter.node_iterator) < (*iter.neighbor_iterator).index) {
            ++iter;
        }
        return iter;
    }

    void push_back(const std::tuple<size_t, size_t, EdgeT>& edge)
    {
        nodes.at(std::get<0>(edge)).neighbors.push_back(neighbor {nodes, std::get<1>(edge), std::get<2>(edge)});
        nodes.at(std::get<1>(edge)).neighbors.push_back(neighbor {nodes, std::get<0>(edge), std::get<2>(edge)});
    }

    void erase(const std::pair<size_t, size_t>& indices)
    {
        /*
        std::vector<neighbor>& ivec = nodes.at(indices.first).neighbors;
        std::vector<neighbor>& jvec = nodes.at(indices.second).neighbors;
        ivec.erase(std::remove_if(ivec.begin(), ivec.end(), [&]( neighbor& n)
        {
            return n.index == indices.second;
        }), ivec.end());
        jvec.erase(std::remove_if(jvec.begin(), jvec.end(), [&]( neighbor& n)
        {
            return n.index == indices.first;
        }), jvec.end());
        */
    }
};

template <typename NodeT, typename EdgeT>
struct graph {

    using node = _node<NodeT, EdgeT>;
    using neighbor = _neighbor<NodeT, EdgeT>;
    using node_iterator = typename std::vector<node>::iterator;
    using edge_iterator = typename _edges_type<NodeT, EdgeT>::iterator;

    std::vector<node> nodes;
    _edges_type<NodeT, EdgeT> edges;

    size_t add_node(const NodeT& value)
    {
        nodes.push_back(node {value, {}});
        return nodes.size() - 1;
    }

    void remove_node(size_t index)
    {
        /*
        for (neighbor& n : nodes.at(index).neighbors) {
            std::vector<neighbor>& jvec = (*n).neighbors;
            jvec.erase(std::remove_if(jvec.begin(), jvec.end(), [&](const neighbor& m)
            {
                return m.index == index;
            }), jvec.end());
        }
        nodes.erase(nodes.begin() + index);
        for (node& n : nodes) {
            for (neighbor& nn : n.neighbors) {
                if (nn.index > index) {
                    nn.index -= 1;
                }
            }
        }
        */
    }

    void add_edge(size_t i, size_t j, const EdgeT& value)
    {
        edges.push_back(std::make_tuple(i, j, value));
    }

    void remove_edge(size_t i, size_t j)
    {
        edges.erase(std::make_pair(i, j));
    }

    template <class Archive>
    void serialize(Archive& archive)
    {
        archive(nodes);
    }

    graph() : nodes {}, edges {nodes} {}
};



} // namespace simple_graph

#endif // SIMPLE_GRAPH_H
