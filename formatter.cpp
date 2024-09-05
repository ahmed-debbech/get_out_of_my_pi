#include <iostream>
#include <vector>

using namespace std;

typedef struct{
    string user;
    string ip;
    string when;
} Attempt;


std::vector<std::string> splitBySpace(const std::string& str) {
    std::vector<std::string> words;
    std::string word = "";

    for(int i =0; i<=str.size()-1; i++){
        if(str[i] != ' '){
            word += str[i];
        }else{
            words.push_back(word);
            word = "";
        }
    }
    vector<string> finalWords;
    for(string wor : words){
        if(wor != ""){
            finalWords.push_back(wor);
        }
    }
    return finalWords;
}


vector<Attempt> beautify(vector<string> messages){
    vector<Attempt> attempts;

    for(string msg : messages){
        vector<string> splitted = splitBySpace(msg);
        
        Attempt att;
        att.ip = splitted[2];
        att.user = splitted[0];
        att.when = splitted[4] + " " + splitted[5] + " " + splitted[6];

        attempts.push_back(att);
    }
    return attempts;
}