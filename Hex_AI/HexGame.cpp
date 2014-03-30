#include "HexGame.h"

#include <cassert>
#include <iostream>
#include <string>

#include "Ai.h"
#include "Move.h"

using namespace std;

void HexGame::RunGame() {
    PrintWelcome();

    bool won = false; //has the game finished
    while (!won) {
        move_cnt_++;
        bool move_completed = false;
        while (!move_completed) {
            cout << *player_ << ", move " << move_cnt_ << ": ";
            Move move = GetNextMove();
            //validate and apply the move
            switch (move.Parse()) {
            case MoveResult::COMPUTER_GAVE_UP:
                cout << "You are good! I give up this time." << endl;
                //no break
            case MoveResult::USER_GAVE_UP:
                won = true;
                move_completed = true;
                ChangePlayerTurn(); //the other player won
                break;
            case MoveResult::VALID_MOVE:
                move_completed = move.Apply(*player_);
                won = board_.HasWon(*player_); //check if the game has been won
                if (!won) ChangePlayerTurn();
                break;
            default:
                //invalid move
                break;
            }
        }
    }

    PrintWinner(*player_);
}

void HexGame::ChangePlayerTurn() {
    if (*player_ == Player::BLUE_PLAYER) {
        player_ = &Player::RED_PLAYER;
    } else {
        player_ = &Player::BLUE_PLAYER;
    }
}

void HexGame::PrintWelcome() {
    if (is_computer_ && computer_first_) {
        cout << endl;
    } else {
        //show board to human
        cout << board_ << endl;
    }
    cout
            << "Blue Player starts and connects letters (north-west to south-east).\n"
            << "You can always write \"I give up\" to start a new game\n"
            << "To make a move, select a position.\n"
            << "Example:\nBlue Player, move 1: A1\n"
            << "---------------------------------------------\n";
}

//get the move from the user or computer
Move HexGame::GetNextMove() {
    if (is_computer_ && player_->PlaysFirst() == computer_first_) {
        //computer's turn
        return ai_->ComputeMove();
    } else {
        string move_input;
        getline(cin, move_input);
        return Move(move_input, board_);
    }
}

void HexGame::PrintWinner(const Player& winner) const {
    if (is_computer_) {
        if (winner.PlaysFirst() == computer_first_) {
            //computer won
            cout << "Better luck next time!\n" << endl;
        } else {
            if (computer_first_) {
                //human won in hard mode
                cout << winner << " is the next John Nash!\n" << endl;
            } else {
                cout << "Can you beat me in hard mode?\n" << endl;
            }
        }
    } else {
        cout << winner << " is the winner!\n" << endl;
    }
}
