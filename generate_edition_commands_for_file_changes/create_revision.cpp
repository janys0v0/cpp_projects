//#include "create_revision.h"
//#include <iostream>
//#include <sstream>  // for istringstream and ostringstream
//#include <string>
//#include <vector>
//#include <utility>
//
//using namespace std;
//
//void createRevision(istream& fold, istream& fnew, ostream& frevision){
//    size_t loc = 0;
//    Hash_table ht; // create hash table for storing
//    
//    vector<pair<string, size_t>> old_segments, new_segments;
////    cout<<"started"<<endl;
//    
//    
//    // Record 4-character segments in old file
//    while (true) {
//        string segment = read_four_chars(fold); // Read four character sequences into table
//        if (segment.empty()) { // Check if end of stream or incomplete read
//            break;
//        }
//        old_segments.push_back(make_pair(segment, loc));
//        loc++;
//        fold.seekg(loc); // reset location
//        
//    }
//    loc = 0;
//    while (true) {
//        string segment = read_four_chars(fnew); // Read four character sequences into table
//        if (segment.empty()) { // Check if end of stream or incomplete read
//            break;
//        }
//        new_segments.push_back(make_pair(segment, loc));
//        loc++;
//        fnew.seekg(loc); // reset location
//    }
//
//    
//    
//    // pair matches in old and new files ----------------
//    
//    for(int i=0; i<old_segments.size();i++){
//        for(int j=0; j<new_segments.size();j++){
//            if(old_segments[i].first == new_segments[j].first){
//                ht.add(old_segments[i].first, old_segments[i].second, 4, new_segments[j].second, 4); // add to hash table
//            }
//        }
//    }
//    
//    vector<pair<string, vector<int>>> copy_terms;
//    
//    // convert hash table results to string and sort ----------------
//    for(int i=0; i<ht.gettable()->size();i++){
//        for(int j=0; j<(*ht.gettable())[i].size();j++){
//            copy_terms.push_back((*ht.gettable())[i][j]);
//        }
//    }
//    sort(copy_terms.begin(),copy_terms.end(),compare_by_key);
//    
//
//    
//    for(auto item:copy_terms){
//        cout<<item.first<<" " <<item.second[0]<<" "<<item.second[1]<<" "<<item.second[2]<<" "<<item.second[3]<<endl;
//    }
//    cout<<"-----------------------------"<<endl;
//    
//    // while loop to combine overlapping terms and remove duplicate terms ----------------
//    bool changed = true;
//    while (changed){
//        changed = false;
//        for (int i = 0; i < copy_terms.size();i++) {
//            for (int j = 0; j < copy_terms.size(); ++j) {
//                if (i !=j) {
//                    int old_start_1 = copy_terms[i].second[0], old_length_1 = copy_terms[i].second[1],
//                    new_start_1 = copy_terms[i].second[2], new_length_1 = copy_terms[i].second[3],
//                    old_start_2 = copy_terms[j].second[0], old_length_2 = copy_terms[j].second[1],
//                    new_start_2 = copy_terms[j].second[2], new_length_2 = copy_terms[j].second[3];
//                    
//                    // Check for overlapping terms
//                    if ((old_start_1 + old_length_1 > old_start_2 &&
//                         old_start_1 + old_length_1 < old_start_2 + old_length_2)
//                        && (new_start_1 + new_length_1 > new_start_2 &&
//                            new_start_1 + new_length_1 < new_start_2 + new_length_2)){
//                        
//                        
//                        // Extend the length of the first term to cover the second term
//                        copy_terms[i].second[1] = old_start_2 + old_length_2 - old_start_1;
//                        copy_terms[i].second[3] = new_start_2 + new_length_2 - new_start_1;
//                        
//                        // Remove the second term as it is now merged with the first term
//                        copy_terms.erase(copy_terms.begin() + j);
//                        --j;
//                        if(j<i){--i;}
//                        changed = true;
//                        break;
//                    }
//                }
//            }
//        }
//    }
//    
//    
//    
//    changed = true;
//    while (changed){
//        changed = false;
////        cerr<<"iterated :::::::"<<endl;
//        for (int i = 0; i < copy_terms.size();i++) {
//            for (int j = 0; j < copy_terms.size(); ++j) {
//                if (i !=j) {
//                    int old_start_1 = copy_terms[i].second[0], old_length_1 = copy_terms[i].second[1],
//                    new_start_1 = copy_terms[i].second[2], new_length_1 = copy_terms[i].second[3],
//                    old_start_2 = copy_terms[j].second[0], old_length_2 = copy_terms[j].second[1],
//                    new_start_2 = copy_terms[j].second[2], new_length_2 = copy_terms[j].second[3];
//                    
//                    // Check for overlapping terms
//                    if (new_start_1 <= new_start_2 && new_start_1 + new_length_1 >= new_start_2 + new_length_2){
//                        // Remove the second term as it is now merged with the first term
//                        copy_terms.erase(copy_terms.begin() + j);
//                        --j;
//                        if(j<i){--i;}
//                        changed = true;
//                        break;
//                    }
//                }
//            }
//        }
//    }
//    
//    cout<<"-----------------------------"<<endl;
//    for(auto item:copy_terms){
//        cout<<item.first<<" " <<item.second[0]<<" "<<item.second[1]<<" "<<item.second[2]<<" "<<item.second[3]<<endl;
//    }
//    cout<<"-----------------------------"<<endl;
//    
////    for(auto item:copy_terms){
////        cout<<item.first<<" " <<item.second[0]<<" "<<item.second[1]<<" "<<item.second[2]<<" "<<item.second[3]<<endl;
////    }
//    
//    
////    // Translate into revision file -----------------------
////    for (int i =0; i< copy_terms.size();i++){
////        if(i<copy_terms.size()-1){
////            frevision<<'#'<<copy_terms[i].second[0]<<','<<copy_terms[i].second[1];
////            if (copy_terms[i+1].second[2] - (copy_terms[i].second[2]+copy_terms[i].second[3]) >= 1){
////                int offset = copy_terms[i].second[1]+copy_terms[i].second[2];
////                int len = copy_terms[i+1].second[1] -(copy_terms[i].second[1]+copy_terms[i].second[2]);
////                string chars =""; // store n characters from fnew into
////                char c = getcharacter(chars);
////                frevision << '+'<< c << chars << c;
////            }
////        }
////    }
////    int offset = copy_terms[copy_terms.size()-1].second[1]+copy_terms[copy_terms.size()-1].second[2];
////    // if there are more characters after the end, then write the rest of them in the +/ABC/ format
////
//  
//    // Translate into revision file -----------------------
//    for (int i = 0; i < copy_terms.size(); ++i) {
//        if (i < copy_terms.size() - 1) {
//            frevision << '#' << copy_terms[i].second[0] << ',' << copy_terms[i].second[1];
//            if (copy_terms[i + 1].second[2] - (copy_terms[i].second[2] + copy_terms[i].second[3]) >= 1) {
//                int offset = copy_terms[i].second[2] + copy_terms[i].second[3];
//                int len = copy_terms[i + 1].second[2] - offset;
//                
//                vector<char> buffer(len);
//                fnew.seekg(offset, ios::beg);
//                fnew.read(buffer.data(), len);
//
//                string chars(buffer.begin(), buffer.end());
//                if (!chars.empty()) {
//                    char c = getcharacter(chars);
//                    frevision << '+' << c << chars << c;
//                }
//            }
//        }
//    }
//
//    int offset = copy_terms.back().second[2] + copy_terms.back().second[3];
//
//    // If there are more characters after the end, then write the rest of them in the +/ABC/ format
//    fnew.seekg(0, ios::end);
//    int file_size = fnew.tellg();
//    if (offset < file_size) {
//        int remaining_len = file_size - offset;
//        vector<char> buffer(remaining_len);
//        fnew.seekg(offset, ios::beg);
//        fnew.read(buffer.data(), remaining_len);
//
//        string remaining_chars(buffer.begin(), buffer.end());
//        if (!remaining_chars.empty()) {
//            char c = getcharacter(remaining_chars);
//            frevision << '+' << c << remaining_chars << c;
//        }
//    }
//    cout<<"finished"<<endl;
//}
//
//string read_four_chars(istream& input) {
//    char buffer[5] = {0}; // create a buffer with space for 4 characters and a null terminator
//    input.read(buffer, 4); // read up to 4 characters
//    if(input.gcount()<4){
//        return "";
//    }
//    else
//        return string(buffer, input.gcount()); // create a string with the characters read
//}
//
//bool compare_by_key(const pair<string, vector<int>>& a, const pair<string, vector<int>>& b) {
//    return a.second[2] < b.second[2]; // compare start
//}
//
//char getcharacter(const string& chars) {
//    string all_chars = "./,[]'()?<>#!@#$%^&*qwertyuiopasdfghjkzxcvbnm";
//    char c;
//    for(int i=0;i<all_chars.size();i++){
//        c = all_chars[i];
//        if (chars.find(c) == string::npos) break;
//    }
//    return c;
//}
