#include "StreamFlix.h"

#include <iostream>
#include <mutex>
#include <regex>
#include <thread>
#include <bits/ostream.tcc>
#include <TMDBServiceProvider.h>
#include <json/single_include/nlohmann/json.hpp>

#include "MovieDatabase.h"

using json = nlohmann::json;

void StreamFlix::DisplayMovies(const std::string& title, const MovieDatabase& movieDatabase)
{
    std::cout << "______________________________________________________" << std::endl;
    std::cout << title << std::endl;
    std::cout << "______________________________________________________" << std::endl;

    const MovieList& movies = movieDatabase.GetMovies();

    for (auto& movie : movies)
    {
        std::cout << movie.GetTitle() << " | " << movie.GetRating() << std::endl;
    }
}

void StreamFlix::DisplayMoviesSortedByTitle(const std::string& title, const MovieDatabase& movieDatabase)
{
    std::cout << "______________________________________________________" << std::endl;
    std::cout << title << " (Sorted Alphabetically)" << std::endl;
    std::cout << "______________________________________________________" << std::endl;

    const MovieList& moviesSortedByTitle = movieDatabase.GetMoviesSortedByTitle();

    for (auto& movie : moviesSortedByTitle)
    {
        std::cout << movie.GetTitle() << " | " << movie.GetRating() << std::endl;
    }
}

void StreamFlix::DisplayMoviesSortedByRating(const std::string& title, const MovieDatabase& movieDatabase)
{
    std::cout << "______________________________________________________" << std::endl;
    std::cout << title << " (Sorted by rating)" << std::endl;
    std::cout << "______________________________________________________" << std::endl;

    const MovieList& moviesSortedByRating = movieDatabase.GetMoviesSortedByRating();

    for (auto& movie : moviesSortedByRating)
    {
        std::cout << movie.GetTitle() << " | " << movie.GetRating() << std::endl;
    }
}

void StreamFlix::Run()
{
    MovieDatabase popularMovies;
    MovieDatabase nowPlayingMovies;

    static std::string TMDB_API_KEY = "";

    TMDBServiceProvider tmdbServiceProvider(TMDB_API_KEY);

    json popularMovieJson;
    json nowPlayingMovieJson;

    std::mutex coutMutex;

    std::thread popularThread([&]
    {
        {
            std::lock_guard lock(coutMutex);
            std::cout << "Popular movies thread ID: " << std::this_thread::get_id() << std::endl;
        }

        for (int i = 1; i <= 5; ++i)
        {
            auto popularMoviesJsonString = tmdbServiceProvider.GetPopularMovies(i);
            popularMovieJson = json::parse(popularMoviesJsonString);

            for (auto& movie : popularMovieJson["results"])
            {
                popularMovies.AddMovie(movie["title"], movie["vote_average"]);
            }
        }
    });

    std::thread nowPlayingThread([&]
    {
        {
            std::lock_guard lock(coutMutex);
            std::cout << "Now playing movies thread ID: " << std::this_thread::get_id() << std::endl;
        }

        auto nowPlayingMoviesJsonString = tmdbServiceProvider.GetNowPlayingMovies(1);
        nowPlayingMovieJson = json::parse(nowPlayingMoviesJsonString);

        for (auto& movie : nowPlayingMovieJson["results"])
        {
            nowPlayingMovies.AddMovie(movie["title"], movie["vote_average"]);
        }
    });

    popularThread.join();
    nowPlayingThread.join();

    DisplayMovies("POPULAR", popularMovies);
    DisplayMoviesSortedByTitle("POPULAR", popularMovies);
    DisplayMoviesSortedByRating("POPULAR", popularMovies);

    DisplayMovies("NOW PLAYING", nowPlayingMovies);
    DisplayMoviesSortedByTitle("NOW PLAYING", nowPlayingMovies);
    DisplayMoviesSortedByRating("NOW PLAYING", nowPlayingMovies);

    std::regex pattern(".*[dD]es.*");

    MovieList matchingMovies;
    const auto& movies = popularMovies.GetMovies();

    std::copy_if(movies.begin(), movies.end(), std::back_inserter(matchingMovies),
                 [&pattern](const Movie& movie)
                 {
                     return std::regex_search(movie.GetTitle(), pattern);
                 });

    for (auto& movie : matchingMovies)
    {
        std::cout << "Matching movie: " << movie.GetTitle() << std::endl;
    }
}

void StreamFlix::Shutdown()
{
    std::cout << std::endl << "Bye Bye" << std::endl;
}
