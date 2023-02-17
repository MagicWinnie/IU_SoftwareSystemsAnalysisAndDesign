#include <iostream>
#include <string>

using namespace std;

class BankAccount
{
private:
    string name;
    double balance;

    bool checkEnoughMoney(double money)
    {
        return balance >= money;
    }
public:
    BankAccount(string name, double balance)
    {
        this->name = name;
        this->balance = balance;
    }
    void putMoney(double money)
    {
        balance += money;
    }
    void getMoney(double money)
    {
        if (checkEnoughMoney(money))
            balance -= money;
        else
            cout << "Not enough money" << endl;
    }
    void printMoney() 
    {
        cout << balance << endl;
    }
};

int main(void)
{
    BankAccount acc("Dmitriy", 100);
    acc.printMoney();

    acc.putMoney(234);
    acc.printMoney();

    acc.getMoney(12);
    acc.printMoney();

    acc.getMoney(10000);

    return 0;
}