#include "Recommender.h"
#include "UserDatabase.h"
#include "MovieDatabase.h"

#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <iostream>
using namespace std;

Recommender::Recommender(const UserDatabase& user_database,
                         const MovieDatabase& movie_database) : udb(user_database), mdb(movie_database)
{
    recommendations.clear();
}

vector<MovieAndRank> Recommender::recommend_movies(const string& user_email, int movie_count)
{
    User* user = udb.get_user_from_email(user_email);
    vector<string> history = user->get_watch_history();
    unordered_set<string> watched(history.begin(), history.end());
    vector<string> directors;
    vector<string> actors;
    vector<string> genres;
    
    for (int i = 0; i < history.size(); i++)
    {
        Movie* movie = mdb.get_movie_from_id(history[i]);
        vector<string> directors2 = movie->get_directors();
        vector<string> actors2 = movie->get_actors();
        vector<string> genres2 = movie->get_genres();

        directors.insert(directors.end(), directors2.begin(), directors2.end());
        actors.insert(actors.end(), actors2.begin(), actors2.end());
        genres.insert(genres.end(), genres2.begin(), genres2.end());
    }
    
    for (int i = 0; i < directors.size(); i++)
    {
        vector<Movie*> movies = mdb.get_movies_with_director(directors[i]);
        for (int j = 0; j < movies.size(); j++)
        {
            if (watched.find(movies[j]->get_id()) != watched.end()) continue;
            unordered_map<Movie*, int>::iterator p = scoreMap.find(movies[j]);
            if (p != scoreMap.end())
            {
                p->second += 20;
            } else scoreMap.insert({movies[j], 20});
        }
    }

    for (int i = 0; i < actors.size(); i++)
    {
        vector<Movie*> movies = mdb.get_movies_with_actor(actors[i]);
        for (int j = 0; j < movies.size(); j++)
        {
            if (watched.find(movies[j]->get_id()) != watched.end()) continue;
            unordered_map<Movie*, int>::iterator p = scoreMap.find(movies[j]);
            if (p != scoreMap.end())
            {
                p->second += 30;
            } else scoreMap.insert({movies[j], 30});
        }
    }

    for (int i = 0; i < genres.size(); i++)
    {
        vector<Movie*> movies = mdb.get_movies_with_genre(genres[i]);
        for (int j = 0; j < movies.size(); j++)
        {
            if (watched.find(movies[j]->get_id()) != watched.end()) continue;
            unordered_map<Movie*, int>::iterator p = scoreMap.find(movies[j]);
            if (p != scoreMap.end())
            {
                p->second++;
            } else scoreMap.insert({movies[j], 1});
        }
    }
    
    vector<pair<Movie*, int>> reverseMap;
    for (auto itr = scoreMap.begin(); itr != scoreMap.end(); ++itr)
        reverseMap.push_back(*itr);

    sort(reverseMap.begin(), reverseMap.end(), [=](pair<Movie*, int>& a, std::pair<Movie*, int>& b)
    {
        if (a.second == b.second)
        {
            if (a.first->get_rating() == b.first->get_rating())
                return a.first->get_title() > b.first->get_title();
            return a.first->get_rating() < b.first->get_rating();
        }
        return a.second < b.second;
    });
    
    int count = movie_count;
    for (vector<pair<Movie*, int>>::reverse_iterator p = reverseMap.rbegin(); p != reverseMap.rend(); p++)
    {
        if (count == 0) break;
        MovieAndRank movieRank(p->first->get_id(), p->second);
        recommendations.push_back(movieRank);
        count--;
    }
    return recommendations;
}
