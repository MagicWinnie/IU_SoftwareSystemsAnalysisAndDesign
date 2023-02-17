#include <iostream>
#include <cmath>
#define ull unsigned long long
#define OPENFILES freopen("input.txt", "r", stdin);freopen("output.txt", "w", stdout);

using namespace std;

ull minimumCountOfInmates(ull n)
{
    /*
    We give each person two potions as one can drink only two potions in 48 hours.
    If the first one is poisoned then we will know it in 20 hours.
    If the second one is poisoned then we will know it in 40 hours.
    If none of two are poisoned then he/she will live.
    To solve the task we need to find minimum integer x that:
    2^x >= n, where x is number of people, n number of potions
    x = ceil(log2(n))
    */
    if (n == 0ULL)
        return 0ULL;
    return ceil(log2(n));
}

int main(void)
{
    OPENFILES

    ull n;
    cin >> n;

    cout << minimumCountOfInmates(n) << endl;

    return 0;
}