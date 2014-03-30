/*
 * Player.cpp
 *
 *  Created on: Mar 28, 2014
 *      Author: zorian
 */

#include "Player.h"

const Player Player::BLUE_PLAYER = Player(1);
const Player Player::RED_PLAYER = Player(2);

std::ostream& operator<<(std::ostream& stream, const Player& player) {
    std::cout << (player == Player::BLUE_PLAYER ? "Blue Player" : "Red Player");
    return stream;
}
