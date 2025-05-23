Okay, this is a fun challenge! Listing *all* features is a monumental task, as there are many small library additions and tweaks. I'll focus on the most impactful language and standard library features introduced in C++11, C++14, and C++17, with examples geared towards a Video-on-Demand (VoD) application.

Let's imagine we're building components for "StreamFlix," our new VoD service.

---

## C++11: The "Modern C++" Revolution

C++11 was a massive update.

1.  **`auto` Keyword (Type Inference)**
    *   Description: Allows the compiler to deduce the type of a variable from its initializer.
    *   StreamFlix Example: Iterating through a list of movies.
        ```cpp
        #include <vector>
        #include <string>
        #include <iostream>

        struct Movie {
            std::string title;
            int releaseYear;
        };

        int main() {
            std::vector<Movie> catalog = {{"Inception", 2010}, {"The Matrix", 1999}};
            auto it = catalog.begin(); // it is std::vector<Movie>::iterator
            std::cout << "First movie: " << it->title << std::endl;

            auto firstMovie = catalog[0]; // firstMovie is Movie
            std::cout << "Also: " << firstMovie.title << std::endl;
            return 0;
        }
        ```

2.  **Range-Based `for` Loops**
    *   Description: Simplifies iterating over elements in a range (e.g., containers, arrays).
    *   StreamFlix Example: Displaying all movie titles in a user's watchlist.
        ```cpp
        #include <vector>
        #include <string>
        #include <iostream>

        struct Movie { std::string title; };

        int main() {
            std::vector<Movie> watchlist = {{"Pulp Fiction"}, {"Parasite"}};
            std::cout << "Your Watchlist:\n";
            for (const auto& movie : watchlist) { // auto& is efficient
                std::cout << "- " << movie.title << std::endl;
            }
            return 0;
        }
        ```

3.  **Lambda Expressions**
    *   Description: Allow defining anonymous functions (closures) inline.
    *   StreamFlix Example: Sorting movies by release year or finding a movie by title.
        ```cpp
        #include <vector>
        #include <string>
        #include <algorithm>
        #include <iostream>

        struct Movie {
            std::string title;
            int releaseYear;
        };

        int main() {
            std::vector<Movie> catalog = {{"Inception", 2010}, {"The Matrix", 1999}};

            // Sort by release year
            std::sort(catalog.begin(), catalog.end(),
                      [](const Movie& a, const Movie& b) {
                          return a.releaseYear < b.releaseYear;
                      });

            std::cout << "Sorted by year:\n";
            for (const auto& movie : catalog) {
                std::cout << movie.title << " (" << movie.releaseYear << ")\n";
            }

            // Find a specific movie
            std::string targetTitle = "Inception";
            auto foundMovieIt = std::find_if(catalog.begin(), catalog.end(),
                [&targetTitle](const Movie& m) { // Capture targetTitle by reference
                    return m.title == targetTitle;
                });

            if (foundMovieIt != catalog.end()) {
                std::cout << "Found: " << foundMovieIt->title << std::endl;
            }
            return 0;
        }
        ```

4.  **`nullptr`**
    *   Description: A type-safe null pointer constant, replacing `NULL` or `0`.
    *   StreamFlix Example: Checking if a user's profile data has been loaded.
        ```cpp
        #include <iostream>
        struct UserProfile { std::string username; /* ... */ };

        UserProfile* getCurrentUserProfile() {
            // In a real app, this would fetch data
            return nullptr; // Simulate no user logged in
        }

        int main() {
            UserProfile* profile = getCurrentUserProfile();
            if (profile == nullptr) {
                std::cout << "No user logged in. Please sign in to StreamFlix." << std::endl;
            } else {
                std::cout << "Welcome, " << profile->username << "!" << std::endl;
            }
            return 0;
        }
        ```

5.  **Smart Pointers (`std::unique_ptr`, `std::shared_ptr`, `std::weak_ptr`)**
    *   Description: Manage dynamic memory automatically, preventing memory leaks.
    *   StreamFlix Example: `unique_ptr` for a video decoder, `shared_ptr` for movie metadata shared across UI elements.
        ```cpp
        #include <memory>
        #include <string>
        #include <iostream>
        #include <vector>

        class VideoDecoder { // Base class for decoders
        public:
            virtual ~VideoDecoder() = default;
            virtual void decodeFrame() = 0;
        };

        class H264Decoder : public VideoDecoder {
        public:
            H264Decoder() { std::cout << "H264Decoder created.\n"; }
            ~H264Decoder() { std::cout << "H264Decoder destroyed.\n"; }
            void decodeFrame() override { std::cout << "Decoding H.264 frame...\n"; }
        };

        struct MovieMetadata {
            std::string title;
            std::string description;
            MovieMetadata(std::string t, std::string d) : title(t), description(d) {
                 std::cout << "Metadata for '" << title << "' created.\n";
            }
            ~MovieMetadata() { std::cout << "Metadata for '" << title << "' destroyed.\n"; }
        };

        int main() {
            // Unique ownership: only one part of the code owns the decoder
            std::unique_ptr<VideoDecoder> decoder = std::make_unique<H264Decoder>();
            if (decoder) {
                decoder->decodeFrame();
            }
            // decoder goes out of scope, H264Decoder is automatically destroyed

            // Shared ownership: metadata can be accessed by multiple parts
            std::shared_ptr<MovieMetadata> inceptionData =
                std::make_shared<MovieMetadata>("Inception", "A mind-bending thriller.");

            std::vector<std::shared_ptr<MovieMetadata>> uiElementsData;
            uiElementsData.push_back(inceptionData); // UIs share the same data
            uiElementsData.push_back(inceptionData);

            std::cout << "Accessing from UI 1: " << uiElementsData[0]->title << std::endl;
            // Metadata destroyed when all shared_ptrs go out of scope.
            return 0;
        }
        ```

6.  **Move Semantics (Rvalue References, `std::move`)**
    *   Description: Allows efficient transfer of resources from one object to another, avoiding costly copies.
    *   StreamFlix Example: Efficiently assigning a large downloaded video buffer to a player.
        ```cpp
        #include <vector>
        #include <string>
        #include <utility> // For std::move
        #include <iostream>

        class VideoBuffer {
        public:
            std::vector<unsigned char> data;
            VideoBuffer(size_t size) : data(size) {
                std::cout << "VideoBuffer of size " << size << " constructed.\n";
            }
            VideoBuffer(const VideoBuffer& other) : data(other.data) {
                std::cout << "VideoBuffer copied.\n";
            }
            VideoBuffer(VideoBuffer&& other) noexcept : data(std::move(other.data)) {
                std::cout << "VideoBuffer moved.\n";
            }
            // ... other members
        };

        VideoBuffer downloadVideoChunk() {
            VideoBuffer chunk(1024 * 1024 * 10); // 10MB chunk
            // Simulate filling buffer with data
            return chunk; // Return by value, eligible for move
        }

        int main() {
            VideoBuffer playerBuffer(0);
            std::cout << "Before assignment:\n";
            playerBuffer = downloadVideoChunk(); // Move assignment will be used
            std::cout << "After assignment: Player buffer size " << playerBuffer.data.size() << "\n";
            return 0;
        }
        ```

