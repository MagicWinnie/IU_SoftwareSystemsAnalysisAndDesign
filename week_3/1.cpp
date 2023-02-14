#include <iostream>

using namespace std;

int fibonacci_1(int a)
{
    if (a == 0)
        return 0;
    if (a == 1)
        return 1;
    return fibonacci_1(a - 1) + fibonacci_1(a - 2);
}

int fibonacci_2(int n)
{
    int a = 0, b = 1;
    for (int i = 0; i < n; i++)
    {
        int tmp = b + a;
        a = b;
        b = tmp;
    }
    return a;
}

int main()
{
    for (int i = 0; i < 10; i++)
        cout << fibonacci_1(i) << endl;
    
    for (int i = 0; i < 10; i++)
        cout << fibonacci_2(i) << endl;

    return 0;
}
