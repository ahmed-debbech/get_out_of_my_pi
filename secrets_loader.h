#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <string>

using namespace std;

typedef struct{

    unordered_map<string, string> keys;

} Secrets;

Secrets load_secrets();