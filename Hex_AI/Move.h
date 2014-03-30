#ifndef __Hex_AI__Move__
#define __Hex_AI__Move__

#include <string>

class Board;
class Player;

enum class MoveResult {
    INVALID_MOVE, VALID_MOVE, COMPUTER_GAVE_UP, USER_GAVE_UP
};

class Move {
public:
    static const std::string AI_GIVE_UP_CODE;
    /**
     * move       The selected move as a letter followed by a number
     * board      The board on which the move will be applied
     */
    Move(std::string move, Board& board) :
            board_(board), move_(move) {
    }

    //Parses the move to a node and outputs a validation result code.
    MoveResult Parse();
    /*If this move has been parsed and is valid, it applies the move and returns true.*/
    bool Apply(const Player&) const;
private:
    int parseNode();
    bool isOccupied();

    Board& board_;
    const std::string move_;
    int position_ = -1;
};

#endif /* defined(__Hex_AI__Move__) */
