#include <iostream>

using namespace std;

int main()
{
    int x = 10;
    int *p = &x;
    int **pp = &p;
    cout << x << ' ' << *p << ' ' << **pp << endl;
    return 0;
}
