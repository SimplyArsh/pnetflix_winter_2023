#include "UserDatabase.h"
#include "User.h"

#include <string>
#include <vector>
#include <iostream>
#include <fstream>

using namespace std;


UserDatabase::UserDatabase()
{
    m_hasLoaded = false;  
}


UserDatabase::~UserDatabase()
{
    for (auto* user : m_users)
    {
        delete user;    
        user = nullptr; 
    }
}


bool UserDatabase::load(const string& filename)
{
    
    if (m_hasLoaded)
        return false;
    
    ifstream infile(filename);  
    if (!infile)
    {
        cerr << "ERROR: Failed to load file: " << filename << endl;
        return false;
    }
    
    int userCount = 0;  
    while (infile)
    {
        string name;
        getline(infile, name);  
        
        
        if (!infile)
            break;
        
        string email;
        getline(infile, email);  
        
        int numMovies;
        infile >> numMovies;  
        infile.ignore(10000, '\n');  

        vector<string> watchHistory;
        for (int i = 0; i < numMovies; i++)
        {
            string movie;
            getline(infile, movie);  
            watchHistory.push_back(movie);
        }
        
        
        User* user = new User(name, email, watchHistory);
        m_users.push_back(user);
        m_tree.insert(email, user);  

        userCount++;  
        infile.ignore(10000, '\n');  
    }
    
    cerr << userCount << " user entries successfully loaded!" << endl;
    
    m_hasLoaded = true;  
    return true;
}


User* UserDatabase::get_user_from_email(const string& email) const
{
    auto userIterator = m_tree.find(email);  
    if (!userIterator.is_valid())  
        return nullptr;  
    return userIterator.get_value();  
}