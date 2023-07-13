#include "parse.cpp"


#include <iostream>
#include<string>
#include <list>
#include <cstddef>
#include <cmath>
#include<vector>
#include <fstream>

#include <cstdlib>
#include <stack>
#include <typeinfo>
#include <algorithm>
#include <cctype>
#include <string>
#include <sstream>



#define pi 3.14159265359
#define e 0.5772156649



#include "term.cpp"

#include "skracanie.cpp"


std::ofstream Plik;

// ERROR


// WYNIK


// klasa bazowa

class Wyznacznik
{
public:
    Term* numerator1;
    Term* denominator1;
    Term* numerator2;
    Term* denominator2;
    Term* numerator3;
    Term* denominator3;
    Term * dx_Special ;
    Term * dy_Special ;
    Term * d_Special;


public:
    Wyznacznik(Term* numa, Term* numb, Term* numc, Term* dena, Term* denb, Term* denc)
    {
        numerator1 = numa;
        denominator1 = dena;
        numerator2 = numb;
        denominator2 = denb;
        numerator3 = numc;
        denominator3 = denc;
        dx_Special;
        dy_Special;
        d_Special;


    }


    void print_new(std::ostream& plik)
    {
        //Term* A = new Var("A");
        //i  Term* B = new Var("B");

        plik << "\\begin{equation}";
        plik << "\\left[\\begin{array}{cc|c}" << std::endl;
        numerator1->print_new(plik);
        plik << "&";
        numerator2->print_new(plik);
        plik << "&  ";
        numerator3->print_new(plik);
        plik << "\\\\\n";
        denominator1->print_new(plik);
        plik << "&";
        denominator2->print_new(plik);
        plik << "&  ";
        denominator3->print_new(plik);
        plik << "\\\\\n";
        plik << "\\end{array}\\right] ";
        plik << "\\end{equation}";
        plik << "\\\\";
        plik << "\\\\";


        Term* det = new Add();
        Term* det_1 = new Multiplication();
        Term* det_2 = new Multiplication();

        det_1->Add_term(numerator1);
        det_1->Add_term(denominator2);
        det_2->Add_term(numerator2);
        det_2->Add_term(denominator1);
        det->Add_term(det_1);
        det->Add_term(new SubtractU(det_2));

        Term* detx1 = new Add();
        Term* detx2 = new Multiplication();
        Term* detx3 = new Multiplication();

        detx2->Add_term(numerator3);
        detx2->Add_term(denominator2);
        detx3->Add_term(numerator2);
        detx3->Add_term(denominator3);
        detx1->Add_term(detx2);
        detx1->Add_term(new SubtractU(detx3));

        /// Term* detx = new Division(detx1, det);
        Term* dety1 = new Add();
        Term* dety2 = new Multiplication();
        Term* dety3 = new Multiplication();

        dety2->Add_term(numerator1);
        dety2->Add_term(denominator3);
        dety3->Add_term(numerator3);
        dety3->Add_term(denominator1);
        dety1->Add_term(dety2);
        dety1->Add_term(new SubtractU(dety3));
        /// Term* dety = new Division(dety1, det);



        d_Special = det ;
        dx_Special = detx1  ;
        dy_Special  = dety1 ;





        // delete W ;


    }


    Result calculate()
    {
        Result a1 = numerator1->calculate();
        Result b1 = numerator2->calculate();
        Result c1 = numerator3->calculate();
        Result a2 = denominator1->calculate();
        Result b2 = denominator2->calculate();
        Result c2 = denominator3->calculate();
        Result result;
        double x, y;
        double det = a1.liczba * b2.liczba - b1.liczba * a2.liczba;
        std::cout << "det " << det;
        if (det == 0)
        {
            std::cout << "UkĂ…â€šad rĂÂłwnaĂ…â€ž jest sprzeczny lub nieoznaczony." << std::endl;
        }
        else
        {
            std::cout << " c1  =  " << c1.liczba << " b2   = " << b2.liczba << "  b1   = " << b1.liczba << "  c2   = " << c2.liczba;




            x = (c1.liczba * b2.liczba - b1.liczba * c2.liczba) / det;
            y = (a1.liczba * c2.liczba - c1.liczba * a2.liczba) / det;
            std::cout << " x = " << x << " y = " << y;
        }
        result.liczba = x;
        result.liczba2 = y;
        return result;
    }


    ~Wyznacznik()
    {
        delete numerator1;
        delete numerator2;
        delete denominator1;
        delete denominator2;
        delete denominator3;
        delete numerator3;

    }
};



class Wyznacznik__2_x_2___
{
public:
    Term* numerator1;
    Term* denominator1;
    Term* numerator2;
    Term* denominator2;

    Wyznacznik__2_x_2___(Term* numa, Term* numb, Term* dena, Term* denb) : numerator1(numa), numerator2(numb), denominator1(dena), denominator2(denb) {}

    void print_new(std::ostream& plik)
    {

        plik << "\\left|\\left[\\begin{array}{cc}" << std::endl;
        numerator1->print_new(plik);
        plik << " & ";
        numerator2->print_new(plik);
        plik << " \\\\\n";
        denominator1->print_new(plik);
        plik << " & ";

        denominator2->print_new(plik);
        plik << " \\\\\n";
        plik << "\\end{array}\\right]\\right|";



    }

    Result calculate() const
    {
        Result a1 = numerator1->calculate();
        Result b1 = numerator2->calculate();
        Result a2 = denominator1->calculate();
        Result b2 = denominator2->calculate();

        double det = a1.liczba * b2.liczba - b1.liczba * a2.liczba;
        if (det == 0)
        {
            std::cout << "Macierz jest osobliwa. Nie moĹĽna obliczyÄ‡ wyznacznika." << std::endl;

        }
        else
        {
            double x = a1.liczba / det;
            double y = b1.liczba / det;
            std::cout << "x = " << x << ", y = " << y << std::endl;

        }
    }

    ~Wyznacznik__2_x_2___()
    {
        delete numerator1;
        delete numerator2;
        delete denominator1;
        delete denominator2;
    }
};







class Parentheses : public Term
{
public:
    Parentheses(Term* t) : Term("VarIndex"), term(t)
    {
        prioriti = -1;
    }
    virtual void print_new(std::ostream& plik)
    {
        plik << "(";
        term->print_new(plik);
        plik << ")";
    }


public:
    Term* term;

    Result calculate() override
    {
        Result ret = term->calculate();
        return ret;
    }
};

