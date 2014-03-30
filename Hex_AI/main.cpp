/*
 * Game of Hex with AI
 *
 * Ivan Soriano
 * for the class C++ for C programmers
 *
 * The user can play against another human or against the computer.
 * The AI runs Monte Carlo simulations to choose its movements.
 *
 */

#include <cctype>
#include <iostream>
#include <sstream>
#include <string>

#include "HexGame.h"

using namespace std;

int GetBoardSize() {
    int size;
    cout << "\nChoose the size of the board (" << HexConst::MIN_BOARD_SIZE << " to "
            << HexConst::MAX_BOARD_SIZE << "): ";

    while (true) {
        string input;
        getline(cin, input);
        stringstream myStream(input);
        if (myStream >> size) {
            if (size >= HexConst::MIN_BOARD_SIZE && size <= HexConst::MAX_BOARD_SIZE) break;
        }
        cout << "Wrong number, try again: ";
    }
    return size;
}

bool IsPlayingAgainstComputer() {
    bool is_computer;
    cout << "\nYou can play against me or against another human.\n" << "Computer or Human: ";
    while (true) {
        string input;
        getline(cin, input);
        if (toupper(input[0]) == 'C') {
            is_computer = true;
            break;
        } else if (toupper(input[0]) == 'H') {
            is_computer = false;
            break;
        }
        cout << "Wrong input, write computer or human: ";
    }
    cout << "Playing against " << (is_computer ? "the computer." : "a human.") << endl;
    return is_computer;
}

bool IsComputerFirst(bool is_computer) {
    bool computer_first = false;
    if (is_computer) {
        cout << "\nIn hard mode, I make the first move.\n" << "Who should make the first move?\n"
                << "Computer or Human: ";
        while (true) {
            string input;
            getline(cin, input);
            if (toupper(input[0]) == 'C') {
                computer_first = true;
                break;
            } else if (toupper(input[0]) == 'H') {
                computer_first = false;
                break;
            }
            cout << "Wrong input, write computer or human: ";
        }
        cout << (computer_first ? "I'll teach you how the pros play." : "Easy mode it is.") << endl;
    }
    return computer_first;
}

bool IsPlayAgain() {
    bool play_again;
    cout << "\nWant to play again?\n" << "Yes or No: ";
    while (true) {
        string input;
        getline(cin, input);
        if (toupper(input[0]) == 'Y') {
            play_again = true;
            break;
        } else if (toupper(input[0]) == 'N') {
            play_again = false;
            cout << "Good match! See you soon!\n";
            break;
        }
        cout << "Wrong input, write yes or no: ";
    }
    return play_again;
}

int main() {
    cout << "Welcome to the game of Hex!" << endl;

    do {
        int board_size = GetBoardSize();
        bool is_computer = IsPlayingAgainstComputer();
        bool computer_first = IsComputerFirst(is_computer);

        HexGame hex(board_size, is_computer, computer_first);
        hex.RunGame();
    } while (IsPlayAgain());

    return 0;
}

