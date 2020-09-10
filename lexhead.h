# include <iostream>
# include <cstdio>
# include <fstream>
# include <cstring>
using namespace std;

// may 2019, antaranyan v, group 214, msu cmc
char ch;
char tmp_ch;
int lines = 1;
FILE *f;
FILE *file;
int key;

void newline() {
    lines++;
    cout << endl;
 }

const char *base_t[] =
{
    "",
    "program",
    "int",
    "string",
    "if",
    "else",
    "while",
    "read",
    "write",
    "not",
    "and",
    "or",
    NULL
};

const char *sign_t[] =
{
    "",
    ";",
    ",",
    ":",
    "*",
    "/",
    "+",
    "-",
    "<",
    ">",
    "<=",
    ">=",
    "==",
    "!=",
    "=",
    "\"",
    "{",
    "}",
    "(",
    ")",
    NULL
};
int d, j;

enum lextype {

    LEX_NULL,		// 0
    LEX_PROGRAM,	// 1
    LEX_INT,		// 2
    LEX_STRING,		// 3
    LEX_IF,		// 4
    LEX_ELSE,		// 5
    LEX_WHILE,		// 6
    LEX_READ,		// 7
    LEX_WRITE,		// 8
    LEX_NOT,		// 9
    LEX_AND,		// 10
    LEX_OR,		// 11

    LEX_SEMICOLON,	// 12 ;
    LEX_COMMA,		// 13 ,
    LEX_COLON,		// 14 :
    LEX_TIMES,		// 15 *
    LEX_SLASH,		// 16 /
    LEX_PLUS,		// 17 +
    LEX_MINUS,		// 18 -
    LEX_LSS,		// 19 <
    LEX_GRT,		// 20 >
    LEX_LEQ,		// 21 <=
    LEX_GEQ,		// 22 >=
    LEX_EQ,		// 23 ==
    LEX_NEQ,		// 24 !=
    LEX_ASSIGN,		// 25 =
    LEX_QUOT,		// 26 "
    LEX_ST,		// 27 {
    LEX_FIN,		// 28 }
    LEX_OPEN,		// 29 (
    LEX_CLOSE,		// 30 )
    
    LEX_NUM,            // 31
    LEX_ID,		// 32
    LEX_CONSTCH,	// 33
    
    POLIZ_LABEL,	// 34
    POLIZ_ADDRESS,	// 35
    POLIZ_GO,		// 36
    POLIZ_FGO		// 37
};


class Ident {
    char *name;
    bool declare;
    lextype type;
    bool assign;
    int value;
public:
    Ident() { declare = assign = false; }
    char *get_name() const { return name; }
    void put_name(const char * n)
    {
        name  = new char [strlen(n)+1];
        strcpy(name, n); 
    }
    bool get_declare() const { return declare; }
    void put_declare() { declare = true; }
    lextype get_type() const { return type; }
    void put_type(lextype t) { type = t; }
    bool get_assign() const { return assign; }
    void put_assign() { assign = true; }
    int get_value() const { return value; }
    void put_value(int v) { value = v; }
    ~Ident() {  } 
};  

class tabl_ident {
    Ident *p;
    int size;
    int top;
public:
    tabl_ident (int max_size) {
        p = new Ident[size = max_size];
        top = 1;
    }
    ~tabl_ident() { 
        for (int j = 1; j < top; ++j)
            delete [] p[j].get_name();
        delete []p;
    }
    Ident & operator[] (int k) { return p[k]; }
    int put (const char *buf);
};

int tabl_ident::put(const char *buf)
{
    for (int j = 1; j < top; j++)
        if (! strcmp(buf, p[j].get_name())) return j;
    p[top].put_name(buf);
    top++;
    return top-1;
}

tabl_ident TID(100);

class tabl_constch {
    char **p;
    int size;
    int top;
public:
    tabl_constch(int max_size) {
        p = new  char*[size = max_size];
        top = 1;
    }
    ~tabl_constch() {
        for (int j = 1; j < top; ++j)
            delete []p[j];
        delete [] p;
    }
    const char * operator[] (int k) { return p[k]; }
    int put(const char *buf) {
        for (int j = 1; j < top; ++j)
            if (!strcmp(buf, p[j]) ) return j;
        p[top] = new  char [strlen(buf)+1];
        strcpy(p[top], buf);
        top++;
        return top-1;
    }
};

tabl_constch TCS(100);

lextype words[] =
{
    LEX_NULL,
    LEX_PROGRAM,
    LEX_INT,
    LEX_STRING,
    LEX_IF,
    LEX_ELSE,
    LEX_WHILE,
    LEX_READ,
    LEX_WRITE,
    LEX_NOT,
    LEX_AND,
    LEX_OR,
    LEX_NULL
};

lextype signs[] =
{
    LEX_NULL,
    LEX_SEMICOLON,
    LEX_COMMA,
    LEX_COLON,
    LEX_TIMES,
    LEX_SLASH,
    LEX_PLUS,
    LEX_MINUS,
    LEX_LSS,
    LEX_GRT,
    LEX_LEQ,
    LEX_GEQ,
    LEX_EQ,
    LEX_NEQ,
    LEX_ASSIGN,
    LEX_QUOT,
    LEX_ST,
    LEX_FIN,
    LEX_OPEN,
    LEX_CLOSE,
    LEX_NULL
};

char buf[100];
int buf_top;

class Lex {
    lextype type;
    int value;
public:

    Lex(lextype t = LEX_NULL, int v = 0): type(t), value(v) {}
    