class Equal : public Term
{
public:
    Equal(Term* l, Term* r) : Term("Equal"), lhs(l), rhs(r) {}
    virtual void print_new(std::ostream& plik)
    {
        lhs->print_new(plik);
        plik << " = ";
        rhs->print_new(plik);


    }
    void print()
    {
        lhs->print_new(std::cout);
        std::cout << " = ";
        rhs->print_new(std::cout);
    }

public:
    Term* lhs;
    Term* rhs;
};
Term* ZwrocmyGlownySZereguuzywajacstosu(FormalSeries* A)
{
    std::stack<Term*> stos; // Tworzenie stosu z wskaĂ…Âşnikami na obiekty typu Term

    // Dodawanie elementĂÂłw na stos
    stos.push(A);


    // Odczytywanie elementu z wierzchoĂ…â€ška stosu
    Term* glownyElement = stos.top();

    // Usuwanie elementĂÂłw ze stosu
    stos.pop();


    // Sprawdzanie, czy stos jest pusty
    if (stos.empty())
    {
        std::cout << "Stos jest pusty." << std::endl;
    }

    return glownyElement;
}
void liczszereg(Term* A, Term* B, Term* Z, Term* J, std::ofstream& plik)
{



    plik << "  \\begin{center}  \\LARGE\\textsc{Równanie  Rekurencyjne}  \\end{center}  \n ";

    plik << " \\paragraph{Metoda: Szereg Formalny}    \n ";

    //   <<<<<<<<<<<<<  Rownanie do obliczenia >>>>>>


    Term* aen = new VarIndex(new Var("a"), new Var("n"));



    Term* WILDAdd = new Add();
    Term* WILDMult1 = new Multiplication();
    Term* WILDMult2 = new Multiplication();

    WILDMult1->Add_term(A);
    WILDMult1->Add_term(new VarIndex(new Var("a"),  new Var("n-1")));

    WILDMult2->Add_term(B);
    WILDMult2->Add_term(new VarIndex(new Var("a"), new Var("n-2")));


    WILDAdd->Add_term(WILDMult1);
    WILDAdd->Add_term(WILDMult2);
    plik << "\\begin{equation}";
    aen->print_new(plik);
    plik << " = \\begin{cases}  ";
    Z->print_new(plik);
    plik << ", &  \\text{dla } n = 0 \\\\ ";
    J->print_new(plik);
    plik << ", &  \\text{dla } n = 1 \\\\ ";
    WILDAdd->print_new(plik);
    plik << ", &  \\text{dla } n > 1 \\\\";
    plik << "\\end{cases}";
    plik << "\\end{equation}";





    Term* x_n = new VarIndex(new Var("x"), new Var("n"));
    Term* x_n_mniej_jeden = new VarIndex(new Var("x"), new Var("n-1"));
    Term* x_n_mniej_dwa = new VarIndex(new Var("x"), new Var("n-2"));

    Term* pierwszy = new Multiplication(new VarIndex(new Var("a"), new RealNumber(0)),
                                        new Power(new Var("x"), new RealNumber(0)));
    Term* drugi = new Multiplication(new VarIndex(new Var("a"), new RealNumber(1)),new Power(new Var("x"), new RealNumber(1)));

    Term* PD = new Add(pierwszy, drugi);
    Term* equation = new Equal(x_n, new Add(new Multiplication(A, x_n_mniej_jeden), new Multiplication(B, x_n_mniej_dwa)));
    Term* IL = new Multiplication(new VarIndex(new Var("a"), new Var("n")), new Power(new Var("x"), new Var("n")));
    Term* cos = new Add(new Multiplication(A, new VarIndex(new Var("a"), new Var("n-1"))), new Multiplication(B, new VarIndex(new Var("a"), new Var("n-2"))));
    FormalSeries* SIL = new FormalSeries(new Var("2"), new Var("\\infty"), IL, new Var("x"));
    FormalSeries* SILL = new FormalSeries(new Var("2"), new Var("\\infty"), new Multiplication(cos, new Power(new Var("x"), new Var("n"))), new Var("i"));
    Term* next = new Add(PD, SILL);
    Term* pierwszemnozenie = new Multiplication(new Multiplication(A, new VarIndex(new Var("a"), new Var("n-1"))), new Power(new Var("x"), new Var("n")));
    Term* drugiemnozenie = new Multiplication(B, new VarIndex(new Var("a"), new Var("n-2")));
    Term* coscos = new Add(pierwszemnozenie, new Multiplication(drugiemnozenie, new Power(new Var("x"), new Var("n"))));

    // <<<<<<<<<<<!!!!!!!! wyciagniecie x z  {X} ^ n  i zostaje x ^ n - 1         i pomnozenie razy a _ {n -1} x ^n - 1    +           x  ^ n-2		 itd ..............  !!!!!!!!!!!! >>>>>>>>>>>>>>>>
    FormalSeries* SILLL = new FormalSeries(new Var("2"), new Var("\\infty"), coscos, new Var("n"));
    Term* SILLLL = new FormalSeries(new Var("2"), new Var("\\infty"), new Multiplication(new Multiplication(A, new Multiplication(new VarIndex(new Var("a"), new Var("n-1")), new Power(new Var("x"), new Var("n-1")))), new Var("x")), new Var("n"));
    Term* SILLLL2 = new FormalSeries(new Var("2"), new Var("\\infty"), new Multiplication(new Multiplication(B, new Multiplication(new VarIndex(new Var("a"), new Var("n-2")), new Power(new Var("x"), new Var("n-2")))), new Power(new Var("x"), new Var("2"))), new Var("n"));
    Term* SILCOMBO = new Add(SILLLL, SILLLL2);

    //  <<<<<<<<<<<<<<<!!!!!!!!!!!!!!!!  WYCIAGAMY A x PRZED SZEREG  !!!!!!!!!>>>>>>>>>>>>>>>>>>>>>
    // n - 1
    Term* pierwszemnozenie_a = new Multiplication(new VarIndex(new Var("a"), new Var("n-1")), new Power(new Var("x"), new Var("n - 1")));
    Term* drugiemnozenie_a = new Multiplication(new VarIndex(new Var("a"), new Var("n-2")), new Power(new Var("x"), new Var("n-2")));


    // pomocnicze !!!!!!!
    Term* pierwszemnozenie_a_temp_HELP = new Multiplication(A, new Var("x"));
    Term* drugiemnozenie_a_temp_HELP = new Multiplication(B, new Var("x"));

    Term* pierwszemnozenie_a_temp_HELP_2 = new Multiplication(new Var("x"), A);
    Term* drugiemnozenie_a_temp_HELP_2 = new Multiplication(B, new Power(new Var("x"), new Var("2")));
    Term* SIK = new Add(new Multiplication(pierwszemnozenie_a_temp_HELP, new FormalSeries(new Var("2"), new Var("\\infty"), pierwszemnozenie_a, new Var("n"))), new Multiplication(drugiemnozenie_a_temp_HELP_2, new FormalSeries(new Var("2"), new Var("\\infty"), drugiemnozenie_a, new Var("n"))));


    //  przejscie na   {j}  {i}   !! <<<<<<<<< >>>>>>>> !!!
    Term* pierwszemnozenie_b = new Multiplication(new VarIndex(new Var("a"), new Var("j")), new Power(new Var("x"), new Var("j")));
    Term* drugiemnozenie_b = new Multiplication(new VarIndex(new Var("a"), new Var("i")), new Power(new Var("x"), new Var("i ")));
    Term* SIE = new Add(new Multiplication(pierwszemnozenie_a_temp_HELP, new FormalSeries(new Var("1"), new Var("\\infty"), pierwszemnozenie_b, new Var("j"))), new Multiplication(drugiemnozenie_a_temp_HELP_2, new FormalSeries(new Var("0"), new Var("\\infty"), drugiemnozenie_b, new Var("i"))));


    //  <<<<<<<<<<<<< !!!!!!!!!!!!!!!!!!!!!! >>>>>>>>>>>>>>>>>>>>
    //  zamiana  sumy a_j * x ^ j    na     f(x) - a_0 * x ^ 0    . Odejmujemy a_x* x ^ 0 bo od 1 sie zaczyna

    //Term* pierwszemnozenie_c = new Subtrac(new Var("f(x)"), new Multiplication(new VarIndex(new Var("a"), new Var("0")), new Power(new Var("x"), new Var("0"))));
    //Term* drugiemnozenie_c = new Var("f(x)");
    //Term* SIEWIE = new Add(new Multiplication(pierwszemnozenie_a_temp_HELP, pierwszemnozenie_c), new Multiplication(drugiemnozenie_a_temp_HELP_2, drugiemnozenie_c));



    Term* azeroxzero = new Multiplication();
    Term* ajedenxjeden = new Multiplication();
    azeroxzero->Add_term(new VarIndex(new Var("a"), new Var("0")));
    ajedenxjeden->Add_term(new Power(new Var("x"), new Var("0")));

    Term* BetterAdd = new Add();
    Term* BetterMultiply2 = new Multiplication();
    {
        Term* BetterMultiply1 = new Multiplication();
        Term* BetterAddsmall = new Add();
        BetterMultiply1->Add_term(A->copy_term());
        BetterMultiply1->Add_term(new Var("x"));
        BetterAddsmall->Add_term(new Var("f(x)"));
        BetterAddsmall->Add_term(new SubtractU(new VarIndex(new Var("a"), new Var("0"))));
        BetterMultiply1->Add_term(new Parentheses(BetterAddsmall));
        BetterMultiply2->Add_term(B->copy_term());
        BetterMultiply2->Add_term(new Power(new Var("x"), new Var("2")));
        BetterMultiply2->Add_term(new Var("f(x)"));
        BetterAdd->Add_term(BetterMultiply1);
        BetterAdd->Add_term(BetterMultiply2->copy_term());
    }


    Term* tempg = new Multiplication();
    tempg->Add_term(A);
    tempg->Add_term(new Var("x"));
    tempg->Add_term(new Var("f(x)"));




    Term* BetterAddlewe = new Add();
    BetterAddlewe->Add_term(new Var("f(x)"));
    BetterAddlewe->Add_term(new SubtractU(tempg));
    BetterAddlewe->Add_term(new SubtractU(BetterMultiply2->copy_term()));









    // Declaration and initialization of VarIndex term
    Term* varIndexTerm = new Multiplication(new VarIndex(new Var("a"), new Var("n")), new Power(new Var("x"), new Var("n")));

    // Declaration and initialization of FormalSeries object
    Term* FunkcjaTworzaca = new FormalSeries(new Var("2"), new Var("\\infty"), varIndexTerm, new Var("n"));



    Term* aa = B;
    Term* bb = new SubtractU(new Multiplication(Z, J));
    //Term* cc = new RealNumber(1);



    Add* wiel = new Add();
    Term* aaa = new SubtractU(B); // Odejmowanie: A - B
    Term* bbb = new SubtractU(new Multiplication(Z, J)); // Odejmowanie: Z * J - B
    Term* ccc = new RealNumber(1); // Liczba rzeczywista: 1
    Term* dd = new RealNumber(0); // Liczba rzeczywista: 0

    Term* ee = new Multiplication(aa, bb); // MnoĂ…ÂĽenie: (A + B) * (C * D)
    Term* ff = new Add(ee, new SubtractU(new RealNumber(1))); // Odejmowanie: (A + B) * (C * D) - (1 - B)
    Term* gg = new Add(ff, dd); // Dodawanie: (A + B) * (C * D) - (1 - B) + 0

    Term* aaaa = new Power(A, B); // PotĂ„â„˘gowanie: A^B
    Term* bbbb = new Multiplication(A, Z); // MnoĂ…ÂĽenie: A * C
    Term* cccc = new Add( new Add(aaaa, new SubtractU(bbbb)),
                          new SubtractU(new RealNumber(1))); // Odejmowanie: (A^B - A*C) - 1
    Term* dddd = new Add(cccc, ff); // Dodawanie: (A^B - A*C) - 1 + 0

    wiel->Add_term(aaa);
    wiel->Add_term(bbb);
    wiel->Add_term(ccc);

    Term* temptemp = new Add(new Var("1"), new SubtractU(pierwszemnozenie_a_temp_HELP));
    Term* temptemptemp = new Add(temptemp, new SubtractU(new Multiplication(J, new Var("x"))));
    Term* temptemptemptemp = new Add(temptemptemp, new SubtractU(drugiemnozenie_a_temp_HELP_2));
    Term* lewe_mnozenie = new Multiplication(new Var("f(x)"), temptemptemptemp);

    //Term* prawe_dodawaie = new Add(new Multiplication(Z, new Power(new Var("x"), new Var("0"))), new Multiplication(pierwszemnozenie_a_temp_HELP, Z));
    Term* prawe_dodawanie = new Add();
    Term* Ferdek = new Multiplication();
    Term* pazdzioch = new Add();
    Term* boczek = new Multiplication();
    boczek->Add_term(new SubtractU(A));
    boczek->Add_term(new VarIndex(new Var("a"), new Var("0")));
    boczek->Add_term(new Power(new Var("x"), new Var("0")));
    pazdzioch->Add_term(new VarIndex(new Var("a"), new Var("1")));
    pazdzioch->Add_term(boczek);
    Ferdek->Add_term(new Var("x"));
    Ferdek->Add_term(new Parentheses(pazdzioch));
    prawe_dodawanie->Add_term(new VarIndex(new Var("a"), new Var("0")));
    prawe_dodawanie->Add_term(Ferdek);








    Term* lewy = new Multiplication();
    Term* MagentaAdd = new Add();
    Term* MagentasmallMult1 = new Multiplication();
    Term* MagentasmallMult2 = new Multiplication();
    Term* MagentasmallMult3 = new Multiplication();

    MagentasmallMult1->Add_term(A->copy_term());
    MagentasmallMult1->Add_term(new Var("x"));


    MagentasmallMult2->Add_term(J);
    MagentasmallMult2->Add_term(new Var("x"));

    MagentasmallMult3->Add_term(B);
    MagentasmallMult3->Add_term(new Power(new Var("x"), new Var("2")));


    // DodaĂ…â€šĂ„â„˘m twarde nawiasy
    MagentaAdd->Add_term(new Var("1"));


    MagentaAdd->Add_term(new SubtractU(MagentasmallMult1));
    //MagentaAdd->Add_term(new SubtractU(MagentasmallMult2));
    MagentaAdd->Add_term(new SubtractU(MagentasmallMult3));



    lewy->Add_term(new Var("f(x)"));
    lewy->Add_term(new Parentheses(MagentaAdd));








    plik << "\n\n\\noindent Skupiamy sie na równaniu:\n";
    plik << "\\begin{equation} ";
    equation->print_new(plik);
    plik << "\\end{equation}\n\n\n";



    Term* nextequation = new Add(PD, FunkcjaTworzaca);


    plik << "\\noindent Dokonujemy obliczeń symbolicznych na szeregu wykorzystując tylko zalezność rekurencyjną:\n";

    plik << "\\begin{equation}\n";
    plik << "\\begin{split}\n";
    plik << "f(x)  &= ";
    nextequation->print_new(plik);
    plik << "\\\\ \n";
    // rowwnanie po f(x)
    plik << "     & = ";
    next->print_new(plik);
    plik << "\\\\ \n";
    // nizej 1 f(x)
    plik << "     & = ";
    Term* next1 = new Add(PD, SILLL);
    next1->print_new(plik);
    plik << "\\\\ \n";
    plik << "     & = ";




    // nizej 2 f(x)
    // nizej 3 f(x)

    Term* ASDZ = new Add(PD, SILCOMBO);
    ASDZ->print_new(plik);



    Term* next222 = new Add(PD, SIK);
    plik << "\\\\ \n";
    plik << "     & = ";
    Term* next3 = new Add(PD, SIE);

    // nizej 4 ????
    next3->print_new(plik);
    plik << "\\\\ \n";
    plik << "     & = ";

    Term* next4 = new Add(PD, BetterAdd);

    next4->print_new(plik);


    plik << "\\\\ \n";
    plik << "\\end{split}\n";
    plik << "\\end{equation}\n";

    plik << "\\noindent Stąd uzyskujemy równość:\n";

    plik << "\\begin{equation}\n";
    BetterAddlewe->print_new(plik);
    Term* tempi = new Multiplication();
    tempi->Add_term(A);
    tempi->Add_term(new Var("x"));
    tempi->Add_term(new SubtractU(new VarIndex(new Var("a"), new Var("0"))));
    //tempi->print_new(plik);

    Term* tempv = new Add();
    tempv->Add_term(PD);
    tempv->Add_term(tempi);
    plik << "=  ";
    tempv->print_new(plik);
    plik << "\\end{equation}\n";
    //	SIEWIE->print_new(plik);



    plik << "Wyodrębniamy $f(x)$:\n";
    plik << "\\begin{equation}\n";
    lewy->print_new(plik);
    plik << " = ";
    prawe_dodawanie->print_new(plik);// jest ok prawe dodawanie
    plik << "\\end{equation}\n";

    plik << "Skąd ostatecznie $f(x)$:\n";
    plik << "\\begin{equation}\n";
    plik << "f(x) =    \\frac{";
    prawe_dodawanie->print_new(plik);
    plik << "}{";
    MagentaAdd->print_new(plik);
    plik << "}\n";
    plik << "\\end{equation}\n";
    plik << "W kolejnym etapie rozkładamy ułamek na ułamki proste.\n";
    plik << "W tym celu odnajdujemy parametry pierwiastków $x_1$, $x_2$ dla których:";
    plik << "\\begin{equation}\n";
    MagentaAdd->print_new(plik);
    plik << "= -";
    B->print_new(plik);
    plik << "\\cdot (x-x_1)\\cdot(x-x_2)";
    plik << "\\end{equation}\n";



    delete lewe_mnozenie, temptemp, temptemptemp, temptemptemptemp, prawe_dodawanie;
    delete pierwszemnozenie_b, drugiemnozenie_b, SIK;
    delete pierwszemnozenie_a, drugiemnozenie_a;
    delete SIK, next3;
    //delete  SILLLLL;
    delete next1;
    delete pierwszemnozenie, drugiemnozenie, coscos;
    delete next;
    delete x_n, x_n_mniej_jeden, x_n_mniej_dwa;
    delete IL, SILLL;
    delete SIL, SILL;
    delete pierwszy, drugi, PD, cos;
    delete SILLLL, SILLLL2, SILCOMBO;
    delete nextequation,varIndexTerm, FunkcjaTworzaca;

    // pomocnicze !!!!!!!usun pamitaj
    delete pierwszemnozenie_a_temp_HELP;
    delete drugiemnozenie_a_temp_HELP;

    delete pierwszemnozenie_a_temp_HELP_2;
    delete drugiemnozenie_a_temp_HELP_2;
}

