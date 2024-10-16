#include "NoteApi.h"

NoteApi::NoteApi(int port, int threads, std::string dbName, std::string dbLogin, std::string dbPass, std::string dbServerIP, std::string dbPort)
{
    /* REST API */
    // Server parameters
    Address addr(Ipv4::any, Port(port));
    auto outs = Http::Endpoint::options().threads(threads);
    
    try
    {
        // DB
        con = new pqxx::connection("dbname=" + dbName + " user=" + dbLogin + "password=" + dbPass + " host=" + dbServerIP + " port=" + dbPort);

        if (con->is_open())
        {
            std::cout << "Connection to a database is successful! DB name = " << con->dbname() << std::endl;
            
            // Server init
            server = new Http::Endpoint(addr);
            server->init(opts);
            
            // Router
            bindRouters();
            server->setHandler(router);

            // Start a database 
            server->serve();
            std::cout << "Server is running..." << std::
        }
    }
    catch (const std::exception& ex)
    {
        std::cerr << ex.what() << std::endl;
    }
}

NoteApi::~NoteApi()
{
    if (con->is_open())
    {
        delete con;
        delete ser
    }
}

void NoteApi::bindRouters()
{
    using namespace Rest;

    Routes::Get(router, "/", Routes::bind(&NoteApi::standartFunction, this));

    /* Add functions */
    Routes::Get(router, "/addaccount/:name/:secret", Routes::bind(&NoteApi::addAccount, this));
    Routes::Get(router, "/addsession/:name/:secret", Routes::bind(&NoteApi::addSession, this));
    Routes::Get(router, "/addnote/:token", Routes::bind(&NoteApi::addNote, this));

    /* Get functions */
    Routes::Get(router, "/getnote/:token/:id", Routes::bind(&NoteApi::getNoteContent, this));
    Routes::Get(router, "/getnotes/:token", Routes::bind(&NoteApi::getNotes, this));

    /* Update functions */
    Routes::Get(router, "/updatenote/:token/:id/:title/:content", Routes::bind(&NoteApi::updateNote, this));
    Routes::Get(router, "/updatepass/:token/:secret", Routes::bind(&NoteApi::updatePass, this));

    /* Delete functions */
    Routes::Get(router, "/deleteaccount/:token", Routes::bind(&NoteApi::deleteAccount, this));
    Routes::Get(router, "/deletesession/:token", Routes::bind(&NoteApi::deleteSession, this));
    Routes::Get(router, "/deletenote/:token/:id", Routes::bind(&NoteApi::standartFunction, this));
}


/* Functions */

void NoteApi::standartFunction(const Rest::Request& request, Http::ResponseWriter response)
{
    std::string text = "Hello, this is my REST API server. You can use this methods:\n";
    text.append("/addaccount/{name}/{secret} - create a new user\n");
    text.append("/addsession/{name}/{secret} - create a new session\n");
    text.append("/addnote/{token} - create a new note\n\n");

    text.append("/getnote/{token}/{id} - get user note by getting note id\n");
    text.append("/getnotes/{token} - get all user notes\n\n");

    text.append("/updatenote/{token}/{id}/{title}/{content} - update note data\n");
    text.append("/updatepass/{token}/{secret} - create a new password\n\n");

    text.append("/deleteaccount/{token} - delete current account\n");
    text.append("/deletesession/{token} - delete current session\n");
    text.append("/deletenote/{token}/{id} - delete note\n");

    response.send(Http::Code::Ok, text);
}

/* Add functions */

void NoteApi::addAccount(const Rest::Request& request, Http::ResponseWriter response)
{

}

void NoteApi::addSession(const Rest::Request& request, Http::ResponseWriter response)
{

}

void NoteApi::addNote(const Rest::Request& request, Http::ResponseWriter response)
{

}

/* Get functions */

void NoteApi::getNoteContent(const Rest::Request& request, Http::ResponseWriter response)
{

}

void NoteApi::getNotes(const Rest::Request& request, Http::ResponseWriter response)
{

}

/* Update functions */

void NoteApi::updateNote(const Rest::Request& request, Http::ResponseWriter response)
{

}

void NoteApi::updatePass(const Rest::Request& request, Http::ResponseWriter response)
{

}

/* Delete functions */

void NoteApi::deleteAccount(const Rest::Request& request, Http::ResponseWriter response)
{

}

void NoteApi::deleteSession(const Rest::Request& request, Http::ResponseWriter response)
{

}

void NoteApi::deleteNote(const Rest::Request& request, Http::ResponseWriter response)
{

}
