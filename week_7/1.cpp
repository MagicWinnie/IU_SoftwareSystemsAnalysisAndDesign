#include <iostream>

using namespace std;

template <typename T>
T custom_max(T a, T b)
{
    return (a > b) ? a : b;
}

int main(void)
{
    cout << custom_max(3, 5) << endl;
    cout << custom_max(3.14, 2.71) << endl;
    cout << custom_max('a', 'A') << endl;

    return 0;
}