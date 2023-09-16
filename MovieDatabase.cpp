#include "MovieDatabase.h"
#include "Movie.h"

#include <string>
#include <vector>
#include <fstream>
#include <sstream>
using namespace std;

MovieDatabase::MovieDatabase()
{
    movieMap = new TreeMultimap<std::string, Movie*>;
    directorMap = new TreeMultimap<std::string, Movie*>;
    actorMap = new TreeMultimap<std::string, Movie*>;
    genreMap = new TreeMultimap<std::string, Movie*>;
    moviePointers.clear();
}

MovieDatabase::~MovieDatabase()
{
    for (Movie* movie : moviePointers) delete movie;
    delete movieMap;
    delete directorMap;
    delete actorMap;
    delete genreMap;
}

bool MovieDatabase::load(const string& filename)
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
        string id = line;
        string title;
        getline(infile, title);
        string year;
        getline(infile, year);
        string directors;
        getline(infile, directors);
        
        vector<string> directorList;
        stringstream ss(directors);
        while(ss.good())
        {
            string director;
            getline(ss, director, ',');
            directorList.push_back(director);
        }
        
        string actors;
        getline(infile, actors);
        
        vector<string> actorList;
        stringstream ss2(actors);
        while (ss2.good())
        {
            string actor;
            getline(ss2, actor, ',');
            actorList.push_back(actor);
        }
        
        string genres;
        getline(infile, genres);
        
        vector<string> genreList;
        stringstream ss3(genres);
        while(ss3.good())
        {
            string genre;
            getline(ss3, genre, ',');
            genreList.push_back(genre);
        }
        
        float rating;
        infile >> rating;
        
        Movie* movie = new Movie(id, title, year, directorList, actorList, genreList, rating);
        moviePointers.push_back(movie);
        movieMap->insert(id, movie);
        for (int i = 0; i < directorList.size(); i++)
        {
            directorMap->insert(directorList[i], movie);
        }
        for (int i = 0; i < actorList.size(); i++)
        {
            actorMap->insert(actorList[i], movie);
        }
        for (int i = 0; i < genreList.size(); i++)
        {
            genreMap->insert(genreList[i], movie);
        }
    }
    return true;
}

Movie* MovieDatabase::get_movie_from_id(const string& id) const
{
    return movieMap->find(id).get_value();
}

vector<Movie*> MovieDatabase::get_movies_with_director(const string& director) const
{
    vector<Movie*> movies;
    for (TreeMultimap<string, Movie*>::Iterator it = directorMap->find(director); it.is_valid(); it.advance())
    {
        movies.push_back(it.get_value());
    }
    return movies;
}

vector<Movie*> MovieDatabase::get_movies_with_actor(const string& actor) const
{
    vector<Movie*> movies;
    for (TreeMultimap<string, Movie*>::Iterator it = actorMap->find(actor); it.is_valid(); it.advance())
    {
        movies.push_back(it.get_value());
    }
    return movies;
}

vector<Movie*> MovieDatabase::get_movies_with_genre(const string& genre) const
{
    vector<Movie*> movies;
    for (TreeMultimap<string, Movie*>::Iterator it = genreMap->find(genre); it.is_valid(); it.advance())
    {
        movies.push_back(it.get_value());
    }
    return movies;
}
