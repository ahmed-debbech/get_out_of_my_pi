#include <iostream>
#include "system.cpp"
#include <vector>
#include "cursor.cpp"
#include "parser.cpp"
#include "network.cpp"
#include "ip_store.cpp"

using namespace std;

int main(){

    cout << "hello" << endl;
    std::vector<std::string> logs;
    try{
        logs = getLogs();
    }catch(int i){
        cout << "log files are empty or system didnt give it to us" <<endl;
        return -1;
    }

    string cursor;
    try{
        cursor = getCursor();
    }catch(int i){
        cout << "error occured in the cursor" << endl;
        return -1;
    }

    vector<Attempt> attempts;
    try{
        attempts = parseLogs(cursor, logs);
        if(attempts.size() == 0 ) return -1;

    }catch(int i){
        if(i == -1) cout << "something went wrong while parsing logs" << endl;
        return -1;
    }

    try{
        stringstream ss;
        ss << logs.size();
        setCursor(ss.str());
    }catch(int i){
        cout << "error occured in the cursor" << endl;
        return -1;
    }

    /*for(Attempt msg: attempts){
        cout << msg.ip << endl;
        cout << msg.when << endl;
        cout << msg.user << endl;
    }*/

    if(attempts.size() == 0){
        cout << "No attempts yet!" << endl;
        return -1;
    }

    exposeOnTelegram(attempts);

    unordered_map<string, int> ip_store = getIpStore();
    //for (auto it : ip_store) cout << it.first << " " << it.second << endl;

    ip_store = updateStore(attempts, ip_store);
    //for (auto it : ip_store) cout << it.first << " " << it.second << endl;

    //ban more than 3 attemts
    banIps(ip_store);

    return 0;
}