void liczszeregVIP(Term* A, Term* B, Term* ZERO, Term* JEDEN, std::ofstream& plik)
{

    /// rownanie kwadratowe


    /// wspolczynnik
    Term* aa =  new SubtractU(B);
    Term* bb = new SubtractU(A);
    Term* cc = new RealNumber(1);


    /// - 4 * a * c
    Term* MinusCzteryarazyc = new Multiplication();
    MinusCzteryarazyc->Add_term(new SubtractU(new RealNumber(4)));
    MinusCzteryarazyc->Add_term(aa);
    MinusCzteryarazyc->Add_term(cc);


    /// D E L T A
    Term* Delta = new Add();
    Delta->Add_term(new Power(bb, new RealNumber(2)));
    Delta->Add_term(MinusCzteryarazyc);
    // Delta->term_tree(plik);


    /// skracanie delty
    plik << "Obliczamy delte:\n";
    plik << "$\\delta" ;
    while (skracanie(Delta,true) )
    {
        plik << "=";
        Delta->print_new(plik);
    }
    plik << "$";
    plik << "\nStąd wyznaczamy pierwiastki $x_1$, $x_2$:";
    //Delta->term_tree(plik);


    plik<< "\\begin{center}\\begin{minipage}{0.49\\textwidth}\n";



    /// <<<<<<<<< x1 i x2 >>>>>>>>>>>>>>>>>>>>>
    Term* minusBEdodacsqrtDELTA = new Add();
    minusBEdodacsqrtDELTA->Add_term(new SubtractU(bb));
    minusBEdodacsqrtDELTA->Add_term(new Sqrt(new RealNumber(2), Delta));
    Term* x1 = new Division(minusBEdodacsqrtDELTA, new Multiplication(new RealNumber(2), aa));
    plik << "\\begin{align}";
    plik << "x_1 = & ";
    x1->print_new(plik);
    while (skracanie(x1,true))
    {
        plik << "\\notag \\\\ &= ";
        x1->print_new(plik);
    }
    plik << "\\notag\\end{align}";
    plik << "\\end{minipage}\n";
    plik<< "\\begin{minipage}{0.49\\textwidth}\n";

//  x1->term_tree(plik);

    Result wynikx1 = x1->calculate();
    Term* minusBEodjacsqrtDELTA = new Add();
    minusBEodjacsqrtDELTA->Add_term(new SubtractU(bb));
    minusBEodjacsqrtDELTA->Add_term(new SubtractU(new Sqrt(new RealNumber(2), Delta)));
    Term* x2 = new Division(minusBEodjacsqrtDELTA, new Multiplication(new RealNumber(2), aa));

    Result wynikx2 = x2->calculate();
    plik << "\\begin{align}";
    plik << "x_2 = & ";
    x2->print_new(plik);

    while (skracanie(x2,true))
    {
        plik << "\\notag \\\\ &= ";
        x2->print_new(plik);
    }

    plik << "\\notag \\end{align}";
    plik << "\\end{minipage}\n";
    plik << "\\end{center}\n";

    plik << "Z twierdzenia o rozkładzie ułamka wymiernego na ułamki proste, istnieją $C_1$, $C_2$ dla których:";

    //  x2->term_tree(plik);



    /// To co jest pod x1 i x2 w pdfie czyli   f(x) =  ... dzielone ...  = C1 / x - x1  + C2 / x - x2



    Term* dodawnie_gorne = new Add();
    Term* mnozenie_glowne_z_x = new Multiplication();
    Term* wewnetrzne_dodawanie = new Add();
    Term* wewnetrzne_mnozenie = new Multiplication();


    wewnetrzne_mnozenie->Add_term(new SubtractU(A));
    wewnetrzne_mnozenie->Add_term(ZERO);
    wewnetrzne_mnozenie->Add_term(new Power(new Var("x"), new Var("0")));
    wewnetrzne_dodawanie->Add_term(JEDEN);
    wewnetrzne_dodawanie->Add_term(wewnetrzne_mnozenie);
    mnozenie_glowne_z_x->Add_term(new Var("x"));
    mnozenie_glowne_z_x->Add_term(wewnetrzne_dodawanie);
    dodawnie_gorne->Add_term(ZERO);
    dodawnie_gorne->Add_term(mnozenie_glowne_z_x);

    plik << "\\begin{equation}\n";
    Term* Ax = new Multiplication(A, new Var("x"));
    Term* A_jeden_razy_x = new Multiplication(JEDEN, new Var("x"));
    Term* B_razy_x_do_kwadratu = new Multiplication(B, new Power(new Var("x"), new Var("2")));
    Term* TEMP = new Multiplication(A, new Var("x"));


    /// dol ulamka  pierwszego
    Term* dol_ulamka = new Add();
    dol_ulamka->Add_term(new Var("1"));
    dol_ulamka->Add_term(new SubtractU(Ax));
    dol_ulamka->Add_term(new SubtractU(B_razy_x_do_kwadratu));
    Term* ulamek = new Division(dodawnie_gorne, dol_ulamka);
    ulamek->print_new(plik);
    plik << " = \\frac{C_1}{x-x_1}+\\frac{C_2}{x-x_2}" ;
    plik << "\\end{equation}\n";
    /// prawa strona po dzieleniu
    /// C1 /  (x - x1 )



    /// Dlugie rownanie

    Term * LEWE_dodawanie = new Add();
    Term* wewnetrzne_mnozenie_x = new Multiplication();
    Term* wewnetrzne_mnozenie_ladniejsze_odjemnik = new Add();
    Term * wewnetrzne_wewnetrzne_mnozenie_A_razy_a_zero = new Multiplication();
    Term * wewnetrzne_odejmowanie = new Add();

    wewnetrzne_wewnetrzne_mnozenie_A_razy_a_zero->Add_term(A);
    wewnetrzne_wewnetrzne_mnozenie_A_razy_a_zero->Add_term(ZERO);
    wewnetrzne_odejmowanie->Add_term(JEDEN);
    wewnetrzne_odejmowanie->Add_term( new SubtractU( wewnetrzne_wewnetrzne_mnozenie_A_razy_a_zero));

    wewnetrzne_mnozenie_x ->Add_term(new Var("x"));
    wewnetrzne_mnozenie_x->Add_term(wewnetrzne_odejmowanie);
    LEWE_dodawanie->Add_term(ZERO);
    LEWE_dodawanie->Add_term(wewnetrzne_mnozenie_x);

    plik << "\\begin{equation}\n";

    LEWE_dodawanie->print_new(plik);




    Term * PRAWE_dodawnie = new Add();
    Term * glowne_dodawanie = new Add();
    Term * podglowne_mnoznie1 = new Multiplication();
    Term * podglowne_mnoznie2 = new Multiplication();
    Term * wewnetrzne_dodawanie1 = new Add();
    Term * wewnetrzne_dodawanie2 = new Add();
    Term*  poboczne_mnozeni1 = new Multiplication();
    Term*  poboczne_mnozeni2 = new Multiplication();
    Term*  poboczne_mnozeni3 = new Multiplication();
    Term*  poboczne_mnozeni4 = new Multiplication();


    poboczne_mnozeni1->Add_term(new SubtractU(B->copy_term()));
    poboczne_mnozeni1->Add_term(new VarIndex(new Var("C"), new Var("1")));


    poboczne_mnozeni2->Add_term(new SubtractU(B->copy_term()));
    poboczne_mnozeni2->Add_term(new VarIndex(new Var("C"), new Var("2")));

    poboczne_mnozeni3->Add_term(new SubtractU(B->copy_term()));
    poboczne_mnozeni3->Add_term(new VarIndex(new Var("C"), new Var("1")));
    poboczne_mnozeni3->Add_term(new VarIndex(new Var("x"), new Var("2")));


    poboczne_mnozeni4->Add_term(new SubtractU(B->copy_term()));
    poboczne_mnozeni4->Add_term(new VarIndex(new Var("C"), new Var("2")));
    poboczne_mnozeni4->Add_term(new VarIndex(new Var("x"), new Var("1")));


    wewnetrzne_dodawanie1->Add_term( poboczne_mnozeni1);
    wewnetrzne_dodawanie1-> Add_term(poboczne_mnozeni2);

    wewnetrzne_dodawanie2-> Add_term(new SubtractU(poboczne_mnozeni3));
    wewnetrzne_dodawanie2-> Add_term(new SubtractU(poboczne_mnozeni4));

    podglowne_mnoznie1->Add_term(new Var("x"));
    podglowne_mnoznie1->Add_term(wewnetrzne_dodawanie1);


    glowne_dodawanie->Add_term(podglowne_mnoznie1);
    glowne_dodawanie->Add_term(wewnetrzne_dodawanie2);

    plik << " = ";
    glowne_dodawanie ->print_new(plik);


    plik << " \\end{equation} \n";

    plik << "Wykorzystujac fakt, ze wielomiany są równe jeśli mają identyczne współczyniki uzyskujemy: ";



    Term*  poboczne_mnozeni1_do_macierzy = new Multiplication();
    Term*  poboczne_mnozeni2_do_macierzy = new Multiplication();
    Term*  poboczne_mnozeni3_do_macierzy = new Multiplication();
    Term*  poboczne_mnozeni4_do_macierzy = new Multiplication();


    poboczne_mnozeni1_do_macierzy->Add_term(new SubtractU(B));
    poboczne_mnozeni1_do_macierzy->Add_term(new VarIndex(new Var("C"), new Var("1")));
    poboczne_mnozeni1_do_macierzy->Add_term(new VarIndex(new Var("x"), new Var("2")));


    poboczne_mnozeni2_do_macierzy->Add_term(new SubtractU(B));
    poboczne_mnozeni2_do_macierzy->Add_term(new VarIndex(new Var("C"), new Var("2")));
    poboczne_mnozeni2_do_macierzy->Add_term(new VarIndex(new Var("x"), new Var("1")));

    poboczne_mnozeni3_do_macierzy->Add_term(new SubtractU(B));
    poboczne_mnozeni3_do_macierzy->Add_term(new VarIndex(new Var("C"), new Var("1")));



    poboczne_mnozeni4_do_macierzy->Add_term(new SubtractU(B));
    poboczne_mnozeni4_do_macierzy->Add_term(new VarIndex(new Var("C"), new Var("2")));




    Term * dodawnie_do_macierzy = new Add();
    Term * dodawanie_do_macierzy_1 = new Add();
    dodawnie_do_macierzy->Add_term(poboczne_mnozeni1_do_macierzy);
    dodawnie_do_macierzy->Add_term(poboczne_mnozeni2_do_macierzy);
    dodawanie_do_macierzy_1->Add_term(poboczne_mnozeni3_do_macierzy);
    dodawanie_do_macierzy_1->Add_term(poboczne_mnozeni4_do_macierzy);

    Term * ajeden_odjac_A_razy_azero = new  Add();
    ajeden_odjac_A_razy_azero->Add_term(JEDEN);
    Term* A_razy_ZERO = new Multiplication();
    A_razy_ZERO->Add_term(A);
    A_razy_ZERO->Add_term(ZERO);


    /// klamerka rownosc to co uprorzadkowane a potem podstawione do wyznacznika
    ajeden_odjac_A_razy_azero->Add_term(new SubtractU(A_razy_ZERO));
    plik << "\\begin{equation}";
    plik << "\\left\\{\\begin{array}{r@{\\;=\\;}l}";
    ZERO->print_new(plik);
    plik << "& " ;
    dodawnie_do_macierzy->print_new(plik);
    plik<<" \\\\ ";
    ajeden_odjac_A_razy_azero ->print_new(plik);
    plik << " & " ;
    dodawanie_do_macierzy_1->print_new(plik);
    plik << " \\end{array}\\right.";
    plik << "\\end{equation}";





    ///  <<<<<<<<<<<<<<<<< wyznacznik >>>>>>>>>>>>>>>>>>>>>>>>
    plik << "Podstawiamy wartości $x_1$, $x_2$ i robimy obliczenia pomocnicze:\n\n";
    Term* num1 = new Multiplication();
    num1->Add_term(aa);
    num1->Add_term(x2);

    plik <<"$-";
    B->print_new(plik);
    plik << " \\cdot x_2 = ";
    num1->print_new(plik);
    while (skracanie(num1,true))
    {
        plik << " = ";
        num1->print_new(plik);
    }
    plik <<"$\\\\";
    Term* num2 = new Multiplication();
    num2->Add_term(aa);
    num2->Add_term(x1);
    plik <<"$-";
    B->print_new(plik);
    plik << " \\cdot x_1 = ";
    num1->print_new(plik);
    while (skracanie(num2,true))
    {
        plik << " = ";
        num2->print_new(plik);
    }
    plik <<"$\\\\";

    Term * num3 = new Add();
    Term * spring_Mult = new Multiplication();
    spring_Mult->Add_term(A->copy_term());
    spring_Mult->Add_term(ZERO);
    num3->Add_term(JEDEN);
    num3->Add_term(new SubtractU(spring_Mult));
    plik <<"$";
    num3->print_new(plik);
    while (skracanie(num3,true))
    {
        plik << " = ";
        num3->print_new(plik);
    }
    plik << "$\\\\";

    Term * den1=aa->copy_term();
    plik <<"$";
    den1->print_new(plik);
    while (skracanie(den1,true))
    {
        plik << " = ";
        den1->print_new(plik);
    }

    plik << "$\\\\";

    plik << "Stąd nasz układ ma postać";
    plik << "\\begin{equation}";
    plik << "\\left\\{\\begin{array}{r@{\\;=\\;}r@{\\;\\cdot\\; C_1\\;+\\;}r@{\\;\\cdot \\;C_2}}";
    ZERO->print_new(plik);
    plik << "&";
    num1->print_new(plik);
    plik << "&";
    num2->print_new(plik);
    plik << "\\\\";
    num3->print_new(plik);
    plik << " & " ;
    den1->print_new(plik);
    plik << " & " ;
    den1->print_new(plik);
    plik << "\\\\";
    plik << " \\end{array}\\right.";
    plik << "\\end{equation}";


    plik << "Rozwiązujemy układ metodą wyznaczników:";

    Term * mult__MULT___ = new Multiplication();
    mult__MULT___->Add_term(new SubtractU(aa));
    Wyznacznik* my_wyznacznik1 = new Wyznacznik(num1->copy_term(), num2->copy_term(), ZERO->copy_term(), den1->copy_term(), den1->copy_term(), num3->copy_term());

    my_wyznacznik1->print_new(plik);

    Term* detx; /// deklaracja detx
    Term* dety; /// deklaracja dety
    Term* det_main;



    /// WYZNACZNIKI 2 X 2

    Wyznacznik__2_x_2___ * __W = new Wyznacznik__2_x_2___(num1->copy_term(), num2->copy_term(),den1->copy_term(), den1->copy_term());
    Wyznacznik__2_x_2___ * __W_c1 = new Wyznacznik__2_x_2___(ZERO->copy_term(), num2->copy_term(), num3->copy_term(), den1->copy_term());
    Wyznacznik__2_x_2___ * __W_c2 = new Wyznacznik__2_x_2___(num1->copy_term(), ZERO->copy_term(), den1->copy_term(), num3->copy_term());
    plik << "\n";





    /// pobranie z wyznacznikia 3x3  juz obliczone ale  nie skrocone wyznacnziki
    det_main = my_wyznacznik1->d_Special;
    detx = my_wyznacznik1->dx_Special;
    dety = my_wyznacznik1->dy_Special;










    /// Wyznacznik W
    plik << "$W = ";
    __W->print_new(plik);
    plik << "=";
    det_main->print_new(plik);

    /// SKRACANIE WYZNACZIKOW
    while (skracanie(det_main,true) )
    {
        plik << "=";
        det_main->print_new(plik);
    }
    plik << "$\\\\";

    /// detx
    plik << "$W_{C_1} = ";
    __W_c1->print_new(plik);
    plik << "=";
    detx->print_new(plik);
    while (skracanie(detx,true) )
    {
        plik << "=";
        detx->print_new(plik);
    }
    plik << "$\\\\";

    /// dety

    plik << "$W_{C_2} = ";
    __W_c2->print_new(plik);
    plik << "=";
    dety->print_new(plik);
    while (skracanie(dety,true) )
    {
        plik << "=";
        dety->print_new(plik);
    }
    plik << "$\\\\";

    ;



    plik << "Wyliczamy parametry $C_1$ i $C_2$:\n\n";

    Term * C_1_ = new Division(   detx, det_main);
    Term * C_2_ = new Division(   dety, det_main);

    plik << " $ C_1 =   \\frac{W_{c_1}}{W }  =";
    C_1_->print_new(plik);
    while (skracanie(C_1_,true))
    {
        plik << " = ";
        C_1_->print_new(plik);
    }
    plik << "$\\\\";

    plik << " $  C_2 =   \\frac{W_{c_2}}{W}  =  ";

    C_2_->print_new(plik);

    while (skracanie(C_2_,true))
    {
        plik << " = ";
        C_2_->print_new(plik);
    }
    plik << "$\\\\";
    plik << "Stad:\n";
    plik << "$$f(x)=\\frac{";
    C_1_->print_new(plik);
    plik<< "}{x - ";
    x1->print_new(plik);
    plik << "}+\\frac{";
    C_2_->print_new(plik);
    plik<< "}{x - ";
    x2->print_new(plik);
    plik << "}$$";
    plik << "\n Aby rozwinąć ułamki w szereg, sprowadzamy je do postaci $\\frac{a}{1-b\\cdot x}$.\n";
    plik << " W tym celu dzielimy licznik i mianownik przez $-x_1$, $-x_2$ odpowiednio.\n\n";


    Term * C1L = new Division(C_1_->copy_term(),new SubtractU(x1->copy_term()));
    Term * C2L = new Division(C_2_->copy_term(),new SubtractU(x2->copy_term()));


    plik << "$\\frac{C_1}{-x_1}=";
    C1L->print_new(plik);
    while (skracanie(C1L,true))
    {
        plik << " = ";
        C1L->print_new(plik);
    }
    plik << "$\n\n";
    plik << "$\\frac{C_2}{-x_2}=";
    C2L->print_new(plik);
    while (skracanie(C2L,true))
    {
        plik << " = ";
        C2L->print_new(plik);
    }
    plik << "$\n\n";


    plik << "Stąd:\n";
    plik << "\\begin{multline}f(x)=\\frac{";
    C1L->print_new(plik);
    plik<< "}{1-\\frac{x}{";
    x1->print_new(plik);
    plik << "}}+\\frac{";
    C2L->print_new(plik);
    plik<< "}{1-\\frac{x}{ ";
    x2->print_new(plik);
    plik << "}}\\\\ = ";
    C1L->print_new(plik);
    plik << "\\cdot \\sum_{n=0}^\\infty";
    plik << "\\left(\\frac{x}{";
    x1->print_new(plik);
    plik << "}\\right)^n\\;+\\;" ;
    C2L->print_new(plik);
    plik << "\\cdot \\sum_{n=0}^\\infty";
    plik << "\\left(\\frac{x}{";
    x2->print_new(plik);
    plik << "}\\right)^n\\\\=";
    plik << "\\sum_{n=0}^\\infty\\;\\left(";
    C1L->print_new(plik);
    plik << "\\cdot ";
    plik << "\\left(\\frac{1}{";
    x1->print_new(plik);
    plik << "}\\right)^n+";
    C2L->print_new(plik);
    plik << "\\cdot ";
    plik << "\\left(\\frac{1}{";
    x2->print_new(plik);
    plik << "}\\right)^n";
    plik << "\\right)\\cdot x^n";
    plik << "\\end{multline}";


    Term *Fn1 =    new Multiplication(C1L,   new Power(new Division(new RealNumber(1), x1->copy_term()),new Var("n")));
    Term *Fn2 =    new Multiplication(C2L,   new Power(new Division(new RealNumber(1), x2->copy_term()),new Var("n")));
    Term *Fn = new Add(Fn1,Fn2);

    plik << "\n\n$a_n";
    while (skracanie(Fn,true))
    {
        plik << " = ";
        Fn->print_new(plik);
    }
    plik << "$\n\n";


}
void liczMetodePrzewidywan(Term* A, Term* B, Term* Z, Term* J, std::ofstream& plik)
{
    plik << " \\paragraph{ Metoda : Przewidywań }    \n ";
    Term* aen = new VarIndex(new Var("a"), new Var("n"));
    Term* Dodawanie_1_ = new Add();
    Term* Mnozenie_1_ = new Multiplication();
    Term* Mnozenie_2_ = new Multiplication();
    Mnozenie_1_->Add_term(A);
    Mnozenie_1_->Add_term(new VarIndex(new Var("a"),  new Var("n-1")));
    Mnozenie_2_->Add_term(B);
    Mnozenie_2_->Add_term(new VarIndex(new Var("a"), new Var("n-2")));
    Dodawanie_1_->Add_term(Mnozenie_1_);
    Dodawanie_1_->Add_term(Mnozenie_2_);
    plik << "  \\begin{equation}";
    aen->print_new(plik);
    plik << " = \\begin{cases}  ";
    Z->print_new(plik);
    plik << ", &  \\text{ dla } n = 0 \\\\ ";
    J->print_new(plik);
    plik << ", &  \\text{ dla } n = 1 \\\\ ";
    Dodawanie_1_->print_new(plik);
    plik << ", &  \\text{ dla } n > 1 \\\\";
    plik << "\\end{cases} \\end{equation}";



    plik << "Skupiamy się na równaniu: ";
    plik << "\\\\ \n";

    plik << " \\begin{equation} ";


    Term* x_potega_n = new VarIndex(new Var("x"), new Var("n"));
    Term* Dodawanie_M = new Add();
    Term* Mnozenie_M = new Multiplication();
    Term* Mnozenie_MM = new Multiplication();

    Mnozenie_M->Add_term(A);
    Mnozenie_M->Add_term(new VarIndex(new Var("x"), new Var("{n - 1}")));
    Mnozenie_MM->Add_term(B);
    Mnozenie_MM->Add_term(new VarIndex(new Var("x"), new Var("{n -2}")));
    Dodawanie_M->Add_term(Mnozenie_M);
    Dodawanie_M->Add_term(Mnozenie_MM);
    x_potega_n->print_new(plik);


    plik << " = ";
    Dodawanie_M->print_new(plik);

    Term* x_potega_dwa_a = new Power(new Var("x"), new RealNumber(2));
    Term* Dodawanie_next = new Add();
    Term* Mnozenie_next = new Multiplication();
    Mnozenie_next->Add_term(A);
    Mnozenie_next->Add_term(new Var("x"));
    Dodawanie_next->Add_term(Mnozenie_next);
    Dodawanie_next->Add_term(B);

    plik << "\\end{equation}    \\\\ \n  \\begin{equation}    ";
    x_potega_dwa_a->print_new(plik);
    plik << " = ";
    Dodawanie_next->print_new(plik);
    plik << "\\end{equation}  Rozwiązujemu równanie kwadratowe:    \\\\ \n  \\begin{equation}  ";

    Term* DodawanieNext_M = new Add();
    DodawanieNext_M->Add_term(new Power(new Var("x"), new RealNumber(2)));
    DodawanieNext_M->Add_term(new SubtractU(new Multiplication(A, new Var("x"))));
    DodawanieNext_M->Add_term(new SubtractU(B));
    DodawanieNext_M->print_new(plik);
    plik << " = 0 ";
    plik << "\\end{equation}";
    Term* Mn = new Multiplication();
    Mn->Add_term(new RealNumber(4));
    Mn->Add_term(new RealNumber(1));
    Mn->Add_term(new SubtractU(B));


    // Delta
    Term* delta = new Add();
    delta->Add_term(new Power(new SubtractU(A), new RealNumber(2)));
    delta->Add_term(new SubtractU( Mn));
    plik << "Obliczamy delte: \n \n $ \\Delta   = ";
    delta->print_new(plik);
    while (skracanie(delta,true))
    {
        plik << " = ";
        delta->print_new(plik);
    }
    plik << "$ \n  ";

    // Obliczenie pierwiastka delty
    Term* pierwiastek = new Sqrt( new RealNumber(2),delta);
    plik << "$$ \\sqrt{\\Delta} =  ";
    pierwiastek->print_new(plik);
    plik << " = ";
    Result resultPierwiastek = pierwiastek->calculate();
    plik << resultPierwiastek.liczba << " $$";
    plik << " \n  \n \n      ";
    Term* gora1 = new Add(A, new SubtractU(new Sqrt(new RealNumber(2), delta)));
    Term* gora2 = new Add(A, new Sqrt(new RealNumber(2), delta));
    Term* x1 = new Division(gora1, new RealNumber(2));
    Term* x2 = new Division(gora2, new RealNumber(2));
    plik << " \n Wyznaczamy pierwiastki równania kwadratowego: $$    ";
    plik << "  x1 =     ";
    while (skracanie( x1,true))
    {

        plik << " = ";
        plik << "$ ";
        x1->print_new(plik);
        plik << "$ ";

    }
    x1->print_new(plik);
    plik << "$$ \n  \n \n  $$    ";
    plik << " \n x2 = ";

    while (skracanie( x2,true))
    {

        plik << " = ";
        plik << "$ ";

        x2->print_new(plik);
        plik << "$ ";

    }


    x2->print_new(plik);
    plik <<  "$$";


    plik << "Podstawiamy pierwiastki równania kwadratowego do jawnego wzoru na n-ty wyraz: ";

    plik << " \n  \n  \\begin{equation}    ";



    Term* vi = new VarIndex(new Var("a"), new Var("n"));
    vi->print_new(plik);
    plik << "  =   ";
    Term* bigAdd = new Add();
    Term* mul1 = new Multiplication();
    Term* mul2 = new Multiplication();
    mul1->Add_term( new VarIndex (new Var("C"), new Var("1")));
    mul1->Add_term(new Power(x1, new Var("n")));
    mul2->Add_term(new VarIndex (new Var("C"), new Var("2")));
    mul2->Add_term(new Power(x2, new Var("n")));
    bigAdd->Add_term(mul1);
    bigAdd->Add_term(mul2);
    bigAdd->print_new(plik);



    Term* bigAdd1    = new Add();
    Term* mul11 = new Multiplication();
    Term* mul22 = new Multiplication();
    mul11->Add_term(new VarIndex(new Var("C"), new Var("1")));
    mul11->Add_term(new Power(x1, new Var("0")));
    mul22->Add_term(new VarIndex(new Var("C"), new Var("2")));
    mul22->Add_term(new Power(x2, new Var("0")));
    bigAdd1->Add_term(mul11);
    bigAdd1->Add_term(mul22);
    plik << "\\end{equation}  \\\\ \n Podstawiamy pierwiastki równania kwadratowego do jawnego wzoru na zerowy wyraz: \\\\ \n\\begin{equation}";


    plik << "a_0 = " ;
    bigAdd1->print_new(plik);
    plik << "\\end{equation} \\\\ \n Podstawiamy pierwiastki równania kwadratowego do jawnego wzoru na pierwszy wyraz: \\\\ \n \\begin{equation}    ";


    Term* bigAdd11 = new Add();
    Term* mul111 = new Multiplication();
    Term* mul222 = new Multiplication();
    mul111->Add_term(new VarIndex(new Var("C"), new Var("1")));
    mul111->Add_term(new Power(x1, new Var("1")));
    mul222->Add_term(new VarIndex(new Var("C"), new Var("2")));
    mul222->Add_term(new Power(x2, new Var("1")));
    bigAdd11->Add_term(mul111);
    bigAdd11->Add_term(mul222);
    plik << "a_1 = ";
    bigAdd11->print_new(plik);
     plik << "\\end{equation} \\\\ \n";
    plik << "Podstawiamy parametry do macierzy: \n ";

    /// wyznacznik 3 x 3
    Wyznacznik * WyznacznikPrzewidywan1 = new Wyznacznik(new RealNumber(1), new RealNumber(1), Z->copy_term(), x1->copy_term(), x2->copy_term(), J->copy_term());



    plik << "\\begin{equation} ";
    WyznacznikPrzewidywan1->print_new(plik);
    plik << "\\end{equation} ";


    /// Wyznaczcziki 2 x 2

    Wyznacznik__2_x_2___* W_1 = new Wyznacznik__2_x_2___(new RealNumber(1), new RealNumber(1),  x1->copy_term(), x2->copy_term() );
    Wyznacznik__2_x_2___* W_2 = new Wyznacznik__2_x_2___(Z->copy_term(), new RealNumber(1), J->copy_term(), x2->copy_term()  );
    Wyznacznik__2_x_2___* W_3 = new Wyznacznik__2_x_2___(new RealNumber(1), Z->copy_term(), x1->copy_term(), J->copy_term()  );


    Term* detx; // deklaracja detx
    Term* dety; // deklaracja dety
    Term * detmain; // deklarcja glownegow wyznacznika

    detmain = WyznacznikPrzewidywan1->d_Special;
    detx = WyznacznikPrzewidywan1->dx_Special;
    dety = WyznacznikPrzewidywan1->dy_Special;

    plik << " Wyliczamy wyznaczniki stąd: \\\\ \n";
    plik << "\\begin{equation} ";
    W_1->print_new(plik);
    plik << "\\end{equation} ";




      plik << "$ W = ";
        detmain->print_new(plik);
        plik << "$ ";
    while (skracanie( detmain,true))
    {

        plik << " = ";
        plik << "$";
        detmain->print_new(plik);
        plik << "$";

    }


    plik << "\\\\ \\\\ \\clearpage  Stąd: \\\\ \n  ";
    plik << "\\begin{equation} ";
    W_2->print_new(plik);
    plik << "\\end{equation} ";

         plik << "$ W_{c_1} = ";
        detx->print_new(plik);
        plik << "$ ";

    while (skracanie( detx,true))
    {

        plik << " = ";

        plik << "$ ";
        detx->print_new(plik);
        plik << "$ ";

    }
    plik << "\\\\ \\\\ \n \n ";


  plik << "\\\\ \n Stąd: \\\\ \n \\begin{equation} ";
    W_3->print_new(plik);
plik << "\\end{equation} ";

plik << "$W_{c_2} = ";
        dety->print_new(plik);
        plik << "$ ";
    while (skracanie( dety,true))
    {

        plik << " = ";
        plik << "$ ";
        dety->print_new(plik);
        plik << "$ "
        ;

    }
    plik << "$$";
    plik << "\n";
    plik << "\n";



    Term * C_1 = new Division(detx, detmain );
    Term * C_2 = new Division(dety, detmain );



    plik << "\n";
       plik << "Wyliczamy wartości parametrów:  $ C_1 $ i $C_2 $ \n \n ";
    plik << " $$ C_1 = ";
     C_1->print_new(plik);
    while (skracanie( C_1,true))
    {
        plik << " = ";
        C_1->print_new(plik);
    }

    plik <<"$$";


    /// C2


    //detx->print_new(plik);
    plik << " \n \n";
    plik << " \n $$   C_2 =   ";
    C_2->print_new(plik);
    while (skracanie( C_2,true))
    {

        plik << " = ";

        C_2->print_new(plik);


    }
    plik << "$$ ";


    plik << "\n  ";


    /// OSTANIE WYRAZENIE SYMBOLICZNE
    Term* Final_ADD = new Add();
    Term* Final_MULT1 = new Multiplication();
    Term* Final_MULT2 = new Multiplication();
    plik << " \n \n I tak otrzymujemy wzór jawny na n-ty wyraz ciągu: ";
  plik << "\\begin{equation} ";
    Term * F_N = new VarIndex(new Var("a"), new Var("n"));
    F_N->print_new(plik);
    plik << " = ";
    Final_MULT1->Add_term(C_1);
    //mul111z->Add_term(new RealNumber(ww1.liczba));
    Final_MULT1->Add_term(new Power(x1, new Var("n")));
    //mul222z->Add_term(new RealNumber(ww1.liczba2));
    Final_MULT2->Add_term(C_2);
    Final_MULT2->Add_term(new Power(x2, new Var("n")));
    Final_ADD->Add_term(Final_MULT1);
    Final_ADD->Add_term(Final_MULT2);
    skracanie(Final_ADD,false);


    Final_ADD->print_new(plik);
    while (skracanie(  Final_ADD,true))
    {
        plik << " = ";
        Final_ADD->print_new(plik);
    }
    plik << "\\end{equation}\n\n";




    Term * MultNumber = new Multiplication();
    MultNumber->Add_term(new RealNumber(2));
    MultNumber->Add_term(new RealNumber(7));
    Term * Add_smal = new Add();
    Add_smal->Add_term(new RealNumber(2));
    Add_smal->Add_term(MultNumber);
    Term * Mult_of_Subtract = new Multiplication();
    //Term * Power__Fun = new Power(new RealNumber(2) , new RealNumber(1));
    Term * Division_of_sqrt_and_number= new Division(new Sqrt(new RealNumber(2), Add_smal ), new RealNumber(2));

    Term * Mult_small = new Multiplication();
    Mult_small->Add_term(new SubtractU(new RealNumber(1)));
    Mult_small->Add_term( new RealNumber(2));

    Term * Division_of_subtractAdd_and_subtructnumber= new Division(new SubtractU (  Mult_small), new SubtractU(new RealNumber(2)));








    Term * division_ADD =  new Add();
    division_ADD->Add_term(Division_of_subtractAdd_and_subtructnumber);
    division_ADD->Add_term(Division_of_sqrt_and_number);








   // division_ADD->term_tree(plik);
    // plik << " YYYY $$" ;
    //
    // division_ADD->print_new(plik);
//plik << "  $$" ;
/*
    plik << "$ ";
    division_ADD->print_new(plik);
    plik << "$ ";
    plik << "$ \n  ";

    while (skracanie( division_ADD,true))
    {
        division_ADD->term_tree(plik);
        plik << " = ";
        plik << "$ ";
        division_ADD->print_new(plik);
        plik << "$ ";

    }
    plik << "$ ";
    division_ADD->print_new(plik);
    plik << "$ ";

    plik << "$ \n  ";
*/

    delete x_potega_n;
    delete Dodawanie_M;
    delete Mnozenie_M;
    delete Mnozenie_MM;
    delete x_potega_dwa_a;
    delete Dodawanie_next;
    delete DodawanieNext_M;







}
void zapiszDoLatex(Term* A, Term* B, Term* Z, Term* J)
{
    Plik.open("wynik.tex");



    if (Plik.is_open())
    {

        Plik << "\\documentclass[12pt]{article}\n";
        Plik << "\\usepackage{amsmath} \n";
        Plik << "\\usepackage{tikz}\n";
        Plik << "\\usetikzlibrary{positioning,shadows,arrows}\n";
        Plik << "\\usepackage[polish]{babel}";
         Plik <<   " \\usepackage{polski} " ;


        Plik <<   "\\tikzset{fact/.style={rectangle, rounded corners=1mm, draw=black,fill=green!20,drop shadow,text centered, anchor=north, text=black},\n";
        Plik << "       leaf/.style={circle, draw=none, draw=black,fill=yellow!20, circular drop shadow,text centered, anchor=north, text=black}}\n";

        Plik << "\\begin{document}\n";
        liczszereg(A, B, Z, J, Plik);
        //Plik << "$$\n  \n \n \n    $$ ";
        //	rownanie->printTex(plik);
        //zapiszDoLatex(A, B, Z ,  J , plik);


        liczszeregVIP(A, B, Z, J, Plik);
        J->print_new(Plik);
        Plik << "$$\n \n \n $$ ";
        J->print_new(Plik);
        liczMetodePrzewidywan(A, B, Z, J, Plik);
        Plik << "$\n  ";
        Plik << "\\end{document}\n";
        Plik.close();
        std::cout << "XWynik zapisano do pliku wynik.tex\n";
    }
    Plik.close(); // zamknij plik
}


