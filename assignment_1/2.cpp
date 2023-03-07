#include <iostream>
#include <string>
#include <fstream>

using namespace std;

string subtract(string a, string b)
{
    if (a.substr(a.size() - b.size()) == b)
        return a.substr(0, a.size() - b.size());
    else
        return "NaN";
}

string add(string a, string b)
{
    return a + b;
}

string multiply(string a, string b)
{
    string out = "";
    for (int i = 0; i < stoi(b); i++)
        out += a;
    return out;
}

string solve(string a, string b, string op)
{
    if (op == "-")
        return subtract(a, b);
    else if (op == "+")
        return add(a, b);
    else if (op == "*")
        return multiply(a, b);
    else
        return "not implemented";
}

int main()
{
    ifstream inp("input.txt");
    ofstream out("output.txt");

    string a, b, op;

    getline(inp, a, ',');
    getline(inp, b, ',');
    getline(inp, op, ',');

    out << solve(a, b, op) << endl;

    inp.close();
    out.close();

    return 0;
}
