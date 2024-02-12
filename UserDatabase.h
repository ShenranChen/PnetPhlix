#ifndef USERDATABASE_INCLUDED
#define USERDATABASE_INCLUDED

#include <string>
#include "treemm.h"


class User;

class UserDatabase
{
  public:
    UserDatabase();
    ~UserDatabase();
    bool load(const std::string& filename);
    User* get_user_from_email(const std::string& email) const;

  private:
    bool loaded;
    TreeMultimap<std::string, User*> userTree;
    std::vector<User*> allUser;
};

#endif // USERDATABASE_INCLUDED
