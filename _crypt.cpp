
/****************

        This program is written to demonstrate the weakness of the xor-cipher with a key length of the small
        (compared with length of open text)

*****************/


#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <vector>
#include <fstream>
#include <cmath>

using namespace std;


typedef vector<double> vd;


bool inline letter(char c)
{
    return (c >= 'a' && c <= 'z' || c >= 'A' && c <= 'Z' || c == '_');
}

string crypt(const string& key, const string& m)
{
    string s;
    for(size_t i = 0; i < m.size(); ++i)
        s += key[i % key.size()] ^ m[i];
    return s;
}

string xor_f(const string& s1, const string& s2)
{
    string s;
    if (s1.size() != s2.size())
    {
        cout << "FAIL!\n";
        return "";
    }

    for(size_t i = 0; i < s1.size(); i++)
    {
        s += s1[i] ^ s2[i];
    }
    return s;
}

void shift(string& s)
{
    string ss;
    ss += s[s.size() - 1];
    ss += s.substr(0, s.size() - 1);
    s = ss;
}


vd count_matches(const string& c)
{
    vd v;
    int count;
    string shift_s = c;
    string xor_s;
    size_t size = c.size();
    double index_match = 0.0;
    for(size_t i = 0; i < size - 1; ++i)
    {
        index_match = 0.0;
        count = 0;
        shift(shift_s);
        xor_s = xor_f(shift_s, c);

        for(int sym = 0; sym < 128; ++sym)
        {
            count = 0;
            for(size_t j = 0; j < xor_s.size(); ++j)
            {
                if (xor_s[j] == (char)sym) ++count;
            }
            index_match += count * (count - 1);
        }
//        if (v.empty())
//        {
            v.push_back(index_match / (size * (size - 1)));
//        }
//        else
//        {
//            v.push_back(fabs(index_match / (size * (size - 1)) - v[v.size() - 1]));
//        }
    }

    return v;
}

string decrypt(const string& c, int len)
{
    bool exit = true;
    vector<char> vc;
    for(int i = 0; i < 127; ++i)
        if (letter((char)i)) vc.push_back(char(i));

    for(int j = 0; j < len; j++)
    {
        for(int k = 0; k < vc.size(); ++k)
        {
            exit = true;
            for(int i = j; i < c.size(); i += len)
            {
                if (!letter(c[i] ^ vc[k]))
                {
                    exit = false;
                    break;
                }
            }

            if (exit)
            {
                cout << vc[k] << "";

            }
        }
        //cout << endl;
    }

    return "";
}


int main()
{
//    ofstream out;
//    out.open("out.txt");
//    string k = "somekey";
//    string m = "Whether these examples can be considered natural language is another questiondubious discuss";
//    string c = crypt(k, m);
//    //cout << c << endl;
//    vd v = count_matches(c);
//    for(int i = 0; i < v.size(); i++)
//    {
//        out << v[i] << endl;
//    }
//    out.close();
    const int LEN_KEY = 24;
    ifstream is("encryptedtext.txt");
    string s;
    if (is.is_open())
    {
        getline(is, s, (char)EOF);
        decrypt(s, LEN_KEY);
        is.close();
    }
    else
    {
        cout << "where input file?\n";
    }

    cout << endl;
    return 0;
}

