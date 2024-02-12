#include "Recommender.h"
#include "UserDatabase.h"
#include "MovieDatabase.h"
#include "User.h"
#include "Movie.h"
#include "utility.h"
#include <string>
#include <vector>
#include <unordered_map>
#include <algorithm>
using namespace std;


//custom comparsion operator for movie objects
bool comp(const MovieObjects& a, const MovieObjects& b)
{
    if(a.m_score != b.m_score)
    {
        return a.m_score > b.m_score;
    }
    else
    {
        if(a.m_rating != b.m_rating)
        {
            return a.m_rating > b.m_rating;
        }
        else
        {
            return a.m_title < b.m_title;
        }
    }
}

Recommender::Recommender(const UserDatabase& user_database,
                         const MovieDatabase& movie_database)
{
    m_mdb = &movie_database;
    m_udb = &user_database;
}

vector<MovieAndRank> Recommender::recommend_movies(const string& user_email, int movie_count) const
{
    vector<MovieAndRank> recomList;
    User* u = m_udb->get_user_from_email(user_email);
    if(u == nullptr)
    {
        return recomList ;
    }
    //get the user watch history
    vector<string> watchHis = m_udb->get_user_from_email(user_email)->get_watch_history();

    unordered_map<string, int> movieAndScores;

    //loop through watch history
    for(int i = 0; i < watchHis.size(); i++)
    {
        Movie* m = m_mdb->get_movie_from_id(watchHis[i]);
        
        if(m == nullptr)
        {
            continue;
        }
        //get all the directors for this movie
        vector<string> tempD = m->get_directors();
        for(int i = 0; i< tempD.size(); i++)
        {
            vector<Movie*> sameDirAsUser = m_mdb->get_movies_with_director(tempD[i]);
            //gives points to all movies with the same director
            for(int i = 0; i < sameDirAsUser.size(); i++)
            {
                movieAndScores.insert({sameDirAsUser[i]->get_id(), 0});
                movieAndScores[sameDirAsUser[i]->get_id()] += 20;
            }
        }
        
        //get all the actor for this movie
        vector<string> tempA = m->get_actors();
        for(int i = 0; i < tempA.size(); i++)
        {
            vector<Movie*> sameActorAsUser = m_mdb->get_movies_with_actor(tempA[i]);
            
            //gives points to all movies with the same actor
            for(int i = 0; i < sameActorAsUser.size(); i++)
            {
                movieAndScores.insert({sameActorAsUser[i]->get_id(), 0});
                movieAndScores[sameActorAsUser[i]->get_id()] += 30;
            }
        }

        //get all the genre for the movie
        vector<string> tempG = m->get_genres();
        for(int i = 0; i < tempG.size(); i++)
        {
            vector<Movie*> sameGenreAsUser = m_mdb->get_movies_with_genre(tempG[i]);

            //gives points to all movies with the same genre
            for(int i = 0; i < sameGenreAsUser.size(); i++)
            {
                movieAndScores.insert({sameGenreAsUser[i]->get_id(), 0});
                movieAndScores[sameGenreAsUser[i]->get_id()] += 1;
            }
        }
    }

    //erase the movie that the user alreayd watched
    for(int i = 0; i < watchHis.size(); i++)
    {
        movieAndScores.erase(toLower(watchHis[i]));
    }
    
    unordered_map<string, int>::iterator it = movieAndScores.begin();
    vector<MovieObjects> movieObj;

    //create a bunch of MovieObject with the quailities I need to sort
    while(it != movieAndScores.end())
    {

        string id = it->first;
        string title = m_mdb->get_movie_from_id(id)->get_title();
        float rating = m_mdb->get_movie_from_id(id)->get_rating();
        MovieObjects a(id, title, rating, it->second);
        movieObj.push_back(a);
        it++;
    }

    sort(movieObj.begin(), movieObj.end(), comp);

    
    if(movie_count <= 0)
    {
        return recomList;
    }
    if(movie_count > movieObj.size())
    {
        movie_count = static_cast<int>(movieObj.size());
    }
    
    //put all the objects into recomList
    for(int i = 0; i < movie_count; i++)
    {
        MovieAndRank a(movieObj[i].m_id, movieObj[i].m_score);
        recomList.push_back(a);
    }

    return recomList;  // Replace this line with correct code.
}
