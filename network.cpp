#include <stdio.h>
#include <thread>
#include <unistd.h>
#include <curl/curl.h>
#include "../rapidjson-master/include/rapidjson/rapidjson.h"
#include "../rapidjson-master/include/rapidjson/document.h"
#include "secrets_loader.h"

using namespace std;

typedef struct {
    string icon;
    string country;
    string region;
    string city;
} Flag;

size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* output) {
    size_t totalSize = size * nmemb;
    output->append(static_cast<const char*>(contents), totalSize);
    return totalSize;
}

Flag getFlag(string ip){
    CURL* curl;
    CURLcode res;
    std::string readBuffer;

    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();
    if(curl) {
        // Set the URL for the request
        curl_easy_setopt(curl, CURLOPT_URL, ("https://ipwho.is/" + ip).c_str());

        // Set the write callback function
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);

        // Perform the request
        res = curl_easy_perform(curl);

        // Check for errors
        if(res != CURLE_OK) {
            std::cout << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
            throw -1;
        } else {
            // Output the response
            std::cout << "Response data: " << readBuffer << std::endl;
        }

        // Clean up
        curl_easy_cleanup(curl);
    }

    curl_global_cleanup();

    const char * json = readBuffer.c_str();
    rapidjson::Document d;
    d.Parse(json);

    /*cout << "flag: " << d["flag"]["emoji"].GetString() << endl; 
    cout << "country: " << d["country"].GetString() << endl; 
    cout << "region: " << d["region"].GetString() << endl; 
    cout << "city: " << d["city"].GetString() << endl; 
    */
    Flag flg;
    flg.icon = d["flag"]["emoji"].GetString();
    flg.country = d["country"].GetString();
    flg.region = d["region"].GetString();
    flg.city = d["city"].GetString();

    return flg;
}

void sendTelegram(Attempt a, Flag f){
    CURL *curl;
    CURLcode res;
    std::string readBuffer;

    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();

    if(curl) {
        Secrets s = load_secrets();
        // Set URL for the POST request
        cout << s.keys["telegram_url"] << endl;
        curl_easy_setopt(curl, CURLOPT_URL, s.keys["telegram_url"].c_str());

        string message = a.ip + " from "+f.icon+" " + f.country + ", " + f.region + ", "+ f.city + " " + "is trying to access with user "+a.user+" at exactly" + a.when ;

        // Data to be sent in the POST request
        std::string postData = R"({"chat_id" : ")"+s.keys["chat_id"]+R"(", "text":")"+message+R"("})";
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, postData.c_str());

        // Set the Content-Type header to indicate JSON data
        struct curl_slist *headers = nullptr;
        headers = curl_slist_append(headers, "Content-Type: application/json");
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

        // Set the callback function to handle the response
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);

        // Set to follow redirections (if any)
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);

        // Perform the request, res will get the return code
        res = curl_easy_perform(curl);

        // Check for errors
        if(res != CURLE_OK) {
            std::cout << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
            throw -1;
        } else {
            // Print the response from the server
            std::cout << "Response data: " << readBuffer << std::endl;
        }

        // Clean up
        curl_slist_free_all(headers); // Free the list of headers
        curl_easy_cleanup(curl);
    }

    curl_global_cleanup();
}

void runnable(vector<Attempt> attempts){ 
    for(int i=0;i<=attempts.size()-1; i++){
        Flag flag;
        try{
            flag = getFlag(attempts[i].ip);
        }catch(int i){
            cout << "could not get flag" << endl;
            continue;
        }
        try{
            sendTelegram(attempts[i], flag);
        }catch(int i){
            cout << "could not send to telegram" << endl;
            continue;
        } 
        sleep(1);
    }
} 

void exposeOnTelegram(vector<Attempt> attempts){
    thread thread_obj(runnable, attempts);
    thread_obj.join();
}