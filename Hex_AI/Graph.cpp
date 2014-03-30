#include "Graph.h"

#include <random>
#include <deque>
#include <iterator>
#include <stack>
#include <fstream>

namespace {

    // Returns a random number between 0 and 1 to represent a probability
    double RandomProbability() {
        static std::default_random_engine engine { std::random_device { }() };
        static std::uniform_real_distribution<double> d { 0.0, 1.0 };
        return d(engine);
    }

    // Returns a random integral number in the given range
    template<typename T>
    typename std::enable_if<std::is_integral<T>::value, T>::type
    RandomEdgeValue(T range_min, T range_max) {
        static std::default_random_engine engine { std::random_device { }() };
        static std::uniform_int_distribution<T> d { range_min, range_max };
        return d(engine);
    }

    // Returns a random floating point number in the given range
    template<typename T>
    typename std::enable_if<std::is_floating_point<T>::value, T>::type
    RandomEdgeValue(T range_min, T range_max) {
        static std::default_random_engine engine { std::random_device { }() };
        static std::uniform_real_distribution<T> d { range_min, range_max };
        return d(engine);
    }
}

template<typename T>
Graph<T>::Graph(int size, double density, T range_min, T range_max) :
        size_(size), edges_(size, std::vector<T>(size)) {
    for (int i = 0; i < size_; i++) {
        for (int j = i; j < size_; j++) {
            if ((i != j) && RandomProbability() < density) {
                edges_[i][j] = edges_[j][i] = RandomEdgeValue<T>(range_min, range_max);
            }
        }
    }
}

template<typename T>
Graph<T>::Graph(std::ifstream& ifp) {
    std::istream_iterator<T> in(ifp), eof; //in(ifp) is an iterator of numbers in the file
    std::vector<T> input_numbers(in, eof);
    size_ = static_cast<int>(input_numbers[0]); //first value contains the size of the graph
    edges_.assign(size_, std::vector<T>(size_)); //create the adjacency matrix

    //Iterate through number triples
    for (int i = 1; i < input_numbers.size() - 2; i += 3) {
        int vertex1 = static_cast<int>(input_numbers[i]);
        int vertex2 = static_cast<int>(input_numbers[i + 1]);
        T value = input_numbers[i + 2];
        edges_[vertex1][vertex2] = value;
    }
}

template<typename T>
std::ostream& operator<<(std::ostream& out, const Graph<T>& graph) {
    for (int i = 0; i < graph.size_; ++i) {
        for (int j = 0; j < graph.size_; ++j) {
            out << graph.edges_[i][j] << ", ";
            if (j == graph.size_ - 1) {
                out << std::endl;
            }
        }
    }
    return out;
}

template<typename T>
std::unordered_set<int> Graph<T>::GetNeighbors(int x) const {
    std::unordered_set<int> list;
    for (int i = 0; i < size_; ++i) {
        if (AreAdjacent(x, i)) {
            list.insert(i);
        }
    }
    return list;
}

template<typename T>
//Uses Depth-first-search
std::unordered_set<int> Graph<T>::GetReachable(int start_vertex) const {
    std::unordered_set<int> reachable;
    std::deque<bool> visited(size_);
    std::stack<int> stack;
    stack.push(start_vertex);
    visited[start_vertex] = true;
    while (!stack.empty()) {
        int current_node = stack.top();
        stack.pop();
        //find next accessible not-yet-visited vertices
        for (int other = size_ - 1; other >= 0; other--) {
            if (!visited[other] && AreAdjacent(current_node, other)) {
                stack.push(other);
                visited[other] = true;
                reachable.insert(other);
            }
        }
    }
    return reachable;
}

template class Graph<int> ;

template std::ostream& operator<<(std::ostream& out, const Graph<int>& graph);
