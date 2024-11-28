//
//  actor.h
//  CS32_proj3
//
//  Created by Janys Li on 5/11/24.
//

#ifndef actor_h
#define actor_h
#include <vector>
#include "utilities.h"
#include "Weapons.h"
#include <queue>
using namespace std;

class Game;

class Actor{
public:
    Actor(pair<int, int> loc, int hit_pt, int arm_pt, int str_pt, int dex_pt, int sleep_t, Weapon* wpn, string name):
    m_hit(hit_pt),m_arm(arm_pt),m_str(str_pt),m_dex(str_pt),m_sleep_t(sleep_t),m_loc(loc),m_wpn(wpn),m_died(false),m_name(name){}
    
    // get functions
    inline pair<int, int> getloc() const {return m_loc;};
    inline int get_hit() const {return m_hit;};
    inline int get_arm() const {return m_arm;};
    inline int get_str() const {return m_str;};
    inline int get_dex() const {return m_dex;};
    inline int get_sleep() const {return m_sleep_t;};
    inline string get_name() const {return m_name;};
    inline Weapon* get_wpn() const {return m_wpn;};
    inline bool is_dead() const {return m_died;};
    
    //set functions
    inline void setloc(int y, int x){m_loc = pair<int,int>(y,x);};
    inline void set_sleep(int sleep_t){m_sleep_t = sleep_t;};
    inline void set_hit(int hit){m_hit = hit;};
    inline void set_arm(int arm){m_arm = arm;};
    inline void set_dex(int dex){m_dex = dex;};
    inline void set_str(int str){m_str = str;};
    inline void set_wpn(Weapon* wpn){m_wpn = wpn;};
    inline void destr_wpn(){delete m_wpn;}; // destroy weapon
    void move_actor(int x, int y, char ac_space[NROW][NCOL]);
    
    // actions
    bool attack(Actor* other,string& log,char ac_space[NROW][NCOL], char nona_space[NROW][NCOL], vector<Tool*>& tools);
    inline virtual void die(char ac_space[NROW][NCOL], char nona_space[NROW][NCOL], vector<Tool*>& tools){}
    
    virtual ~Actor(){delete m_wpn;}
    
    
private:
    pair<int, int> m_loc;
    int m_hit,m_arm,m_str,m_dex,m_sleep_t;
    Weapon* m_wpn;
    bool m_died;
    string m_name;
    
};

class Player: public Actor{
public:
    Player(pair<int,int> loc): Actor(loc,20,2,2,2,0,new ShortSword(loc),"Player"),m_max_health(20){};
    inline int get_max_health() const {return m_max_health;};
    inline void set_max_health(int m){m_max_health = m;};
private:
    int m_max_health;
    
};

class Monster: public Actor{
public:
    Monster(pair<int,int> loc,int hit_pt, int arm_pt, int str_pt, int dex_pt, int sleep_t, Weapon* wpn, string name):
    Actor(loc,hit_pt,arm_pt,str_pt,dex_pt,sleep_t,wpn,name){}
    virtual void action(Actor* p, char ac_space[NROW][NCOL], char nona_space[NROW][NCOL], string& log)=0;
    void wander(char ac_space[NROW][NCOL]);
    virtual void die(char ac_space[NROW][NCOL], char nona_space[NROW][NCOL], vector<Tool*>& tools);
};

class Snakewoman: public Monster{
public:
    Snakewoman(pair<int,int> loc):
    Monster(loc,randInt(3, 7),3,2,3,0,new Fangs(loc),"Snakewoman"){};
    virtual void action(Actor* p, char ac_space[NROW][NCOL],char nona_space[NROW][NCOL], string& log);
    virtual void die(char ac_space[NROW][NCOL], char nona_space[NROW][NCOL], vector<Tool*>& tools);
};

class Bogeyman: public Monster{
public:
    Bogeyman(pair<int,int> loc):
    Monster(loc,randInt(5, 10),2, randInt(2,3),randInt(2,3),0,new ShortSword(loc),"Bogeyman"){};
    virtual void action(Actor* p, char ac_space[NROW][NCOL],char nona_space[NROW][NCOL], string& log);
    virtual void die(char ac_space[NROW][NCOL], char nona_space[NROW][NCOL], vector<Tool*>& tools);
//private:
};

class Dragon: public Monster{
public:
    Dragon(pair<int,int> loc):
    Monster(loc,randInt(20,25),4,4,4,0,new LongSword(loc),"Dragon"){
        m_max_hit = get_hit();
    };
    virtual void action(Actor* p, char ac_space[NROW][NCOL],char nona_space[NROW][NCOL], string& log);
    virtual void die(char ac_space[NROW][NCOL], char nona_space[NROW][NCOL], vector<Tool*>& tools);
private:
    int m_max_hit;
};


class Goblin: public Monster{
public:
    Goblin(pair<int,int> loc, int smell_dist):
    Monster(loc,randInt(15,20),1,3,1,0,new ShortSword(loc),"Goblin"),m_smell_dist(smell_dist){
        m_max_hit = get_hit();
    };
    virtual void action(Actor* p, char ac_space[NROW][NCOL],char nona_space[NROW][NCOL], string& log);
    virtual void die(char ac_space[NROW][NCOL], char nona_space[NROW][NCOL], vector<Tool*>& tools);
private:
    int m_max_hit,m_smell_dist;
};


vector<pair<int, int>> pathExists(char maze[NROW][NCOL], int sr, int sc, int er, int ec, int steps_limit);
#endif /* actor_h */
