# include <iostream>
# include <cstdio>
# include <fstream>
# include <cstring>
# include <cstdlib>
# include "lexhead.h"
using namespace std;

// may 2019, antaranyan v, group 214, msu cmc

#define STGT (stack[st_top]).get_type() 

// Процедуры для синтаксического анализа

void A();
void B();
void C();

void E();
void F();

void J();
void K();

void R();

void T();
void U();
void V();
void W();
void X();
void Y();

lextype tmp_lt;
Lex expression[100];
int i = 0;


// перевод программы во внутреннее представление

struct Poland {
    Lex *program;
    Lex *stack;
    int size;
    int pr_top;
    int st_top;

    void refresh(Lex *what, Lex token, int &top) {
        what[++top] = token;
        if (top == size - 3)
        {
            size <<= 1;
            what = (Lex*) realloc(what, size);
        }
    }

    void pop() {
        program[++pr_top] = stack[st_top--];
    }
    
    Lex & operator[] (int k) {
        if (i > pr_top)
        {
            cout << "\nerror: out of poliz\n";
            return curr_lex;
        } 
        else
            return program[k];
    }
    
    void expr(Lex token = curr_lex);

    void expr_poliz() {
        int j = 0;
        while (j != i)
            expr(expression[j++]);
    }
    
        
    Poland(): size(100), pr_top(-1), st_top(-1) {
        program = (Lex*)calloc(size, sizeof(Lex));
        stack = (Lex*)calloc(size, sizeof(Lex));
    }
    
    void print() {
        for (int ind = 0; ind <= pr_top; ++ind)
            cout << program[ind] << ' ';
        cout << endl;
    }
    ~Poland() { delete [] program; delete [] stack; }
};


void Poland::expr(Lex token) {
//STGT - defined stack[st_top].get_type() 
    tmp_lt = token.get_type();
    
    if ( (tmp_lt == LEX_ID) || (tmp_lt == LEX_NUM) || (tmp_lt == LEX_CONSTCH)
      || (tmp_lt == LEX_NOT) )
        refresh(program, token, pr_top);

    else if ( (tmp_lt == LEX_READ) || (tmp_lt == LEX_WRITE) || (tmp_lt == LEX_OPEN) )
        refresh(stack, token, st_top);

    else if (tmp_lt == LEX_COMMA)
        while (STGT != LEX_OPEN)
            pop();

    else if (tmp_lt == LEX_CLOSE)
    {
        while (STGT != LEX_OPEN)
            pop();
        st_top--;
        if ( (STGT == LEX_WRITE) || (STGT == LEX_READ) )
            pop();
    }

    else if ( (tmp_lt == LEX_TIMES) || (tmp_lt == LEX_SLASH) )
    {
        while ( (STGT == LEX_NOT) || (STGT == LEX_TIMES) || (STGT == LEX_SLASH) )
            pop();
        refresh(stack, token, st_top);
    }

    else if ( (tmp_lt == LEX_PLUS) || (tmp_lt == LEX_MINUS) )
    {
        while ( (STGT == LEX_NOT) || (STGT == LEX_TIMES) || (STGT == LEX_SLASH)
             || (STGT == LEX_PLUS) || (STGT == LEX_MINUS) )
            pop();
        refresh(stack, token, st_top);
    }

    else if ( (tmp_lt == LEX_LSS) || (tmp_lt == LEX_GRT) || (tmp_lt == LEX_LEQ)
             || (tmp_lt == LEX_GEQ) || (tmp_lt == LEX_NEQ) || (tmp_lt == LEX_EQ) )
    {
        while ( (STGT == LEX_PLUS) || (STGT == LEX_MINUS) || (STGT == LEX_SLASH) 
              || (STGT == LEX_TIMES) || (STGT == LEX_NOT) || (STGT == LEX_LSS)
              || (STGT == LEX_GRT) || (STGT == LEX_LEQ) || (STGT == LEX_GEQ) || (STGT == LEX_NEQ)
              || (STGT == LEX_EQ) )
            pop();
        refresh(stack, token, st_top);
    }

    else if (tmp_lt == LEX_AND)
    {
        while ( (STGT == LEX_PLUS) || (STGT == LEX_MINUS) || (STGT == LEX_SLASH)
             || (STGT == LEX_TIMES) || (STGT == LEX_NOT) || (STGT == LEX_LSS)
             || (STGT == LEX_GRT) || (STGT == LEX_LEQ) || (STGT == LEX_GEQ) || (STGT == LEX_NEQ)
             || (STGT == LEX_EQ) )
            pop();
        refresh(stack, token, st_top);
    }

    else if (tmp_lt == LEX_OR)
    {
        while ( (STGT == LEX_PLUS) || (STGT == LEX_MINUS) || (STGT == LEX_SLASH)
             || (STGT == LEX_TIMES) || (STGT == LEX_NOT) || (STGT == LEX_LSS) || (STGT == LEX_GRT)
             || (STGT == LEX_LEQ) || (STGT == LEX_GEQ) || (STGT == LEX_NEQ) || (STGT == LEX_EQ)
             ||  (STGT == LEX_AND) )
            pop();
        refresh(stack, token, st_top);
    }

    else if (tmp_lt == LEX_ASSIGN)
    {
        program[pr_top].put_type(POLIZ_ADDRESS);

        while ( (STGT == LEX_PLUS) || (STGT == LEX_MINUS) || (STGT == LEX_SLASH)
             || (STGT == LEX_TIMES) || (STGT == LEX_NOT) || (STGT == LEX_LSS)
             || (STGT == LEX_GRT) || (STGT == LEX_LEQ) || (STGT == LEX_GEQ)
             || (STGT == LEX_NEQ) || (STGT == LEX_EQ) ||  (STGT == LEX_AND) || (STGT == LEX_OR) )
            pop();
        refresh(stack, token, st_top);
    }

}