7.  **`constexpr` - Compile-time Expressions**
    *   Description: Allows expressions and functions to be evaluated at compile time.
    *   StreamFlix Example: Defining a maximum number of recommendations or a default bitrate.
        ```cpp
        #include <iostream>

        constexpr int MAX_RECOMMENDATIONS = 10;
        constexpr int DEFAULT_HD_BITRATE_KBPS = 5000;

        constexpr int calculateBufferSize(int seconds, int bitrate_kbps) {
            return (seconds * bitrate_kbps * 1024) / 8; // Bytes
        }

        int main() {
            int recommendations[MAX_RECOMMENDATIONS]; // Array size known at compile time
            std::cout << "Max recommendations: " << MAX_RECOMMENDATIONS << std::endl;
            std::cout << "Default HD Bitrate: " << DEFAULT_HD_BITRATE_KBPS << " kbps\n";

            constexpr int bufferFor10SecHD = calculateBufferSize(10, DEFAULT_HD_BITRATE_KBPS);
            std::cout << "Buffer for 10s HD video: " << bufferFor10SecHD << " bytes\n";
            return 0;
        }
        ```

8.  **Initializer Lists (`std::initializer_list`)**
    *   Description: Provides a uniform way to initialize objects with a list of values.
    *   StreamFlix Example: Initializing a list of supported video formats.
        ```cpp
        #include <vector>
        #include <string>
        #include <initializer_list>
        #include <iostream>

        class SupportedCodecs {
        public:
            std::vector<std::string> codecs;
            SupportedCodecs(std::initializer_list<std::string> list) : codecs(list) {}

            void print() const {
                std::cout << "Supported Codecs by StreamFlix:\n";
                for(const auto& codec : codecs) {
                    std::cout << "- " << codec << std::endl;
                }
            }
        };

        int main() {
            SupportedCodecs ourCodecs = {"H.264", "H.265", "AV1", "VP9"};
            ourCodecs.print();
            return 0;
        }
        ```

9.  **`static_assert` - Compile-time Assertions**
    *   Description: Perform assertions at compile time; if false, compilation fails.
    *   StreamFlix Example: Ensuring a struct used for network messages has a specific size.
        ```cpp
        #include <cstdint>

        // Packet structure for sending playback commands (e.g., play, pause)
        struct PlaybackCommand {
            uint8_t command_id;
            uint32_t timestamp;
            uint16_t sequence_number;
            // char padding[1]; // Uncomment to make it fail
        };

        // Ensure our network protocol structure is exactly 8 bytes for compatibility.
        static_assert(sizeof(PlaybackCommand) == 7, "PlaybackCommand size is not 7 bytes!");

        int main() {
            // Code using PlaybackCommand...
            return 0;
        }
        ```

10. **Type Traits (`<type_traits>`)**
    *   Description: Provides compile-time information about types.
    *   StreamFlix Example: Generic function to serialize metadata, behaving differently for numeric vs. string types.
        ```cpp
        #include <type_traits>
        #include <string>
        #include <iostream>

        template<typename T>
        void serialize_metadata_field(const std::string& key, const T& value) {
            if constexpr (std::is_integral_v<T>) { // C++17 if constexpr, for C++11 use SFINAE or tag dispatch
                std::cout << "Serializing integral field '" << key << "': " << value << std::endl;
            } else if constexpr (std::is_same_v<T, std::string>) {
                std::cout << "Serializing string field '" << key << "': \"" << value << "\"\n";
            } else {
                std::cout << "Serializing unknown field type for '" << key << "'\n";
            }
        }
        // C++11 way (using SFINAE for overload selection)
        template<typename T>
        typename std::enable_if<std::is_integral<T>::value>::type
        serialize_field_cpp11(const std::string& key, const T& value) {
            std::cout << "(C++11) Serializing integral field '" << key << "': " << value << std::endl;
        }

        template<typename T>
        typename std::enable_if<std::is_same<T, std::string>::value>::type
        serialize_field_cpp11(const std::string& key, const T& value) {
            std::cout << "(C++11) Serializing string field '" << key << "': \"" << value << "\"\n";
        }


        int main() {
            serialize_metadata_field("episode_count", 24);
            serialize_metadata_field("genre", std::string("Sci-Fi"));
            serialize_field_cpp11("rating_stars", 5);
            serialize_field_cpp11("director_name", std::string("Nolan"));
            return 0;
        }
        ```

11. **Variadic Templates**
    *   Description: Templates that can take a variable number of arguments.
    *   StreamFlix Example: A logging function that can accept various arguments.
        ```cpp
        #include <iostream>
        #include <string>

        void log_event(const std::string& event_type) {
            std::cout << "[LOG] Event: " << event_type << std::endl;
        }

        template<typename T, typename... Args>
        void log_event(const std::string& event_type, T first_arg, Args... rest_args) {
            std::cout << "[LOG] Event: " << event_type << ", Arg: " << first_arg;
            // Simplified: just print first arg, then indicate more. A real one would print all.
            if constexpr (sizeof...(rest_args) > 0) {
                 std::cout << " (+ " << sizeof...(rest_args) << " more args)" << std::endl;
                 // Recursive call could be: log_event_details(rest_args...);
            } else {
                std::cout << std::endl;
            }
        }
        // For full printing:
        void print_args() { std::cout << std::endl; } // Base case for recursion

        template<typename T, typename... Args>
        void print_args(T first, Args... rest) {
            std::cout << first << " ";
            print_args(rest...); // Recursive call
        }

        template<typename... Args>
        void log_event_full(const std::string& event_type, Args... args) {
            std::cout << "[FULL LOG] Event: " << event_type << " | Data: ";
            print_args(args...);
        }


        int main() {
            log_event("UserLogin", "user123");
            log_event("PlaybackStarted", "movie_id_789", "1080p", 0.75 /* volume */);
            log_event_full("PlaybackError", "E_NETWORK_TIMEOUT", "stream_url_xyz", 5000 /*ms*/);
            return 0;
        }
        ```

