//
//  Weapons.cpp
//  CS32_proj3
//
//  Created by Janys Li on 5/12/24.
//

#include "Weapons.h"
#include "Actor.h"


void Scr_tel::read(Player *p,char space[NROW][NCOL],string& log){
    int x,y;
    cerr<<"teleporting";
    
    // loop to choose valid location
    do{
        x = randInt(0, NROW-1);
        y = randInt(0, NCOL-1);
    }while(space[x][y]!=' ');
    
    // transport player
    space[p->getloc().first][p->getloc().second] = ' ';
    p->setloc(x, y);
    space[x][y] = '@';
    
    log ="You read the scroll called scroll of teleportation\nYou feel your body wrenched in space and time.\n";
}

void Scr_arm::read(Player *p,char nona_space[NROW][NCOL],string& log){
    p->set_arm(p->get_arm()+randInt(1, 3));
    log ="You read the scroll called scroll of enhance armor\nYour armor grows blue.\n";
    
}
void Scr_str::read(Player *p,char nona_space[NROW][NCOL],string& log){
    p->set_str(p->get_str()+randInt(1, 3));
    log ="You read the scroll called scroll of strength\nYour muscles bulge.\n";
}
void Scr_hth::read(Player *p,char nona_space[NROW][NCOL],string& log){
    p->set_max_health(p->get_max_health()+randInt(3, 8));
    log ="You read the scroll called scroll of enhance health\nYou feel your heart beating stronger.\n";
}
void Scr_dex::read(Player *p,char nona_space[NROW][NCOL],string& log){
    p->set_dex(p->get_dex()+1);
    log ="You read the scroll called scroll of enhance dexterity\nYou feel like less of a klutz.\n";
}
