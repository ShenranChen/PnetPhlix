#include "MovieDatabase.h"
#include "Movie.h"

#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include "utility.h"
using namespace std;

MovieDatabase::MovieDatabase()
{
    loaded = false;
}

MovieDatabase::~MovieDatabase()
{
    //delete all the dynamically allocated movies
    vector<Movie*>::iterator it = allMovies.begin();
    
    while(it != allMovies.end())
    {
        delete *it;
        it = allMovies.erase(it);
    }
}

//string MovieDatabase::toLower(string s) const
//{
//    for(int i = 0; i< s.size(); i++)
//    {
//        s[i] = tolower(s[i]);
//    }
//    return s;
//}

bool MovieDatabase::load(const string& filename)
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
                
                string id = toLower(s);
                
                
                getline(infile, s);
                string name = s;
                
                getline(infile, s);
                string year = s;
                
                vector<string> directors;
                getline(infile, s);
                
                //parse string to get all individual directors
                while(s.size() >0)
                {
                    int pos = static_cast<int>(s.find(","));
                    string d = s.substr(0, pos);
                    d = toLower(d);
                    directors.push_back(d);
                    s = s.substr(pos+1);

                    if(pos < 0)
                    {
                        break;
                    }
                }
                
                vector<string> actors;
                getline(infile, s);
                
                //parse the string to get all of the individual actors
                while(s.size() >0)
                {
                    int pos = static_cast<int>(s.find(","));
                    string a = s.substr(0, pos);
                    a = toLower(a);
                    actors.push_back(a);
                    s = s.substr(pos+1);

                    if(pos < 0)
                    {
                        break;
                    }
                }
                
                vector<string> genre;
                getline(infile, s);
                
                //parse to get all the genre
                while(s.size() >0)
                {
                    int pos = static_cast<int>(s.find(","));
                    string g = s.substr(0, pos);
                    g = toLower(g);
                    genre.push_back(g);
                    s = s.substr(pos+1);

                    if(pos < 0)
                    {
                        break;
                    }
                }
                
                float rating;
                infile >> rating;
                infile.ignore(10000, '\n');

                Movie* m = new Movie(id, name, year, directors, actors, genre, rating);
                moviesIdTree.insert(id, m);
                
                //insert all of the actors, directors, genre into their respective tree
                for(int i = 0; i < actors.size(); i++)
                {
                    moviesActorTree.insert(actors[i], m);
                }
                
                for(int i = 0; i < directors.size(); i++)
                {
                    moviesDirectorTree.insert(directors[i], m);
                }
                
                for(int i = 0; i < genre.size(); i++)
                {
                    moviesGenreTree.insert(genre[i], m);
                }

                allMovies.push_back(m);
            }
            loaded = true;
        }
    }
    return loaded;
}

Movie* MovieDatabase::get_movie_from_id(const string& id) const
{
    string ID = toLower(id);
    TreeMultimap<string, Movie*>::Iterator it = moviesIdTree.find(ID);
    if(it.is_valid())
    {
        return it.get_value();
    }
    return nullptr;
}

vector<Movie*> MovieDatabase::get_movies_with_director(const string& director) const
{
    string findD = toLower(director);
    vector<Movie*> moviesWithDir;
    TreeMultimap<string, Movie*>::Iterator it = moviesDirectorTree.find(findD);
    while(it.is_valid())
    {
        moviesWithDir.push_back(it.get_value());
        it.advance();
    }
    return moviesWithDir;
}

vector<Movie*> MovieDatabase::get_movies_with_actor(const string& actor) const
{
    string findA = toLower(actor);
    vector<Movie*> moviesWithActors;
    TreeMultimap<string, Movie*>::Iterator it = moviesActorTree.find(findA);
    while(it.is_valid())
    {
        moviesWithActors.push_back(it.get_value());
        it.advance();
    }
    return moviesWithActors;
}

vector<Movie*> MovieDatabase::get_movies_with_genre(const string& genre) const
{
    string findG = toLower(genre);
    vector<Movie*> moviesWithGenre;
    TreeMultimap<string, Movie*>::Iterator it = moviesGenreTree.find(findG);
    while(it.is_valid())
    {
        moviesWithGenre.push_back(it.get_value());
        it.advance();
    }
    return moviesWithGenre;
}