12. **Threading Library (`<thread>`, `<mutex>`, `<future>`, etc.)**
    *   Description: Standardized support for multithreading.
    *   StreamFlix Example: Downloading a video in a background thread.
        ```cpp
        #include <thread>
        #include <mutex>
        #include <chrono>
        #include <iostream>
        #include <string>

        std::mutex cout_mutex; // To protect std::cout

        void downloadVideo(std::string videoId, int* progress) {
            {
                std::lock_guard<std::mutex> lock(cout_mutex);
                std::cout << "Starting download for video: " << videoId << std::endl;
            }
            for (int i = 0; i <= 100; i += 10) {
                std::this_thread::sleep_for(std::chrono::milliseconds(200));
                *progress = i;
                {
                    std::lock_guard<std::mutex> lock(cout_mutex);
                    std::cout << "Download " << videoId << ": " << *progress << "%" << std::endl;
                }
            }
            {
                std::lock_guard<std::mutex> lock(cout_mutex);
                std::cout << "Download complete for video: " << videoId << std::endl;
            }
        }

        int main() {
            int downloadProgress = 0;
            std::thread downloaderThread(downloadVideo, "matrix_4_trailer", &downloadProgress);

            // Main thread can do other things, like update UI
            while (downloadProgress < 100) {
                {
                    std::lock_guard<std::mutex> lock(cout_mutex);
                    std::cout << "Main thread: Checking progress... " << downloadProgress << "%" << std::endl;
                }
                std::this_thread::sleep_for(std::chrono::milliseconds(500));
            }

            downloaderThread.join(); // Wait for the download thread to finish
            std::cout << "Main thread: Downloader finished." << std::endl;
            return 0;
        }
        ```

13. **`std::chrono` - Time Utilities**
    *   Description: Library for handling time durations, time points, and clocks.
    *   StreamFlix Example: Measuring how long a video segment takes to buffer.
        ```cpp
        #include <chrono>
        #include <thread>
        #include <iostream>

        void simulateBufferSegment() {
            std::this_thread::sleep_for(std::chrono::milliseconds(150));
        }

        int main() {
            auto startTime = std::chrono::high_resolution_clock::now();

            simulateBufferSegment(); // Simulate work

            auto endTime = std::chrono::high_resolution_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::microseconds>(endTime - startTime);

            std::cout << "Buffering segment took: " << duration.count() << " microseconds." << std::endl;
            return 0;
        }
        ```

14. **`std::function` - Polymorphic Function Wrapper**
    *   Description: A general-purpose type that can store, copy, and invoke any callable target (function, lambda, bind expression, functor).
    *   StreamFlix Example: Setting a callback for when a "Play" button is clicked.
        ```cpp
        #include <functional>
        #include <string>
        #include <iostream>

        struct Movie { std::string title; };

        class PlayButton {
        public:
            std::function<void(const Movie&)> onClick; // Callback

            void press(const Movie& movieToPlay) {
                if (onClick) {
                    onClick(movieToPlay);
                }
            }
        };

        void startPlayback(const Movie& movie) {
            std::cout << "Starting playback for: " << movie.title << std::endl;
        }

        int main() {
            PlayButton button;
            Movie selectedMovie = {"Interstellar"};

            button.onClick = startPlayback; // Assign a free function
            button.press(selectedMovie);

            button.onClick = [&](const Movie& m) { // Assign a lambda
                std::cout << "Lambda: Now playing '" << m.title << "' on your big screen!" << std::endl;
            };
            button.press(selectedMovie);
            return 0;
        }
        ```

15. **`std::tuple`**
    *   Description: A fixed-size collection of heterogeneous values.
    *   StreamFlix Example: Returning multiple details about a movie from a function.
        ```cpp
        #include <tuple>
        #include <string>
        #include <iostream>

        // Returns: Title, Director, Release Year
        std::tuple<std::string, std::string, int> getMovieDetails(int movieId) {
            if (movieId == 1) {
                return std::make_tuple("Blade Runner 2049", "Denis Villeneuve", 2017);
            }
            return std::make_tuple("Unknown Movie", "N/A", 0);
        }

        int main() {
            auto movieInfo = getMovieDetails(1);
            std::cout << "Title: " << std::get<0>(movieInfo) << std::endl;
            std::cout << "Director: " << std::get<1>(movieInfo) << std::endl;
            std::cout << "Year: " << std::get<2>(movieInfo) << std::endl;
            return 0;
        }
        ```

16. **Regular Expressions (`<regex>`)**
    *   Description: Support for regular expression matching.
    *   StreamFlix Example: Validating a user's search query format.
        ```cpp
        #include <regex>
        #include <string>
        #include <iostream>

        bool isValidSearchQuery(const std::string& query) {
            // Example: Allow alphanumeric, spaces, hyphens. Min 3 chars.
            std::regex query_regex("^[a-zA-Z0-9\\s-]{3,}$");
            return std::regex_match(query, query_regex);
        }

        int main() {
            std::string searchQuery1 = "Sci-Fi movies";
            std::string searchQuery2 = "S!"; // Too short, invalid char

            std::cout << "'" << searchQuery1 << "' is valid: "
                      << (isValidSearchQuery(searchQuery1) ? "true" : "false") << std::endl;
            std::cout << "'" << searchQuery2 << "' is valid: "
                      << (isValidSearchQuery(searchQuery2) ? "true" : "false") << std::endl;
            return 0;
        }
        ```

17. **`override` and `final` Specifiers**
    *   Description: `override` ensures a virtual function is indeed overriding a base class method. `final` prevents further overriding of a virtual function or inheritance from a class.
    *   StreamFlix Example: Defining different types of user accounts.
        ```cpp
        #include <string>
        #include <iostream>

        class UserAccount {
        public:
            virtual std::string getSubscriptionType() const { return "Basic"; }
            virtual void displayFeatures() const { std::cout << "Basic features: SD streaming.\n"; }
            virtual ~UserAccount() = default;
        };

        class PremiumUserAccount final : public UserAccount { // 'final' class cannot be inherited from
        public:
            std::string getSubscriptionType() const override { return "Premium"; } // 'override' check
            void displayFeatures() const override {
                std::cout << "Premium features: 4K streaming, downloads.\n";
            }
        };

        // class UltraPremiumUserAccount : public PremiumUserAccount {}; // ERROR: PremiumUserAccount is final

        int main() {
            UserAccount basicUser;
            PremiumUserAccount premiumUser;
            std::cout << "Basic user: " << basicUser.getSubscriptionType() << std::endl;
            basicUser.displayFeatures();
            std::cout << "Premium user: " << premiumUser.getSubscriptionType() << std::endl;
            premiumUser.displayFeatures();
            return 0;
        }
        ```

