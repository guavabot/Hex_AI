#ifndef __HW_2__Graph__
#define __HW_2__Graph__

#include <iostream>
#include <unordered_set>
#include <vector>

template<typename T>
class Graph;

template<typename T>
std::ostream& operator<<(std::ostream& out, const Graph<T>& graph);

/*
 * Uses a connectivity matrix to represent the edges of a graph
 * Each element of edges_ contains the distance to another vertex
 * or 0 if there is no edge.
 * Edges are unidirectional.
 */
template<typename T>
class Graph {
    static_assert(std::is_arithmetic<T>::value,
            "template argument must be an arithmetic type");
public:
    /*
     * Builds an empty graph (with no edges).
     */
    explicit Graph(int size) :
            size_(size), edges_(size, std::vector<T>(size)) {
    }
    /*
     * Builds a graph with an initial amount of edges dependent of
     * the given density with edge values in the given range (inclusive).
     */
    Graph(int size, double density, T range_min, T range_max);
    /*
     * This constructor receives a file and uses it to build the graph.
     * The file format is an initial integer that is the size of the graph
     * and the rest of values are number triples: (vertex 1, vertex 2, edge value).
     */
    Graph(std::ifstream& ifs);

    /* Sets an edge an edge value of 1 between the vertices if the edge doesn't exist already. */
    void AddEdge(int x, int y);
    /* Sets the value of the edge between the two given vertices. */
    void SetEdge(int x, int y, T edge_value);
    //Returns the connected neighbors of the given vertex.
    std::unordered_set<int> GetNeighbors(int x) const;
    //Returns all the vertices that can be reached by the specified start vertex through any connection.
    std::unordered_set<int> GetReachable(int start_vertex) const;
    //Returns true if vertices x and y have an edge.
    bool AreAdjacent(int x, int y) const {
        return (edges_[x][y] != 0);
    }
    //Returns the number of vertices (size of the graph).
    int GetSize() const {
        return size_;
    }
    //Gets the value of the edge between the two given vertices.
    double GetEdgeValue(int x, int y) const {
        return edges_[x][y];
    }
private:
    friend std::ostream& operator<<<>(std::ostream& out, const Graph<T>& graph);

    int size_;
    std::vector<std::vector<T>> edges_; //adjacency matrix with value of the edges
};

template<typename T>
inline void Graph<T>::AddEdge(int x, int y) {
    if (!AreAdjacent(x, y)) {
        SetEdge(x, y, 1);
    }
}

template<typename T>
inline void Graph<T>::SetEdge(int x, int y, T edge_value) {
    if (x != y) {
        //no loops
        edges_[x][y] = edges_[y][x] = edge_value;
    }
}

#endif /* defined(__HW_2__Graph__) */
