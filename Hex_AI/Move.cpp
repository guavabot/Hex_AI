#include "Move.h"
#include <ctype.h>
#include <iostream>
#include "Player.h"
#include "Board.h"

using namespace std;

const string Move::AI_GIVE_UP_CODE = "Not being the smartest AI today";

MoveResult Move::Parse() {
    if (move_ == AI_GIVE_UP_CODE) {
        return MoveResult::COMPUTER_GAVE_UP;
    }
    if (move_.find("GIVE UP") != std::string::npos || move_.find("give up") != std::string::npos) {
        return MoveResult::USER_GAVE_UP;
    }

    position_ = parseNode();
    if (position_ < 0 || isOccupied()) {
        position_ = -1;
        return MoveResult::INVALID_MOVE;
    }
    return MoveResult::VALID_MOVE;
}

//Returns the node number or -1 if the move node could not be parsed.
int Move::parseNode() {
    char move_letter = toupper(move_[0]); //capitalizes it
    string move_copy = move_;
    string move_number_s = move_copy.erase(0, 1); //remove first char
    //check if input is in the correct format
    if (!isdigit(move_number_s[0])) {
        std::cout << "Invalid move. Write a letter followed by a number." << endl;
        return -1;
    } else {
        int move_number = stoi(move_number_s);
        if (move_letter < 'A' || move_letter > ('A') - 1 + board_.GetSize() || move_number < 1
                || move_number > board_.GetSize()) {
            cout << "Invalid move input. Try again." << endl;
            return -1;
        }
        return (move_number - 1) * board_.GetSize() + (move_letter - 'A');
    }
}

//Returns true if the parsed node is already occupied.
bool Move::isOccupied() {
    if (position_ > 0 && board_.IsOccupied(position_)) {
        cout << "That position is already occupied" << endl;
        return true;
    }
    return false;
}

bool Move::Apply(const Player& player) const {
    if (position_ >= 0) {
        board_.AbstractBoard::Occupy(position_, player);
        cout << board_ << "\n---------------------------------------------" << endl;
        return true;
    }
    return false;
}
