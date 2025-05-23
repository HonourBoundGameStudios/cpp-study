#ifndef TMDBSERVICEPROVIDER_H
#define TMDBSERVICEPROVIDER_H
#include <cstdint>
#include <string>
#include <utility>

class TMDBServiceProvider
{
public:
    explicit TMDBServiceProvider(std::string apiKey): apiKey(std::move(apiKey)) {}
    static std::string MakeHttpGetRequest(const std::string& url);

    [[nodiscard]] std::string GetMovieDetails(const std::string& movieId) const
    {
        // Construct the URL for the API request
        std::string url = BASE_URL + "movie/" + movieId + "?api_key=" + apiKey;

        // Make the HTTP request and return the response
        // (Assuming you have a function to make HTTP requests)
        return MakeHttpGetRequest(url);
    }

    [[nodiscard]] std::string GetPopularMovies(uint32_t page) const
    {
        return MakeHttpGetRequest("http://api.themoviedb.org/3/movie/popular?api_key=" + apiKey + "&language=en-US&page=" + std::to_string(page));
    }

    [[nodiscard]] std::string GetNowPlayingMovies(uint32_t page) const
    {
        return MakeHttpGetRequest("http://api.themoviedb.org/3/movie/now_playing?api_key=" + apiKey + "&language=en-US&page="+ std::to_string(page));
    }

private:
        std::string apiKey;
        const std::string BASE_URL = "https://api.themoviedb.org/3/";
        const std::string imageBaseUrl = "https://image.tmdb.org/t/p/w500";
};

#endif
