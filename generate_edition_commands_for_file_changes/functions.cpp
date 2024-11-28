#include "functions.h"

void Hash_table::add(const string& key, int value) {
    // Compute hash bucket index
    size_t hash_index = hash<string>{}(key) % n_buckets;
    // insert item
    (*table)[hash_index].push_back(make_pair(key, value));
    ++n_items;
}

vector<int> Hash_table::find(const string& key) const {
    size_t hashValue = hash<string>{}(key) % table->size();
    vector<int> matches;
    const list<pair<string, int>>& bucket = (*table)[hashValue];
    
    // loop through bucket to find matches
    for (list<pair<string, int>>::const_iterator it = bucket.begin(); it != bucket.end(); ++it) {
        // if matched, append to matches
        if (it->first == key) {
            matches.push_back(it->second);
        }
    }
    return matches;
}


bool revise(istream& fold, istream& frevision, ostream& fnew){
    // while condition to extract characters until there is no more
    char cmd, delim;
    int offset = 0, length = 0;
    
    // get bounds
    fold.seekg(0, ios::end);
    streampos fold_length = fold.tellg();
    fold.seekg(0, ios::beg);
    
    // get length of frevision
    frevision.seekg(0, ios::end);
    streampos frev_length = frevision.tellg();
    frevision.seekg(0, ios::beg);
    
    if(frev_length == 0) return true; // do nothing
    
    while(frevision.get(cmd)  &&  isascii(cmd)){
        frevision.unget();
        
        // get command
        if(getCommand(frevision, cmd, delim, length, offset)){
            if(cmd=='+'){
                // keep getting characters until encountering delimiter
                while(frevision.get(cmd)  &&  isascii(cmd) && cmd != delim){
                    fnew.put(cmd);
                }
                if (cmd!= delim) return false;
            }
            else if(cmd == '#'){
                if (offset > fold_length) { // out of bounds
                    return false;
                }
                fold.seekg(0); // reset position
                fold.ignore(offset);
                for (int i=0;i<length;i++){
                    if(fold.get(cmd)  &&  isascii(cmd))
                        fnew.put(cmd);
                    else
                        return false;
                }
            }
        }
        else{
            return false;
        }
    }
    return true; // finished with no error
}


bool getInt(istream& inf, int& n)
{
    char ch;
    if (!inf.get(ch)  ||  !isascii(ch)  ||  !isdigit(ch))
        return false;
    inf.unget();
    inf >> n;
    return true;
}

bool getCommand(istream& inf, char& cmd, char& delim, int& length, int& offset)
{
    if (!inf.get(cmd))
    {
        cmd = 'x';  // signals end of file
        return true;
    }
    switch (cmd)
    {
      case '+':
        return inf.get(delim).good();
      case '#':
        {
            char ch;
            return getInt(inf, offset) && inf.get(ch) && ch == ',' && getInt(inf, length);
        }
      case '\r':
      case '\n':
        return true;
    }
    return false;
}


void createRevision(istream& fold, istream& fnew, ostream& frevision) {
    
    // convert to string for easier extraction
    string oldContent((istreambuf_iterator<char>(fold)), istreambuf_iterator<char>());
    string newContent((istreambuf_iterator<char>(fnew)), istreambuf_iterator<char>());

    // through experimenting, 8 is optimal segment size for starting to use #n,n vs +/nnnn/
    const int SEGMENT_SIZE = 8;
    string additionBuffer;
    Hash_table segmentTable(oldContent.size() / SEGMENT_SIZE);

    if(oldContent.size()>=SEGMENT_SIZE && newContent.size() >= SEGMENT_SIZE){ // don't go out of bounds for short files
        // insert all length-4 segments into hash table
        for (size_t pos = 0; pos <= oldContent.size() - SEGMENT_SIZE; pos++) {
            string segment = oldContent.substr(pos, SEGMENT_SIZE);
            segmentTable.add(segment, pos);
        }
    }
    int newPos = 0;
    // iterate through the new content to find longest matches
    while (newPos < newContent.size()) {
        int longestMatch = 0;
        int matchPosition = -1; // for storing best match
        
        if (newPos <= int(newContent.size()) - SEGMENT_SIZE) {
            vector<int> potentialMatches = segmentTable.find(newContent.substr(newPos, SEGMENT_SIZE)); // get all the potential matches

            // loop through potential matches
            for (size_t k = 0; k < potentialMatches.size(); k++) {
                int oldPos = potentialMatches[k];
                int matchLength = SEGMENT_SIZE;
                
                // loop to find longest match
                while ((newPos + matchLength < newContent.size() // does not exceed new file size
                       && oldPos + matchLength < oldContent.size()) // does not exceed old file size
                       && newContent[newPos + matchLength] == oldContent[oldPos + matchLength]) // contents match
                {
                    matchLength+= 1;
                }
                if (matchLength > longestMatch) {
                    longestMatch = matchLength;
                    matchPosition = oldPos;
                }
            }
        }

        // writing commands
        char c= findDelimiter(additionBuffer);
        // if matched, write # command
        if (longestMatch >= SEGMENT_SIZE) {
            // add this from last iteration
            if (!additionBuffer.empty()) {
                frevision << "+"<< c << additionBuffer << c;
                additionBuffer.clear();
            }
            frevision << "#" << matchPosition << "," << longestMatch;
            newPos += longestMatch;
        } else {
            // if not matched, store + command
            additionBuffer += newContent[newPos];
            ++newPos;
        }
    }

    char c= findDelimiter(additionBuffer);
    // Write any remaining characters in the addition buffer
    if (!additionBuffer.empty()) {
        frevision << "+" <<c<< additionBuffer << c;
    }
    
    
}


char findDelimiter(const string& buffer) {
    string all_delimiters = "./,[]'()?<>#!@~:;\{}#$%^&*qwertyuiopasdfghjkzxcvbnmQWERTYUIOPASDFGHJKLZXCVBNM`";
    
    for (char c : all_delimiters) {
        if (buffer.find(c) == string::npos) {
            return c; // Return the first character not found in the buffer
        }
    }
    return '\0'; // Return null character if no delimiter is found (unlikely case)
}
