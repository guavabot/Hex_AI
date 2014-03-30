#include "Board.h"

#include <cassert>
#include <utility>

#include "Player.h"

using namespace std;

Board::Board(int size) :
        AbstractBoard(size), //4 is the rows (of strings) per hexagon
        drawing_(1 + 4 * size, std::string()), //2 extra vertices in the graphs for virtual nodes
        graphs_(2, Graph<int>(size * size + 2)) {
    InitDrawing();
    MakeVirtualNodes();
}

/*
 * Creates a board with the following shape:
                    ____
                  C/    \1
              ____/      \____
            B/    \      /    \2
        ____/      \____/      \____
      A/    \      /    \      /    \3
      /      \____/      \____/      \
      \      /    \      /    \      /
      1\____/      \____/      \____/C
            \      /    \      /
            2\____/      \____/B
                  \      /
                  3\____/A

 The size of the board depends on user selection.
*/
void Board::InitDrawing() {
    //4 string rows are needed to represent the board for every increase in 1 in board size
    //plus one row for the very top
    const int rows_per_hexagon = 4;
    const int cols_per_hexagon = 6;
    const int half_row_size = rows_per_hexagon / 2;
    const int row_of_A = half_row_size * GetSize() - 1; //calculate the vector's row number where "A" is printed

    CreateDrawingBottom(row_of_A, half_row_size, cols_per_hexagon);
    CreateDrawingTop(row_of_A, half_row_size, cols_per_hexagon);
    CreateTopmostRow();
}

//Creates the bottom half of the board
//For each new game row there are 2 string rows created
//Example string 1 (see reference above):
//            \      /    \      /
//Example string 2:
//            2\____/      \____/B
void Board::CreateDrawingBottom(const int row_of_A,
        const int half_row_size, const int cols_per_hexagon) {
    for (int i = 0; i < GetSize(); i++) {
        //current_row is the row number of string 1 in each iteration
        int current_row = row_of_A + half_row_size * i + half_row_size;
        //Following is for string 1
        for (int j = 0; j < cols_per_hexagon * i; j++) {
            drawing_[current_row].push_back(' '); //create the blank space
        }
        for (int j = 0; j < GetSize() - i; j++) {
            drawing_[current_row].append("\\      /    "); //create the hexagons
        }
        //Following is for string 2
        current_row++; //the position of this string is just below string 1
        for (int j = 0; j < cols_per_hexagon * i; j++) {
            drawing_[current_row].push_back(' '); //create the blank space
        }
        //now we add the Hex board row number
        if (i + 1 > 9) {
            //if the number is bigger than 9, we need an extra space
            drawing_[current_row].pop_back();
        }
        drawing_[current_row].append(to_string(i + 1));
        for (int j = 1; j < GetSize() - i; j++) {
            drawing_[current_row].append("\\____/      "); //create the hexagons
        }
        drawing_[current_row].append("\\____/"); //last hexagon
        drawing_[current_row].push_back('A' - 1 + GetSize() - i); //add the Hex board column letter
    }
}

//Creates the upper part of the board
//For each new game row there are 2 string rows created
//Example string 1 (see reference above):
//            B/    \      /    \2
//Example string 2:
//        ____/      \____/      \____
void Board::CreateDrawingTop(const int row_of_A,
        const int half_row_size, const int cols_per_hexagon) {
    for (int i = 0; i < GetSize(); i++) {
        //current_row is the row number of string 1 in each iteration
        int current_row = row_of_A - half_row_size * i;
        //Following is for string 1
        for (int j = 0; j < cols_per_hexagon * i; j++) {
            drawing_[current_row].push_back(' '); //create the blank space
        }
        drawing_[current_row].push_back('A' + i); //add the Hex board column letter
        //create the hexagons
        drawing_[current_row].append("/    \\");
        for (int j = 1; j < GetSize() - i; j++) {
            drawing_[current_row].append("      /    \\");
        }
        drawing_[current_row].append(to_string(GetSize() - i)); //add the Hex board row number
        //Following is for string 2
        current_row++;
        if (i == 0) {
            //the first (central) row is different
            drawing_[current_row] = "/      \\";
            for (int i = 0; i < GetSize() - 1; i++) {
                drawing_[current_row].append("____/      \\");
            }
        } else {
            //create the blank space
            drawing_[current_row].append("  ");
            for (int j = 0; j < cols_per_hexagon * (i - 1); j++) {
                drawing_[current_row].push_back(' ');
            }
            //add the hexagons
            for (int j = 0; j < GetSize() - i; j++) {
                drawing_[current_row].append("____/      \\");
            }
            drawing_[current_row].append("____");
        }
    }
}

void Board::CreateTopmostRow() {
    //create the extra row at the top of the uppermost position
    for (int j = 1; j < GetSize(); j++) {
        drawing_[0].append("      "); //blank space
    }
    drawing_[0].append("  ____"); //top of a hexagon
}

//There are 2 virtual nodes connecting to the edges of the board
//The user wins when the two virtual nodes are connected
void Board::MakeVirtualNodes() {
    //a node for every position plus two virtual nodes
    int total_nodes = GetSize() * GetSize() + 2;
    int first_virtual = total_nodes - 1;
    int second_virtual = total_nodes - 2;
    //the board edges to which the virtual nodes connect depend on player number
    for (int i = 0; i < GetSize(); i++) {
        //Graph 0:
        //virtual node nodes-2 connects to all last row
        MarkConnected(second_virtual, second_virtual - 1 - i, Player::BLUE_PLAYER);
        //virtual node nodes-1 connects to all row 1
        MarkConnected(first_virtual, i, Player::BLUE_PLAYER);
        //Graph 1:
        //virtual node nodes-2 connects to all column A
        MarkConnected(second_virtual, i * GetSize(), Player::RED_PLAYER);
        //virtual node nodes-1 connects to all last column
        MarkConnected(first_virtual, i * GetSize() + GetSize() - 1, Player::RED_PLAYER);
    }
}

std::ostream& operator<<(std::ostream& out, const Board& board) {
    for (auto i : board.drawing_) {
        out << i << endl;
    }
    return out;
}

//Marks a position as occupied by a player in occupied_
//and updates the board position accordingly.
//It also updates the adjacency to neighboring positions
//if they are occupied by the same player
//x is the number and y the letter
// Implements pure virtual function
void Board::OccupyImpl(int pos, const Player& player) {
    assert(pos >= 0);
    SetStone(pos, player);
    MarkBoard(pos / GetSize(), pos % GetSize(), player);
}

//Adds player's mark to the board
//x is the number and y the letter
void Board::MarkBoard(int x, int y, const Player& player) {
    //locate the position of A1 in the strings
    const int cols_per_hexagon = 6;
    const int half_row_size = 2;
    const int col_A1 = cols_per_hexagon / 2;
    const int row_A1 = half_row_size * GetSize();
    //calculate where the marks should be placed
    int col_pos = col_A1 + cols_per_hexagon * y + cols_per_hexagon * x;
    int row_pos = row_A1 + half_row_size * x - half_row_size * y;

    //divide the mark in a top and a bottom part
    const int mark_length = 2;
    string upper_mark = player.GetBoardMark().substr(0, mark_length);
    string lower_mark = player.GetBoardMark().substr(mark_length, mark_length);
    //place the mark
    drawing_[row_pos].replace(col_pos, mark_length, upper_mark);
    drawing_[row_pos + 1].replace(col_pos, mark_length, lower_mark);
}
