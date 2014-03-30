#ifndef __Hex_AI__RealBoard__
#define __Hex_AI__RealBoard__

#include <iostream>
#include <string>
#include <unordered_set>
#include <vector>

#include "AbstractBoard.h"
#include "Graph.h"

/* This class represents a Hex board and can display it on screen.
 * It saves the appearance of the board in a vector of strings.
 * For example, for a board of size 3, it looks like this:
 *               ____
 *             C/    \1
 *         ____/      \____
 *       B/    \      /    \2
 *   ____/      \____/      \____
 * A/    \      /    \      /    \3
 * /      \____/      \____/      \
 * \      /    \      /    \      /
 * 1\____/      \____/      \____/C
 *       \      /    \      /
 *       2\____/      \____/B
 *             \      /
 *             3\____/A
 *
 * The class also has a graph for each player, each with a node for every position
 * in the board, plus 2 virtual nodes that represent the edges that the
 * players have to connect. This way it can be more quickly calculated if
 * a player has won.
 */
class Board: public AbstractBoard {
public:
    //size  The number of positions per board side.
    explicit Board(int size);
private:
    void OccupyImpl(int pos, const Player&) override;
    void MarkConnected(int pos1, int pos2, const Player& player) override {
        assert(pos1 >= 0 && pos2 >= 0);
        graphs_[GetGraphIndex(player)].AddEdge(pos1, pos2);
    }
    bool AreConnected(int pos1, int pos2, const Player& player) const override {
        assert(pos1 >= 0 && pos2 >= 0);
        return graphs_[GetGraphIndex(player)].AreAdjacent(pos1, pos2);
    }
    int GetGraphIndex(const Player& player) const {
        return player == Player::BLUE_PLAYER ? 0 : 1;
    }
    void MarkBoard(int x, int y, const Player&); //place a mark for a player on the board
    void MakeVirtualNodes();
    void InitDrawing(); //creates a graphical representation of the board
    void CreateDrawingBottom(const int row_of_A, const int half_row_size,
            const int cols_per_hexagon);
    void CreateDrawingTop(const int row_of_A, const int half_row_size, const int cols_per_hexagon);
    void CreateTopmostRow();
    friend std::ostream& operator<<(std::ostream& stream, const Board& RealBoard);

    std::vector<std::string> drawing_; //appearance of the board
    std::vector<Graph<int>> graphs_;
};
#endif /* defined(__Hex_AI__RealBoard__) */