18. **`default` and `delete` for Special Member Functions**
    *   Description: Explicitly request compiler-generated special member functions (`=default`) or prevent their generation (`=delete`).
    *   StreamFlix Example: Making a `VideoStream` class non-copyable.
        ```cpp
        #include <iostream>
        #include <string>

        class VideoStream {
        public:
            std::string streamUrl;
            VideoStream(std::string url) : streamUrl(std::move(url)) {
                std::cout << "VideoStream for " << streamUrl << " opened.\n";
            }
            ~VideoStream() { std::cout << "VideoStream for " << streamUrl << " closed.\n"; }

            // Prevent copying, as a stream is a unique resource
            VideoStream(const VideoStream&) = delete;
            VideoStream& operator=(const VideoStream&) = delete;

            // Allow moving
            VideoStream(VideoStream&& other) noexcept : streamUrl(std::move(other.streamUrl)) {
                 std::cout << "VideoStream for " << streamUrl << " moved.\n";
                 other.streamUrl.clear(); // Ensure the moved-from object is in a valid state
            }
            VideoStream& operator=(VideoStream&& other) noexcept {
                if (this != &other) {
                    streamUrl = std::move(other.streamUrl);
                    other.streamUrl.clear();
                    std::cout << "VideoStream for " << streamUrl << " move-assigned.\n";
                }
                return *this;
            }
        };

        int main() {
            VideoStream stream1("rtsp://server/movie1");
            // VideoStream stream2 = stream1; // COMPILE ERROR: copy constructor is deleted
            VideoStream stream3 = std::move(stream1); // OK: move constructor
            std::cout << "Stream1 URL after move: " << stream1.streamUrl << std::endl;
            return 0;
        }
        ```

19. **Strongly-Typed Enums (`enum class`)**
    *   Description: Enumerations whose enumerators are scoped and don't implicitly convert to integers.
    *   StreamFlix Example: Defining playback states.
        ```cpp
        #include <iostream>

        enum class PlaybackState { Playing, Paused, Stopped, Buffering, Error };
        enum class VideoQuality { SD, HD, UHD4K };

        void setPlaybackState(PlaybackState state) {
            if (state == PlaybackState::Playing) {
                std::cout << "StreamFlix: Playback started." << std::endl;
            } else if (state == PlaybackState::Paused) {
                std::cout << "StreamFlix: Playback paused." << std::endl;
            }
            // ... other states
        }

        int main() {
            PlaybackState currentState = PlaybackState::Paused;
            // int stateInt = currentState; // COMPILE ERROR: no implicit conversion
            setPlaybackState(currentState);
            setPlaybackState(PlaybackState::Playing);

            VideoQuality quality = VideoQuality::HD;
            // if (currentState == quality) {} // COMPILE ERROR: different enum types
            return 0;
        }
        ```

20. **`noexcept` Specifier and Operator**
    *   Description: `noexcept` specifier indicates a function will not throw exceptions. `noexcept` operator checks if an expression can throw.
    *   StreamFlix Example: Marking a simple getter as non-throwing.
        ```cpp
        #include <string>
        #include <iostream>
        #include <utility> // For std::move

        class MovieDetails {
            std::string m_title;
        public:
            MovieDetails(std::string title) : m_title(std::move(title)) {}
            // This getter is simple and won't throw.
            const std::string& getTitle() const noexcept {
                return m_title;
            }
            // Destructors are implicitly noexcept(true) in C++11 unless specified otherwise or member/base dtors can throw
            ~MovieDetails() noexcept { /* clean up, guaranteed not to throw */ }

            // Example of noexcept operator
            void risky_operation() {} // might throw
            void safe_swap(MovieDetails& other) noexcept(noexcept(std::swap(m_title, other.m_title))) {
                std::swap(m_title, other.m_title); // std::string::swap is noexcept
            }
        };

        int main() {
            MovieDetails movie("Dune");
            std::cout << "Movie: " << movie.getTitle() << std::endl;
            std::cout << "getTitle is noexcept: " << std::boolalpha
                      << noexcept(movie.getTitle()) << std::endl; // true

            MovieDetails otherMovie("Oppenheimer");
            std::cout << "safe_swap is noexcept: " << std::boolalpha
                      << noexcept(movie.safe_swap(otherMovie)) << std::endl; // true
            return 0;
        }
        ```

---

## C++14: Refinements and Conveniences

C++14 built upon C++11, adding mostly smaller features and conveniences.

1.  **Generic Lambdas**
    *   Description: Lambda parameters can be declared with `auto`, making them templates.
    *   StreamFlix Example: A generic "add to favorites" function that works for `Movie` or `TVShow` objects (if they have a `getId()` method).
        ```cpp
        #include <string>
        #include <iostream>
        #include <set>

        struct Movie {
            int id; std::string title;
            int getId() const { return id; }
        };
        struct TVShow {
            int id; std::string name;
            int getId() const { return id; }
        };

        std::set<int> favoriteMovieIds;
        std::set<int> favoriteShowIds;

        int main() {
            auto addToFavorites = [](const auto& mediaItem, std::set<int>& favSet) {
                favSet.insert(mediaItem.getId());
                std::cout << "Added item with ID " << mediaItem.getId() << " to favorites." << std::endl;
            };

            Movie m = {101, "Avatar"};
            TVShow s = {202, "Stranger Things"};

            addToFavorites(m, favoriteMovieIds);
            addToFavorites(s, favoriteShowIds);
            return 0;
        }
        ```

2.  **Lambda Capture Initializers (Generalized Lambda Capture)**
    *   Description: Allows creating new variables within the lambda capture clause, useful for moving objects into lambdas or more complex captures.
    *   StreamFlix Example: Moving a `unique_ptr` to a `DownloadTask` into a lambda for asynchronous execution.
        ```cpp
        #include <memory>
        #include <string>
        #include <iostream>
        #include <functional> // For std::function

        class DownloadTask {
        public:
            std::string url;
            DownloadTask(std::string u) : url(std::move(u)) {
                std::cout << "DownloadTask for " << url << " created.\n";
            }
            void execute() { std::cout << "Executing download for " << url << std::endl; }
            ~DownloadTask() { std::cout << "DownloadTask for " << url << " destroyed.\n"; }
        };

        int main() {
            auto task_ptr = std::make_unique<DownloadTask>("http://streamflix.com/movie.mp4");

            // Move task_ptr into the lambda. The lambda now owns it.
            std::function<void()> async_task = [task = std::move(task_ptr)]() {
                if (task) {
                    task->execute();
                }
            };
            // task_ptr is now nullptr here

            async_task(); // Simulate running it later
            // When async_task (and its captured 'task') is destroyed, DownloadTask is destroyed.
            return 0;
        }
        ```

