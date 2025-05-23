#include "MovieDatabase.h"

void MovieDatabase::AddMovie(const std::string& title, float rating)
{
    movies.push_back(Movie{title.c_str(), rating});
}

void MovieDatabase::PopulateWithFakeData()
{
    movies = {
        Movie{"The Shawshank Redemption", 9.3f},
        Movie{"The Godfather", 9.2f},
        Movie{"The Dark Knight", 9.4f},
        Movie{"Pulp Fiction", 9.0f},
        Movie{"Forrest Gump", 8.9f},
        Movie{"Inception", 9.1f},
        Movie{"Fight Club", 8.8f},
        Movie{"The Matrix", 9.0f},
        Movie{"Goodfellas", 9.1f},
        Movie{"The Lord of the Rings: The Return of the King", 9.3f},
    };
}
