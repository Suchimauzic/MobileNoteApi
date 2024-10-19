#include "NoteApi.h"

NoteApi::NoteApi(int port, int threads, std::string dbName, std::string dbLogin, std::string dbPass, std::string dbServerIP, std::string dbPort)
{
    /* REST API */
    // Server parameters
    Address addr(Ipv4::any(), Port(port));
    auto outs = Http::Endpoint::options().threads(threads);
    
    try
    {
        // DB
        con = new pqxx::connection("dbname=" + dbName + " user=" + dbLogin + " password=" + dbPass + " host=" + dbServerIP + " port=" + dbPort);

        if (con->is_open())
        {
            std::cout << "Connection to a database is successful! DB name = " << con->dbname() << std::endl;
            
            // Server init
            server = new Http::Endpoint(addr);
            server->init(outs);
            
            // Router
            bindRouters();
            server->setHandler(router.handler());

            // Start a database 
            server->serve();
        }
    }
    catch (const std::exception& ex)
    {
        std::cerr << ex.what() << std::endl;
    }
}

NoteApi::~NoteApi()
{
    std::cout << "Close the server" << std::endl;
    if (con->is_open())
    {
        delete con;
        delete server;
    }
}

void NoteApi::bindRouters()
{
    using namespace Rest;

    /* Account actions */

    // Add account
    Routes::Post(router, "/account", Routes::bind(&NoteApi::manageAccount, this));
    // Update password
    Routes::Put(router, "/account", Routes::bind(&NoteApi::manageAccount, this));
    // Delete accout
    Routes::Delete(router, "/account", Routes::bind(&NoteApi::manageAccount, this));

    /* Session actions */
    
    // Add session
    Routes::Post(router, "/accout", Routes::bind(&NoteApi::manageSession, this));
    // Delete session
    Routes::Post(router, "/session", Routes::bind(&NoteApi::manageSession, this));
    
    /* Note actions */
    
    // Add note
    Routes::Post(router, "/note", Routes::bind(&NoteApi::manageNote, this));
    // Get note
    Routes::Get(router, "/note/:id", Routes::bind(&NoteApi::manageNote, this));
    // Get notes
    Routes::Get(router, "/note", Routes::bind(&NoteApi::manageNote, this));
    // Update note
    Routes::Put(router, "/note", Routes::bind(&NoteApi::manageNote, this));

    // Shutdown the server
    Routes::Get(router, "/shutdown", Routes::bind(&NoteApi::shutdownServer, this));
}

/* Account actions*/
void NoteApi::manageAccount(const Rest::Request& request, Http::ResponseWriter response)
{
    json body = json::parse(request.body());
    pqxx::work work(*con);

    // Create account
    if (request.method() == Http::Method::Post)
    {
        std::string name = body["name"];
        std::string secret = body["secret"];

        pqxx::result result = work.exec("SELECT * FROM data.account WHERE account_name = '" + name + "';");

        if (result[0][0].is_null())     // This account is not exist
        {
            work.exec("INSERT INTO data.account (account_name, account_secret) VALUES ('" + name + "', '" + secret + "');");
            response.send(Http::Code::Ok, "true");
        }
        else
        {
            response.send(Http::Code::Ok, "false");
        }

        work.commit();
    }

    // Update password
    if (request.method() == Http::Method::Put)
    {
        std::string token = body["token"];
        std::string secret = body["secret"];

        work.exec("UPDATE data.account SET account_secret = '" + secret + "' WHERE account_id = (SELECT session_account from data.session WHERE session_token = '" + token + "');");
        work.commit();

        response.send(Http::Code::Ok, "true");
    }

    // Delete account
    if (request.method() == Http::Method::Delete)
    {
        std::string token = body["token"];

        work.exec("DELETE FROM data.account WHERE account_id = (SELECT session_account FROM data.session WHERE session_token = '" + token + "');");
        work.commit();

        response.send(Http:Code::Ok, "true");
    }
}

/* Session actions */
void NoteApi::manageSession(const Rest::Request& request, Http::ResponseWriter response)
{
    
}

/* Note actions */
void NoteApi::manageNote(const Rest::Request& request, Http::ResponseWriter response)
{
    
}

/* Shutdown the server */
void NoteApi::shutdownServer(const Rest::Request& request, Http::ResponseWriter response)
{
    response.send(Http::Code::Ok);
    server->shutdown();
}
