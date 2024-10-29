#include "NoteApi.h"

NoteApi::NoteApi()
{
    Address addr(Ipv4::any(), Port(9080));
    auto outs = Http::Endpoint::options().threads(5);

    std::cout << "Connecting to the database..." << std::endl;
    manageDB = new ManageDB();

    if (manageDB->con->is_open())
    {
        std::cout << "Connection to the database is successful! DB name = " << manageDB->con->dbname() << std::endl;
    }
    else
    {
        std::cout << "Cannot connect to the database! Program is stopping" << std::endl;
    }

    try
    {
        // Server init
        server = new Http::Endpoint(addr);
        server->init(outs);
        
        // Router
        bindRouters();
        server->setHandler(router.handler());

        // Start a database
        server->serve();
    }
    catch(const std::exception& ex)
    {
        std::cerr << ex.what() << '\n';
    }
    
}

NoteApi::NoteApi(int port, int threads, std::string dbName, std::string dbLogin, std::string dbPass, std::string dbServerIP, std::string dbPort)
{
    /* REST API */
    // Server parameters
    Address addr(Ipv4::any(), Port(port));
    auto outs = Http::Endpoint::options().threads(threads);
    
    std::cout << "Connecting to the database..." << std::endl;
    manageDB = new ManageDB(dbName, dbLogin, dbPass, dbServerIP, dbPort);

    if (manageDB->con->is_open())
    {
        std::cout << "Connection to the database is successful! DB name = " << manageDB->con->dbname() << std::endl;
    }
    else
    {
        std::cout << "Cannot connect to the database! Program is stopping..." << std::endl;
        return;
    }

    try
    {
        // Server init
        server = new Http::Endpoint(addr);
        server->init(outs);
        
        // Router
        bindRouters();
        server->setHandler(router.handler());

        // Start a database
        server->serve();
    }
    catch (const std::exception& ex)
    {
        std::cerr << ex.what() << std::endl;
    }
}

NoteApi::~NoteApi()
{
    std::cout << "Closing the database..." << std::endl;
    delete manageDB;
    std::cout << "Closing the server..." << std::endl;
    delete server;
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
    Routes::Post(router, "/session", Routes::bind(&NoteApi::manageSession, this));
    // Delete session
    Routes::Delete(router, "/session", Routes::bind(&NoteApi::manageSession, this));
    
    /* Note actions */
    
    // Add note
    Routes::Post(router, "/note", Routes::bind(&NoteApi::manageNote, this));
    // Get note
    Routes::Get(router, "/note/:token/:id", Routes::bind(&NoteApi::manageNote, this));
    // Get notes
    Routes::Get(router, "/note/:token", Routes::bind(&NoteApi::manageNote, this));
    // Update note
    Routes::Put(router, "/note", Routes::bind(&NoteApi::manageNote, this));
    // Delete note
    Routes::Delete(router, "/note", Routes::bind(&NoteApi::manageNote, this));

    // Shutdown the server
    Routes::Get(router, "/shutdown", Routes::bind(&NoteApi::shutdownServer, this));
}


/* Account actions*/

void NoteApi::manageAccount(const Rest::Request& request, Http::ResponseWriter response)
{
    json body = json::parse(request.body());
    

    // Create account
    if (request.method() == Http::Method::Post)
    {
        std::string name = body["name"];
        std::string secret = body["secret"];
        response.send(Http::Code::Ok, manageDB->addAccount(name, secret));
    }

    // Update password
    if (request.method() == Http::Method::Put)
    {
        std::string token = body["token"];
        std::string secret = body["secret"];
        response.send(Http::Code::Ok, manageDB->updatePass(token, secret));
    }

    // Delete account
    if (request.method() == Http::Method::Delete)
    {
        std::string token = body["token"];
        response.send(Http::Code::Ok, manageDB->deleteAccount(token));
    }
}


/* Session actions */

void NoteApi::manageSession(const Rest::Request& request, Http::ResponseWriter response)
{
    json body = json::parse(request.body());

    // Add session
    if (request.method() == Http::Method::Post)
    {
        std::string name = body["name"];
        std::string secret = body["secret"];
        response.send(Http::Code::Ok, manageDB->addSession(name, secret));
    }

    // Delete session
    if (request.method() == Http::Method::Delete)
    {
        std::string token = body["token"];
        response.send(Http::Code::Ok, manageDB->deleteSession(token));
    }
}


/* Note actions */

void NoteApi::manageNote(const Rest::Request& request, Http::ResponseWriter response)
{
    json body;

    if (request.method() != Http::Method::Get)
        body = json::parse(request.body());
    
    // Create a note
    if (request.method() == Http::Method::Post)
    {
        std::string token = body["token"];
        response.send(Http::Code::Ok, manageDB->addNote(token));
    }

    // Get note or notes
    if (request.method() == Http::Method::Get)
    {
        if (request.hasParam(":id"))        // Get note
        {
            std::string token = request.param(":token").as<std::string>();
            std::string id = request.param(":id").as<std::string>();
            response.send(Http::Code::Ok, manageDB->getNote(token, id));
        }
        else    // Get notes
        {
            std::string token = request.param(":token").as<std::string>();
            response.send(Http::Code::Ok, manageDB->getNotes(token));
        }
    }

    // Update note
    if (request.method() == Http::Method::Put)
    {
        std::string token = body["token"];
        std::string id = body["id"];
        std::string title = body["title"];
        std::string content = body["content"];
        response.send(Http::Code::Ok, manageDB->updateNote(token, id, title, content));
    }

    // Delete note
    if (request.method() == Http::Method::Delete)
    {
        std::string token = body["token"];
        std::string id = body["id"];
        response.send(Http::Code::Ok, manageDB->deleteNote(token, id));
    }
}


/* Shutdown the server */

void NoteApi::shutdownServer(const Rest::Request& request, Http::ResponseWriter response)
{
    response.send(Http::Code::Ok, "Shutdown");
    server->shutdown();
}