int main()
{
/*


    std::string inputString1;
    std::string inputString2;
    std::string inputString3;
    std::string inputString4;





    inputString1 = "1";
    inputString2 = "1";
    inputString3 = "0";
    inputString4 = "1";


    Term *A=parse(inputString1);
    Term *B=parse(inputString2);
    Term *Z=parse(inputString3);
    Term *J=parse(inputString4);

    //if (W) W->print_new(std::cout);

  //  zapiszDoLatex(A, B, Z, J);

*/


    std::string inputString;
        Term* A = nullptr;
        Term* B = nullptr;
        Term* Z = nullptr;
        Term* J = nullptr;

        // PrzykĹ‚ad pÄ™tli, ktĂłra prosi uĹĽytkownika o wprowadzenie danych i sprawdza, czy wystÄ…piĹ‚ bĹ‚Ä…d
        while (A == nullptr) {
            std::cout << "Wprowadź  A: ";
            std::getline(std::cin, inputString);
            A = parse(inputString);
        }

        while (B == nullptr) {
            std::cout << "Wprowadź  B: ";
            std::getline(std::cin, inputString);
            B = parse(inputString);
        }

        while (Z == nullptr) {
            std::cout << "Wprowadź  Z: ";
            std::getline(std::cin, inputString);
            Z = parse(inputString);
        }

        while (J == nullptr) {
            std::cout << "Wprowadź J: ";
            std::getline(std::cin, inputString);
            J = parse(inputString);
        }

        zapiszDoLatex(A , B , Z , J);






}

