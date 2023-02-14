#include <iostream>
#include <string>

using namespace std;

class Animal
{
protected:
    string name;

public:
    Animal(string name)
    {
        this->name = name;
    }
    virtual void sound()
    {
        cout << name << ": silence" << endl;
    }
};

class Dog : public Animal
{
public:
    Dog(string name) : Animal(name) {}
    void sound() override
    {
        cout << name << ": woof" << endl;
    }
};

class Cat : Animal
{
public:
    Cat(string name) : Animal(name) {}
    void sound() override
    {
        cout << name << ": meow" << endl;
    }
};

int main(void)
{
    Dog dog("Name_1");
    Cat cat("Name_2");

    dog.sound();
    cat.sound();

    return 0;
}