#include <cstdlib>
#include <string>
#include <iostream>
#include <fstream>
#include <vector>

int executeShell(std::string cmd){
    int res = system(cmd.c_str());
    if(res == -1){
        std::cout << "error occured in executeing bash command " << std::endl;
        return -1;
    }
    return 0;
}

std::vector<std::string> getContent(){
    
    std::ifstream file("logs");

    if (!file) {
        std::cerr << "could not open file of result log" << std::endl;
        return std::vector<std::string>();
    }

    std::vector<std::string> lines;
    std::string line;
    while (std::getline(file, line)) {
        lines.push_back(line);
    }

    file.close();

    return lines;
}

std::vector<std::string> getLogs(){

    if(executeShell("lastb | tac | sed 1,2d > logs") != 0) throw -1;

    std::vector<std::string> log = getContent();

    if(log.size() == 0) throw -1;
    //if(log[0] == "") throw -1;

    return log;
}

void systemBan(const std::string ipToBan){
    executeShell("iptables -I INPUT -s "+ipToBan+" -j DROP");
}