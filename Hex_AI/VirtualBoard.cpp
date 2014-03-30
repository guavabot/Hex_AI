#include "VirtualBoard.h"

using namespace std;

//only one player in the virtual board, which can be either blue or red player.
const Player VirtualBoard::VIRTUAL_PLAYER = Player::BLUE_PLAYER;

//There are 2 virtual nodes connecting to the edges of the board
//The user wins when the two virtual nodes are connected
void VirtualBoard::MakeVirtualNodes(bool connect_letters) {
    //a node for every position plus two virtual nodes
    int total_nodes = GetSize() * GetSize() + 2;
    int first_virtual = total_nodes - 1;
    int second_virtual = total_nodes - 2;

    int edge_pos;
    for (int i = 0; i < GetSize(); i++) {
        edge_pos = connect_letters ? second_virtual - 1 - i : i * GetSize();
        MarkConnected(second_virtual, edge_pos);
        edge_pos = connect_letters ? i : i * GetSize() + GetSize() - 1;
        MarkConnected(first_virtual, edge_pos);
    }
}

// Occupy by the AI the specified amount of positions in the free positions.
void VirtualBoard::FillBoard(const std::vector<int> &free_nodes, int to_fill) {
    for (int i = 0; i < to_fill; i++) {
        Occupy(free_nodes[i]);
    }
}
