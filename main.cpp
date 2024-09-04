// Define the file paths for user and movie data
const string USER_DATAFILE = "/Users/rolandyang/Documents/GitHub/CS32/Project_4/pnetphlix/pnetphlix/users.txt";
const string MOVIE_DATAFILE = "/Users/rolandyang/Documents/GitHub/CS32/Project_4/pnetphlix/pnetphlix/movies.txt";

// Include necessary headers
#include "MovieDatabase.h"
#include "Movie.h"
#include "Recommender.h"
#include <vector>
#include <chrono>

using namespace std;

// Function to find and display movie recommendations
void findMatches(const Recommender& recommender, const MovieDatabase& movieDB, const string& email, int recommendationCount)
{
    // Start timing the recommendation process
    auto startTime = chrono::steady_clock::now();
    
    // Get a list of recommended movies for the user
    vector<MovieAndRank> recommendedMovies = recommender.recommend_movies(email, recommendationCount);
    
    // Stop timing the recommendation process
    auto endTime = chrono::steady_clock::now();
    
    // Check if there are any recommendations
    if (recommendedMovies.empty())
    {
        cout << "No movie recommendations available :(" << endl;
    }
    else
    {
        // Loop through the recommendations and display them
        for (int i = 0; i < recommendedMovies.size(); ++i) 
        {
            const MovieAndRank& movieRank = recommendedMovies[i];
            Movie* movie = movieDB.get_movie_from_id(movieRank.movie_id);
            cout << (i + 1) << ". " << movie->get_title() << " (" << movie->get_release_year() << ")" << endl;
            cout << "Rating: " << movie->get_rating() << endl;
            cout << "Compatibility Score: " << movieRank.compatibility_score << "\n" << endl;
        }
    }

    // Output the time taken to generate recommendations
    cout << "Generated " << recommendationCount << " recommendations for " << email << " in " 
         << chrono::duration_cast<chrono::milliseconds>(endTime - startTime).count() << "ms." << endl;
}

