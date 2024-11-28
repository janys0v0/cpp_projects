//
//  actor.cpp
//  CS32_proj3
//
//  Created by Janys Li on 5/11/24.
//

#include "Actor.h"
#include "Game.h"


bool Actor::attack(Actor* other, string& log, char ac_space[NROW][NCOL], char nona_space[NROW][NCOL], vector<Tool*>& tools){
    log=""; // initialize log message
    bool sleeplog=false;
    
    // if weapon is magic fangs of sleep, put opponent into sleep with chance
    if(m_wpn->get_name() == "magic fangs of sleep"){
        if(trueWithProbability(0.2)){
            sleeplog = true;
            int sleeptime = randInt(2,6);
            // if already asleep, make sleep time max(x,y)
            if(m_sleep_t>0){
                other->set_sleep(max(m_sleep_t,sleeptime));
            }
            else{
                other->set_sleep(sleeptime);
            }
        }
    }
    
    // calculate regular damage points
    int attackerPoints = m_dex + m_wpn->get_dex_bon();
    int defenderPoints = other->get_dex() + other->get_arm();
    
    // execute regular attack
    if(randInt(1,attackerPoints) >= randInt(1, defenderPoints)){
        int damage_amount = randInt(0,m_str+m_wpn->get_damage()-1);
        other->set_hit(other->get_hit() - damage_amount);
        
        // if health <=0, actor dies
        if(other->get_hit()<=0){
            other->m_died = true;
            other->die(ac_space,nona_space,tools);
            log = m_name+" "+m_wpn->action_str()+" the "+other->get_name()+" dealing a final blow.\n";
            return true; // successfully hit
        }
        if(sleeplog){
            log = m_name+" "+m_wpn->action_str()+" at the "+other->get_name()+" and hits, putting " + other->get_name() +" to sleep.\n";
            return true; // put to sleep
        }
        else{
            log = m_name+" "+m_wpn->action_str()+" at the "+other->get_name()+" and hits.\n";
            return true; // successfully hit
        }
    }
    log = m_name+" "+m_wpn->action_str()+" at the "+other->get_name()+" and missed.\n";
    return false; // missed
    
}

void Actor::move_actor(int x, int y, char ac_space[NROW][NCOL]){
    // store and swap character locations on actor space
    char c = ac_space[getloc().first][getloc().second];
    ac_space[getloc().first][getloc().second]=' ';
    ac_space[x][y] = c;
    setloc(x, y);
}


void Monster::die(char ac_space[NROW][NCOL], char nona_space[NROW][NCOL], vector<Tool*>& tools){
    // remove from map
    int x = getloc().first;
    int y = getloc().second;
    ac_space[x][y] = ' ';
}

void Snakewoman::die(char ac_space[NROW][NCOL], char nona_space[NROW][NCOL], vector<Tool*>& tools) {
    // drop magic fangs with probability
    if (trueWithProbability(0.333)) {
        int x = getloc().first;
        int y = getloc().second;

        if (nona_space[x][y] == ' ') {
            nona_space[x][y] = ')';
            tools.push_back(new Fangs(make_pair(x, y)));
        }
    }

    // call base class die method
    Monster::die(ac_space, nona_space, tools);
}



void Snakewoman::action(Actor* p,char ac_space[NROW][NCOL], char nona_space[NROW][NCOL],string& log) {
    log="";
    int dx = p->getloc().first - getloc().first;
    int dy = p->getloc().second - getloc().second;
    int absDx = std::abs(dx);
    int absDy = std::abs(dy);
    int dist = absDx + absDy;

    // Attack if the player is adjacent
    if (absDx + absDy == 1) {
        vector<Tool*> v;
        attack(p,log,ac_space,nona_space, v);
    }
    // move closer if player is within a distance of 3
    else if (dist <= 3) {
        // Determine best move towards player
        int signX = (dx > 0) ? 1 : (dx < 0 ? -1 : 0);
        int signY = (dy > 0) ? 1 : (dy < 0 ? -1 : 0);
        int targetX = getloc().first + signX;
        int targetY = getloc().second + signY;

        // move if destination not blocked
        if (ac_space[targetX][getloc().second] == ' ') {  // try moving in x direction
            move_actor(targetX, getloc().second, ac_space);
        } else if (ac_space[getloc().first][targetY] == ' ') {  // try y direction
            move_actor(getloc().first, targetY, ac_space);
        }
        return;
    }
}


