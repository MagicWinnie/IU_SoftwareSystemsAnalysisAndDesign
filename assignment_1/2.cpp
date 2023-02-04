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

int main()
{
    ifstream inp("input.txt");
    ofstream out("output.txt");

    string a, b, tmp;
    char op;
    
    getline(inp, a, ',');
    getline(inp, b, ',');
    getline(inp, tmp, ',');
    op = tmp.at(0);

    switch (op)
    {
    case '-':
        out << subtract(a, b) << endl;
        break;
    case '+':
        out << add(a, b) << endl;
        break;
    case '*':
        out << multiply(a, b) << endl;
        break;

    default:
        out << "not implemented" << endl;
        break;
    }

    inp.close();
    out.close();

    return 0;
}
