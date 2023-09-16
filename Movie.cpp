#include "Movie.h"

#include <string>
#include <vector>
using namespace std;

Movie::Movie(const string& id, const string& title, const string& release_year, const vector<string>& directors, const vector<string>& actors, const vector<string>& genres, float rating) : id(id), name(title), year(release_year), directors(directors), actors(actors), genres(genres), rating(rating) { }

string Movie::get_id() const
{
    return id;  // Replace this line with correct code.
}

string Movie::get_title() const
{
    return name;  // Replace this line with correct code.
}

string Movie::get_release_year() const
{
    return year;  // Replace this line with correct code.
}

float Movie::get_rating() const
{
    return rating;  // Replace this line with correct code.
}

vector<string> Movie::get_directors() const
{
    return directors;  // Replace this line with correct code.
}

vector<string> Movie::get_actors() const
{
    return actors;  // Replace this line with correct code.
}

vector<string> Movie::get_genres() const
{
    return genres;  // Replace this line with correct code.
}