void Bogeyman::action(Actor* p,char ac_space[NROW][NCOL], char nona_space[NROW][NCOL],string& log) {
    log="";
    int dx = p->getloc().first - getloc().first;
    int dy = p->getloc().second - getloc().second;
    int absDx = std::abs(dx);
    int absDy = std::abs(dy);
    int dist = absDx + absDy;

    // attack if the player is adjacent
    if (absDx + absDy == 1) {
        vector<Tool*> v;
        attack(p,log,ac_space,nona_space, v);
    }
    // move closer if player is within a distance of 5
    else if (dist <= 5) {
        // Determine best move towards player
        int signX = (dx > 0) ? 1 : (dx < 0 ? -1 : 0);
        int signY = (dy > 0) ? 1 : (dy < 0 ? -1 : 0);
        int targetX = getloc().first + signX;
        int targetY = getloc().second + signY;

        // move if destination not blocked
        if (ac_space[targetX][getloc().second] == ' ') {  // try moving in x direction
            move_actor(targetX, getloc().second, ac_space);
        } else if (ac_space[getloc().first][targetY] == ' ') {  // try y direction
            move_actor(getloc().first, targetY, ac_space);
        }
        return;
    }
}

void Bogeyman::die(char ac_space[NROW][NCOL], char nona_space[NROW][NCOL], vector<Tool*>& tools){
    if(trueWithProbability(0.1)){
        // if empty space, drop magic axes
        if(nona_space[getloc().first][getloc().second] == ' '){
            nona_space[getloc().first][getloc().second] = ')';
            tools.push_back(new MagicAxes(pair<int,int>(getloc())));
        }
    }
    Monster::die(ac_space,nona_space,tools);
}

void Dragon::action(Actor* p,char ac_space[NROW][NCOL], char nona_space[NROW][NCOL],string& log) {
    log="";
    
    // before each turn, 1/10 chance of regain 1 hit point
    if(trueWithProbability(0.1)){
        if(get_hit() != m_max_hit){
            set_hit(get_hit()+1);
        }
    }
    
    int dx = p->getloc().first - getloc().first;
    int dy = p->getloc().second - getloc().second;
    int absDx = std::abs(dx);
    int absDy = std::abs(dy);

    // attack if the player is adjacent
    if (absDx + absDy == 1) {
        vector<Tool*> v;
        attack(p,log,ac_space,nona_space, v);
    }

}

void Dragon::die(char ac_space[NROW][NCOL], char nona_space[NROW][NCOL], vector<Tool*>& tools){
    
    // if empty space, drop scroll
    pair<int,int> loc = getloc();
    if(nona_space[loc.first][loc.second] == ' '){
        nona_space[loc.first][loc.second] = '?';
        
        // synthesize ranom scroll
        int i = randInt(5);
        switch(i){
            case 0:
                tools.push_back(new Scr_str(loc));
                break;
            case 1:
                tools.push_back(new Scr_arm(loc));
                break;
            case 2:
                tools.push_back(new Scr_dex(loc));
                break;
            case 3:
                tools.push_back(new Scr_hth(loc));
                break;
            case 4:
                tools.push_back(new Scr_tel(loc));
                break;
        }
    }
    Monster::die(ac_space,nona_space,tools);
}


