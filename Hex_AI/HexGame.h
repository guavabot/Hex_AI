#ifndef __Hex_AI__HexGame__
#define __Hex_AI__HexGame__

#include <cassert>
#include <memory>

#include "Ai.h"
#include "Board.h"
#include "Player.h"

namespace HexConst {
    const int MIN_BOARD_SIZE = 5;
    const int MAX_BOARD_SIZE = 14;
}

class Move;

/*
 * Manages a Hex game from start to finish and prints out who won the game.
 */
class HexGame {
public:
    /**
     * board_size		From HexConst:MIN_BOARD_SIZE to MAX_BOARD_SIZE
     * is_computer		Playing against computer or another user
     * computer_first	True if the computer makes the first move. Ignored if !is_computer
     */
    HexGame(int board_size, bool is_computer = false, bool computer_first = false) :
                    board_(board_size),
                    is_computer_(is_computer),
                    computer_first_(computer_first),
                    ai_(is_computer_ ? new Ai(board_, computer_first_) : nullptr) {
        assert(board_size >= HexConst::MIN_BOARD_SIZE && board_size <= HexConst::MAX_BOARD_SIZE);
    }

    void RunGame();
private:
    void PrintWelcome();
    Move GetNextMove();
    void PrintWinner(const Player& winner) const;
    void ChangePlayerTurn();
    HexGame(const HexGame&) = delete;
    HexGame& operator=(const HexGame&) = delete;

    Board board_;
    bool is_computer_;
    bool computer_first_;
    int move_cnt_ = 0;
    const Player* player_ = &Player::BLUE_PLAYER; //Blue Player starts
    std::unique_ptr<Ai> ai_;
};

#endif /* defined(__Hex_AI__HexGame__) */
