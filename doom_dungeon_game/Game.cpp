// Game.cpp

#include "Game.h"
using namespace std;

// Implement these and other Game member functions you may have added.


void Game::play()
{
    // Initialize reusable variables
    pair<int,int> loc;
    bool nextlevel;
    int num_mons=0, num_bog=0, num_gob=0, num_drag=0, num_snake=0; // initialize number of monsters to be 0
    
    vector<Monster*> monsters;
    vector<Tool*> tools;
    vector<Tool*> inventory;
    
    Player p(pair<int,int>(0,0));
    
    // Loop for levels
    for(int level=0;level<5;level++){
        
        // Create new room
        generate_room();
        nextlevel = false;
        
        // SYNTHESIZE PLAYER -------------------------------
        loc = random_pickloc(get_ac_space());
        p.setloc(loc.first,loc.second);
        m_ac_space[loc.first][loc.second] = '@';
        
        
        // SYNTHESIZE MONSTERS -------------------------------
        
            // delete dynamically allocated objects
        for(Monster*& mon:monsters){
            if(mon){
                delete mon;
                mon = nullptr;
            }
        }
        monsters.clear();
        
        // Generate random number of monsters
        num_mons = randInt(2, 5*(level+1)+1);
        
        // CASE LV 3+: Dragons
        if(level>=3){
            num_drag = randInt(2,num_mons/4);
            for (int mon = 0; mon < num_drag; mon++) {
                loc = random_pickloc(get_ac_space());
                
                // Place the monster at the generated location
                monsters.push_back(new Dragon(loc));
                m_ac_space[loc.first][loc.second] = 'D';
            }
        }
        
        // CASE LV 2+: BOGEYMAN
        if(level>=2){
            num_bog = randInt(2,(num_mons-num_drag)/2);
            for (int mon = 0; mon < num_bog; mon++) {
                loc = random_pickloc(get_ac_space());
                
                // Place the monster at the generated location
                monsters.push_back(new Bogeyman(loc));
                m_ac_space[loc.first][loc.second] = 'B';
            }
        }
        
        
        // ALL: Goblins
        num_gob = randInt(0,2);
        for (int mon = 0; mon < num_gob; mon++) {
            loc = random_pickloc(get_ac_space());
            
            // Place the monster at the generated location
            monsters.push_back(new Goblin(loc,m_goblinSmellDistance));
            m_ac_space[loc.first][loc.second] = 'G';
        }
        
        
        // ALL LEVELS: Snakewoman
        num_snake = num_mons - num_bog - num_drag - num_gob;
        for (int mon = 0; mon < num_snake; mon++) {
            loc = random_pickloc(get_ac_space());
            
            // Place the monster at the generated location
            monsters.push_back(new Snakewoman(loc));
            m_ac_space[loc.first][loc.second] = 'S';
        }
        
        // SYNTHESIZE OBJECTS -----------------------------
        // Case Level 0~3: Synthesize stairway
        if(level <= 3){
            loc = random_pickloc(get_nona_space());
            m_nona_space[loc.first][loc.second] = '>';
        }
        
        // Case Level 4: golden idol on the final floor
        if(level == 4){
            // Synthesize golden idol
            loc = random_pickloc(get_nona_space());
            m_nona_space[loc.first][loc.second] = '&';
        }
        
        // Synthesize 2~3 Random scrolls or weapons
        
        // clear tools and inventory before generating each level
        for(Tool*& tol:tools){
            if(tol){
                delete tol;
                tol = nullptr;
            }
        }
        tools.clear();
        
        
        // Synthesize Weapons
        // Generate random number of objects
        int num_objs = randInt(2,3);
        
        for (int tl = 0; tl < num_objs; tl++) {
            
            // Place the weapon or scroll at a randomly generated location
            loc = random_pickloc(get_nona_space());
            
            // randomly synthesize weapon or scroll
            int tool = randInt(7);
            switch(tool){
                case 0:
                    tools.push_back(new Maces(loc));
                    break;
                case 1:
                    tools.push_back(new ShortSword(loc));
                    break;
                case 2:
                    tools.push_back(new LongSword(loc));
                    break;
                case 3:
                    tools.push_back(new Scr_str(loc));
                    break;
                case 4:
                    tools.push_back(new Scr_arm(loc));
                    break;
                case 5:
                    tools.push_back(new Scr_dex(loc));
                    break;
                case 6:
                    tools.push_back(new Scr_hth(loc));
                    break;
            }
            
            // mark the spot with the generated weaopn or scroll
            if(tool<3){
                m_nona_space[loc.first][loc.second] = ')';
            }
            else{
                m_nona_space[loc.first][loc.second] = '?';
            }
        }
        
        
        // check key input
        char c;
        
        // initial display
        clearScreen();
        display();
        cout<<"Level: "<<level<<", Hit points: "<<p.get_hit()<<", Armor: "<<p.get_arm()<<", Strength: "<<p.get_str()<<", Dexterity: "<<p.get_dex()<<endl<<endl;
        
        // initialize variables
        vector<string> logs;
        string log;
        inventory.push_back(new ShortSword(p.getloc()));
        Monster* m;
        int x,y,i,new_y, new_x;
        char letter;
        string disp[NROW][NCOL]; // for display
        
        // STARTED GAME LOOP ------------------------------------------------
        while(true){ // loop until broken by winning or quitting conditions
            
            // reset parameters
            logs.clear();
            log = "";
            
            // Before each round, 1 in 10 chance for the player to regain 1 hit point
            if(trueWithProbability(0.1)){
                if(p.get_hit() < p.get_max_health()){
                    p.set_hit(p.get_hit()+1);
                }
            }
            
            // PLAYER ACTION ---------------------------
            c = getCharacter();
            
            // case q: quit game
            if(c=='q'){
                
                // delete dynamically allocated objects
                for(Monster*& mon:monsters){
                    if(mon){
                        delete mon;
                        mon = nullptr;
                    }
                }
                for(Tool*& tol:tools){
                    if(tol){
                        delete tol;
                        tol = nullptr;
                    }
                }
                for (Tool*& inv : inventory) {
                    if (inv) {
                        delete inv;
                        inv = nullptr;
                    }
                }
                return; // break loop and quit game
            }
            
            // if asleep, player cannot move
            if(p.get_sleep()==0){
                
                switch(c) {
                        // ACTION: Move around and hit monster
                    case ARROW_RIGHT:
                    case ARROW_LEFT:
                    case ARROW_UP:
                    case ARROW_DOWN: {
                            // calculate new coordinates
                        int dx = 0, dy = 0;
                        if (c == ARROW_RIGHT) dy = 1;
                        else if (c == ARROW_LEFT) dy = -1;
                        else if (c == ARROW_UP) dx = -1;
                        else if (c == ARROW_DOWN) dx = 1;
                        
                        new_x = p.getloc().first + dx;
                        new_y = p.getloc().second + dy;
                        
                            // check if the destination is wall
                        if (m_ac_space[new_x][new_y] == '#') {
                            break;
                        }
                        
                        // ACTION: attack monster at the new location
                        
                        // encountered monster
                        if (m_ac_space[new_x][new_y] == 'S' ||
                            m_ac_space[new_x][new_y] == 'B' ||
                            m_ac_space[new_x][new_y] == 'D' ||
                            m_ac_space[new_x][new_y] == 'G') {
                            m = nullptr;
                            if (find_target(monsters, new_x, new_y, m) && m) {
                                log = "";
                                // attack and log the result
                                p.attack(m, log, m_ac_space,m_nona_space, tools);
                                
                                if(log.size()!=0){
                                    logs.push_back(log);
                                }
                                
                                // If monster is dead, remove it from the map and delete it
                                if (m->is_dead()) {
                                    m_ac_space[m->getloc().first][m->getloc().second] = ' ';
                                }
                            }
                        } else {
                            // move the player to the new location if it's empty
                            p.move_actor(new_x, new_y, m_ac_space);
                        }
                        break;
                    }
                        
                        // ACTION: pick up item
                    case 'g':
                        x = p.getloc().first;
                        y = p.getloc().second;
                        
                        // case winning
                        if(m_nona_space[x][y]=='&'){
                            cout<<"You pick up the golden idol\nCongratulations, you won!"<<endl;
                            
                            // delete dynamically allocated objects
                            for(Monster*& mon:monsters){
                                if(mon){
                                    delete mon;
                                    mon = nullptr;
                                }
                            }
                            for(Tool*& tol:tools){
                                if(tol){
                                    delete tol;
                                    tol = nullptr;
                                }
                            }
                            for (Tool*& inv : inventory) {
                                if (inv) {
                                    delete inv;
                                    inv = nullptr;
                                }
                            }
                            return;
                        }
                        
                        // case weapon
                        else if (m_nona_space[x][y] == ')') {
                            Tool* w = nullptr;
                            if (find_target(tools, x, y, w)) {
                                logs.push_back("You picked up " + w->get_name() + "\n");
                                inventory.push_back(w);
                                
                                // find and remove the tool from tools
                                auto it = std::find(tools.begin(), tools.end(), w);
                                if (it != tools.end()) {
                                    tools.erase(it);
                                }
                                for(Tool* tol:tools){
                                    cout<<tol->get_name()<<endl;
                                }
                                
                                m_nona_space[x][y] = ' ';
                            }
                            break;
                        }
                        
                        // case scroll
                        else if (m_nona_space[x][y] == '?') {
                            
                            Tool* w = nullptr;
                            if (find_target(tools, x, y, w)) {
                                logs.push_back("You picked up a scroll called " + w->get_name() + "\n");
                                inventory.push_back(w);
                                
                                // find and remove the scroll from tools
                                auto it = std::find(tools.begin(), tools.end(), w);
                                if (it != tools.end()) {
                                    tools.erase(it);
                                }
                                m_nona_space[x][y] = ' ';
                            }
                        }
                        break;
                        
                        // ACTION: Descend deeper
                    case '>':
                        x = p.getloc().first;
                        y = p.getloc().second;
                        // case next level
                        if(m_nona_space[x][y]=='>'){
                            nextlevel = true;
                        }
                        break;
                        
                        // ACTION: Wield a weapon
                    case 'w':
                        clearScreen();
                        cout<<"Inventory:\n";
                        for(i=0;i<inventory.size();i++){
                            letter = 'a'+i;
                            if(inventory[i]->get_name().find("scroll")==0){
                                cout<<string(1,letter)+". A scroll called "+inventory[i]->get_name()+"\n";
                            } else{
                                cout<<string(1,letter)+". "+inventory[i]->get_name()+"\n";
                            }
                        }
                        
                        // if selected letter is within inventory
                        c = getCharacter();
                        if(c - 'a'>=0 && c - 'a' <= inventory.size()){
                            
                            // set weapon to the selected weapon
                            string nm = (inventory[c-'a'])->get_name();
                            if (nm == "maces") {
                                p.destr_wpn();
                                p.set_wpn(new Maces(pair<int,int>(0,0)));
                                logs.push_back("You are now wielding "+p.get_wpn()->get_name()+"\n");
                            } else if (nm == "short sword") {
                                p.destr_wpn();
                                p.set_wpn(new ShortSword(pair<int,int>(0,0)));
                                logs.push_back("You are now wielding "+p.get_wpn()->get_name()+"\n");
                            } else if (nm == "long sword") {
                                p.destr_wpn();
                                p.set_wpn(new LongSword(pair<int,int>(0,0)));
                                logs.push_back("You are now wielding "+p.get_wpn()->get_name()+"\n");
                            } else if (nm == "magic axe") {
                                p.destr_wpn();
                                p.set_wpn(new MagicAxes(pair<int,int>(0,0)));
                                logs.push_back("You are now wielding "+p.get_wpn()->get_name()+"\n");
                            } else if (nm == "magic fangs of sleep") {
                                p.destr_wpn();
                                p.set_wpn(new Fangs(pair<int,int>(0,0)));
                                logs.push_back("You are now wielding "+p.get_wpn()->get_name()+"\n");
                            } else{
                                logs.push_back("You can't wield "+nm+"\n");
                            }
                        }
                        break;
                        
                        // ACTION: Read Scroll
                    case 'r':
                    {
                        // Print inventory
                        clearScreen();
                        cout << "Inventory:\n";
                        for (i = 0; i < inventory.size(); i++) {
                            char letter = 'a' + i;
                            if (inventory[i]->get_name().find("scroll") == 0) {
                                cout << string(1, letter) + ". A scroll called " + inventory[i]->get_name() + "\n";
                            } else {
                                cout << string(1, letter) + ". " + inventory[i]->get_name() + "\n";
                            }
                        }
                        
                        
                        // if selected letter is within inventory
                        char c = getCharacter();
                        int index = c - 'a';
                        if (index >= 0 && index < inventory.size()) {
                            // set tool to the selected tool
                            Tool* tool = inventory[index];
                            string nm = tool->get_name();
                            bool validScroll = true;
                            
                            if (nm == "scroll of teleportation") {
                                Scr_tel scr(pair<int, int>(0, 0));
                                scr.read(&p, m_ac_space, log);
                            } else if (nm == "scroll of enhance armor") {
                                Scr_arm scr(pair<int, int>(0, 0));
                                scr.read(&p, m_ac_space, log);
                            } else if (nm == "scroll of health") {
                                Scr_hth scr(pair<int, int>(0, 0));
                                scr.read(&p, m_ac_space, log);
                            } else if (nm == "scroll of dexterity") {
                                Scr_dex scr(pair<int, int>(0, 0));
                                scr.read(&p, m_ac_space, log);
                            } else if (nm == "scroll of strength") {
                                Scr_str scr(pair<int, int>(0, 0));
                                scr.read(&p, m_ac_space, log);
                            } else {
                                logs.push_back("You can't read " + nm + "\n");
                                validScroll = false;
                            }
                            
                            // delete read scroll from inventory
                            if (validScroll) {
                                logs.push_back(log);
                                delete inventory[index];
                                inventory.erase(inventory.begin() + index);
                            }
                        }
                        break;
                    }



                        
                        // ACTION: See inventory
                    case 'i':
                        clearScreen();
                        cout<<"Inventory:\n";
                        for(i=0;i<inventory.size();i++){
                            letter = 'a'+i;
                            cout<<string(1,letter)+". "+inventory[i]->get_name()+"\n";
                        }
                        c = getCharacter();
                        break;
                        
                        // ACTION: Cheat
                    case 'c':
                        p.set_str(9);
                        p.set_max_health(50);
                        p.set_hit(50);
                }
            }
            // if asleep
            else{
                p.set_sleep(p.get_sleep()-1);
            }
            
            
            // MONSTERS MOVE ----------------------------
            for (size_t i = 0; i < monsters.size();) {
                Monster* m = monsters[i];
                
                if (!m->is_dead()) {
                    string log = "";
                    
                    // Monster can only act if not asleep
                    if (m->get_sleep() == 0) {
                        m->action(&p, m_ac_space, m_nona_space, log);
                    } else {
                        m->set_sleep(m->get_sleep() - 1);
                    }
                    
                    // Add log
                    if (log.size() != 0) {
                        logs.push_back(log);
                    }
                    // only increment if not removing the monster
                    ++i;
                } else {
                    // If monster is dead, remove it from the map and delete it
                    m_ac_space[m->getloc().first][m->getloc().second] = ' ';
                    delete m;
                    monsters.erase(monsters.begin() + i);
                }
            }
            
            // DISPLAY: print map and outputs after each round
            display();
            cout<<"Level: "<<level<<", Hit points: "<<p.get_hit()<<", Armor: "<<p.get_arm()<<", Strength: "<<p.get_str()<<", Dexterity: "<<p.get_dex()<<endl<<endl;
            
            // print stored messages
            for(string log:logs){
                cout<<log;
            }
            
            
            
            
            // IF PLAYER DIED: quit
            if(p.is_dead()){
                // delete dynamically allocated objects
                for(Monster*& mon:monsters){
                    if(mon){
                        delete mon;
                        mon = nullptr;
                    }
                }
                for(Tool*& tol:tools){
                    if(tol){
                        delete tol;
                        tol = nullptr;
                    }
                }
                int i =0;
                for(Tool*& tol:inventory){
                    if(tol){
                        delete tol;
                        tol = nullptr;
                        i++;
                    }
                }
                return;
            }
            
            // Pass to next level if stepped on stairway
            if(nextlevel){
                break;
            }
        }
    }
}

