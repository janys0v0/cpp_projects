#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <iterator>
#include <cassert>
#include "functions.h"
using namespace std;

bool runtest(string oldName, string newName, string revisionName, string newName2)
{
    if (revisionName == oldName  ||  revisionName == newName  ||
        newName2 == oldName  ||  newName2 == revisionName  ||
            newName2 == newName)
    {
        cerr << "Files used for output must have names distinct from other files" << endl;
        return false;
    }
    ifstream oldFile(oldName, ios::binary);
    if (!oldFile)
    {
        cerr << "Cannot open " << oldName << endl;
        return false;
    }
    ifstream newFile(newName, ios::binary);
    if (!newFile)
    {
        cerr << "Cannot open " << newName << endl;
        return false;
    }
    ofstream revisionFile(revisionName, ios::binary);
    if (!revisionFile)
    {
        cerr << "Cannot create " << revisionName << endl;
        return false;
    }
    createRevision(oldFile, newFile, revisionFile);
    revisionFile.close();

    oldFile.clear();   // clear the end of file condition
    oldFile.seekg(0);  // reset back to beginning of the file
    ifstream revisionFile2(revisionName, ios::binary);
    if (!revisionFile2)
    {
        cerr << "Cannot read the " << revisionName << " that was just created!" << endl;
        return false;
    }
    ofstream newFile2(newName2, ios::binary);
    if (!newFile2)
    {
        cerr << "Cannot create " << newName2 << endl;
        return false;
    }
    assert(revise(oldFile, revisionFile2, newFile2));
    newFile2.close();

    newFile.clear();
    newFile.seekg(0);
    ifstream newFile3(newName2, ios::binary);
    if (!newFile)
    {
        cerr << "Cannot open " << newName2 << endl;
        return false;
    }
    if ( ! equal(istreambuf_iterator<char>(newFile), istreambuf_iterator<char>(),
                 istreambuf_iterator<char>(newFile3), istreambuf_iterator<char>()))
    {
        cerr << newName2 << " is not identical to " << newName
                 << "; test FAILED" << endl;
        return false;
    }
    return true;
}

void runtest(string oldtext, string newtext)
{
    istringstream oldFile(oldtext);
    istringstream newFile(newtext);
    ostringstream revisionFile;
    createRevision(oldFile, newFile, revisionFile);
    string result = revisionFile.str();
    cout << "The revision file length is " << result.size()
         << " and its text is " << endl;
    cout << result << endl;

    oldFile.clear();   // clear the end of file condition
    oldFile.seekg(0);  // reset back to beginning of the stream
    istringstream revisionFile2(result);
    ostringstream newFile2;
    assert(revise(oldFile, revisionFile2, newFile2));
    cout<<newtext<<endl;
    cout<<newFile2.str();
    assert(newtext == newFile2.str());
    
}

int main()
{
//    assert(runtest("greeneggs1.txt", "greeneggs2.txt", "newfile.txt", "newfile2.txt"));
//    assert(runtest("mallmart1.txt", "mallmart2.txt", "newfile.txt", "newfile2.txt"));
//    assert(runtest("warandpeace1.txt", "warandpeace2.txt", "newfile.txt", "newfile2.txt"));
    assert(runtest("strange1.txt", "strange2.txt", "newfile.txt", "newfile2.txt"));
    cerr << "Test PASSED" << endl;
    runtest("There's a bathroom on the right.",
            "There's a bad moon on the rise.");
    runtest("ABCDEFGHIJBLAHPQRSTUVPQRSTUV",
            "XYABCDEFGHIJBLETCHPQRSTUVPQRSTQQ/OK");
    runtest("ABCDEFGHIJBLAHPQRSTUVPQRSTUV",
            "ABCDEFGHIJBLAHPQRSTUVPQRSTUV");
    runtest("asdf","asdf");
    runtest("","");
    runtest("./#$[]@[]","./#$[]@[]");
    cout << "All tests passed" << endl;
}
//
//
//#include <iostream>
//#include <sstream>  // for istringstream and ostringstream
//#include <string>
//#include <cassert>
//#include "functions.h"
//using namespace std;
//

//
//int main()
//{
//    runtest("There's a bathroom on the right.",
//            "There's a bad moon on the rise.");
//    runtest("ABCDEFGHIJBLAHPQRSTUVPQRSTUV",
//            "XYABCDEFGHIJBLETCHPQRSTUVPQRSTQQ/OK");
//    cout << "All tests passed" << endl;
//}
