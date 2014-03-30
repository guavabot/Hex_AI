#ifndef __Hex_AI__AbstractBoard__
#define __Hex_AI__AbstractBoard__

#include <cassert>
#include <unordered_set>
#include <vector>

#include "Player.h"

/*
 * Base for real or virtual boards.
 * It keeps track of which position if occupied by each player
 * by filling a vector with the player IDs.
 */
class AbstractBoard {
public:
    //size  The number of positions per board side.
    explicit AbstractBoard(int size) :
            size_(size), stones_(size * size) {
    }
    virtual ~AbstractBoard() {
    }

    /* Returns true if the player has connected the two edges of the board. */
    bool HasWon(const Player&) const;
    //Returns true if any user has occupied the position.
    bool IsOccupied(int pos) const {
        return stones_[pos] != 0;
    }
    //Returns the number of positions per board side.
    int GetSize() const {
        return size_;
    }
    //Returns a list of the occupied positions in the  board.
    std::unordered_set<int> GetOccupiedPositions() const;
    //Returns a list of the free positions in the board.
    std::unordered_set<int> GetFreePositions() const;
    //Marks a position as occupied by a player and updates the board accordingly.
    void Occupy(int pos, const Player& player) {
        OccupyImpl(pos, player);
    }
protected:
    //Used when a player occupies a position.
    void SetStone(int pos, const Player& player) {
        stones_[pos] = player.GetId();
        AddMissingConnections(pos, player);
    }
private:
    //Set stones, mark the board and/or mark connections as needed.
    virtual void OccupyImpl(int pos, const Player&) = 0;
    //Mark two positions as connected by a player.
    virtual void MarkConnected(int pos1, int pos2, const Player& player) = 0;
    //Returns true if pos1 and pos2 have stones of the same player.
    virtual bool AreConnected(int pos1, int pos2, const Player& player) const = 0;
    //After a stone was set, mark surrounding positions as connected
    //by the player if they also have player's stones.
    void AddMissingConnections(int pos, const Player&);
    //Returns true if the user has set a stone in the given position.
    bool Belongs(int pos, const Player& player) const {
        return stones_[pos] == player.GetId();
    }

    const int size_;
    //Stores occupied positions with the players IDs.
    std::vector<int> stones_;
};

template<typename Applier, typename Approver>
void ApplyAroundPosition(int pos, const Player* p, int size_, Applier apl, Approver apr) {
    int i = pos / size_; //board numbers
    int j = pos % size_; //board letters
    //top, bottom, left and right in the comments are from the perspective
    //of player A, so numbers of the board are horizontal and letters vertical
    //check hexagon to the left
    if (j > 0 && apr(pos - 1, p)) {
        apl(pos, pos - 1, p);
    }
    //check hexagon to the right
    if (j < size_ - 1 && apr(pos + 1, p)) {
        apl(pos, pos + 1, p);
    }
    //check top-left hexagon
    if (i > 0 && apr(pos - size_, p)) {
        apl(pos, pos - size_, p);
    }
    //check top-right hexagon
    if (i > 0 && j < size_ - 1 && apr(pos - size_ + 1, p)) {
        apl(pos, pos - size_ + 1, p);
    }
    //check bottom-left hexagon
    if (i < size_ - 1 && j > 0 && apr(pos + size_ - 1, p)) {
        apl(pos, pos + size_ - 1, p);
    }
    //check bottom-right hexagon
    if (i < size_ - 1 && apr(pos + size_, p)) {
        apl(pos, pos + size_, p);
    }
}

#endif /* defined(__Hex_AI__AbstractBoard__) */