void Game::display() const{
    clearScreen();
    
    // update display from actor and non-actor spaces
    for(int i=0;i<NROW;i++){
        for(int j=0;j<NCOL;j++){
            if(m_ac_space[i][j] != ' ' && m_nona_space[i][j] != m_ac_space[i][j]){ //if overlap, display actor
                cout<<m_ac_space[i][j];
            }
            else{
                cout<<m_nona_space[i][j];
            }
        }
        cout<<endl;
    }
}

void Game::generate_room() {
    m_space.clear();
    // generate default = fill with #
    for (int i = 0; i < NROW; i++) {
        for (int j = 0; j < NCOL; j++) {
            m_ac_space[i][j] = '#';
            m_nona_space[i][j] = '#';
        }
    }
    
    // generate random number of rooms
    int num_rooms = randInt(4, 5);
    
    // store room center coords and room dims
    vector<pair<int, int>> room_centers;
    vector<pair<pair<int, int>, pair<int, int>>> room_dimensions; // store corners to check if room separate
    
    for (int i = 0; i < num_rooms; i++) {
        int room_w, room_h, room_y, room_x;
        bool room_nottaken;
        do {
            room_nottaken = true;
            // generate room coords and dims
            room_w = randInt(9, 13);
            room_h = randInt(5, 9);
            room_y = randInt(1, NCOL - room_w - 2); // prevent rooms from touching the periphery
            room_x = randInt(1, NROW - room_h - 2);
            
            // check separation from all other rooms
            for (size_t i = 0; i < room_dimensions.size(); ++i) {
                pair<pair<int, int>, pair<int, int>>& dim = room_dimensions[i];
                int existing_x = dim.first.first;
                int existing_y = dim.first.second;
                int existing_w = dim.second.first;
                int existing_h = dim.second.second;
                
                if (!(room_y + room_w < existing_y - 1 || // left of the existing room
                      room_y > existing_y + existing_w + 1 || // right of the existing room
                      room_x + room_h < existing_x - 1 || // above the existing room
                      room_x > existing_x + existing_h + 1)) { // below the existing room
                    room_nottaken = false;
                    break;
                }
            }
        } while (!room_nottaken);
        
        // store room dimensions
        room_dimensions.push_back({{room_x, room_y}, {room_h, room_w}});
        room_centers.push_back({room_x + room_h / 2, room_y + room_w / 2});
        
        for (int x = room_x; x < room_x + room_h; x++) {
            for (int y = room_y; y < room_y + room_w; y++) {
                m_ac_space[x][y] = ' ';
                m_nona_space[x][y] = ' ';
                m_space.push_back(pair<int,int>(x, y));
            }
        }
    }
    
//     connect rooms with paths
    for (size_t i = 0; i < room_centers.size() - 1; i++) {
        carve_path(room_centers[i].first, room_centers[i].second, room_centers[i+1].first, room_centers[i+1].second);
    }
}