3.  **`auto` Return Type Deduction for Functions**
    *   Description: The return type of a normal function can be deduced by the compiler from its `return` statements (all return statements must deduce to the same type).
    *   StreamFlix Example: A factory function that creates different types of player UI elements.
        ```cpp
        #include <string>
        #include <iostream>

        struct BasicPlayerUI { void render() { std::cout << "Rendering Basic Player UI\n"; } };
        struct AdvancedPlayerUI { void render() { std::cout << "Rendering Advanced Player UI\n"; } };

        // Note: C++14 auto return type requires all return paths to yield compatible types.
        // This example is slightly contrived; usually used when the return type is complex or templated.
        // For genuinely different types, std::variant (C++17) or a base class pointer would be better.
        auto createPlayerUI_impl(bool useAdvanced) {
            if (useAdvanced) {
                // return AdvancedPlayerUI{}; // This would cause an error if BasicPlayerUI is also returned directly
                                           // as they are different types. We'd need a common base or variant.
                                           // For this simple example, let's assume we're just returning a message.
                return std::string("Advanced UI selected");
            } else {
                return std::string("Basic UI selected");
            }
        }

        // Better example with templates:
        template<typename T>
        auto process_data(T val) { // Return type depends on T
            return val * 2.5; // e.g., if T is int, returns double
        }

        int main() {
            auto ui_type_msg = createPlayerUI_impl(true);
            std::cout << ui_type_msg << std::endl;

            auto result = process_data(10); // result is double
            std::cout << "Processed data: " << result << std::endl;
            return 0;
        }
        ```

4.  **`constexpr` Relaxations**
    *   Description: More constructs are allowed in `constexpr` functions (e.g., local variables, loops, if statements).
    *   StreamFlix Example: A more complex compile-time calculation for optimal streaming segment size.
        ```cpp
        #include <iostream>

        // C++14 allows more complex logic in constexpr functions
        constexpr int calculateOptimalSegmentDuration(int networkSpeedMbps) {
            int durationSec = 2; // Start with a base
            if (networkSpeedMbps < 5) {
                durationSec = 4; // Slower network, larger segments to reduce overhead
            } else if (networkSpeedMbps > 50) {
                durationSec = 1; // Faster network, smaller segments for quicker start
            }
            // Could have loops here too in C++14
            int adjustment = 0;
            for(int i = 0; i < networkSpeedMbps / 10; ++i) {
                if (durationSec > 1) adjustment--; // a bit silly, just for demo
            }
            return durationSec + (adjustment/10);
        }

        int main() {
            constexpr int durationForSlowNet = calculateOptimalSegmentDuration(3);
            constexpr int durationForFastNet = calculateOptimalSegmentDuration(100);

            std::cout << "Optimal segment for 3Mbps: " << durationForSlowNet << "s\n";
            std::cout << "Optimal segment for 100Mbps: " << durationForFastNet << "s\n";
            return 0;
        }
        ```

5.  **`std::make_unique`**
    *   Description: Similar to `std::make_shared`, provides a safe and convenient way to create `std::unique_ptr`s. (Technically a library addition, but very important).
    *   StreamFlix Example: Creating a `unique_ptr` for a `VideoPlayer` instance.
        ```cpp
        #include <memory>
        #include <string>
        #include <iostream>

        class VideoPlayer {
            std::string currentMovie;
        public:
            VideoPlayer() { std::cout << "VideoPlayer created.\n"; }
            ~VideoPlayer() { std::cout << "VideoPlayer destroyed.\n"; }
            void loadMovie(const std::string& title) { currentMovie = title; std::cout << "Loaded: " << title << std::endl; }
        };

        int main() {
            // Before C++14 (or without std::make_unique):
            // std::unique_ptr<VideoPlayer> player1(new VideoPlayer());

            // With C++14 std::make_unique:
            auto player2 = std::make_unique<VideoPlayer>(); // Simpler, exception-safe
            player2->loadMovie("Gladiator");
            // player2 is automatically destroyed when it goes out of scope
            return 0;
        }
        ```

6.  **Variable Templates**
    *   Description: Allows defining templates for variables.
    *   StreamFlix Example: Defining a platform-dependent max texture size for UI rendering.
        ```cpp
        #include <iostream>

        // Assume we have some way to know the platform at compile time (simplified)
        #define PLATFORM_DESKTOP 1
        #define PLATFORM_MOBILE 2
        #define CURRENT_PLATFORM PLATFORM_DESKTOP

        template<int Platform>
        constexpr int max_texture_size_kb = 2048; // Default

        template<> // Specialization for mobile
        constexpr int max_texture_size_kb<PLATFORM_MOBILE> = 1024;

        // C++14 Variable Template for PI
        template<typename T>
        constexpr T pi = T(3.1415926535897932385L);

        int main() {
            std::cout << "Max texture size for current platform: "
                      << max_texture_size_kb<CURRENT_PLATFORM> << "KB" << std::endl;

            std::cout << "Pi for float: " << pi<float> << std::endl;
            std::cout << "Pi for double: " << pi<double> << std::endl;
            return 0;
        }
        ```

7.  **Binary Literals and Digit Separators**
    *   Description: Binary literals (`0b` or `0B`) for integers. Digit separators (`'`) for numeric literals to improve readability.
    *   StreamFlix Example: Defining bit flags for video features or large timeout values.
        ```cpp
        #include <iostream>
        #include <cstdint>

        const uint8_t FEATURE_HDR = 0b00000001;      // High Dynamic Range
        const uint8_t FEATURE_DOLBY_VISION = 0b00000010;
        const uint8_t FEATURE_SURROUND_SOUND = 0b00000100;

        const long network_timeout_ms = 15'000; // 15,000 milliseconds
        const int max_buffer_size_bytes = 100'000'000; // 100 MB

        int main() {
            uint8_t videoFeatures = FEATURE_HDR | FEATURE_SURROUND_SOUND;
            if (videoFeatures & FEATURE_HDR) {
                std::cout << "HDR is enabled.\n";
            }
            std::cout << "Network timeout: " << network_timeout_ms << "ms\n";
            std::cout << "Max buffer size: " << max_buffer_size_bytes << " bytes\n";
            return 0;
        }
        ```

8.  **`[[deprecated]]` Attribute**
    *   Description: Marks an entity as deprecated, causing a compiler warning if used.
    *   StreamFlix Example: Deprecating an old API for fetching movie recommendations.
        ```cpp
        #include <vector>
        #include <string>
        #include <iostream>

        [[deprecated("Use getPersonalizedRecommendationsV2() instead for better results.")]]
        std::vector<std::string> getOldRecommendations(int userId) {
            std::cout << "Called deprecated getOldRecommendations for user " << userId << std::endl;
            return {"Generic Movie 1", "Generic Movie 2"};
        }

        std::vector<std::string> getPersonalizedRecommendationsV2(int userId) {
            // New fancy recommendation logic
            return {"Tailored Movie A for " + std::to_string(userId), "Tailored Movie B"};
        }

        int main() {
            auto recs = getOldRecommendations(123); // Compiler warning here
            for (const auto& r : recs) std::cout << r << std::endl;
            return 0;
        }
        ```

---

## C++17: More Major Features and Library Enhancements

C++17 brought another set of significant features.