class Test
{
public:
public:
    Test() {}
    void testu()
    {


        // Test Rzeczywiste
        std::cout << "\n";
        std::cout << "---------------------";
        std::cout << "\n";
        std::cout << "Test Rzeczywiste 11.2 , 4 , 8 , -742";
        std::cout << "\n";
        Term* rn1 = new RealNumber(4);
        Term* rn2 = new RealNumber(8);
        RealNumber rrr4 = RealNumber(11.2);
        RealNumber rrr5 = RealNumber(-742);
        //rn1 += rn2; nie dziala : (
        rn1->print_new(std::cout);
        rn2->print_new(std::cout);
        rrr4.print_new(std::cout);
        rrr5.print_new(std::cout);

        std::cout << "\n";
        std::cout << "------------------";
        std::cout << "\n";
        std::cout << "Testy pierwiastka (pierwiastek 3 stopnia z 2 ) ";
        std::cout << "\n";

        // pierwszy pierwszy co pod pierwiastkiem , drugi stopien pierwiastka
        // TEST  Pierwiastek
        Term* t3 = new RealNumber(2);
        Term* t4 = new RealNumber(3);
        Sqrt* ss1 = new Sqrt(t3, t4);
        ss1->print_new(std::cout);
        Result res = ss1->calculate();
        std::cout << res.liczba;
        std::cout << "\n";

        // liczba pod pierwiastkiem ujemna wyrzuci blad, mozna sprawdzic
        // t3 zmienic na -4

        // TEST  dzielenie   ( 3 | 5 ) | 5 gdzie 3 | 5 to ulamek

        std::cout << "\n";
        std::cout << "------------------";
        std::cout << "\n";
        std::cout << "Testy dzielenia ( 3 | 5 ) | 5 gdzie 3 | 5 to ulamek  ";


        //f1->print();
        RealNumber* r1 = new  RealNumber(4);
        Term* t1 = r1;
        //t1->print() ;
        Term* bazowa = new RealNumber(5);
        //bazowa->print();
        Term* s1 = new Division(bazowa, bazowa);
        //s1->print() ;




        // Test mnozenia


        std::cout << "\n";
        std::cout << "------------------";
        std::cout << "\n";
        std::cout << "Test mnozenia ";
        std::cout << "\n";


        // Test potegi
        std::cout << "\n";
        std::cout << "------------------";
        std::cout << "\n";
        std::cout << "Test potegi 5 do potego 5  ";
        std::cout << "\n";



        // POTEGA 5 do potegi 5
        Power* ptga = new Power(bazowa, bazowa);
        //  ptga->print();
        Result resultpo = ptga->calculate();
        std::cout << resultpo.liczba;




        // Test dodawania
        std::cout << "\n";
        std::cout << "------------------";
        std::cout << "\n";
        std::cout << "Test dodawania  ";
        std::cout << "\n";
        Term* dd1 = new Add(ss1, ptga);
        dd1->print_new(std::cout);


        // silnia i zespolone nieobowiazkowe
        //Zespolona* zs1 = new Zespolona(1, -2);
        //zs1->print();
        //Silnia* ssss = new Silnia(bazowa);
        //ssss->print();


        // Test mnozenia 2
        std::cout << "\n";
        std::cout << "------------------";
        std::cout << "\n";
        std::cout << "Test mnozenie 2   ";
        std::cout << "\n";

        // TEST  mnozenie  2
        Multiplication* mm1 = new Multiplication(bazowa, bazowa);
        mm1->print_new(std::cout);

        // Test wyswetlania szeregu
        std::cout << "\n";
        std::cout << "------------------";
        std::cout << "\n";
        std::cout << "Test wyswietlania szeregu   ";
        std::cout << "\n";

        Var* V = new Var("n");
        Term* id = new RealNumber(0);
        Term* ig = new RealNumber(10);

        Term* W1 = new Var("x");
        Term* P = new RealNumber(2);
        Term* PW1 = new Power(W1, P);



        // Szereg Formalny
        FormalSeries* Sz = new FormalSeries(id, ig, PW1, V);
        Sz->print_new(std::cout);
        std::cout << "\n";
        std::cout << "Test Rename\n";
        std::cout << "-------------\n";

        Sz->print_new(std::cout);

        // TEST  OBLICZENIA COMPLICATED

        std::cout << "\n";
        std::cout << "------------------";
        std::cout << "\n";
        std::cout << "Copplicated obliczenie ";
        std::cout << "\n";
        Term* trzy = new RealNumber(3.000);
        Term* dwa = new RealNumber(2.000);





        Term* vvtest = new VarIndex(new Var("x"), new RealNumber(1));
        vvtest->print_new(std::cout);

        /*

        	Term* testx = new Add();
        	Term* testxx = new Add();
        	Term* testxxx = new Add();
        	Term* testxxxx = new Multiplication();
        	testx->Add_term(new RealNumber(1));
        	testx->Add_term(new RealNumber(2));
        	testx->Add_term(new RealNumber(4));



        	testxx->Add_term(new RealNumber(3));
        	testxx->Add_term(new RealNumber(7));


        	testxxxx->Add_term(new RealNumber(34));
        	testxxxx->Add_term(new RealNumber(75));

        	testxxx->Add_term(testx);
        	testxxx->Add_term(testxx);
        	testxxx->Add_term(new Sqrt(new RealNumber(3), new RealNumber(5)));


        	Term* klkl = new Add();
        	klkl->Add_term(testx);
        	klkl->Add_term(testxxx);
        	klkl->Add_term(new SubtractU(testxxxx));
        	Term* odej = new SubtractU(klkl);

        	//testyy->print_new(plik);

        	plik << "$$ \\\\ \\\\$$ $$ \\\\ \\\\$$";
        	plik << "  TEST  TEST TEST TEST wyglad pelnego symbolu $$ \\\\ \\\\$$ ";
        	klkl->print_new(plik);
        	plik << " TEST SKROTU $$ \\\\ \\\\$$ ";


        	odej->print_new(plik);



        	// TEST next



        	Term* Delta = new Add();
        	Delta->Add_term(new Power(new RealNumber(2), new RealNumber(2)));
        	Delta->Add_term(new RealNumber(2));
        	Delta->print_new(plik);

        	Delta->print_new(plik);

        	Term* minusBEdodacsqrtDELTA = new Add();

        	minusBEdodacsqrtDELTA->Add_term(new Sqrt(new RealNumber(2), Delta));

        	plik << " $$ $$  \\n \\n  $$ $$   $$\\n  \\\\ $$  TESTTTTTTTTTT           x1  = ";


        	minusBEdodacsqrtDELTA->print_new(plik);


        	plik << "   $$  \\\\n \\\\n  $$ TEST monozenia ";



        	Term* Amultf = new Add();
        	Term* Amultg = new Add();
        	Term* Amulth = new Multiplication();


        	Amultf->Add_term(new RealNumber(3));
        	Amultf->Add_term(new RealNumber(4));
        	Term* test = new Multiplication();
        	test->Add_term(new RealNumber(1));
        	test->Add_term(Amultf);
        	Amultg->Add_term(new RealNumber(5));
        	Amultg->Add_term(new RealNumber(6));
        	Amulth->Add_term(test);
        	Amulth->Add_term(Amultg);

        	Term* multf = new Multiplication();
        	Term* multg = new Multiplication();
        	Term* multh = new Multiplication();
        	multf->Add_term(Amulth);
        	multf->Add_term(new RealNumber(4));
        	multg->Add_term(new RealNumber(5));
        	multg->Add_term(new RealNumber(6));
        	multh->Add_term(multf);
        	multh->Add_term(multg);
        	plik << " \\\\ bla \\\\";
        	multh->print_new(plik);
        	//splaszcz(multh);
        	plik << " \\\\ bla \\\\";
        	multh->print_new(plik);

        	plik << " \\\\ bla \\\\";

        	///skracanie(multh);
        	plik << " \\\\ bla \\\\";
        	multh->print_new(plik);
        	plik << " \\\\ bla \\\\";
        	multh->print_new(plik);
        	plik << " \\\\ minusy";


        	Term* H= new SubtractU(new RealNumber(4));
        	H = new SubtractU(H); H = new SubtractU(H); H = new SubtractU(H);
        	H->print_new(plik);
        	plik << " \\\\ minusy";
        	skracanie(H);
        	plik << " \\\\ minusy";
        	H->print_new(plik);


        	Term* multp = new Multiplication();
        	multp->Add_term(new SubtractU(multh));
        	multp->Add_term(new RealNumber(222));
        	multp->print_new(plik);
        	plik << " \\\\";

        	plik << " x\\\\ ";
        	multp->print_new(plik);

        	Add* wiel = new Add();
        	Term* aaa = new SubtractU(B); // Odejmowanie: A - B
        	Term* bbb = new SubtractU(new Multiplication(Z, J)); // Odejmowanie: Z * J - B
        	Term* ccc = new RealNumber(1); // Liczba rzeczywista: 1
        	Term* dd = new RealNumber(0); // Liczba rzeczywista: 0

        	Term* ee = new Multiplication(aaa, bbb); // MnoĂ…ÂĽenie: (A + B) * (C * D)
        	Term* ff = new Subtrac(ee, ccc); // Odejmowanie: (A + B) * (C * D) - (1 - B)
        	Term* gg = new Add(ff, dd); // Dodawanie: (A + B) * (C * D) - (1 - B) + 0

        	Term* aaaa = new Power(A, B); // PotĂ„â„˘gowanie: A^B
        	Term* bbbb = new Multiplication(A, Z); // MnoĂ…ÂĽenie: A * C
        	Term* cccc = new Subtrac(new Subtrac(aaaa, bbbb), new RealNumber(1)); // Odejmowanie: (A^B - A*C) - 1
        	Term* dddd = new Add(cccc, ff); // Dodawanie: (A^B - A*C) - 1 + 0

        	wiel->Add_term(aaa);
        	wiel->Add_term(bbb);
        	wiel->Add_term(ccc);

        	plik << "***&&&&";

        	wiel->print_new(plik);
        	plik << "****&&&";
        	gg->print_new(plik);
        	plik << "****&&&";

        	dddd->print_new(plik);



        	// Zwolnienie pamiĂ„â„˘ci
        	delete A;
        	delete B;
        	delete delta;
        	delete pierwiastek;
        */

        delete vvtest;

        delete t1, r1;
        delete dd1;
        delete mm1;

    }

};
