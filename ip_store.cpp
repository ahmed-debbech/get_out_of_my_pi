#include <iostream>
#include <unordered_map>
#include <fstream>
#include <vector>

using namespace std;


bool doesFileExist(const std::string& filename) {
    std::ifstream file(filename, std::ios::binary | std::ios::ate);
    if (!file) {
        return true; // File does not exist or error opening the file
    }

    std::streamsize size = file.tellg();
    return size <= 0; // File is empty
}

std::vector<std::string> splitBySpecefic(const std::string& str) {
    std::vector<std::string> words;
    std::string word = "";

    for(int i =0; i<=str.size()-1; i++){
        if(((str[i] >= '0') && (str[i] <= '9')) || (str[i] == '.')){
            word += str[i];
        }else{
            words.push_back(word);
            word = "";
        }
    }
    words.push_back(word);
    return words;
}

void createStore(){
    std::ofstream outfile("ips");
    if (!outfile) {
        std::cout << "error creating ip store file" << std::endl;
        throw -1;
    }
    outfile << "";
    outfile.close();
}

unordered_map<string, int> readStoreFile(){
    std::ifstream file("ips");

    if (!file) {
        std::cout << "could not open file of ip store" << std::endl;
        return unordered_map<string, int>();
    }
    unordered_map<string, int> lines;
    std::string line;
    while (std::getline(file, line)) {
        vector<string> splitted = splitBySpecefic(line);
        lines[splitted[0]] = stoi(splitted[1]);
    }

    file.close();
    return lines;
}

unordered_map<string, int> getIpStore(){
    unordered_map<string, int> map;
    if(doesFileExist("ips")){
        try{
            createStore();
        }catch(int i){
            return map;
        }
    }else{
        return readStoreFile();
    }
    return map;
}

void writeToStoreFile(unordered_map<string, int> map){
    std::ofstream outfile("ips");
    if (!outfile) {
        std::cout << "error writing to ip store file" << std::endl;
        throw -1;
    }
    for (auto it : map)
        outfile << it.first << " " << it.second << endl;

    outfile.close();
}

unordered_map<string, int> updateStore(vector<Attempt> atempts, unordered_map<string, int> map){

    for(Attempt att : atempts){
        if(map.find(att.ip) != map.end()){
            map[att.ip] = map[att.ip] + 1;
        }else{
            map[att.ip] = 1;
        }
    }
    writeToStoreFile(map);
    return map;
}

