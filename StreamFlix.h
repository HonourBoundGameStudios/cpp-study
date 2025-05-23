#ifndef STREAM_FLIX_H
#define STREAM_FLIX_H
#include "MovieDatabase.h"

class StreamFlix
{
public:
    StreamFlix() = default;
    static void Run();
    static void Shutdown();

    static void DisplayMovies(const std::string& title, const MovieDatabase& movieDatabase);
    static void DisplayMoviesSortedByTitle(const std::string & title, const MovieDatabase& movieDatabase);
    static void DisplayMoviesSortedByRating(const std::string& title, const MovieDatabase& movieDatabase);
};

#endif
