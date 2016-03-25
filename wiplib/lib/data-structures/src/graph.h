#pragma once

#include <vector>

namespace wiplib {
    
    template <typename DataType>
    typedef Graph BasicGraph<DataType, UnweightedEdge>;

    template <typename DataType>
    typedef WeightedGraph BasicGraph<DataType, WeightedEdge>;

    template <typename DataType, typename EdgeType>
    class BasicGraph {
    private:
        class Iterator;
    public:

        using Iterator = iterator;
        typedef iterator BreadthFirstIterator;

        /**
         * \brief Construct an empty graph.
         */
        Graph();

        Graph(const Graph& other);

        ~Graph();

        Graph& operator=(const Graph& other);

        bool operator==(const Graph& other);

        bool operator!=(const Graph& other);

        void addVertex(const T& value);

        void addEdge(iterator src, iterator dest);

        std::vector<Node> getNeighbors(Node);

    private:

        class Iterator {

        };

    };

}
