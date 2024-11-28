// Game.h

#ifndef GAME_INCLUDED
#define GAME_INCLUDED

// You may add data members and other member functions to this class.
#include "utilities.h"
#include <string>
#include <vector>
#include <iostream>
#include "Actor.h"
#include <algorithm>
using namespace std;


class Game
{
public:
    Game(int goblinSmellDistance):
    m_goblinSmellDistance(goblinSmellDistance)
    {}
    void play();
    void display() const;
    void generate_room();
    void carve_path(int x1, int y1, int x2, int y2);
    void move_actor(Actor& a, int y, int x);
    inline char (&get_ac_space())[NROW][NCOL] { return m_ac_space; }
    inline char (&get_nona_space())[NROW][NCOL] { return m_nona_space; }
private:
    int m_goblinSmellDistance;
    char m_ac_space[NROW][NCOL]; // space for storing actors
    char m_nona_space[NROW][NCOL]; // space for storing non-actors
    vector<pair<int,int>> m_space;
    
};

void printloc(pair<int,int> loc);

// template function for finding monsters, scrolls, and weapons
template<typename T>
bool find_target(vector<T*>& objects, int x, int y,T*& found_obj){
    // iterate through items to find the one at corresponding location
    for(T* obj:objects){
        if(obj->getloc().first == x && obj->getloc().second == y){
            found_obj = obj;
            return true;
        }
    }
    found_obj = nullptr;
    return false; // not possible
}

pair<int,int> random_pickloc(char ac_space[NROW][NCOL]);

#endif // GAME_INCLUDED
