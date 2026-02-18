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