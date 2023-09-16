#include "UserDatabase.h"
#include "User.h"
#include "treemm.h"

#include <string>
#include <vector>
#include <fstream>
using namespace std;

UserDatabase::UserDatabase()
{
    users = new TreeMultimap<string, User*>;
    userPointers.clear();
}

UserDatabase::~UserDatabase()
{
    for (User* user : userPointers) delete user;
    delete users;
}

bool UserDatabase::load(const string& filename)
{
    ifstream infile(filename);
    if (!infile) return false;
    
    string line;
    while(getline(infile, line))
    {
        if (line == "")
        {
            infile.ignore(10000, '\n');
            getline(infile, line);
        }
        string name = line;
        string email;
        infile >> email;
        int count;
        infile >> count;
        vector<string> movieHistory;
        for (int i = 0; i < count; i++)
        {
            string movie;
            infile >> movie;
            movieHistory.push_back(movie);
        }
        User* user = new User(name, email, movieHistory);
        userPointers.push_back(user);
        users->insert(email, user);
    }
    return true;
}

User* UserDatabase::get_user_from_email(const string& email) const
{
    return users->find(email).get_value();
}
