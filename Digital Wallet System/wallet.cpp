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

    Wallet& getWallet(){
        return wallet;
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
        ofstream file("users.txt");

        for(auto &u:users){
            file<<u.toFileFormat()<<endl;
        }
        file.close();
    }
    
    static void saveTransaction(string record) {
        ofstream file("transactions.txt", ios::app);
        file << record << endl;
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
/*-----------------------------------The Main Function--------------------------*/

int main(){
    
    while(true){

        cout<<"\n-----Welcome to DigiPay------\n";
        cout<<"1.Register(New User)\n";
        cout<<"2.Login\n";
        cout<<"3.Exit\n";
        cout<<"Enter a choice: ";

        int choice;
        cin>>choice;

        /*---------------- REGISTER ----------------*/
        if(choice==1){

            vector<User> users=FileManager::loadusers();
            string name,phone,pin;

            cout<<"Enter your Name: ";
            cin>>name;

            cout<<"Enter your phone number: ";
            cin>>phone;

            bool exists=false;
            for(auto &u:users){
                if(u.getPhone()==phone){
                    exists=true;
                    break;
                }
            }

            if(exists){
                cout<<"Hey You're Already Registered! Kindly Login.\n";
                continue;
            }

            cout<<"Set Pin: ";
            cin>>pin;

            users.push_back(User(name,phone,pin,0));
            FileManager::Saveusers(users);

            cout<<"Registration Successful\n";
        }

        /*---------------- LOGIN ----------------*/
        else if(choice==2){

            vector<User> users=FileManager::loadusers();
            string phone,pin;

            cout<<"Enter your phone number: ";
            cin>>phone;

            cout<<"Enter your pin: ";
            cin>>pin;

            bool loggedIn=false;

            for(auto &u:users){

                if(u.getPhone()==phone && u.VerifyPin(pin)){

                    loggedIn=true;

                    cout<<"Welcome Back to DigiPay "
                        <<u.getName()<<endl;

                    while(true){

                        cout<<"\n-----Choose From Your Wallet Menu:-----\n";
                        cout<<"1.Add Money\n";
                        cout<<"2.Send Money\n";
                        cout<<"3.Check Balance\n";
                        cout<<"4.Transactions\n";
                        cout<<"5.Logout\n";
                        cout<<"Enter your choice: ";

                        int opt;
                        cin>>opt;

                        if(opt==1){

                            double amount;
                            cout<<"Enter amount: ";
                            cin>>amount;

                            u.getWallet().addMoney(amount);
                            FileManager::Saveusers(users);

                            Transaction t("Credit", amount);
                            FileManager::saveTransaction(
                                t.toFileFormat(phone));

                            cout<<"Your account is credited with "
                                <<amount<<" rupees\n";
                        }

                        else if(opt==2){

                            string receiver;
                            double amount;

                            cout<<"Enter Receiver's Phone: ";
                            cin>>receiver;

                            cout<<"Enter amount to be sent: ";
                            cin>>amount;

                            bool receiverFound=false;

                            for(auto &r:users){

                                if(r.getPhone()==receiver){

                                    receiverFound=true;

                                    if(u.getWallet().deductMoney(amount)){

                                        r.getWallet().addMoney(amount);
                                        FileManager::Saveusers(users);

                                        Transaction t1("Debit",amount);
                                        FileManager::saveTransaction(
                                            t1.toFileFormat(phone));

                                        Transaction t2("Credit",amount);
                                        FileManager::saveTransaction(
                                            t2.toFileFormat(receiver));

                                        cout<<"Transaction Successful\n";
                                    }
                                    else{
                                        cout<<"Insufficient Balance!\n";
                                    }
                                    break;
                                }
                            }

                            if(!receiverFound){
                                cout<<"Receiver Not Found!\n";
                            }
                        }

                        else if(opt==3){
                            cout<<"Your current balance is: "
                                <<u.getWallet().getBalance()<<endl;
                        }

                        else if(opt==4){
                            FileManager::showTransactions(phone);
                        }

                        else if(opt==5){
                            cout<<"Logged Out.\n";
                            break;
                        }

                        else{
                            cout<<"Invalid Choice!\n";
                        }
                    }

                    break;
                }
            }

            if(!loggedIn){
                cout<<"Ohh NO! Invalid Credentials.\n";
            }
        }

        /*---------------- EXIT ----------------*/
        else if(choice==3){
            cout<<"Thank you for using DigiPay!\n";
            break;
        }

        else{
            cout<<"Invalid Choice!\n";
        }
    }

    return 0;
}
