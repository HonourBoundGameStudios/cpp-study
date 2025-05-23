#ifndef MOVIE_DATABASE_H
#define MOVIE_DATABASE_H

#include <list>

#include "Movie.h"
#include "json/single_include/nlohmann/json.hpp"

using MovieList = std::list<Movie>;

class MovieDatabase
{
public:
    MovieDatabase() = default;

    void PopulateWithFakeData();

    const MovieList &GetMovies() const
    {
        return movies;
    }

    MovieList GetMoviesSortedByTitle() const
    {
        auto sortedMovies = movies;

        sortedMovies.sort([](const Movie &a, const Movie &b) {
            return a.GetTitle() < b.GetTitle();
        });

        return sortedMovies;
    }

    MovieList GetMoviesSortedByRating() const
    {
        auto sortedMovies = movies;

        sortedMovies.sort([](const Movie &a, const Movie &b) {
            return a.GetRating() > b.GetRating();
        });

        return sortedMovies;
    }

    void AddMovie(const std::string& title, float rating);

private:

    MovieList movies;
};

#endif
