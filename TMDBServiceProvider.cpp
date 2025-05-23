#include "TMDBServiceProvider.h"

#include <iostream>

#include "HTTPRequest/include/HTTPRequest.hpp"


std::string TMDBServiceProvider::MakeHttpGetRequest(const std::string& url)
{
    try
    {
        http::Request request{url};
        const auto response = request.send("GET");
        return std::string{response.body.begin(), response.body.end()};
    }
    catch (const std::exception& e)
    {
        std::cerr << "Request failed, error: " << e.what() << '\n';
    }

    return {};
}
