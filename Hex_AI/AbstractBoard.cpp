//
//  AbstractBoard.cpp
//  Hex_AI
//
//  Created by Zorian on 12/5/13.
//  Copyright (c) 2013 Ivan Soriano. All rights reserved.
//

#include "AbstractBoard.h"

#include <deque>
#include <stack>

//After a stone was set, mark surrounding positions as connected
//by the player if they also have player's stones.
void AbstractBoard::AddMissingConnections(int pos, const Player& player) {
    ApplyAroundPosition(pos, &player, size_, [this](int x, int y, const Player* p) {
        MarkConnected(x, y, *p);
    }, [this](int x, const Player* p) -> bool {
        return Belongs(x, *p);
    });
}

//Uses depth first search to try to find a connection between
//the two virtual nodes that represent the board edges.
bool AbstractBoard::HasWon(const Player& player) const {
    int num_vertices = size_ * size_ + 2;
    int first_virtual = num_vertices - 1;
    int second_virtual = num_vertices - 2;
    std::stack<int> stack;
    std::vector<char> visited(num_vertices); //1 if visited
    //start with the first virtual node
    stack.push(first_virtual);
    visited[first_virtual] = 1;
    while (!stack.empty()) {
        int current_node = stack.top();
        stack.pop();
        //find next reachable not-yet-visited positions
        for (int possible_match = second_virtual; possible_match >= 0; possible_match--) {
            if (visited[possible_match] == 0
                    && AreConnected(current_node, possible_match, player)) {
                if (possible_match == second_virtual) {
                    return true; //game was won
                }
                stack.push(possible_match);
                visited[possible_match] = 1;
            }
        }
    }
    return false;
}

std::unordered_set<int> AbstractBoard::GetOccupiedPositions() const {
    std::unordered_set<int> nodes_list;
    int total_nodes = size_ * size_;
    for (int i = 0; i < total_nodes; i++) {
        if (IsOccupied(i)) {
            nodes_list.insert(i);
        }
    }
    return nodes_list;
}

std::unordered_set<int> AbstractBoard::GetFreePositions() const {
    std::unordered_set<int> nodes_list;
    int total_nodes = size_ * size_;
    for (int i = 0; i < total_nodes; i++) {
        if (!IsOccupied(i)) {
            nodes_list.insert(i);
        }
    }
    return nodes_list;
}
