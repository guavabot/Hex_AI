#ifndef __Hex_AI__AI__
#define __Hex_AI__AI__

#include <set>
#include <unordered_set>

#include "Player.h"
#include "Board.h"
#include "VirtualBoard.h"

class AbstractBoard;
class Move;
class VirtualBoard;

/*
 This class returns computer generated Moves
 It simulates the moves that the computer's opponent can make in response
 to every possible computer movement.
 It estimates results by running a Monte Carlo simulation of 1000 outcomes.
 
 Hence for n free positions,
 there are (n)(n-1)(1000) outcomes simulated
 
 To reduce the number of simulations two things are done:
 1. (Alpha–beta pruning) For each of the computer possible moves, if an opponent
 move is found that is better for him than his current best move, simulations
 for that computer possible move are stopped.
 2. Only positions that have at least one occupied position in the neighbors of 
 its neighbors are considered. The center of the board is always considered.
 */
class Ai {
public:
    /**
     * board            The game's board
     * computer_first   True if the computer makes the first move
     */
    Ai(Board& board, bool computer_first) :
            board_(board), virtual_board_(board.GetSize(), computer_first) {
    }

    //Runs a Monte Carlo simulation to compute the next move.
    Move ComputeMove();
private:
    int ChoosePosition();
    void TestOccupyingPos(const int pos,
                           const std::unordered_set<int>& free_pos,
                           int& best_pos,
                           double& win_prob);
    bool FindBetterChances(const std::unordered_set<int>& selectable,
                           const std::unordered_set<int>& free_pos,
                           const VirtualBoard& test_board,
                           double& win_prob) const;
    std::unordered_set<int> GetSelectable(const AbstractBoard& board) const;

    Board& board_;
    //we keep a virtual board with the current occupied positions
    //by the computer, so that we don't have
    //to initialize it every time we ComputeMove()
    VirtualBoard virtual_board_;
};

#endif /* defined(__Hex_AI__AI__) */
