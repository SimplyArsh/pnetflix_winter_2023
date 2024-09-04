#ifndef USERDATABASE_INCLUDED
#define USERDATABASE_INCLUDED

#include <string>
#include "treemm.h" // TODO: CHECK

#include <vector>

class User;

class UserDatabase
{
  public:

    UserDatabase();
    ~UserDatabase();

    bool load(const std::string& filename);
    User* get_user_from_email(const std::string& email) const;

  private:

    TreeMultimap<std::string, User*> m_tree;
    std::vector<User*> m_users;
    bool m_hasLoaded;
    
};

#endif // USERDATABASE_INCLUDED
