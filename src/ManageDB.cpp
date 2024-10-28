#include "ManageDB.h"

/* Account actions */

std::string ManageDB::addAccount(const std::string name, const std::string secret)
{

}

std::string ManageDB::updatePass(const std::string token, const std::string pass)
{

}

std::string ManageDB::deleteAccount(const std::string token)
{

}


/* Session actions */

std::string ManageDB::addSession(const std::string name, const std::string secret)
{

}

std::string ManageDB::deleteSession(const std::string token)
{

}


/* Note actions */
std::string ManageDB::addNote(const std::string token)
{

}

std::string ManageDB::getNote(const std::string token, const std::string id)
{

}

std::string ManageDB::getNotes(const std::string token)
{

}

std::string ManageDB::updateNote(const std::string token, const std::string id, const std::string title, const std::string content)
{
    
}

std::string ManageDB::deleteNote(const std::string token, const std::string id)
{

}


/* Generate token */

std::string ManageDB::generateToken(int length)
{
    
}
