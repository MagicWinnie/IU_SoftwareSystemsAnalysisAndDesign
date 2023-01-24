#include <iostream>

using namespace std;

int main()
{
    int a, b;
    cin >> a >> b;
    if (a > b)
        cout << "A > B" << endl;
    else if (a < b)
        cout << "A < B" << endl;
    else
        cout << "A = B" << endl;
    return 0;
}