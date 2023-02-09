#include <iostream>
#include <string>
#include <fstream>

using namespace std;

void findMaxNumber(string line, int size, int &index, int &value)
{
    int max_value = -10;
    int max_index = -1;
    for (int i = 0; i < size; i++)
    {
        if (i + 1 < size and line[i] == '-' and isdigit(line[i + 1]))
        {
            if (-(line[i + 1] - '0') > max_value)
            {
                max_value = -(line[i + 1] - '0');
                max_index = i + 1;
            }
            i += 1;
        }
        else if (isdigit(line[i]) and (line[i] - '0' > max_value))
        {
            max_value = line[i] - '0';
            max_index = i;
        }
    }
    index = max_index;
    value = max_value;
}

int main()
{
    ifstream inp("input.txt");
    ofstream out("output.txt");

    string input;
    inp >> input;

    int index, value;
    findMaxNumber(input, input.size(), index, value);

    if (index != -1)
        out << value << ',' << index << endl;
    else
        out << index << endl;

    inp.close();
    out.close();
    return 0;
}
