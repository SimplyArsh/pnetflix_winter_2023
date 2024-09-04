#ifndef RECOMMENDER_INCLUDED
#define RECOMMENDER_INCLUDED

#include <string>
#include <vector>
#include <unordered_set>
#include <unordered_map>

class UserDatabase;  // Forward declaration of UserDatabase class
class MovieDatabase; // Forward declaration of MovieDatabase class

class Movie;  // Forward declaration of Movie class


struct MovieAndRank
{
    MovieAndRank(const std::string& id, int score)
     : movie_id(id), compatibility_score(score) 
    {}

    std::string movie_id;      
    int compatibility_score;   // Score representing how compatible the movie is with user preferences
};


class Recommender
{
public:
    // Constructor initializes the recommender with references to user and movie databases
    Recommender(const UserDatabase& user_database,
                const MovieDatabase& movie_database);

    
    std::vector<MovieAndRank> recommend_movies(const std::string& user_email,
                                               int movie_count) const;

private:
    const UserDatabase* udb;  
    const MovieDatabase* mdb; // Pointer to the movie database
    
    // Helper function to add points to movies based on their association with user's preferences
    void addPoints(int numPoints, std::vector<Movie*>& movies, 
                   std::unordered_set<Movie*>& visited, 
                   std::unordered_map<Movie*, int>& map) const;
    
    // Comparator structure to sort movies based on specific criteria
    struct comp {
        template<typename T>
        bool operator()(const T& l, const T& r) const;
    };
};

#endif // RECOMMENDER_INCLUDED