1.  **Structured Bindings**
    *   Description: Allows decomposing objects like pairs, tuples, arrays, and structs into individual named variables.
    *   StreamFlix Example: Getting movie ID, title, and genre from a tuple or struct.
        ```cpp
        #include <tuple>
        #include <string>
        #include <map>
        #include <iostream>

        struct MovieInfo {
            int id;
            std::string title;
            std::string genre;
        };

        MovieInfo getMovieById(int id) {
            if (id == 42) return {42, "The Hitchhiker's Guide", "Sci-Fi Comedy"};
            return {0, "Unknown", "Unknown"};
        }

        int main() {
            // Decomposing a struct
            auto [movieId, movieTitle, movieGenre] = getMovieById(42);
            std::cout << "ID: " << movieId << ", Title: " << movieTitle
                      << ", Genre: " << movieGenre << std::endl;

            // Decomposing a std::pair (e.g., from a map iteration)
            std::map<int, std::string> userWatchProgress = {{101, "01:30:00"}, {102, "00:15:32"}};
            for (const auto& [contentId, progress] : userWatchProgress) {
                std::cout << "Content ID: " << contentId << " at " << progress << std::endl;
            }
            return 0;
        }
        ```

2.  **`if constexpr`**
    *   Description: A compile-time `if` statement that discards branches not taken, based on a compile-time condition.
    *   StreamFlix Example: A template function to process media items, with different logic for `AudioTrack` vs. `VideoTrack`.
        ```cpp
        #include <string>
        #include <iostream>

        struct AudioTrack {
            std::string codec = "AAC";
            static constexpr bool hasVideo = false;
            void process() { std::cout << "Processing audio track with codec: " << codec << std::endl; }
        };
        struct VideoTrack {
            std::string codec = "H.265";
            int resolution_width = 1920;
            static constexpr bool hasVideo = true;
            void process() {
                std::cout << "Processing video track (" << codec << ") at "
                          << resolution_width << "p" << std::endl;
            }
        };

        template<typename MediaType>
        void configureStream(MediaType& media) {
            media.process();
            if constexpr (MediaType::hasVideo) {
                // This code only compiles if MediaType::hasVideo is true
                std::cout << "Video specific configuration: Setting up renderer.\n";
                // std::cout << media.resolution_width; // This would be valid here
            } else {
                // This code only compiles if MediaType::hasVideo is false
                std::cout << "Audio specific configuration: Setting up audio output device.\n";
                // std::cout << media.resolution_width; // This would be a COMPILE ERROR here if AudioTrack has no resolution_width
            }
        }

        int main() {
            AudioTrack audio;
            VideoTrack video;

            configureStream(audio);
            configureStream(video);
            return 0;
        }
        ```

3.  **`std::optional`**
    *   Description: Represents an optional value; it can either contain a value or be empty.
    *   StreamFlix Example: A function that tries to find a user's subtitle preference, which might not exist.
        ```cpp
        #include <optional>
        #include <string>
        #include <iostream>
        #include <map>

        std::map<int, std::string> userSubtitlePrefs = {{123, "English"}, {456, "Spanish"}};

        std::optional<std::string> getSubtitlePreference(int userId) {
            auto it = userSubtitlePrefs.find(userId);
            if (it != userSubtitlePrefs.end()) {
                return it->second; // Contains a value
            }
            return std::nullopt; // Empty
        }

        int main() {
            int currentUser = 123;
            int newUser = 789;

            if (auto pref = getSubtitlePreference(currentUser); pref.has_value()) {
                std::cout << "User " << currentUser << " preference: " << pref.value() << std::endl;
            } else {
                std::cout << "User " << currentUser << " has no subtitle preference set. Defaulting to off.\n";
            }

            std::cout << "User " << newUser << " preference: "
                      << getSubtitlePreference(newUser).value_or("English (default)") << std::endl;
            return 0;
        }
        ```

4.  **`std::variant`**
    *   Description: A type-safe union; it can hold a value from a set of specified alternative types.
    *   StreamFlix Example: A media player event that can be a `PlaybackStarted`, `PlaybackPaused`, or `ErrorOccurred` event.
        ```cpp
        #include <variant>
        #include <string>
        #include <iostream>

        struct PlaybackStartedEvent { std::string movieId; };
        struct PlaybackPausedEvent { int pausePositionMs; };
        struct ErrorOccurredEvent { int errorCode; std::string message; };

        using PlayerEvent = std::variant<PlaybackStartedEvent, PlaybackPausedEvent, ErrorOccurredEvent>;

        void handlePlayerEvent(const PlayerEvent& event) {
            std::visit([](const auto& specificEvent) {
                using T = std::decay_t<decltype(specificEvent)>;
                if constexpr (std::is_same_v<T, PlaybackStartedEvent>) {
                    std::cout << "Event: Playback Started for movie ID: " << specificEvent.movieId << std::endl;
                } else if constexpr (std::is_same_v<T, PlaybackPausedEvent>) {
                    std::cout << "Event: Playback Paused at " << specificEvent.pausePositionMs << "ms" << std::endl;
                } else if constexpr (std::is_same_v<T, ErrorOccurredEvent>) {
                    std::cout << "Event: Error " << specificEvent.errorCode << " - " << specificEvent.message << std::endl;
                }
            }, event);
        }

        int main() {
            PlayerEvent e1 = PlaybackStartedEvent{"inception_2010"};
            PlayerEvent e2 = ErrorOccurredEvent{503, "Network unavailable"};

            handlePlayerEvent(e1);
            handlePlayerEvent(e2);
            return 0;
        }
        ```

5.  **`std::any`**
    *   Description: A type-safe container for a single value of any copyable type.
    *   StreamFlix Example: Storing arbitrary plugin configuration values (e.g., a string for API key, an int for max connections).
        ```cpp
        #include <any>
        #include <string>
        #include <map>
        #include <iostream>

        int main() {
            std::map<std::string, std::any> streamflixPluginConfig;
            streamflixPluginConfig["drm_plugin_path"] = std::string("/usr/lib/drm/widevine.so");
            streamflixPluginConfig["max_concurrent_downloads"] = 5;
            streamflixPluginConfig["enable_experimental_codec"] = true;

            // Retrieve and use a value
            if (auto it = streamflixPluginConfig.find("max_concurrent_downloads"); it != streamflixPluginConfig.end()) {
                try {
                    int maxDownloads = std::any_cast<int>(it->second);
                    std::cout << "Max concurrent downloads: " << maxDownloads << std::endl;
                } catch (const std::bad_any_cast& e) {
                    std::cerr << "Error casting max_concurrent_downloads: " << e.what() << std::endl;
                }
            }

            if (auto it = streamflixPluginConfig.find("enable_experimental_codec"); it != streamflixPluginConfig.end()) {
                // Safe check before casting
                if (it->second.type() == typeid(bool)) {
                     bool useExperimental = std::any_cast<bool>(it->second);
                     std::cout << "Use experimental codec: " << std::boolalpha << useExperimental << std::endl;
                }
            }
            return 0;
        }
        ```

