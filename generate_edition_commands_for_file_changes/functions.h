//
//  revision.h
//  project_4
//
//  Created by Janys Li on 6/6/24.
//

#ifndef functions_h
#define functions_h
#include <iostream>
#include <sstream>  // for istringstream and ostringstream
#include <string>
#include <vector>
#include <utility>
#include <functional>
#include <list>

using namespace std;

class Hash_table{
public:
    Hash_table(int buckets):n_buckets(buckets), n_items(0){table = new vector<list<pair<string, int>>>(n_buckets);} // initialize with empty
    void add(const string& key, int value);
    vector<int> find(const string& key) const;
    ~Hash_table(){delete table;}
private:
    vector<list<pair<string, int>>>* table;
    int n_buckets,n_items;
};



bool revise(istream& fold, istream& frevision, ostream& fnew);
bool getInt(istream& inf, int& n);
bool getCommand(istream& inf, char& cmd, char& delim, int& length, int& offset);

void createRevision(istream& fold, istream& fnew, ostream& frevision);
char findDelimiter(const string& buffer);

#endif /* revision_h */
