/* 
    The purpose of this program is to provide the users with
    two services: to find the averages of all of the books in the system
    or to get personalized recommendations based on the user's ratings
    and their similarity to other users
    Elise James
    Project 4
    19 April, 2023
*/
#include "BookRecommender.h"
using namespace std;

//#define DEBUG

BookRecommender::BookRecommender()
{

}

BookRecommender::BookRecommender(string fileName) 
{
    ifstream inFile(fileName, ios::in | ios::binary);
    set<string> book;
    string line;
    string username; 
    string booktitle; 
    string ratingStrn;
    double rating;

    while (getline(inFile, username)) 
    {
		 if (!username.empty() && username.back() == '\r')
			{
			 username.pop_back();
			}

        getline(inFile, booktitle);
       
			if(!booktitle.empty() && booktitle.back() == '\r') 
			{
				booktitle.pop_back();
			}

		 getline(inFile, ratingStrn);
        rating = stoi(ratingStrn);

        //Adds the book title to the set
        book.insert(booktitle);
    }

    //Converts the book set to a vector
    for (auto& it : book) 
    {
        books.push_back(it);
    }

    inFile.clear();
    inFile.seekg(0, ios::beg);

    while (getline(inFile, username)) 
    {
        if (!username.empty() && username.back() == '\r')
			{
			 username.pop_back();
			}

        getline(inFile, booktitle);
       
			if(!booktitle.empty() && booktitle.back() == '\r') 
			{
				booktitle.pop_back();
			}

		 getline(inFile, ratingStrn);
        rating = stoi(ratingStrn);

        //Finds the index
        int bookIndex = distance(books.begin(), find(books.begin(), books.end(), booktitle));

        if (ratedBooks.find(username) == ratedBooks.end()) 
        {
            ratedBooks[username] = vector<double>(books.size(), 0);
        }

        //Sets the user's rating for the book
        ratedBooks[username][bookIndex] = rating;
    }

    inFile.close();

    //Calculates the average rating for each book
    for (string booktitle : books) 
    {
        int bookIndex = distance(books.begin(), find(books.begin(), books.end(), booktitle));
        double sum = 0;
        int count = 0;
        for (auto& pair : ratedBooks) 
        {
            double rating = pair.second[bookIndex];
            if (rating != 0) 
            {
                sum += rating;
                count++;
            }
        }
        double average = count > 0 ? sum / count : 0;
        averages[booktitle] = average;
    }

    #ifdef DEBUG
    cout << "Book List:\n";
    for (const string& book : books) {
        cout << book;
    }
    #endif

    #ifdef DEBUG
    cout << "Ratings: " << endl;
    for (map<string, vector<double>>::iterator it = ratedBooks.begin(); it != ratedBooks.end(); ++it) {
        cout << it->first << ": [";
        for (double rating : it->second) {
            cout << rating << ", ";
        }
        cout << "]" << endl;
    }
    #endif

}  