void Game::carve_path(int x1, int y1, int x2, int y2) {
    // carve a horizontal path
    int x = x1;
    while (x != x2) {
        m_ac_space[x][y1] = ' ';
        m_nona_space[x][y1] = ' ';
        m_space.push_back(pair<int,int>(x, y1));
        x += (x2 > x1) ? 1 : -1;
    }
    // carve a vertical path
    int y = y1;
    while (y != y2) {
        m_ac_space[x2][y] = ' ';
        m_nona_space[x2][y] = ' ';
        m_space.push_back(pair<int,int>(x2, y));
        y += (y2 > y1) ? 1 : -1;
    }
}



pair<int,int> random_pickloc(char space[NROW][NCOL]) {
    
    int x,y;
    do{
        x = randInt(0, NROW-1);
        y = randInt(0, NCOL-1);
    }while(space[x][y]!=' ');
    
    return pair<int,int>(x,y);
}






// You will presumably add to this project other .h/.cpp files for the various
// classes of objects you need to play the game:  player, monsters, weapons, 
// etc.  You might have a separate .h/.cpp pair for each class (e.g., Player.h,
// Bogeyman.h, etc.), or you might put the class declarations for all actors in
// Actor.h, all game objects in GameObject.h, etc.



// helper function
void printloc(pair<int,int> loc){
    cerr<<"("<<loc.first<<","<<loc.second<<")"<<endl;
}