// Main function
int main()
{
    // Start timer for loading user data
    auto userLoadStart = chrono::steady_clock::now();
    UserDatabase userDB;

    // Attempt to load user data from file
    if (!userDB.load(USER_DATAFILE))  // 'load' method in the skeleton always returns false
    {
        cout << "Error: Unable to load user data from " << USER_DATAFILE << "!" << endl;
        return 1;  // Exit if loading fails
    }
    auto userLoadEnd = chrono::steady_clock::now();  // End timer for loading user data

    // Start timer for loading movie data
    auto movieLoadStart = chrono::steady_clock::now();
    MovieDatabase movieDB;

    // Attempt to load movie data from file
    if (!movieDB.load(MOVIE_DATAFILE))  // 'load' method in the skeleton always returns false
    {
        cout << "Error: Unable to load movie data from " << MOVIE_DATAFILE << "!" << endl;
        return 1;  // Exit if loading fails
    }
    auto movieLoadEnd = chrono::steady_clock::now();  // End timer for loading movie data

    // Instantiate recommender with loaded user and movie databases
    Recommender recommender(userDB, movieDB);

    // Output time taken to load databases
    cout << "User database loaded in " 
         << chrono::duration_cast<chrono::milliseconds>(userLoadEnd - userLoadStart).count() 
         << "ms." << endl;
    cout << "Movie database loaded in " 
         << chrono::duration_cast<chrono::milliseconds>(movieLoadEnd - movieLoadStart).count() 
         << "ms." << endl;

    cout << "\nWelcome to Roland's Ultimate Recommender System" << endl;

    // Main interaction loop
    while (true)
    {
        cout << "\nChoose an option from the menu:\n"
             << "1 - Search for user by email\n"
             << "2 - Search for movie by ID\n"
             << "3 - Search for movies by director\n"
             << "4 - Search for movies by actor\n"
             << "5 - Search for movies by genre\n"
             << "6 - Get movie recommendations\n"
             << "7 - Exit\n";

        int userChoice;
        cin >> userChoice;
        cin.ignore(10000, '\n');  // Clear input buffer

        // Handle user's choice
        if (userChoice == 1)
        {
            cout << "\nEnter user email to search: ";
            string email;
            getline(cin, email);
            
            auto start = chrono::steady_clock::now();
            User* user = userDB.get_user_from_email(email);
            auto end = chrono::steady_clock::now();

            if (!user)
                cout << "No user found with the provided email." << endl;
            else
            {
                cout << "Name: " << user->get_full_name() << endl;
                cout << "Watch History: " << endl;
                for (const auto& movie : user->get_watch_history())
                    cout << movie << ", ";
                cout << endl;
            }
            cout << "Search completed in " 
                 << chrono::duration_cast<chrono::milliseconds>(end - start).count() 
                 << "ms." << endl;
        }
        else if (userChoice == 2)
        {
            cout << "\nEnter movie ID to search: ";
            string movieID;
            getline(cin, movieID);
            
            auto start = chrono::steady_clock::now();
            Movie* movie = movieDB.get_movie_from_id(movieID);
            auto end = chrono::steady_clock::now();

            if (!movie)
                cout << "No movie found with the provided ID." << endl;
            else
            {
                cout << "Title: " << movie->get_title() << endl;
                cout << "Year: " << movie->get_release_year() << endl;
                cout << "Rating: " << movie->get_rating() << endl;
                
                cout << "Directors: ";
                for (const auto& director : movie->get_directors())
                    cout << director << ", ";
                cout << endl;

                cout << "Actors: ";
                for (const auto& actor : movie->get_actors())
                    cout << actor << ", ";
                cout << endl;

                cout << "Genres: ";
                for (const auto& genre : movie->get_genres())
                    cout << genre << ", ";
                cout << endl;
            }
            cout << "Search completed in " 
                 << chrono::duration_cast<chrono::milliseconds>(end - start).count() 
                 << "ms." << endl;
        }
        else if (userChoice == 3)
        {
            cout << "\nEnter director's name to search for movies: ";
            string directorName;
            getline(cin, directorName);
            
            auto start = chrono::steady_clock::now();
            vector<Movie*> movies = movieDB.get_movies_with_director(directorName);
            auto end = chrono::steady_clock::now();

            if (movies.empty())
                cout << "No movies found for the given director." << endl;
            else
            {
                cout << "Movies by " << directorName << ": " << endl;
                for (auto* movie : movies)
                    cout << movie->get_title() << " (" << movie->get_id() << "), ";
                cout << endl;
            }
            cout << "Search completed in " 
                 << chrono::duration_cast<chrono::milliseconds>(end - start).count() 
                 << "ms." << endl;
        }
        else if (userChoice == 4)
        {
            cout << "\nEnter actor's name to search for movies: ";
            string actorName;
            getline(cin, actorName);
            
            auto start = chrono::steady_clock::now();
            vector<Movie*> movies = movieDB.get_movies_with_actor(actorName);
            auto end = chrono::steady_clock::now();

            if (movies.empty())
                cout << "No movies found for the given actor." << endl;
            else
            {
                cout << "Movies featuring " << actorName << ": " << endl;
                for (auto* movie : movies)
                    cout << movie->get_title() << " (" << movie->get_id() << "), ";
                cout << endl;
            }
            cout << "Search completed in " 
                 << chrono::duration_cast<chrono::milliseconds>(end - start).count() 
                 << "ms." << endl;
        }
        else if (userChoice == 5)
        {
            cout << "\nEnter genre to search for movies: ";
            string genre;
            getline(cin, genre);
            
            auto start = chrono::steady_clock::now();
            vector<Movie*> movies = movieDB.get_movies_with_genre(genre);
            auto end = chrono::steady_clock::now();

            if (movies.empty())
                cout << "No movies found for the given genre." << endl;
            else
            {
                cout << "Movies in " << genre << " genre: " << endl;
                for (auto* movie : movies)
                    cout << movie->get_title() << " (" << movie->get_id() << "), ";
                cout << endl;
            }
            cout << "Search completed in " 
                 << chrono::duration_cast<chrono::milliseconds>(end - start).count() 
                 << "ms." << endl;
        }
        else if (userChoice == 6)
        {
            cout << "\nEnter user email for recommendations: ";
            string email;
            getline(cin, email);

            cout << "How many recommendations would you like?: ";
            int numRecommendations;
            cin >> numRecommendations;
            cin.ignore(10000, '\n');

            findMatches(recommender, movieDB, email, numRecommendations);

            cout << endl;
        }
        else if (userChoice == 7)
        {
            cout << "Exiting the program. Goodbye!" << endl;
            break;
        }
        else
        {
            cout << "Invalid selection. Please choose a valid option." << endl;
        }
    }

    return 0;
}