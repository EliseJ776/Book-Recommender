#include <iostream>
#include <iomanip>
#include <string>
#include <cmath>
#include <vector>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <string>
#include <set>
#include <map>
#include <unordered_map>
#include <numeric>

#define BOOK_LIST vector<string>
#define USER_LIST vector<string>
#define USER_RATINGS_MAP map<string, vector<double>>
#define BOOK_AVG_MAP map<string, double>
#define BOOK_AVG_LIST vector<pair<string, double>>

//#define DEBUG

using namespace std;
class BookRecommender
{
private:
    BOOK_LIST books;
    USER_LIST users;
    USER_RATINGS_MAP ratedBooks;
    BOOK_AVG_MAP averages;

public:
    BookRecommender();
    BookRecommender(string fileName);
    void printRecommend(string userName);
    void printAverages();
    double getAverage(string bookTitle);
    double getSimilarity(string userName1, string userName2);
    int getBookCount();
    int getUserCount();
    double getUserBookRating(string userName, string bookTitle);
};

class Book {
public:
    Book(string t) : title(t), ratingCount(0), totalRating(0) {}

    string title;
    int ratingCount;
    int totalRating;
    double averageRating() const {
        if (ratingCount == 0) {
            return 0.0;
        }
        return (double)totalRating / ratingCount;
    }
};

class User {
public:
    User(string n, int numBooks) : name(n), ratedBooks(numBooks, 0) {}

    string name;
    vector<int> ratedBooks;
};

