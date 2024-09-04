#ifndef MOVIEDATABASE_INCLUDED
#define MOVIEDATABASE_INCLUDED

#include <string>
#include <vector>
#include "treemm.h"

class Movie;

class MovieDatabase
{
  public:

    MovieDatabase();
    ~MovieDatabase();
    bool load(const std::string& filename);
    Movie* get_movie_from_id(const std::string& id) const;
    
    std::vector<Movie*> get_movies_with_genre(const std::string& genre) const;
    std::vector<Movie*> get_movies_with_director(const std::string& director) const;
    std::vector<Movie*> get_movies_with_actor(const std::string& actor) const;

  private:

    bool m_hasLoaded;
    std::vector<Movie*> m_movies;

    TreeMultimap<std::string, Movie*> m_id_movie_tree;
    TreeMultimap<std::string, Movie*> m_actor_movie_tree;
    TreeMultimap<std::string, Movie*> m_director_movie_tree;
    TreeMultimap<std::string, Movie*> m_genre_movie_tree;
    
    std::string getLowercase(const std::string word) const;
    std::vector<Movie*> getMovies(int search, std::string query) const;
};

#endif
