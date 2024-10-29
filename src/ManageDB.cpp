#include "ManageDB.h"

ManageDB::ManageDB()
{
    std::string dbName = "serega-notes";
    std::string dbLogin = "postgres";
    std::string dbPass = "12345";
    std::string dbServerIP = "127.0.0.1";
    std::string dbPort = "5432";
    
    try
    {
        con = new pqxx::connection("dbname=" + dbName + " user=" + dbLogin + " password=" + dbPass + " host=" + dbServerIP + " port=" + dbPort);
    }
    catch (const std::exception& ex)
    {
        std::cerr << ex.what() << std::endl;
    }
}

ManageDB::ManageDB(std::string dbName, std::string dbLogin, std::string dbPass, std::string dbServerIP, std::string dbPort)
{
    try
    {
        // DB
        con = new pqxx::connection("dbname=" + dbName + " user=" + dbLogin + " password=" + dbPass + " host=" + dbServerIP + " port=" + dbPort);
    }
    catch (const std::exception& ex)
    {
        std::cerr << ex.what() << std::endl;
    }
}

ManageDB::~ManageDB()
{
    if (con->is_open())
    {
        con->close();
        delete con;
    }
}


/* Account actions */

std::string ManageDB::addAccount(const std::string name, const std::string secret)
{
    pqxx::work work(*con);
    std::string response;

    // Get an account using a name
    pqxx::result result = work.exec("SELECT * FROM data.account WHERE account_name = '" + name + "';");

    if (result[0][0].is_null())     // This account does not exist
    {
        // Add a new account
        work.exec("INSERT INTO data.account (account_name, account_secret) VALUES ('" + name + "', '" + secret + "');");
        response = "true";
    }
    else                            // This account exists
    {
        response = "false";
    }

    work.commit();
    return response;
}

std::string ManageDB::updatePass(const std::string token, const std::string pass)
{
    pqxx::work work(*con);
    // Update account pass
    work.exec("UPDATE data.account SET account_secret = '" + pass + "' WHERE account_id = (SELECT session_account from data.session WHERE session_token = '" + token + "');");
    work.commit();
    return "true";
}

std::string ManageDB::deleteAccount(const std::string token)
{
    pqxx::work work(*con);

    // Get an account using a token
    pqxx::result result = work.exec("SELECT * FROM data.account WHERE account_id = (SELECT session_account from data.session WHERE session_token = '" + token + "');");

    // Delete this account
    work.exec("DELETE FROM data.account WHERE account_id = " + result[0][0].as<std::string>() + ";");

    // Delete all sessions that belonged to this account
    work.exec("DELETE FROM data.session WHERE session_account = " + result[0][0].as<std::string>() + ";");
    work.commit();

    return "true";
}


/* Session actions */

std::string ManageDB::addSession(const std::string name, const std::string secret)
{
    pqxx::work work(*con);

    // Get the account
    pqxx::result result = work.exec("SELECT * FROM data.account WHERE account_name = '" + name + "' AND account_secret = '" + secret + "';");
    std::string token = generateToken(32);

    // Add session
    work.exec("INSERT INTO data.session (session_account, session_token) VALUES (" + result[0][0].as<std::string>() + ", '" + token + "');");
    work.commit();

    return token;
}

std::string ManageDB::deleteSession(const std::string token)
{
    pqxx::work work(*con);

    // Delete this session
    work.exec("DELETE FROM data.session WHERE session_token = '" + token + "';");
    work.commit();
    return "true";
}


/* Note actions */

std::string ManageDB::addNote(const std::string token)
{
    pqxx::work work(*con);
    
    // Insert new note
    work.exec("INSERT INTO data.note (note_account, note_title, note_content) VALUES ((SELECT session_account FROM data.session WHERE session_token = '" + token + "'), 'New note', 'Some text');");
    work.commit();
    return "true";
}

std::string ManageDB::getNote(const std::string token, const std::string id)
{
    pqxx::work work(*con);

    // Get the note
    pqxx::result result = work.exec("SELECT * FROM data.note WHERE note_id = " + id + " AND note_account = (SELECT session_account FROM data.session WHERE session_token = '" + token + "');");
    work.commit();

    // Fill the object with data 
    json info;
    info["note_id"] = result[0][0].as<std::string>();
    info["note_account"] = result[0][1].as<std::string>();
    info["note_title"] = result[0][2].as<std::string>();
    info["note_content"] = result[0][3].as<std::string>();

    return info.dump();

}

std::string ManageDB::getNotes(const std::string token)
{
    pqxx::work work(*con);

    // Get notes
    pqxx::result result = work.exec("SELECT * FROM data.note WHERE note_account = (SELECT session_account FROM data.session WHERE session_token = '" + token + "');");
    work.commit();

    // Create a json response
    std::string keys[] { "\"note_id\"", "\"note_account\"", "\"note_title\"", "\"note_content\"" };
    std::string json = "[";
    for (int i = 0; i < result.size(); i++)
    {
        if (i > 0)
            json += ",";
        json += "{";
        for (int j = 0; j < 4; j++)
        {
            json += keys[j] + ":\"" + result[i][j].as<std::string>() + "\"";
            if (j < 3)
                json += ",";
        }
        json += "}";
    }
    json += "]";

    // Get the final json response
    std::string jsonResponse = "{\"notes\":" + json + "}";
    
    return jsonResponse;
}

std::string ManageDB::updateNote(const std::string token, const std::string id, const std::string title, const std::string content)
{
    pqxx::work work(*con);

    // Update note
    work.exec("UPDATE data.note SET note_title = '" + title + "', note_content = '" + content + "' WHERE note_id = " + id + " AND note_account = (SELECT session_account FROM data.session WHERE session_token = '" + token + "');");
    work.commit();
    return "true";
}

std::string ManageDB::deleteNote(const std::string token, const std::string id)
{
    pqxx::work work(*con);

    // Delete note
    work.exec("DELETE FROM data.note WHERE note_id = " + id + " AND note_account = (SELECT session_account FROM data.session WHERE session_token = '" + token + "');");
    work.commit();
    return "true";
}


/* Generate token */

std::string ManageDB::generateToken(int length)
{
    const std::string tokenChars = "0123456789abcdef";
    
    std::string token;
    token.resize(length);

    for (int i = 0; i < length; i++)
    {
        token[i] = tokenChars[rand() % tokenChars.length()];
    }

    return token;
}
