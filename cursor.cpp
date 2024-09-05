#include <string>
#include <iostream>
#include <fstream>


using namespace std;

std::string filename = "cursor";


bool fileDoesNotExistOrIsEmpty(const std::string& filename) {
    std::ifstream file(filename, std::ios::binary | std::ios::ate);

    if (!file) {
        return true; // File does not exist or error opening the file
    }

    std::streamsize size = file.tellg();
    return size <= 0; // File is empty
}

void setCursor(string cursor){
    std::ofstream outfile(filename);
    if (!outfile) {
        std::cout << "error creating cursor file" << std::endl;
        throw -1;
    }
    outfile << cursor;
    outfile.close();
}

string getCursor(){

    if (fileDoesNotExistOrIsEmpty(filename)) {
        std::cout << "cursor file does not exist OR is empty... creating new one" << std::endl;

        setCursor("");
    }else{

        std::ifstream file(filename);

        if (!file) {
            std::cout << "could not open cursor file" << std::endl;
            throw -1;
        }

        std::string line;
        std::getline(file, line);

        file.close();

        return line;
    }
    return "";
}   