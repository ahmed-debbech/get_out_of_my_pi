#include <iostream>
#include <vector>
#include <sstream>
#include "formatter.cpp"

using namespace std;


vector<Attempt> parseLogs(string cursor, vector<string> logs){

    size_t startingLine;

    if(cursor == ""){
        startingLine = 0;
    }else{
        stringstream sstream(cursor);
        sstream >> startingLine;
    }

    cout << startingLine << endl;

    vector<string> messages;

    for(size_t i=startingLine; i<=logs.size()-1; i++){

        string line;
        vector<string> splitted = splitBySpace(logs[i]);

        for(int j=0; j<=splitted.size()-1; j++){
            line += splitted[j];
            line += " ";
        }
        messages.push_back(line);
    }

    vector<Attempt> attempts;
    if(messages.size() == 0) return attempts;

    attempts = beautify(messages); 

    return attempts;

}
