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

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

//#define DEBUG

using namespace std;


int main(int argc, char* argv[]) 
{
    string option;
    string programUser;
    int i = 0;

    BookRecommender recommender(argv[1]);

    cin >> option;
    cin.ignore();
    #ifdef DEBUG
    cout << "Option: " << option << endl;
    #endif

    while (option != "quit") //If quit is entered, the program quits
    {
        if (option == "recommend")
        {
            //Recommends a given user a book
            cin >> programUser;
            cin.ignore();
            
            #ifdef DEBUG
            cout << "User: " << programUser << endl;
            #endif
            recommender.printRecommend(programUser);
        }
        else if (option == "averages")
        {
            //Prints the averages of all the ratings of all the books
            recommender.printAverages();
        }
        else
        {
        }
        cin >> option;
        #ifdef DEBUG
        cout << "Option: " << option << endl;
        #endif
    }

    return 0;
}

