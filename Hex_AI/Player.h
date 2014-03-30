// This class represents the player who is making a move
// It can be used to return the name of the player,
// its number and its mark in the board

#ifndef __Hex_AI__Player__
#define __Hex_AI__Player__

#include <cassert>
#include <iostream>
#include <string>

/*
 * This class works like a Java enum.
 */
class Player {
public:
    static const Player BLUE_PLAYER;
    static const Player RED_PLAYER;

    bool operator==(const Player& other) const {
        return id_ == other.id_;
    }
    int GetId() const {
        return id_;
    }
    Player NextPlayer() {
        return (id_ == 1) ? RED_PLAYER : BLUE_PLAYER;
    }
    std::string GetBoardMark() const {
        return (id_ == 1) ? "BLUE" : "red ";
    }
    bool PlaysFirst() const {
        return id_ == 1;
    }
private:
    explicit Player(int num) :
            id_(num) {
    }
    const int id_;
};

std::ostream& operator<<(std::ostream& stream, const Player& player);

#endif /* defined(__Hex_AI__Player__) */
