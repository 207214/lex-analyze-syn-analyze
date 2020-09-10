# include <iostream>
# include <cstdio>
# include <fstream>
# include <cstring>
# include "synhead.h"
using namespace std;


int main(int argc, char **argv)
{
    key = 0;
    ofstream out(argv[2]);
    streambuf *coutbuf = cout.rdbuf();
    cout.rdbuf(out.rdbuf());
    f = fopen(argv[1], "r");
    

    State * st1;
    State * tmp;
    H * st2 = new H;
    st1 = st2->Step();
    while (ch != EOF && st1 != 0)
    {
        tmp = st1;
        st1 = st1->Step();
        delete tmp;
    }

    if (ch != '\n') cout << endl;
    cout.rdbuf(coutbuf);
    fclose(f);
    delete st1;
    delete st2;
    cout << "number of lines: " << lines << endl;
    out.close();

    file  = fopen(argv[2], "r");
    try {
        curr_lex.refresh();
        A();
//        cout << "вышел!" << endl;
    }
    catch(Lex c) { cout << "Err:" << c << endl; return 1;}
    fclose(file);
    return 0;
}

