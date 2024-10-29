#include <gtest/gtest.h>
#include "ManageDB.h"

#include <iostream>

ManageDB* manageDB;
std::string dbName;
std::string dbLogin;
std::string dbPass;
std::string dbServerIP;
std::string dbPort;

TEST(TestAccount, CreateAccount)
{
    manageDB = new ManageDB(dbName, dbLogin, dbPass, dbServerIP, dbPort);

    // Arrange
    std::string name = "UnitAccTestName";
    std::string secret = "UnitAccTestSecret";

    // Act
    std::string result = manageDB->addAccount(name, secret);

    delete manageDB;

    // Assert
    ASSERT_EQ(result, "true");
}

TEST(TestAccount, UpdatePassword)
{
    manageDB = new ManageDB(dbName, dbLogin, dbPass, dbServerIP, dbPort);

    // Arrange
    std::string name = "UnitAccTestName";
    std::string secret = "UnitAccTestSecret";
    std::string newSecret = "UnitAccTestNewSecret";
    std::string token = manageDB->addSession(name, secret);
    
    // Act
    std::string result = manageDB->updatePass(token, newSecret);
    delete manageDB;

    // Assert
    ASSERT_EQ(result, "true");
}

TEST(TestAccount, DeleteAccount)
{
    manageDB = new ManageDB(dbName, dbLogin, dbPass, dbServerIP, dbPort);

    // Arrange
    std::string name = "UnitAccTestName";
    std::string secret = "UnitAccTestNewSecret";
    std::string token = manageDB->addSession(name, secret);

    // Act
    std::string result = manageDB->deleteAccount(token);
    delete manageDB;

    // Assert
    ASSERT_EQ(result, "true");
}

TEST(TestSession, AddSession)
{
    manageDB = new ManageDB(dbName, dbLogin, dbPass, dbServerIP, dbPort);

    // Arrange
    std::string name = "UnitSesTestName";
    std::string secret = "UnitSesTestSecret";
    manageDB->addAccount(name, secret);

    // Act
    std::string token = manageDB->addSession(name, secret);
    delete manageDB;

    // Assert
    ASSERT_EQ(token.length(), 32);
}

TEST(TestSession, DeleteSession)
{
    manageDB = new ManageDB(dbName, dbLogin, dbPass, dbServerIP, dbPort);

    // Arrange
    std::string name = "UnitSesTestName";
    std::string secret = "UnitSesTestSecret";
    std::string token = manageDB->addSession(name, secret);

    // Act
    std::string result = manageDB->deleteSession(token);

    // Clear
    manageDB->deleteAccount(manageDB->addSession(name, secret));
    delete manageDB;

    // Assert
    ASSERT_EQ(result, "true");
}

TEST(TestNote, AddNote)
{
    manageDB = new ManageDB(dbName, dbLogin, dbPass, dbServerIP, dbPort);

    // Arrange
    std::string name = "UnitNoteTestName";
    std::string secret = "UnitNoteTestSecret";
    manageDB->addAccount(name, secret);
    std::string token = manageDB->addSession(name, secret);

    // Act
    std::string result = manageDB->addNote(token);
    delete manageDB; 

    // Assert
    ASSERT_EQ(result, "true");
}

TEST(TestNote, GetOneUserNote)
{
    manageDB = new ManageDB(dbName, dbLogin, dbPass, dbServerIP, dbPort);

    // Arrange
    std::string name = "UnitNoteTestName";
    std::string secret = "UnitNoteTestSecret";
    std::string token = manageDB->addSession(name, secret);
    std::string id = "5";
    
    // Act
    std::string result = manageDB->getNote(token, id);
    delete manageDB;

    // Assert
    ASSERT_NE(result.find("id"), std::string::npos);
}

TEST(TestNote, GetAllUserNotes)
{
    manageDB = new ManageDB(dbName, dbLogin, dbPass, dbServerIP, dbPort);

    // Arrange
    std::string name = "UnitNoteTestName";
    std::string secret = "UnitNoteTestSecret";
    std::string token = manageDB->addSession(name, secret);
    
    // Act
    std::string result = manageDB->getNotes(token);
    delete manageDB;

    // Assert
    ASSERT_NE(result.find("id"), std::string::npos);
}

TEST(TestNote, UpdateNote)
{
    manageDB = new ManageDB(dbName, dbLogin, dbPass, dbServerIP, dbPort);

    // Arrange
    std::string name = "UnitNoteTestName";
    std::string secret = "UnitNoteTestSecret";
    std::string token = manageDB->addSession(name, secret);
    std::string id = "5";
    
    std::string newTitle = "UnitTestTitle";
    std::string newContent = "UnitTestContent";

    // Act
    std::string result = manageDB->updateNote(token, id, newTitle, newContent);
    delete manageDB;

    // Assert
    ASSERT_EQ(result, "true");
}

TEST(TestNote, DeleteNote)
{
    manageDB = new ManageDB(dbName, dbLogin, dbPass, dbServerIP, dbPort);

    // Arrange
    std::string name = "UnitNoteTestName";
    std::string secret = "UnitNoteTestSecret";
    std::string token = manageDB->addSession(name, secret);
    std::string id = "5";

    // Act
    std::string result = manageDB->deleteNote(token, id);
    
    // Clear
    manageDB->deleteAccount(manageDB->addSession(name, secret));
    delete manageDB;

    // Assert
    ASSERT_EQ(result, "true");
}

int main(int argc, char **argv)
{
    dbName = "serega-notes";
    dbLogin = "postgres";
    dbPass = "ch3Q1542@";
    dbServerIP = "87.228.17.41";
    dbPort = "5431";

    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}