6.  **`std::string_view`**
    *   Description: A non-owning, read-only view of a character sequence (a string). Avoids copying string data.
    *   StreamFlix Example: Parsing movie titles or descriptions from a large manifest file without making copies.
        ```cpp
        #include <string_view>
        #include <string>
        #include <iostream>

        // Function that processes a subtitle line, doesn't need to own the string
        void displaySubtitleLine(std::string_view line) {
            // In a real app, this might send to a renderer
            std::cout << "Subtitle: " << line << std::endl;
        }

        int main() {
            std::string fullSubtitleFile = "1\n00:00:20,000 --> 00:00:22,500\nStreamFlix Presents\n\n2\n...";
            // Imagine parsing this file
            std::string_view firstLine(fullSubtitleFile.c_str() + 26, 17); // Points to "StreamFlix Presents"

            displaySubtitleLine(firstLine);
            displaySubtitleLine("A new adventure begins!"); // Also works with string literals

            // Substring without allocation
            std::string_view movieTitle = "The Grand Budapest Hotel";
            std::string_view grand = movieTitle.substr(4, 5); // "Grand"
            std::cout << "Substring: " << grand << std::endl;
            return 0;
        }
        ```

7.  **Parallel Algorithms (Execution Policies)**
    *   Description: Many standard library algorithms gained overloads that accept an execution policy (e.g., `std::execution::par` for parallel execution).
    *   StreamFlix Example: Sorting a large catalog of movies in parallel.
        ```cpp
        #include <vector>
        #include <string>
        #include <algorithm>
        #include <execution> // For std::execution::par
        #include <iostream>
        #include <random>    // For generating test data

        struct Movie {
            int id;
            std::string title;
            float rating;
        };

        int main() {
            std::vector<Movie> catalog;
            // Populate with a lot of movies (e.g., 1,000,000)
            std::mt19937 rng(std::random_device{}());
            std::uniform_real_distribution<float> dist(1.0, 5.0);
            for (int i = 0; i < 100000; ++i) { // Reduced for quicker demo
                catalog.push_back({i, "Movie " + std::to_string(i), dist(rng)});
            }

            std::cout << "Sorting " << catalog.size() << " movies by rating...\n";
            auto startTime = std::chrono::high_resolution_clock::now();

            // Sort in parallel (if supported by implementation and beneficial for data size)
            std::sort(std::execution::par, catalog.begin(), catalog.end(),
                      [](const Movie& a, const Movie& b) {
                          return a.rating > b.rating; // Sort descending by rating
                      });

            auto endTime = std::chrono::high_resolution_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);
            std::cout << "Sorting took: " << duration.count() << " ms.\n";

            // std::cout << "Top 5 movies:\n";
            // for(int i=0; i<5 && i < catalog.size(); ++i)
            //     std::cout << catalog[i].title << " - Rating: " << catalog[i].rating << std::endl;

            return 0;
        }
        ```

8.  **Filesystem Library (`<filesystem>`)**
    *   Description: Provides operations for manipulating paths, files, and directories.
    *   StreamFlix Example: Managing downloaded video files or cache directories.
        ```cpp
        #include <filesystem> // Standard header
        #include <iostream>
        #include <fstream>

        namespace fs = std::filesystem;

        int main() {
            fs::path appCacheDir = fs::temp_directory_path() / "StreamFlixCache";

            try {
                if (!fs::exists(appCacheDir)) {
                    if (fs::create_directories(appCacheDir)) {
                        std::cout << "Created cache directory: " << appCacheDir << std::endl;
                    }
                } else {
                    std::cout << "Cache directory already exists: " << appCacheDir << std::endl;
                }

                fs::path downloadedMovie = appCacheDir / "inception.mp4";
                // Simulate downloading a file
                std::ofstream(downloadedMovie) << "dummy video data";

                if (fs::exists(downloadedMovie)) {
                    std::cout << "Movie file size: " << fs::file_size(downloadedMovie) << " bytes." << std::endl;
                    // fs::remove(downloadedMovie); // Clean up
                }
            } catch (const fs::filesystem_error& e) {
                std::cerr << "Filesystem error: " << e.what() << std::endl;
                std::cerr << "Path1: " << e.path1() << std::endl;
                if (!e.path2().empty()) std::cerr << "Path2: " << e.path2() << std::endl;
            }
            return 0;
        }
        ```

9.  **Template Argument Deduction for Class Templates (CTAD)**
    *   Description: The compiler can deduce class template arguments from constructor arguments, similar to function template argument deduction.
    *   StreamFlix Example: Creating `std::pair` or a custom templated `MediaItem` without explicit template arguments.
        ```cpp
        #include <vector>
        #include <string>
        #include <utility> // For std::pair
        #include <tuple>   // For std::tuple
        #include <iostream>

        template<typename T1, typename T2>
        struct MediaPair {
            T1 first;
            T2 second;
            MediaPair(T1 f, T2 s) : first(f), second(s) {}
            void print() { std::cout << "MediaPair: (" << first << ", " << second << ")\n"; }
        };
        // Deduction guide (often not needed for simple cases like above with C++17)
        // template<typename T1, typename T2> MediaPair(T1, T2) -> MediaPair<T1, T2>;


        int main() {
            // Before C++17 (or without CTAD)
            // std::pair<int, std::string> movieRatingOld(101, "Awesome");
            // MediaPair<int, const char*> customPairOld(202, "TV Show");

            // With C++17 CTAD
            std::pair movieRating(101, std::string("Awesome")); // Deduces std::pair<int, std::string>
            std::tuple userWatchHistory(123, "movie_abc", 1200); // Deduces std::tuple<int, const char*, int>

            MediaPair customPair(202, "TV Show"); // Deduces MediaPair<int, const char*>
            customPair.print();

            std::cout << "Movie Rating ID: " << movieRating.first << ", Desc: " << movieRating.second << std::endl;

            return 0;
        }
        ```

