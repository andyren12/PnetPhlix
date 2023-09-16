#ifndef RECOMMENDER_INCLUDED
#define RECOMMENDER_INCLUDED

#include <string>
#include <vector>
#include <unordered_map>
#include "User.h"
#include "Movie.h"
#include "MovieDatabase.h"
#include "UserDatabase.h"

struct MovieAndRank
{
    MovieAndRank(const std::string& id, int score)
     : movie_id(id), compatibility_score(score)
    {}

    std::string movie_id;
    int compatibility_score;
};

class Recommender
{
  public:
    Recommender(const UserDatabase& user_database,
                const MovieDatabase& movie_database);
    std::vector<MovieAndRank> recommend_movies(const std::string& user_email,
                                               int movie_count);

  private:
    std::unordered_map<Movie*, int> scoreMap;
    std::vector<MovieAndRank> recommendations;
    const MovieDatabase& mdb;
    const UserDatabase& udb;
};

#endif // RECOMMENDER_INCLUDED
