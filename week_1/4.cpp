#include <iostream>
#include <string>

using namespace std;

int main()
{
    string s;
    cin >> s;
    bool isPalindrome = true;
    for (int i = 0; i < s.size() / 2; i++)
    {
        if (s[i] != s[s.size() - i - 1])
        {
            isPalindrome = false;
            break;
        }
    }
    cout << (isPalindrome ? "It is palindrome" : "It is not palindrome") << endl;
    return 0;
}