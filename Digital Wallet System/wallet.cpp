#include<iostream>
#include<string>
#include<fstream>
#include<vector>
#include<iomanip>
#include<ctime>
using namespace std;
/*---------------------------Get Current Date and Time--------------------------*/
string getCurrentDate(){
    time_t now=time(0);
    string dt= ctime(&now);
    dt.pop_back();
    return dt;
}

class User{
private:
    string name;
    string phone;
    string pin;
    string wallet;

public:
    User(string n, string p, string pi, string w)
};