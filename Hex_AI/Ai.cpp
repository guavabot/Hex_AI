#include "Ai.h"

#include <algorithm>
#include <random>
#include <map>
#include <vector>
#include <utility>
#include <ctime>
#include <cassert>
#include <cstddef>
#include <future>
#include <atomic>
#include <cstdbool>
#include <functional>
#include <iostream>
#include <iterator>
#include <string>

#include "Graph.h"
#include "Move.h"
#include "Player.h"
#include "ThreadPool.h"
#include "VirtualBoard.h"

using namespace std;

namespace {

    atomic<bool> abort_sim(false);

    const double GIVE_UP_FACTOR = 0.25; //higher values make the AI give up more easily

    const int SIMULATIONS = 1100;

    /*
     * Runs a Monte Carlo simulation of 1000 possible results of the opponent occupying a position.
     * This function will run in multiple threads for each possible position.
     * Simulations can be aborted with the flag abort_sim.
     * pos              The position occupied by the opponent
     * free_pos_set     The free positions in the board before the opponent's movement
     * board            The board initialized with the computer's occupied positions
     * nodes_to_fill    The pre-calculated amount of positions that the computer can occupy
     *                  (half of the board+)
     * Returns the computer's calculated win ratio if its opponent occupies the given position.
     */
    double GetMonteCarloWinRatio(int pos,
                                 unordered_set<int> free_pos_set,
                                 const VirtualBoard& board,
                                 int pos_to_fill) {
        if (abort_sim) return 0;
        default_random_engine engine { random_device { }() };
        int wins = 0;
        free_pos_set.erase(pos);
        vector<int> free_nodes(free_pos_set.begin(), free_pos_set.end());
        for (int sims = 0; sims < SIMULATIONS && !abort_sim; sims++) {
            shuffle(free_nodes.begin(), free_nodes.end(), engine); //shuffle free positions
            //board is preinitialized with already occupied positions in the real board
            //plus one position occupied by AI in the top level of the simulation
            VirtualBoard new_board = board;
            //fill half the board with AI stones and check if it connected the edges
            new_board.FillBoard(free_nodes, pos_to_fill);
            if (new_board.HasWon()) {
                wins++; //we have a winner
            }
        }
        return static_cast<double>(wins) / SIMULATIONS;
    }

    //Sets all the edges surrounding a vertex in a graph as connected.
    template<typename T>
    void AddSurroundingEdges(const int node, const int board_size, Graph<T>& graph) {
        ApplyAroundPosition(node, nullptr, board_size, [&graph](int x, int y, const Player* p){
            graph.AddEdge(x, y);
        }, [](int x, const Player* p){ return true; });
    }

    //Compares two entries in a map and returns true if the value of the second is higher.
    bool LessWins(const pair<int, double>& first_node, const pair<int, double>& second_node) {
        return first_node.second < second_node.second;
    }
}

Move Ai::ComputeMove() {
    cout << "... ";
    fflush(stdout);
    int pos = ChoosePosition();
    if (pos < 0) {
        return Move(Move::AI_GIVE_UP_CODE, board_);
    }

    int board_size = board_.GetSize();
    int row = pos / board_size;
    int col = pos % board_size;
    char letter = 'A' + col;
    string move;
    move.push_back(letter);
    move.append(to_string(1 + row));
    cout << move << endl;
    return Move(move, board_);
}

//Returns the best position that the AI can find or -1 if it decides to give up.
int Ai::ChoosePosition() {
    unordered_set<int> free_nodes = board_.GetFreePositions();
    assert(free_nodes.size() > 0);
    if (free_nodes.size() == 1) {
        //last position free, win game!
        return *free_nodes.begin();
    }
    //eliminate positions too far from the action
    unordered_set<int> selectable = GetSelectable(board_);
    assert(selectable.size() > 0);

    int best_pos = -1;
    double win_prob = 0;
    for (int pos : selectable) {
        TestOccupyingPos(pos, free_nodes, best_pos, win_prob);
    }

    if (win_prob < GIVE_UP_FACTOR) {
        return -1; //too slim chances, give up
    } else {
        //keep the virtual board updated
        virtual_board_.Occupy(best_pos);
        return best_pos;
    }
}

