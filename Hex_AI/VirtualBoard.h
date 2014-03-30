#ifndef __Hex_AI__VirtualBoard__
#define __Hex_AI__VirtualBoard__

#include <vector>

#include "AbstractBoard.h"
#include "Graph.h"

/*
 * More efficient implementation of AbstractBoard to create and destroy
 * multiple times during the Monte Carlo simulations.
 * It manages the occupied positions by only one player (the AI).
 * It stores which positions the AI has connected in a graph with two extra
 * nodes that represent the two edges of the board that it has to connect.
 */
class VirtualBoard: public AbstractBoard {
public:
    //We only set AI stones in the virtual board, so the player number is irrelevant.
    static const Player VIRTUAL_PLAYER; // = BLUE_PLAYER
    /*
     * size             The number of positions per board side
     * connect_letters  True if the AI needs to connect letters to win
     */
    VirtualBoard(int size, bool connect_letters) :
            //2 extra vertices in the graph for virtual nodes
            AbstractBoard(size), graph_(size * size + 2) {
        MakeVirtualNodes(connect_letters);
    }

    void FillBoard(const std::vector<int> &shuffled_free_nodes, int);
    void Occupy(int pos) {
        OccupyImpl(pos, VIRTUAL_PLAYER);
    }
    bool HasWon() {
        return AbstractBoard::HasWon(VIRTUAL_PLAYER);
    }
private:
    void MakeVirtualNodes(bool connect_letters);
    void OccupyImpl(int pos, const Player& player) override {
        SetStone(pos, player);
    }
    void MarkConnected(int pos1, int pos2, const Player& player) override {
        MarkConnected(pos1, pos2);
    }
    void MarkConnected(int pos1, int pos2) {
        assert(pos1 >= 0 && pos2 >= 0);
        graph_.AddEdge(pos1, pos2);
    }
    bool AreConnected(int pos1, int pos2, const Player& player) const override {
        assert(pos1 >= 0 && pos2 >= 0);
        return graph_.AreAdjacent(pos1, pos2);
    }

    //Stores which positions the computer has connected
    Graph<int> graph_;
};
#endif /* defined(__Hex_AI__VirtualBoard__) */
