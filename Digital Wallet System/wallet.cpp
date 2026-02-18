#include<iostream>
#include<string>
#include<fstream>
#include<vector>
#include<sstream>
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
/*----------------------------Transaction-------------------------------------------*/
class Transaction{
private:
    string type;
    double amount;
    string date;

public:
    Transaction(string t, double amt){
        this->type=t;
        this->amount=amt;
        this->date=getCurrentDate();
    }

    string toFileFormat(string phone){
        return phone+ '|'+ type + "|"+ to_string(amount)+"|"+date;
    }
};
/*-----------------------------Wallet-----------------------------------------------------*/
class Wallet{
private:
    double balance;
public:
    Wallet(double b=0){
        this->balance=b;
    }
    double getBalance(){
        return balance;
    }

    void addMoney(double amount){
        this->balance+=amount;
    }

    bool deductMoney(double amount){
        if(balance>=amount){
            balance-=amount;
            return true;
        }
        return false;
    }
};
/*------------------------------user----------------------------------------------------*/
class User{
private:
    string name;
    string phone;
    string pin;
    Wallet wallet;

public:
    User(string n, string p, string pi, double w){
    this->name=n;
    this->phone=p;
    this->pin=pi;
    this->wallet=w;
    }
    string getName(){
        return name;
    }

    string getPhone(){
        return phone;
    }

    string getPin(){
        return pin;
    }

    double getWallet(){
        return wallet.getBalance();
    }
    
    bool VerifyPin(string inputPin){
        return this->pin==inputPin;
    }
string toFileFormat(){
    return name + "|" + phone + "|" + pin + "|" + to_string(wallet.getBalance());
}
};

/*-----------------------------Managing File----------------------------------*/

class FileManager{
public:
    static vector<User>loadusers(){
        vector<User>users;
        ifstream file("users.txt");
        string line;

    while (getline(file,line)){
    stringstream ss(line);
    string name,phone,pin,balance;

    getline(ss,name,'|');
    getline(ss,phone,'|');
    getline(ss,pin,'|');
    getline(ss,balance,'|');

    users.push_back(
        User(name,phone,pin,stod(balance))
    );
    }
    file.close();
    return users;
}

    static void Saveusers(vector<User>&users){
        ofstream file("users.text");

        for(auto &u:users){
            file<<u.toFileFormat()<<endl;
        }
        file.close();
    }

    static void showTransactions(string phone){
        ifstream file("transactions.txt");
        string line;

        cout<<"\n-----Transaction History----";

        while (getline(file,line)){
            stringstream ss(line);
            string fphone,type,amount,date;

            getline(ss,fphone,'|');
            getline(ss,type,'|');
            getline(ss,amount,'|');
            getline(ss,date,'|');

            if(fphone==phone){
                cout<<type<<"|"<<amount<<"||"<<date<<endl;
            }
        }
        file.close();
    }
};