Poland generator;


void A() {
//    cout << "A\n";
    curr_lex.check_type(LEX_PROGRAM);
    try { curr_lex.refresh(); }
    catch (int) { cout << "error: empty program " << endl; return; }
    curr_lex.check_type(LEX_ST);
    try {
        curr_lex.refresh();
        B();
        J();
    }
    catch (int) { cout << "error: '}' expected" << endl; return; }
    curr_lex.check_type(LEX_FIN);
    try { curr_lex.refresh(); }
    catch (int) { cout << "Success!\n"; return; }
    cout << "error: extra tokens after the program" << endl;
}

void B() {
//    cout << "B\n";
    while ((lt == LEX_INT) || (lt == LEX_STRING)) {
        tmp_lt = lt;
        C();
        curr_lex.check_type(LEX_SEMICOLON);
        curr_lex.refresh();
    }
}

void C() {
//    cout << "C\n";
    curr_lex.refresh();
    E();
    while (lt == LEX_COMMA) {
        curr_lex.refresh();
        E();
    }
}


void E() {
//    cout << "E\n";
    curr_lex.check_type(LEX_ID);
    if (TID[curr_lex.get_value()].get_declare())
    {
        cout << "\nattempt to declare once again\n";
        throw curr_lex;
    }
    else TID[curr_lex.get_value()].put_declare();
    curr_lex.refresh();
    if (lt == LEX_ASSIGN)
    {
//        cout << "!!!\n";
        curr_lex.refresh();
        F();
    }
}

void F() {
//    cout << "F\n";
    if ( (lt == LEX_PLUS) || (lt == LEX_MINUS) || (lt == LEX_NUM) )
    {
        if (tmp_lt == LEX_STRING)
        {
            cout << "\nconflicting types while initializing\n";
            throw curr_lex;
        }
        if ( (lt == LEX_PLUS) || (lt == LEX_MINUS))
        {
            curr_lex.refresh();
            curr_lex.check_type(LEX_NUM);
            curr_lex.refresh();
        }
        else if (lt == LEX_NUM)
        {
            curr_lex.refresh();
        }
    }
    else
    {
        if (tmp_lt == LEX_INT)
        {
            cout << "\nconflicting types while initializing\n";
            throw curr_lex;
        }
        curr_lex.check_type(LEX_CONSTCH);
        curr_lex.refresh();
    }
}


void J() {
//    cout << "J\n";
    while ( (lt == LEX_IF) || (lt == LEX_WHILE) || (lt == LEX_READ) || (lt == LEX_WRITE)
          || (lt == LEX_ST) || (lt == LEX_ID) || (lt == LEX_NUM) )
    {
        if ( (lt == LEX_ID)&&(!(TID[curr_lex.get_value()].get_declare())) )
        {
            cout << "\nobject not declared\n";
            throw curr_lex;
        }
        K();
    }
}

