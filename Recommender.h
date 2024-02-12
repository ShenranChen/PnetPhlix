#ifndef RECOMMENDER_INCLUDED
#define RECOMMENDER_INCLUDED

#include <string>
#include <vector>

class UserDatabase;
class MovieDatabase;

struct MovieAndRank
{
    MovieAndRank(const std::string& id, int score)
     : movie_id(id), compatibility_score(score)
    {}

    std::string movie_id;
    int compatibility_score;
};

struct MovieObjects
{
    MovieObjects(std::string id, std::string title, float rating, int score)
    : m_id(id), m_title(title), m_rating(rating), m_score(score)
    {}
    
    std::string m_id;
    std::string m_title;
    float m_rating;
    int m_score;
};
    
class Recommender
{
  public:
    Recommender(const UserDatabase& user_database,
                const MovieDatabase& movie_database);
    std::vector<MovieAndRank> recommend_movies(const std::string& user_email,
                                               int movie_count) const;

  private:
    const UserDatabase* m_udb;
    const MovieDatabase* m_mdb;
};

#endif // RECOMMENDER_INCLUDED
