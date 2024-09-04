#ifndef MOVIE_INCLUDED
#define MOVIE_INCLUDED

#include <string>
#include <vector>

class Movie
{
public:
    // Constructor to initialize a Movie object with various attributes
    Movie(const std::string& id, const std::string& title,
          const std::string& releaseYear,
          const std::vector<std::string>& directors,
          const std::vector<std::string>& actors,
          const std::vector<std::string>& genres, 
          float rating);

    // Accessor methods to retrieve movie details
    std::string get_id() const;                // Returns the movie ID
    std::string get_title() const;             // Returns the movie title
    std::string get_release_year() const;      // Returns the release year of the movie
    float get_rating() const;                  // Returns the movie rating
    std::vector<std::string> get_directors() const; // Returns a list of directors
    std::vector<std::string> get_actors() const;    // Returns a list of actors
    std::vector<std::string> get_genres() const;    // Returns a list of genres

private:
    // Private member variables to store movie data
    std::string m_id;                          // Movie identifier
    std::string m_title;                       // Movie title
    std::string m_release_year;                // Release year of the movie
    std::vector<std::string> m_directors;      // List of directors
    std::vector<std::string> m_actors;         // List of actors
    std::vector<std::string> m_genres;         // List of genres
    float m_rating;                            // Movie rating
};

#endif // MOVIE_INCLUDED