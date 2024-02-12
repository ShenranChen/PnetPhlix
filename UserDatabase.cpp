#include "UserDatabase.h"
#include "User.h"

#include <fstream>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

UserDatabase::UserDatabase()
{
    loaded = false;
}
UserDatabase::~UserDatabase()
{
    vector<User*>::iterator it = allUser.begin();
    
    while(it != allUser.end())
    {
        delete *it;
        it = allUser.erase(it);
    }
}

bool UserDatabase::load(const string& filename)
{
    if(!loaded)
    {
        ifstream infile(filename);
        if(!infile)
        {
            cerr << "fail to load" << endl;
        }
        else
        {
            bool first = true;
            string s;
            //the file is not empty
            while(getline(infile, s))
            {
                if(s == "")
                {
                    first = false;
                }
                if(!first)
                {
                    getline(infile, s);
                }
                string name = s;
                
                
                getline(infile, s);
                string email = s;
                
                int numM;
                infile >> numM;
                infile.ignore(10000, '\n');
                
                vector<string> watchHistory;
                
                while(numM > 0)
                {
                    getline(infile, s);
                    watchHistory.push_back(s);
                    numM--;
                }
                
                //create new user, add to the tree, and add it to a vector that store all user
                User* u = new User(name, email, watchHistory);
                userTree.insert(email, u);
                allUser.push_back(u);
            }
            loaded = true;
        }
    }
    return loaded;
}

User* UserDatabase::get_user_from_email(const string& email) const
{
    //return an iterator to the user, if not nullptr
    TreeMultimap<string, User*>::Iterator it = userTree.find(email);
    if(it.is_valid())
    {
        return it.get_value();
    }
    
    return nullptr;
}