//Tests the result of occupying one position in the board and updates the best_pos and win_prob
//if it find a winning probability better than the one of the current best position.
//Uses Alpha-Beta pruning by skipping simulations of branches that the opponent can choose to minimize
//the AI's winning probability.
void Ai::TestOccupyingPos(const int pos,
                          const unordered_set<int>& free_pos,
                          int& best_pos,
                          double& win_prob) {
    unordered_set<int> test_free_pos = free_pos;
    test_free_pos.erase(pos);
    VirtualBoard test_board = virtual_board_;
    test_board.Occupy(pos);
    unordered_set<int> test_selectable = GetSelectable(test_board); //TODO
    if (FindBetterChances(test_selectable, test_free_pos, test_board, win_prob)) {
        best_pos = pos;
    }
}

//Runs a Monte Carlo simulation for every position that the opponent can choose as a response.
//Returns false if it brakes the search because it found any win_prob worse than the current win_prob
//(because the opponent can choose that branch to minimize the AI's winning ratio)
//Returns true if completes, so the explored branch is better and the win_prob gets updated.
bool Ai::FindBetterChances(const unordered_set<int>& selectable,
                           const unordered_set<int>& free_pos,
                           const VirtualBoard& test_board,
                           double& win_prob) const {
    //The M C simulations will randomly fill half of the board,
    //we calculate how many positions that is.
    //One free position is for the opponent.
    int pos_to_fill = free_pos.size() / 2;
    abort_sim = false;
    deque<future<double>> tasks;
    int max_threads = 4;
    //int max_threads = thread::hardware_concurrency();
    //if (max_threads == 0) max_threads = 2;
    ThreadPool pool(max_threads);
    for (int pos : selectable) {
        tasks.push_back(pool.enqueue(GetMonteCarloWinRatio,
                                       pos,
                                       free_pos,
                                       cref(test_board),
                                       pos_to_fill));
    }
    set<double> win_ratios;
    for (auto& it : tasks) {
        double win_ratio = it.get();
        if (win_ratio < win_prob) {
            abort_sim = true;
            return false;
        }
        win_ratios.insert(win_ratio);
    }
    //All the win ratios were higher than the current one,
    //we replace it by the lowest one we find
    //(since the opponent will try to minimize the AI's chances).
    win_prob = *win_ratios.begin();
    return true;
}

//Returns the free positions of the board but ignoring the ones that are far from (the action)
//the center and far from any other occupied nodes. That is, if a node is neighbor of an
//occupied node, or neighbor of a neighbor of an occupied node, it will be considered
//selectable.
unordered_set<int> Ai::GetSelectable(const AbstractBoard& board) const {
    const int board_size = board_.GetSize();
    Graph<int> graph(board_size * board_size);
    //center is always selectable even if not yet occupied
    //if board_size is even, center will be off a bit, doesn't matter
    int middle = board_size / 2;
    int center_pos = middle * board_size + middle;
    //connect center to the 6 positions around it
    AddSurroundingEdges(center_pos, board_size, graph);
    //connect also all occupied positions and their neighbors
    unordered_set<int> occupied = board.GetOccupiedPositions();
    for (int node : occupied) {
        AddSurroundingEdges(node, board_size, graph);
    }
    unordered_set<int> selectable = graph.GetReachable(center_pos);
    //increase the selectable by adding another level of neighbors
    for (int node : selectable) {
        AddSurroundingEdges(node, board_size, graph);
    }
    selectable = graph.GetReachable(center_pos);
    selectable.insert(center_pos);
    //remove all occupied nodes
    for (int node : occupied)
        selectable.erase(node);
    return selectable;
}