    void put_type(lextype new_type) { type = new_type; } // 13 may 2019, for poliz
    void put_value(int new_value) { value = new_value; } //
    lextype get_type() { return type; }
    void check_type(lextype optype) { // для синт. анализа
        if (type != optype)
        {
            if (optype <= 11)
                cout << "error: expected lexeme: \"" <<  base_t[optype] << "\"" << endl;
            else if (optype <= 30)
                cout << "error: expected lexeme: \"" << sign_t[optype-11] << "\"" << endl;
            else if (optype == 32)
                cout << "error: expected identifier" << endl;
            throw *this;
        }
    }
    int get_value() const { return value; }
    void refresh();
    friend ostream& operator<<(ostream & show, Lex lexem);
};

ostream& operator <<(ostream & show, Lex lexem)
{
    show << '(' << lexem.type << ',' << lexem.value << ')' << ' ';
    return show;
}

lextype lt;

void Lex::refresh() {
    int ch = 0;
    int t = 0;
    while ( ((ch=fgetc(file)) != EOF)&&(ch != '(') );
    if (ch == EOF)
        throw ch;
    fscanf(file, "%d", &t);
    type = (lextype) t;
    lt = type;
    fgetc(file);
    fscanf(file, "%d", &value);
}
        




Lex curr_lex;






// Лексический анализ
    
class State {
protected: 
    void clear()
    {
        buf_top = 0;
        for (int j = 0; j < 100; ++j)
            buf[j] = '\0';
    }

    void add() { buf[buf_top++] = ch;}

    int look(char *buf, const char **where)
    {
        int i = 0;
        while (where[i])
        {
            if (!strcmp(buf, where[i]))
                return i;
            i++;
        }
        return 0;
    }
public:    
    virtual State * Step() = 0;
    virtual ~State() {}
};


class H: public State {
public:
    virtual State * Step();
};

class WORD: public State {
public:
    virtual State * Step()
    {
        while  ( (isalpha(ch = fgetc(f))) || (isdigit(ch)) )
            add();
        if ( (j = look(buf, base_t)) != 0 )
            cout << Lex(words[j], j);
        else {
            j = TID.put(buf);
            cout << Lex(LEX_ID, j);
        }
        if (ch == '\n') newline();
        return new H();
    }
};

class NUMB: public State {
public:
    virtual State * Step()
    {
        while (isdigit(ch = fgetc(f)))
            d = d*10 + (ch - '0');
        cout << Lex(LEX_NUM, d);
//        cout << d << ' ';
        if (ch == '\n') newline();
        return new H();
    }
};

class COM: public State {
public:
    virtual State * Step();
};

class SLASH: public State {
public:
    virtual State * Step()
    {
        clear();add();
        ch = fgetc(f);
        if (ch == '*')
            return new COM();
         j = look(buf, sign_t);
        cout << Lex(LEX_SLASH, j);
        clear(); add();
        return new H(); 
    }
};

class BYECOM: public State {
public:
    virtual State * Step();
};

class ERR: public State {
public:
    virtual State * Step() {
        cerr << "Lexical error : unknown character " << tmp_ch << endl;
        return NULL;
    }
};

State* COM::Step()
{
    while (!((ch = fgetc(f)) == '*') || (ch == EOF) );
    if (ch == '*') return new BYECOM();
    return new ERR();
}

State* BYECOM::Step()
{
    ch = fgetc(f);
    if(ch == '/')
    {
        cout << endl;
        ch = fgetc(f);
        return new H();
    }
    return new COM();
}

class PLUSMIN: public State {
public:
    virtual State * Step()
    {
        clear(); add();
        tmp_ch = ch; 
        ch = fgetc(f);
        if ((j = look(buf, sign_t)) > 0)
        {
            cout << Lex(signs[j], j);
            if (ch == '\n') newline();
            return new H();
        }
        return new ERR();
    }
};

class NEQ: public State {
public:
    virtual State * Step()
    {
        ch = fgetc(f);
        if (ch == '=')
        {
            add(); j = look(buf, sign_t);
            cout << Lex(LEX_NEQ, j);
            ch = fgetc(f);
            return new H();
        }
        return new ERR();
    }
};
    
class VARIOUS: public State {
public:
    virtual State * Step()
    {
        ch = fgetc(f);
        if (ch == '=')
            add();
        j = look(buf, sign_t);
        cout << Lex(signs[j], j);
//        if (isspace(ch))
        ch = fgetc(f); // may be bad here 11/05/19
        return new H();
    }
};

// "abc" - ident или const ???

class STR: public State {
public:
    virtual State * Step()
    {
        while (((ch = fgetc(f)) != '"') && (ch != EOF))
            add();
        if (ch == EOF) return 0;
        j = TCS.put(buf);
        cout << Lex(LEX_CONSTCH, j);
        ch = fgetc(f);
        if (ch == '\n') newline();
        return new H();
    } 
};

       
State* H::Step()
{
    if (!key)
    {
        while (isspace(ch = fgetc(f)))
            if (ch == '\n') lines++;
        key = 1;
    }
    if (isspace(ch))
        while (isspace(ch = fgetc(f)))
            if (ch == '\n') lines++;
    if (isalpha(ch))
    {
        clear(); add();
        return new WORD();
    }
    else if (isdigit(ch))
    {
        d = ch -'0';
        return new NUMB();
    }
    else if (ch == '"') { clear(); return new STR(); }
    else if (ch == '/') { add(); return new SLASH(); }
    else if (ch == '!') { clear(); add(); return new NEQ(); }
    else if ((ch == '<') || (ch == '>') || (ch == '=')) { clear(); add(); return new VARIOUS();}
    else return new PLUSMIN();
}



