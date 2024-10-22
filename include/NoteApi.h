#pragma once

#include <pistache/endpoint.h>
#include <pistache/http.h>
#include <pistache/router.h>

#include <pqxx/pqxx>
#include <nlohmann/json.hpp>

#include <iostream>
#include <format>

using namespace Pistache;
using json = nlohmann::json;

class NoteApi
{
public:
    NoteApi(int port, int threads, std::string dbName, std::string dbLogin, std::string dbPass, std::string dbServerIP, std::string dbPort);
    ~NoteApi();
    
private:
    /* Server parameters */
    Http::Endpoint* server;
    Rest::Router router;

    /* DB */
    pqxx::connection* con;

    void bindRouters();         // bind all functions for rest api

    /* Server functions */

    // Actions with account
    void manageAccount(const Rest::Request& request, Http::ResponseWriter response);
    
    // Actions with session
    void manageSession(const Rest::Request& request, Http::ResponseWriter response);
    
    // Actions with note
    void manageNote(const Rest::Request& request, Http::ResponseWriter response);

    // Shutdown the server
    void shutdownServer(const Rest::Request& request, Http::ResponseWriter response);

    // Generate token
    std::string generateToken(int length);
};
