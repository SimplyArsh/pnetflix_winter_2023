#include "MovieDatabase.h"
#include "Movie.h"

#include <string>
#include <vector>
#include <fstream>
#include <sstream>

using namespace std;

// Constructor to initialize the MovieDatabase object
MovieDatabase::MovieDatabase()
{
    m_hasLoaded = false;  // Initially set to false; data not loaded yet
}

// Load movie data from a specified file
bool MovieDatabase::load(const string& filename)
{
    // Check if data has already been loaded
    if (m_hasLoaded)
        return false;
    
    ifstream infile(filename);  // Open the file
    if (!infile)
    {
        cerr << "ERROR: Unable to open file: " << filename << endl;
        return false;
    }
    
    int movieCount = 0;  // Track the number of movies loaded
    while (infile)
    {
        string id;
        getline(infile, id);  // Read movie ID
        
        // If reading fails, exit the loop
        if (!infile)
            break;
        
        string title;
        getline(infile, title);  // Read movie title
        
        string year;
        getline(infile, year);  // Read release year
        
        string directors;
        getline(infile, directors);  // Read directors
        stringstream ssDirectors(directors);
        vector<string> vDirectors;
        while (ssDirectors.good())
        {
            string substr;
            getline(ssDirectors, substr, ',');
            vDirectors.push_back(substr);
        }
        
        string actors;
        getline(infile, actors);  // Read actors
        stringstream ssActors(actors);
        vector<string> vActors;
        while (ssActors.good())
        {
            string substr;
            getline(ssActors, substr, ',');
            vActors.push_back(substr);
        }
        
        string genres;
        getline(infile, genres);  // Read genres
        stringstream ssGenres(genres);
        vector<string> vGenres;
        while (ssGenres.good())
        {
            string substr;
            getline(ssGenres, substr, ',');
            vGenres.push_back(substr);
        }
        
        float rating;
        infile >> rating;  // Read rating
        infile.ignore(10000, '\n');  // Skip the rest of the line
        
        // Create a new movie object and add it to the database
        Movie* movie = new Movie(id, title, year, vDirectors, vActors, vGenres, rating);
        m_movies.push_back(movie);
        m_id_movie_tree.insert(toLowerCase(id), movie);

        // Index movies by director, actor, and genre for quick search
        for (const auto& director : vDirectors)
            m_director_movie_tree.insert(toLowerCase(director), movie);
        for (const auto& actor : vActors)
            m_actor_movie_tree.insert(toLowerCase(actor), movie);
        for (const auto& genre : vGenres)
            m_genre_movie_tree.insert(toLowerCase(genre), movie);
        
        movieCount++;
        infile.ignore(10000, '\n'); // Skip empty line between records
    }
    
    cerr << movieCount << " movies successfully loaded!" << endl;
    
    m_hasLoaded = true;  // Mark as loaded
    return true;
}

// Retrieve a movie by its ID
Movie* MovieDatabase::get_movie_from_id(const string& id) const
{
    auto movieIterator = m_id_movie_tree.find(toLowerCase(id));
    if (!movieIterator.is_valid())
        return nullptr;
    return movieIterator.get_value();
}

// Retrieve movies directed by a specific director
vector<Movie*> MovieDatabase::get_movies_with_director(const string& director) const
{
    return getMovies(1, director);
}

// Retrieve movies featuring a specific actor
vector<Movie*> MovieDatabase::get_movies_with_actor(const string& actor) const
{
    return getMovies(2, actor);
}

// Retrieve movies belonging to a specific genre
vector<Movie*> MovieDatabase::get_movies_with_genre(const string& genre) const
{
    return getMovies(3, genre);
}

// Destructor to clean up dynamically allocated Movie objects
MovieDatabase::~MovieDatabase()
{
    for (auto& movie : m_movies)
    {
        delete movie;
        movie = nullptr;  // Nullify pointer for safety
    }
}

// Convert a string to lowercase
string MovieDatabase::toLowerCase(const std::string& word) const
{
    string result;
    for (char c : word)
        result += tolower(c);
    return result;
}

// Generalized method to retrieve movies by director, actor, or genre
vector<Movie*> MovieDatabase::getMovies(int searchType, const string& query) const
{
    TreeMultimap<string, Movie*>::Iterator movieIterator;
    
    // Determine search type and get the corresponding iterator
    switch (searchType)
    {
        case 1:  // Search by director
            movieIterator = m_director_movie_tree.find(toLowerCase(query));
            break;
        case 2:  // Search by actor
            movieIterator = m_actor_movie_tree.find(toLowerCase(query));
            break;
        case 3:  // Search by genre
            movieIterator = m_genre_movie_tree.find(toLowerCase(query));
            break;
        default:
            cerr << "ERROR: Invalid search type! Query: " << searchType << endl;
            throw 1;
    }

    vector<Movie*> results;
    while (movieIterator.is_valid())
    {
        results.push_back(movieIterator.get_value());
        movieIterator.advance();
    }
    return results;
}