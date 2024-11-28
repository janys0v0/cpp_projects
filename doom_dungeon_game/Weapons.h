//
//  Weapons.h
//  CS32_proj3
//
//  Created by Janys Li on 5/12/24.
//

#ifndef Weapons_h
#define Weapons_h

const int NROW = 19, NCOL = 71; // one more than dimension for \0

#include <string>
#include <iostream>
#include "utilities.h"

using namespace std;
class Actor;
class Player;

class Tool{
public:
    Tool(string name,pair<int,int> loc,bool is_wpn):
    m_name(name),m_loc(loc),m_is_wpn(is_wpn) {}
    
    // get functions
    inline string get_name() const {return m_name;};
    inline pair<int,int> getloc() const {return m_loc;};
    inline bool is_wpn() const {return m_is_wpn;};
    
    virtual ~Tool(){}
    
private:
    pair<int,int> m_loc;
    string m_name;
    bool m_is_wpn;
};

// Weapons ---------------------------
class Weapon: public Tool{
public:
    Weapon(int dex_bon, int damage, string name,string actionstr, pair<int,int> loc):
    Tool(name,loc,true),m_dex_bon(dex_bon),m_damage(damage),m_actionstr(actionstr){}
    
    // get functions
    inline int get_dex_bon() const{return m_dex_bon;};
    inline int get_damage() const{return m_damage;};
    inline string action_str()const{return m_actionstr;};
private:
    pair<int,int> m_loc;
    int m_dex_bon, m_damage;
    string m_name,m_actionstr;
};

class Maces:public Weapon{
public:
    Maces(pair<int,int> loc):Weapon(0,2,"maces","slashes maces at",loc){};
};

class ShortSword:public Weapon{
public:
    ShortSword(pair<int,int> loc):Weapon(0,2,"short sword","slashes short sword at",loc){};
};


class LongSword:public Weapon{
public:
    LongSword(pair<int,int> loc):Weapon(2,4,"long sword","swings long sword at",loc){};
};

class MagicAxes:public Weapon{
public:
    MagicAxes(pair<int,int> loc):Weapon(5,5,"magic axe","chops magic axes at",loc){};
};

class Fangs:public Weapon{
public:
    Fangs(pair<int,int> loc):Weapon(3,2,"magic fangs of sleep","strikes magic fangs of sleep at",loc){};
};


// Scrolls ---------------------------
class Scroll: public Tool {
public:
    inline Scroll(string name, pair<int,int> loc):
    Tool(name,loc,false) {}
    virtual void read(Player* p,char ac_space[NROW][NCOL],string& log) =0; // virtual function for read scroll
private:
    pair<int,int> m_loc;
    string m_name;
};

class Scr_tel: public Scroll{
public:
    Scr_tel(pair<int,int> loc):
    Scroll("scroll of teleportation", loc){}
    virtual void read(Player *p,char ac_space[NROW][NCOL],string& log);
};

class Scr_arm: public Scroll{
public:
    Scr_arm(pair<int,int> loc):
    Scroll("scroll of enhance armor", loc){}
    virtual void read(Player *p,char ac_space[NROW][NCOL],string& log);
};

class Scr_str: public Scroll{
public:
    Scr_str(pair<int,int> loc):
    Scroll("scroll of strength", loc){}
    virtual void read(Player *p,char ac_space[NROW][NCOL],string& log);
};

class Scr_hth: public Scroll{
public:
    Scr_hth(pair<int,int> loc):
    Scroll("scroll of health", loc){}
    virtual void read(Player *p,char ac_space[NROW][NCOL],string& log);
};

class Scr_dex: public Scroll{
public:
    Scr_dex(pair<int,int> loc):
    Scroll("scroll of dexterity", loc){}
    virtual void read(Player *p,char ac_space[NROW][NCOL],string& log);
};


#endif /* Weapons_h */
