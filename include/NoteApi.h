#pragma once

#include <pistache/endpoint.h>
#include <pistache/http.h>
#include <pistache/router.h>

#include <pqxx/pqxx>
#include <iostream>

using namespace Pistache;

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

    /* Functions */
    void standartFunction(const Rest::Request& request, Http::ResponseWriter response);

    /* Add functions */
    void addAccount(const Rest::Request& request, Http::ResponseWriter response);
    void addSession(const Rest::Request& request, Http::ResponseWriter response);
    void addNote(const Rest::Request& request, Http::ResponseWriter response);

    /* Get functions */
    void getNoteContent(const Rest::Request& request, Http::ResponseWriter response);
    void getNotes(const Rest::Request& request, Http::ResponseWriter response);

    /* Update functions */
    void updateNote(const Rest::Request& request, Http::ResponseWriter response);
    void updatePass(const Rest::Request& request, Http::ResponseWriter response);

    /* Delete functions */
    void deleteAccount(const Rest::Request& request, Http::ResponseWriter response);
    void deleteSession(const Rest::Request& request, Http::ResponseWriter response);
    void deleteNote(const Rest::Request& request, Http::ResponseWriter response);
};