void Goblin::action(Actor* p,char ac_space[NROW][NCOL], char nona_space[NROW][NCOL],string& log) {
    log="";
    // attack if the player is adjacent
    int dx = p->getloc().first - getloc().first;
    int dy = p->getloc().second - getloc().second;
    int absDx = std::abs(dx);
    int absDy = std::abs(dy);

    if (absDx + absDy == 1) {
        vector<Tool*> v;
        attack(p,log,ac_space,nona_space, v);
        return;
    }
    
    // if shortest route is within 15 steps, move toward player
        // make copy of original maze
    char temp_space[NROW][NCOL];
    for(int i=0;i<NROW;i++){
        for(int j=0;j<NCOL;j++){
            temp_space[i][j] = ac_space[i][j];
        }
    }
        // set start and end to empty cell to adapt with the pathExists function
    temp_space[p->getloc().first][p->getloc().second] = ' ';
    temp_space[getloc().first][getloc().second] = ' ';

    // find shortest path and store
    vector<pair<int, int>> path = pathExists(temp_space, p->getloc().first, p->getloc().second, getloc().first, getloc().second, m_smell_dist);
    
    if(path.size()>=2){
        pair<int, int> firstMove = path[1];
        move_actor(firstMove.first, firstMove.second, ac_space);
    } else {
        return;
    }
}

void Goblin::die(char ac_space[NROW][NCOL], char nona_space[NROW][NCOL], vector<Tool*>& tools){
    
    // if empty space, drop scroll
    pair<int,int> loc = getloc();
    if(trueWithProbability(0.333)){
        if(nona_space[loc.first][loc.second] == ' '){
            nona_space[loc.first][loc.second] = ')';
            
            // synthesize random weapon
            int i = randInt(2);
            switch(i){
                case 0:
                    tools.push_back(new MagicAxes(loc));
                    break;
                case 1:
                    tools.push_back(new Fangs(loc));
                    break;
            }
        }
    }
    Monster::die(ac_space,nona_space,tools);
}



vector<pair<int, int>> pathExists(char maze[NROW][NCOL], int sr, int sc, int er, int ec, int steps_limit) {
    // if absolute distance sum is greater than 15, return {} indicating no path exists
    if(abs(er-sr)+abs(ec-sc)>steps_limit){
        return {};
    }
    // if steps run out, return {} indicating no path exists
    if (steps_limit < 0 ) {
        return {};
    }
    // if blocked, return {} indicating no path exists
    if (maze[sr][sc]!=' ' ) {
        return {};
    }
    
    vector<pair<int, int>> path;
    
    // case reached player: return path of length 1
    if (sr == er && sc == ec) {
        path.push_back({sr, sc});
        return path;
    }
    
    // mark the location with 'o'
    maze[sr][sc] = 'o';
    
    // try going in all directions and store the paths
    vector<pair<int, int>> path1 = pathExists(maze, sr+1, sc, er, ec, steps_limit-1);
    vector<pair<int, int>> path2 = pathExists(maze, sr, sc+1, er, ec, steps_limit-1);
    vector<pair<int, int>> path3 = pathExists(maze, sr-1, sc, er, ec, steps_limit-1);
    vector<pair<int, int>> path4 = pathExists(maze, sr, sc-1, er, ec, steps_limit-1);
    vector<vector<pair<int, int>>> paths = {path1, path2, path3, path4};

    vector<pair<int, int>> shortestPath;
    int minLength = 16;
    
    // reset mark
    maze[sr][sc] = ' ';
    
    // if no path exists, return {}
    if(path1.size()==0 && path2.size()==0 && path3.size()==0 && path4.size()==0){return {};}
    
    // choose shortest path
    for (vector<pair<int, int>>& path : paths) {
        if (!path.empty() && path.size() < minLength) {
            minLength = path.size();
            shortestPath = path;
        }
    }
   
    // store and return shortest path
    shortestPath.push_back(pair<int,int>(sr,sc));
    return shortestPath;
}
