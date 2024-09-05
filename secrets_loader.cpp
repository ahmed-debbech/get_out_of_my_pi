#include "secrets_loader.h"

using namespace std;

/*
*  This function extracts all secrets in the file called 'secrets' it take every twi lines and converts them to a key pair value in a map
*/
Secrets load_secrets(){

    std::ifstream file("secrets");

    Secrets secret;
    if (!file) {
        std::cout << "could not open file of secrets" << std::endl;
        return secret;
    }

    std::vector<std::string> lines;
    std::string line;
    while (std::getline(file, line)) {
        lines.push_back(line);
        std::getline(file, line);
        lines.push_back(line);
    }

    file.close();

    if(lines.size() == 0) return secret;

    for(int k =0; k<=lines.size()-1; k = k +2){
        cout << lines[k+1] << endl;
        secret.keys[lines[k]] = lines[k+1];
    }
    
    return secret;
}