#include <iostream>
#include <cctype>

using namespace std;

template <typename T, typename U>
auto custom_max(T a, U b)
{
    return (a > b) ? a : b;
}
template <>
auto custom_max<char>(char a, char b)
{
    char a_lower = tolower(a);
    char b_lower = tolower(b);
    return (a_lower > b_lower) ? a : b;
}

int main(void)
{
    cout << custom_max(5, 7) << endl;
    cout << custom_max(2.5, 1.5) << endl;
    cout << custom_max('A', 'b') << endl;
    cout << custom_max(5, 2.5) << endl;

    return 0;
}