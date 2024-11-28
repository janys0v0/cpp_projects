// main.cpp

// Note:  YOU MUST NOT MAKE ANY CHANGE TO THIS FILE, except for the argument
// to the Game constructor.

#include "Game.h"
#include <iostream>
#include "utilities.h"

using namespace std;


int main()
{
      // Your program must use whatever value is passed to the Game constructor
      // as the distance a goblin can smell.  The spec stated this is 15.  Some
      // people might implement the recursive goblin algorithm for determining
      // the shortest path to the player in a way that takes an uncomfortably
      // long time for that distance.  If you're one of them, then until you
      // improve your algorithm, pass a value smaller than 15 to the game
      // constructor so your game is comfortable to play.

    
    //  Run full game ----
	Game g(15);
	g.play();
    
    
    // For testing pathExists
//    char a[NROW][NCOL] = {
//            "#################.....................................................",
//            "#       #        .....................................................",  // First row of the array
//            "#D# ### ##### ###.....................................................",  // Second row of the array
//            "# #     #        .....................................................",  // Third row of the array
//            "#  D  #   ####   ....................................................",  // Fourth row of the array
//            "############   ##....................................................",  // Fifth row of the array
//            ".....................................................................",  // Sixth row of the array
//            ".....................................................................",  // Seventh row of the array
//            ".....................................................................",  // Eighth row of the array
//            ".....................................................................",  // Ninth row of the array
//            ".....................................................................",  // Tenth row of the array
//            ".....................................................................",  // Eleventh row of the array
//            ".....................................................................",  // Twelfth row of the array
//            ".....................................................................",  // Thirteenth row of the array
//            ".....................................................................",  // Fourteenth row of the array
//            ".....................................................................",  // Fifteenth row of the array
//            ".....................................................................",  // Sixteenth row of the array
//            "....................................................................."  // Eighteenth row of the array
//        };
//
//    
//    vector<pair<int,int>> path1 = pathExists(a,3,5, 1, 5, 15);
//    vector<pair<int,int>> path2 = pathExists(a, 3,5,4, 2, 15);
//    vector<pair<int,int>> path3 = pathExists(a, 3,5,5, 12, 15);
//    vector<pair<int,int>> path4 = pathExists(a, 3,5,1, 16, 15);
//    vector<pair<int,int>> path5 = pathExists(a, 3,5,4, 16, 15);
//    vector<pair<int,int>> path6 = pathExists(a, 3,5,1, 9, 15);
//    
//    cout<<path1.size()<<" ";
//    cout<<path2.size()<<" ";
//    cout<<path3.size()<<" ";
//    cout<<path4.size()<<" ";
//    cout<<path5.size()<<" ";
//    cout<<path6.size()<<" ";
//    cout<<endl;
////    for (auto i:path1){
////        cout<<i.first<<" "<<i.second<<endl;
////    }
//    cout<<path1[1].first<<" "<<path1[1].second<<endl;
//    cout<<path3[1].first<<" "<<path3[1].second<<endl;
//    cout<<path4[1].first<<" "<<path4[1].second<<endl;
//    cout<<path5[1].first<<" "<<path5[1].second<<endl;
    
}
