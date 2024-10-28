#pragma once

#include <pqxx/pqxx>

class ManageDB
{
public:

    /* Account actions */
    static std::string addAccount(const std::string name, const std::string secret);
    static std::string updatePass(const std::string token, const std::string pass);
    static std::string deleteAccount(const std::string token);


    /* Session actions */
    static std::string addSession(const std::string name, const std::string secret);
    static std::string deleteSession(const std::string token);


    /* Note actions */
    static std::string addNote(const std::string token);
    static std::string getNote(const std::string token, const std::string id);
    static std::string getNotes(const std::string token);
    static std::string updateNote(const std::string token, const std::string id, const std::string title, const std::string content);
    static std::string deleteNote(const std::string token, const std::string id);

private:

    /* Generate token */
    std::string generateToken(int length);
}