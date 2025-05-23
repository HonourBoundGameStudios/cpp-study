#ifndef MOVIE_H
#define MOVIE_H

#include <string>

class Movie
{
public:
    Movie() = default;
    explicit Movie(const char* str, float rating = 0) : title(str), rating(rating){};
    const std::string & GetTitle() const { return title; }
    float GetRating() const { return rating; }

private:
    std::string title;
    float rating = 0;
};
#endif