10. **`[[nodiscard]]` Attribute**
    *   Description: Encourages compilers to issue a warning if the return value of a function marked `[[nodiscard]]` is ignored.
    *   StreamFlix Example: A function that attempts to acquire a playback lock, whose result (success/failure) must be checked.
        ```cpp
        #include <iostream>

        // It's important to check if the lock was acquired
        [[nodiscard]]
        bool acquirePlaybackLock(int resourceId) {
            std::cout << "Attempting to acquire lock for resource: " << resourceId << std::endl;
            // Simulate lock acquisition logic
            if (resourceId % 2 == 0) {
                std::cout << "Lock acquired for " << resourceId << std::endl;
                return true;
            }
            std::cout << "Failed to acquire lock for " << resourceId << std::endl;
            return false;
        }

        void playContent(int contentId) {
             std::cout << "Playing content " << contentId << std::endl;
        }

        int main() {
            // acquirePlaybackLock(100); // Compiler warning: ignoring return value of function declared with 'nodiscard' attribute

            if (acquirePlaybackLock(100)) { // Good: result is used
                playContent(100);
                // releasePlaybackLock(100);
            }

            if (acquirePlaybackLock(101)) {
                playContent(101);
                // releasePlaybackLock(101);
            } else {
                std::cout << "Cannot play content 101, lock not acquired.\n";
            }
            return 0;
        }
        ```

11. **Fold Expressions**
    *   Description: A concise way to apply a binary operator over all arguments of a parameter pack in variadic templates.
    *   StreamFlix Example: Calculating the total duration of several video clips in a playlist.
        ```cpp
        #include <iostream>
        #include <string>

        // Unary right fold
        template<typename... Args>
        void printAllItems(Args... args) {
            // (std::cout << ... << args) << '\n'; // This doesn't work as expected directly
            // Correct way for printing is often a C-style array or initializer list trick pre-C++17
            // or recursive template, or with C++17 fold:
            ((std::cout << args << " "), ...); // Prints "item1 item2 item3 "
            std::cout << std::endl;
        }

        // Binary left fold
        template<typename... Durations>
        int sumDurations(Durations... durations) {
            return (durations + ... + 0); // or (... + durations) for right fold. 0 for empty pack.
        }

        int main() {
            printAllItems("Clip1.mp4", "Ad_Break.mp4", "Clip2.mp4");

            int totalPlaylistTimeSec = sumDurations(120, 30, 180); // 120s, 30s, 180s
            std::cout << "Total playlist duration: " << totalPlaylistTimeSec << " seconds." << std::endl;

            int emptyPlaylistTime = sumDurations();
            std::cout << "Empty playlist duration: " << emptyPlaylistTime << " seconds." << std::endl;

            return 0;
        }
        ```

12. **`inline` Variables**
    *   Description: Allows defining static data members within a class definition in a header file (ODR-safe).
    *   StreamFlix Example: Defining a global default `StreamSettings` object accessible across translation units.
        ```cpp
        // In a header file (e.g., stream_settings.h)
        #ifndef STREAM_SETTINGS_H
        #define STREAM_SETTINGS_H

        #include <string>
        #include <iostream> // For example

        struct StreamSettings {
            int defaultBitrateKbps = 2500;
            std::string defaultAudioCodec = "AAC";

            // C++17 inline static data member
            inline static const StreamSettings GlobalDefaults;
            // Before C++17, you'd declare `static const StreamSettings GlobalDefaults;` here
            // and define it in one .cpp file: `const StreamSettings StreamSettings::GlobalDefaults;`

            void print() const {
                 std::cout << "Bitrate: " << defaultBitrateKbps
                           << ", Audio: " << defaultAudioCodec << std::endl;
            }
        };
        #endif // STREAM_SETTINGS_H

        // In main.cpp (or any other .cpp file that includes stream_settings.h)
        // #include "stream_settings.h" // (assuming it's in a separate file)
        // int main() {
        //     std::cout << "Global default stream settings:\n";
        //     StreamSettings::GlobalDefaults.print();

        //     StreamSettings customSettings;
        //     customSettings.defaultBitrateKbps = 5000;
        //     std::cout << "Custom settings:\n";
        //     customSettings.print();
        //     return 0;
        // }
        ```
        To make the above example runnable directly:
        ```cpp
        #include <string>
        #include <iostream>

        // Content of stream_settings.h
        struct StreamSettings {
            int defaultBitrateKbps = 2500;
            std::string defaultAudioCodec = "AAC";

            inline static const StreamSettings GlobalDefaults;
            // For non-const static members, or pre-C++17 const static members of non-integral/enum type:
            // inline static StreamSettings modifiableGlobalDefaults; // C++17 only

            void print() const {
                 std::cout << "Bitrate: " << defaultBitrateKbps
                           << "kbps, Audio Codec: " << defaultAudioCodec << std::endl;
            }
        };
        // End of stream_settings.h content

        int main() {
            std::cout << "Global default stream settings from StreamFlix app:\n";
            StreamSettings::GlobalDefaults.print();

            StreamSettings userPreferredSettings;
            userPreferredSettings.defaultBitrateKbps = 8000; // User wants higher quality
            userPreferredSettings.defaultAudioCodec = "Opus";
            std::cout << "User preferred settings for StreamFlix:\n";
            userPreferredSettings.print();
            return 0;
        }
        ```

13. **`[[fallthrough]]`, `[[maybe_unused]]` Attributes**
    *   Description:
        *   `[[fallthrough]]`: Indicates intentional fall-through in a `switch` statement to suppress warnings.
        *   `[[maybe_unused]]`: Suppresses warnings about an unused entity (variable, parameter, etc.).
    *   StreamFlix Example: Handling different subscription tiers with fallthrough, and a parameter only used in debug builds.
        ```cpp
        #include <iostream>
        #include <string>

        enum class SubscriptionTier { Free, Basic, Premium, Ultra };

        void configureFeatures(SubscriptionTier tier, [[maybe_unused]] bool enableDebugLogs) {
            // enableDebugLogs might only be used in #ifdef DEBUG blocks
            #ifdef DEBUG
            if (enableDebugLogs) std::cout << "Debug logs enabled for feature config.\n";
            #endif

            std::cout << "Configuring features for tier: ";
            switch (tier) {
                case SubscriptionTier::Ultra:
                    std::cout << "Ultra - Enabling 8K streaming, offline everything.\n";
                    [[fallthrough]]; // Ultra users get all Premium features too
                case SubscriptionTier::Premium:
                    std::cout << "Premium - Enabling 4K streaming, multi-device, downloads.\n";
                    [[fallthrough]]; // Premium users get all Basic features
                case SubscriptionTier::Basic:
                    std::cout << "Basic - Enabling HD streaming, single device.\n";
                    [[fallthrough]]; // Basic users get Free features (ads)
                case SubscriptionTier::Free:
                    std::cout << "Free - Enabling SD streaming with ads.\n";
                    break;
                default:
                    std::cout << "Unknown tier.\n";
            }
        }

        int main() {
            configureFeatures(SubscriptionTier::Premium, true);
            std::cout << "\n";
            configureFeatures(SubscriptionTier::Free, false);
            return 0;
        }
        ```

---

This list covers many of the most prominent features. Each C++ standard also included numerous smaller library additions (e.g., new math functions, container member functions, utilities in `<algorithm>` and `<numeric>`) which are too many to list exhaustively here but contribute to the overall power and convenience of modern C++.