void BookRecommender::printRecommend(string userName) 
{
    if (ratedBooks.find(userName) == ratedBooks.end())
    {
        vector<pair<string, double>> sortedAverages(averages.begin(), averages.end());
    
    //Sorts the averages first by rating, then alphabetically
    sort(sortedAverages.begin(), sortedAverages.end(), [](const pair<string, double>& a, const pair<string, double>& b) 
    {
        if (a.second != b.second) 
        {
            return a.second > b.second; //Sorts by rating
        } 
        else 
        {
            return a.first < b.first; //Sort alphabetically
        }
    });
    
    //Prints out the book averages
    for (const auto& avgRating : sortedAverages) 
    {
        cout << fixed << setprecision(2) << avgRating.first << " " << avgRating.second << endl;
    }
    }
    //Calculates the similarities ratings between the program user and other users
    vector<pair<double, string>> similarities; 
    for (auto& entry : ratedBooks) 
    {
        string name = entry.first;
        if (name != userName) //Filters out the user who requests the recommendation
        { 
            double similarity = 0;
            for (size_t i = 0; i < entry.second.size(); i++) 
            {
                similarity += entry.second[i] * ratedBooks[userName][i];
            }
            similarities.push_back(make_pair(similarity, name));
        }
    }
    sort(similarities.begin(), similarities.end(), greater<pair<double, string>>()); //Sorts by descending similarity
    
    vector<double> avg(books.size(), 0);

    for (size_t i = 0; i < books.size(); i++) //loops through the books
    { 
        int notZero = 0;
        double sum = 0;
        for (size_t j = 0; j < 3; j++) //Loops through the top three most similar users
        {
            string name = similarities[j].second;
            if (ratedBooks[name][i] != 0) //Ignores ratings of zero
            { 
                sum += ratedBooks[name][i];
                notZero++;
            }
        }
        if (notZero != 0) 
        {
            avg[i] = sum / notZero;
        }
    }   
    
    //Creates list of recommended books and sorted by average rating and alphabetically
    vector<pair<double, string>> recommendations; 
    
    for (size_t i = 0; i < averages.size(); i++) 
    {
        if (avg[i] > 0) 
        {
            recommendations.push_back(make_pair(avg[i], books[i]));
        }
    }
    sort(recommendations.begin(), recommendations.end(), greater<pair<double, string>>());
    stable_sort(recommendations.begin(), recommendations.end(), [](auto& a, auto& b) 
    {
        if (a.first != b.first) 
        {
            return a.first > b.first; //Sorts by average rating in descending order
        }
        return a.second < b.second; //Sorts by book title in alphabetical order
    });    

    #ifdef DEBUG
    cout << "Recommendations for: " << userName;
    for (const string& book : books) {
        cout << book;
    }
    #endif
    
    // Prints recommendations
    for (auto& rec : recommendations) 
    {
        cout << fixed << setprecision(2) << rec.second << " " << rec.first << endl;
    }

}
    
void BookRecommender::printAverages() 
{
    //Creates a vector of pairs from the map
    vector<pair<string, double>> sortedAverages(averages.begin(), averages.end());
    
    //Sorts the averages first by rating, then alphabetically
    sort(sortedAverages.begin(), sortedAverages.end(), [](const pair<string, double>& a, const pair<string, double>& b) 
    {
        if (a.second != b.second) 
        {
            return a.second > b.second; //Sorts by rating
        } 
        else 
        {
            return a.first < b.first; //Sort alphabetically
        }
    });
    
    //Prints out the book averages
    for (const auto& avgRating : sortedAverages) 
    {
        cout << fixed << setprecision(2) << avgRating.first << " " << avgRating.second << endl;
    }
}


double BookRecommender::getAverage(string bookTitle)
{
    if (averages.find(bookTitle) != averages.end()) //Returns the average of a given book
    {
        return averages[bookTitle];
    } 
    else 
    {
        return 0;
    }
}
    
double BookRecommender::getSimilarity(string userName1, string userName2)
{
    int rec = 0;
    int i = 0;

        //Calculates the similarity of two given users' ratings
        for (map<string, vector<double>>::iterator it = ratedBooks.begin(); it != ratedBooks.end(); ++it) 
        {
            i = i + 1;
            rec += (ratedBooks[userName1][i] * ratedBooks[userName2][i]);
        }
        return rec;
}

int BookRecommender::getBookCount()
{
    //returns the size of vector books
    return sizeof(books);
}

int BookRecommender::getUserCount()
{
    //returns the size of vector users
    return sizeof(users);
}

double BookRecommender::getUserBookRating(string userName, string bookTitle) 
{
    //Checks if the requested user and book are in ratings
    if (ratedBooks.count(userName) == 0) {
        return 0;
    }
    int bookIndex = distance(books.begin(), find(books.begin(), books.end(), bookTitle));
    if (bookIndex == books.size()) {
        return 0;
    }
    if (ratedBooks[userName][bookIndex] == 0) {
        return 0;
    }
    //returns a given user's rating for a given book
    return ratedBooks[userName][bookIndex];
}














