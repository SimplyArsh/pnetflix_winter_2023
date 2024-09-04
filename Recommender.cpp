#include "Recommender.h"
#include "UserDatabase.h"
#include "MovieDatabase.h"

#include <string>
#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <set>

#include "User.h"
#include "Movie.h"

using namespace std;

// Constructor initializes the recommender with user and movie databases
Recommender::Recommender(const UserDatabase& user_database,
                         const MovieDatabase& movie_database)
{
    udb = &user_database;  // Store reference to user database
    mdb = &movie_database; // Store reference to movie database
}

// Method to recommend movies based on a user's email
vector<MovieAndRank> Recommender::recommend_movies(const string& user_email, int movie_count) const
{
    User* user = udb->get_user_from_email(user_email); // Find user by email; complexity: O(logU)
    
    // If movie count is invalid or user not found, return an empty vector
    if (movie_count <= 0 || user == nullptr)
        return vector<MovieAndRank>();
    
    unordered_set<Movie*> visitedMovies;  // Set to track visited movies
    unordered_map<Movie*, int> movieScores; // Map to store movie scores
    
    // Retrieve user's watch history
    vector<string> watch_history = user->get_watch_history(); // Complexity: O(1)
    vector<Movie*> watched_movies; // List of movies the user has already watched

    // Populate watched movies and mark them as visited
    for (const auto& movieId : watch_history) // Complexity: O(historySize * logM)
    {
        Movie* movie = mdb->get_movie_from_id(movieId); // Complexity: O(logM)
        watched_movies.push_back(movie);
        visitedMovies.insert(movie);
    }

    // Compute recommendations based on watched movies
    for (const auto& watched_movie : watched_movies)
    {
        // Get related entities: directors, actors, genres
        vector<string> directors = watched_movie->get_directors(); // Complexity: O(1)
        vector<string> actors = watched_movie->get_actors();       // Complexity: O(1)
        vector<string> genres = watched_movie->get_genres();       // Complexity: O(1)
        
        // Add points for movies with the same directors
        for (const auto& director : directors)
        {
            vector<Movie*> directorMovies = mdb->get_movies_with_director(director);
            addPoints(20, directorMovies, visitedMovies, movieScores);
        }
        
        // Add points for movies with the same actors
        for (const auto& actor : actors)
        {
            vector<Movie*> actorMovies = mdb->get_movies_with_actor(actor);
            addPoints(30, actorMovies, visitedMovies, movieScores);
        }
        
        // Add points for movies with the same genres
        for (const auto& genre : genres)
        {
            vector<Movie*> genreMovies = mdb->get_movies_with_genre(genre);
            addPoints(1, genreMovies, visitedMovies, movieScores);
        }
    }
    
    // Sort movies by their scores
    set<pair<Movie*, int>, comp> sortedMovies(movieScores.begin(), movieScores.end());
    
    vector<MovieAndRank> recommendations; // Final list of recommended movies
    
    int addedCount = 0;
    for (const auto& pair : sortedMovies)
    {
        if (addedCount >= movie_count) // Limit the number of recommendations
            break;
        recommendations.push_back(MovieAndRank(pair.first->get_id(), pair.second));
        addedCount++;
    }
    
    return recommendations; // Return the list of recommended movies
}

// Helper function to add points to movies based on shared attributes
void Recommender::addPoints(int points, vector<Movie*>& movies, unordered_set<Movie*>& visited, unordered_map<Movie*, int>& scoreMap) const
{
    for (auto* movie : movies)
    {
        if (visited.find(movie) == visited.end())
        {
            visited.insert(movie);   // Mark movie as visited
            scoreMap[movie] = points; // Initialize score
        }
        else if (scoreMap.find(movie) != scoreMap.end())
        {
            scoreMap[movie] += points; // Increment score if movie already visited
        }
    }
}

// Comparator for sorting movies first by score, then by rating, and finally alphabetically
template<typename T>
bool Recommender::comp::operator()(const T& a, const T& b) const
{
    // Compare based on compatibility score
    if (b.second > a.second)
        return false;
    else if (b.second < a.second)
        return true;
    
    // Compare based on rating if scores are equal
    if (b.first->get_rating() > a.first->get_rating())
        return false;
    else if (b.first->get_rating() < a.first->get_rating())
        return true;
    
    // Compare alphabetically if both score and rating are equal
    return b.first->get_title().compare(a.first->get_title()) >= 0;
}