void K() {
//    cout << "K\n";
    if (lt == LEX_IF) {
        curr_lex.refresh();
        curr_lex.check_type(LEX_OPEN);
        curr_lex.refresh();
        R();
        curr_lex.check_type(LEX_CLOSE);
//        generator.program[++generator.pr_top].put_type(POLIZ_LABEL);
//        int remember_place_1 = generator.pr_top;
//        generator.program[++generator.pr_top].put_type(POLIZ_FGO);

        curr_lex.refresh();
        K();
        curr_lex.check_type(LEX_ELSE);

//        generator.program[++generator.pr_top].put_type(POLIZ_LABEL);
//        int remember_place_2 = generator.pr_top;
//        generator.program[++generator.pr_top].put_type(POLIZ_GO);
//        generator.program[remember_place_1].put_value(generator.pr_top+1);

        curr_lex.refresh();
        K();
//        generator.program[remember_place_2].put_value(generator.pr_top+1);
    }
    else if (lt == LEX_WHILE) {
        curr_lex.refresh();
        curr_lex.check_type(LEX_OPEN);
        curr_lex.refresh();
        R();
        curr_lex.check_type(LEX_CLOSE);
        curr_lex.refresh();
        K();
    }
    else if (lt == LEX_READ) {
//        generator.expr();
        curr_lex.refresh();
        curr_lex.check_type(LEX_OPEN);
//        generator.expr();
        curr_lex.refresh();
        curr_lex.check_type(LEX_ID);
//        generator.expr();
        if (!(TID[curr_lex.get_value()].get_declare()))
        {
            cout << "\nobject not declared\n";
            throw curr_lex;
        }
        curr_lex.refresh();
        curr_lex.check_type(LEX_CLOSE);
//        generator.expr();
//        generator.print();
        curr_lex.refresh();
        curr_lex.check_type(LEX_SEMICOLON);
//        generator.expr();
        curr_lex.refresh();
    }
    else if (lt == LEX_WRITE) {
//        generator.expr();
        curr_lex.refresh();
        curr_lex.check_type(LEX_OPEN);
//        generator.expr();
        curr_lex.refresh();
        R();
        while (lt == LEX_COMMA)
        {
//            generator.expr();
            curr_lex.refresh();
            R();
        }
        curr_lex.check_type(LEX_CLOSE);
//        generator.print();
        curr_lex.refresh();
        curr_lex.check_type(LEX_SEMICOLON);
        curr_lex.refresh();
    }
    else if (lt == LEX_ST) {
        curr_lex.refresh();
        J();
        curr_lex.check_type(LEX_FIN);
        curr_lex.refresh();
    }
    else if ( (lt == LEX_ID) || (lt == LEX_NOT) || (lt == LEX_PLUS) || (lt == LEX_MINUS) || (lt == LEX_NUM) )
    {
        if ( (lt == LEX_ID) && (!(TID[curr_lex.get_value()].get_declare())) )
        {
            cout << "\nobject not declared\n";
            throw curr_lex;
        }
        R();
        curr_lex.check_type(LEX_SEMICOLON);
        generator.refresh(generator.program, curr_lex, generator.pr_top);
        generator.print();
        curr_lex.refresh();
    }
}


void R() {
//    cout << "R\n";
    i = 0;
    expression[i++] = curr_lex;
    T();
    while (lt == LEX_ASSIGN)
    {
        if ( (!( (i == 2)||(expression[i-3].get_type()==LEX_ASSIGN))) || (expression[i-2].get_type() != LEX_ID) )
        {
            cout << "\nrvalue as the left operand of assign\n";
            throw curr_lex;
        }
        curr_lex.refresh();
        expression[i++] = curr_lex;
        T();
    }

    
    generator.expr_poliz();
    while (generator.st_top != -1)
        generator.pop();
    generator.print();
    
    
}

void T() {
//    cout << "T\n";
    U();
    while (lt == LEX_OR)
    {
        curr_lex.refresh();
        expression[i++] = curr_lex;
        U();
    }
}

void U() {
//    cout << "U\n";
    V();
    while (lt == LEX_AND)
    {
        curr_lex.refresh();
        expression[i++] = curr_lex;
        V();
    }
}

void V() {
//    cout << "V\n";
    W();
    while ( (lt == LEX_LSS) || (lt == LEX_GRT) || (lt == LEX_LEQ)
            || (lt == LEX_GEQ) || (lt == LEX_EQ) || (lt == LEX_NEQ) )
    {
            curr_lex.refresh();
            expression[i++] = curr_lex;
            W();
    }
}


void W() {
//    cout << "W\n";
    
    X();
    while ( (lt == LEX_PLUS) || (lt == LEX_MINUS) )
    {
        curr_lex.refresh();
        expression[i++] = curr_lex;
        X();
    }
}

void X() {
//    cout << "X\n";
    if (lt == LEX_CONSTCH) {
        curr_lex.refresh();
        expression[i++] = curr_lex;
        if (lt == LEX_MINUS)
            throw curr_lex;
        return;
    } //
    Y();
    while ( (lt == LEX_TIMES) || (lt == LEX_SLASH) )
    {
        curr_lex.refresh();
        expression[i++] = curr_lex;
        Y();
    }
}

void Y() {
//    cout << "Y\n";
    if (lt == LEX_NOT)
    {
        curr_lex.refresh();
        expression[i++] = curr_lex;
        Y();
    }
    else
    if ( (lt != LEX_NUM) && (lt != LEX_ID) )
    {
        throw curr_lex;
    }
    else
    {
        if ( (lt == LEX_ID) && (!(TID[curr_lex.get_value()].get_declare())) )
        {
            cout << "\nobject not declared\n";
            throw curr_lex;
        }
        curr_lex.refresh();
        expression[i++] = curr_lex;
    }
}





