#pragma once

#include <pqxx/pqxx>
#include <nlohmann/json.hpp>

#include <iostream>

using json = nlohmann::json;

class ManageDB
{
public:

    pqxx::connection* con;

    ManageDB();
    ManageDB(std::string dbName, std::string dbLogin, std::string dbPass, std::string dbServerIP, std::string dbPort);
    ~ManageDB();

    /* Account actions */
    std::string addAccount(const std::string name, const std::string secret);
    std::string updatePass(const std::string token, const std::string pass);
    std::string deleteAccount(const std::string token);


    /* Session actions */
    std::string addSession(const std::string name, const std::string secret);
    std::string deleteSession(const std::string token);


    /* Note actions */
    std::string addNote(const std::string token);
    std::string getNote(const std::string token, const std::string id);
    std::string getNotes(const std::string token);
    std::string updateNote(const std::string token, const std::string id, const std::string title, const std::string content);
    std::string deleteNote(const std::string token, const std::string id);

private:

    /* Generate token */
    std::string generateToken(int length);
};