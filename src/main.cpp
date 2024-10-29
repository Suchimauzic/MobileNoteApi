#include "NoteApi.h"

int main()
{
    // Random
    srand(time(NULL));

    int port = 9080;
    int threads = 5;
    
    std::string dbName = "serega-notes";
    std::string dbLogin = "postgres";
    std::string dbPass = "ch3Q1542@";
    std::string dbServerIP = "87.228.17.41";
    std::string dbPort = "5431";

    NoteApi noteApi(port, threads, dbName, dbLogin, dbPass, dbServerIP, dbPort);

    return 0;
}