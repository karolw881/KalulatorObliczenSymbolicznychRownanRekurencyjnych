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



#include "term.cpp"

#ifndef skracanie_cpp
#define skracanie_cpp

bool splaszcz(Term* &T)
{
    int zmiana = 0;

    for (std::vector<Term*>::iterator term = T->subterms.begin(); term != T->subterms.end(); ++term)
    {
        zmiana+=splaszcz(*term);
    }


    if (T->name == "dodawanie" || T->name == "mnozeniesymboliczne" )
    {
        std::vector<Term*> newSubTerms;
        for (std::vector<Term*>::iterator term = T->subterms.begin(); term != T->subterms.end(); ++term)
        {
            if ((*term)->name == T->name)
            {
                for (std::vector<Term*>::iterator Sterm = (*term)->subterms.begin(); Sterm != (*term)->subterms.end(); ++Sterm)
                {
                    newSubTerms.push_back((*Sterm)->copy_term());
                    zmiana = true;
                }
            }
            else
                newSubTerms.push_back((*term)->copy_term());
        }
        T->subterms = newSubTerms;
    }

    return zmiana;
}

bool  skracanie_1(Term* &T)    //mnozenie liczba
{
    if (T->name == "mnozeniesymboliczne")
    {
        for (std::vector<Term*>::iterator term1 = T->subterms.begin(); term1 != T->subterms.end(); ++term1)
        {
            if ((*term1)->name == "Rzeczywista")
            {
                for (std::vector<Term*>::iterator term2 = term1+1; term2 != T->subterms.end(); ++term2)
                {
                    if ((*term2)->name == "Rzeczywista")
                    {
                        RealNumber* R1 = (RealNumber*)*term1;
                        RealNumber* R2 = (RealNumber*)*term2;
                        Term *R12 =new RealNumber(R1->number*R2->number);

                        std::vector<Term*> newSubTerms;
                        for (std::vector<Term*>::iterator t = T->subterms.begin(); t != term1; t++)
                            newSubTerms.push_back((*t)->copy_term());
                        newSubTerms.push_back(R12);
                        for (std::vector<Term*>::iterator t = term1+1; t != term2; t++)
                            newSubTerms.push_back((*t)->copy_term());
                        for (std::vector<Term*>::iterator t = term2+1; t!=T->subterms.end(); t++)
                            newSubTerms.push_back((*t)->copy_term());


                        T->subterms=  newSubTerms;
                        return true;
                    }
                }
            }
        }
    }
    return false;
}


bool skracanie_2(Term* &T)     ///mnoz 1 wyrzuca z iloczynu 1
{
    if (T->name == "mnozeniesymboliczne" && T->subterms.size()>1)
    {
        for (std::vector<Term*>::iterator term1 = T->subterms.begin(); term1 != T->subterms.end(); ++term1)
        {
            if ((*term1)->name == "Rzeczywista")
            {
                RealNumber* R1 = (RealNumber*)*term1;
                if (R1->number == 1)
                {
                    std::vector<Term*> newSubTerms;
                    for (std::vector<Term*>::iterator t = T->subterms.begin(); t != term1; t++)
                        newSubTerms.push_back((*t)->copy_term());
                    for (std::vector<Term*>::iterator t = term1+1;  t!=T->subterms.end(); t++)
                        newSubTerms.push_back((*t)->copy_term());


                    T->subterms=  newSubTerms;
                    return true;
                }

            }
        }
    }
    return false;
}

bool skracanie_3(Term * (& T))     ///mnoz 0 to zastap przez 0
{
    if (T->name == "mnozeniesymboliczne")
    {
        for (std::vector<Term*>::iterator term1 = T->subterms.begin(); term1 != T->subterms.end(); ++term1)
        {
            if ((*term1)->name == "Rzeczywista")
            {
                RealNumber* R1 = (RealNumber*)*term1;
                if (R1->number == 0)
                {
                    T = new RealNumber(0);
                    return true;
                }
            }
        }

    }
    return false;
}

bool skracanie_4(Term* (&T))     ///tylko *(number) lub +(number)
{
    if (T->name == "mnozeniesymboliczne" || T->name == "dodawanie")
    {
        if (T->subterms.size() == 1)
        {
            T = (*(T->subterms.begin()))->copy_term();
            return true;
        }
    }
    return false;
}
bool skracanie_5(Term* &T)    //suma liczba
{
    if (T->name == "dodawanie")
    {
        for (std::vector<Term*>::iterator term1 = T->subterms.begin(); term1 != T->subterms.end(); ++term1)
        {
            if ((*term1)->name == "Rzeczywista")
            {
                for (std::vector<Term*>::iterator term2 = term1+1; term2 != T->subterms.end(); ++term2)
                {
                    if ((*term2)->name == "Rzeczywista")
                    {
                        RealNumber* R1 = (RealNumber*)*term1;
                        RealNumber* R2 = (RealNumber*)*term2;

                        Term *R12 = new RealNumber(R1->number + R2->number);

                        std::vector<Term*> newSubTerms;
                        for (std::vector<Term*>::iterator t = T->subterms.begin(); t != term1; t++)
                            newSubTerms.push_back((*t)->copy_term());
                        newSubTerms.push_back(R12);
                        for (std::vector<Term*>::iterator t = term1+1; t != term2; t++)
                            newSubTerms.push_back((*t)->copy_term());
                        for (std::vector<Term*>::iterator t = term2+1; t!=T->subterms.end(); t++)
                            newSubTerms.push_back((*t)->copy_term());


                        T->subterms=  newSubTerms;
                        return true;
                    }
                }
            }
        }
    }
    return false;
}
bool skracanie_6(Term* &T)     ///dodawanie 0
{
    if (T->name == "dodawanie" && T->subterms.size()>1)
    {
        for (std::vector<Term*>::iterator term1 = T->subterms.begin(); term1 != T->subterms.end(); ++term1)
        {
            if ((*term1)->name == "Rzeczywista")
            {
                RealNumber* R1 = (RealNumber*)*term1;
                if (R1->number == 0)
                {
                    std::vector<Term*> newSubTerms;
                    for (std::vector<Term*>::iterator t = T->subterms.begin(); t != term1; t++)
                        newSubTerms.push_back((*t)->copy_term());
                    for (std::vector<Term*>::iterator t = term1+1;  t!=T->subterms.end(); t++)
                        newSubTerms.push_back((*t)->copy_term());
                    T->subterms=  newSubTerms;
                    return true;
                }

            }
        }
    }
    return false;
}
bool skracanie_7(Term* &T)     ///- liczba => (liczba przeciwna)
{
    if (T->name == "unarneodejmowanie")
    {
        Term* pierw = T->get_term(0);
        if (pierw->name == "Rzeczywista")
        {
            RealNumber* R1 = (RealNumber*)pierw;
            T = new RealNumber(-R1->number);
            return true;
        }
    }
    return false;
}
bool skracanie_8(Term* &T)     /// - - term to term
{
    if (T->name == "unarneodejmowanie")
    {
        //std::cout << "unarneodejmowanie" << std::endl;
        Term* pierw = T->get_term(0);
        if (pierw->name == "unarneodejmowanie")
        {
            Term* pierw_pierw = pierw->get_term(0);
            T =pierw_pierw->copy_term();
            return true;
        }
    }
    return false;
}
bool skracanie_9(Term* &T)   /// jeĹ›li w mianowniku jest liczba ktĂłra ma nwd!=1 z jakÄ…Ĺ› liczbÄ… w liczniku
{
    if (T->name == "Dzielenie")
    {
        Division* division = (Division*)(T);

        Term* L = division->subterms[0];
        Term* M = division->subterms[1];
        if (M->name=="Rzeczywista" && L->name=="mnozeniesymboliczne")
        {
            for (std::vector<Term*>::iterator term = L->subterms.begin(); term != L->subterms.end(); ++term)
                if ((*term)->name=="Rzeczywista")
                {
                    RealNumber * RM = (RealNumber *)M, *RL = (RealNumber *)(*term);
                    int iL = round(RL->number),iM = round(RM->number);
                    if ((iL==RL->number)&&(iM==RM->number))
                    {
                        if (iL<0) iL=-iL;
                        if (iM<0) iM=-iM;

                        int nwd =1;
                        for (int d=2; d<=iL; d++)
                        {
                            if ((iL%d ==0) && (iM%d==0)) nwd=d;
                        };
                        if (nwd!=1)
                        {
                            if (nwd == RM->number)
                            {

                                T=new Multiplication();

                                std::vector<Term*> newSubTerms;
                                for (std::vector<Term*>::iterator t = L->subterms.begin(); t != term; t++)
                                    T->Add_term((*t)->copy_term());
                                for (std::vector<Term*>::iterator t = term+1;  t!=L->subterms.end(); t++)
                                    T->Add_term((*t)->copy_term());
                            }
                            else
                            {
                                Term * NewL=new Multiplication();
                                std::vector<Term*> newSubTerms;
                                for (std::vector<Term*>::iterator t = L->subterms.begin(); t != term; t++)
                                    NewL->Add_term((*t)->copy_term());
                                NewL->Add_term(new RealNumber(RL->number/nwd));
                                for (std::vector<Term*>::iterator t = term+1;  t!=L->subterms.end(); t++)
                                    NewL->Add_term((*t)->copy_term());
                                T = new Division(NewL,new RealNumber(RM->number/nwd));
                            }
                            return true;
                        }
                    }

                }
        }
        else if (M->name=="Rzeczywista" && L->name== "unarneodejmowanie" && L->get_term(0)->name=="mnozeniesymboliczne")
        {
            for (std::vector<Term*>::iterator term = L->get_term(0)->subterms.begin(); term != L->get_term(0)->subterms.end(); ++term)
                if ((*term)->name=="Rzeczywista")
                {
                    RealNumber * RM = (RealNumber *)M, *RL = (RealNumber *)(*term);
                    int iL = round(RL->number),iM = round(RM->number);
                    if ((iL==RL->number)&&(iM==RM->number))
                    {
                        if (iL<0) iL=-iL;
                        if (iM<0) iM=-iM;

                        int nwd =1;
                        for (int d=2; d<=iL; d++)
                        {
                            if ((iL%d ==0) && (iM%d==0)) nwd=d;
                        };
                        if (nwd!=1)
                        {
                            if (nwd == RM->number)
                            {

                                Term * Mnew =new Multiplication();

                                std::vector<Term*> newSubTerms;
                                for (std::vector<Term*>::iterator t = L->get_term(0)->subterms.begin(); t != term; t++)
                                    Mnew->Add_term((*t)->copy_term());
                                for (std::vector<Term*>::iterator t = term+1;  t!=L->get_term(0)->subterms.end(); t++)
                                    Mnew->Add_term((*t)->copy_term());
                                T = new SubtractU(Mnew);
                            }
                            else
                            {
                                Term * NewL=new Multiplication();
                                std::vector<Term*> newSubTerms;
                                for (std::vector<Term*>::iterator t = L->get_term(0)->subterms.begin(); t != term; t++)
                                    NewL->Add_term((*t)->copy_term());
                                NewL->Add_term(new RealNumber(RL->number/nwd));
                                for (std::vector<Term*>::iterator t = term+1;  t!=L->get_term(0)->subterms.end(); t++)
                                    NewL->Add_term((*t)->copy_term());
                                T = new Division(new SubtractU(NewL),new RealNumber(RM->number/nwd));
                            }
                            return true;
                        }
                    }

                }
        }

    }
    return false;
}


bool skracanie_9_1(Term* &T)    //  dzielenie przez 1 i dzielenie liczba przez liczba
{

    if (T->name == "Dzielenie")
    {
        Division* division = (Division*)(T);

        Term* L = division->subterms[0];
        Term* M = division->subterms[1];
        if (M->name=="Rzeczywista")
        {
            RealNumber * RM = (RealNumber *)M;
            if (RM->number==1)
            {
                T = L->copy_term();
                return true;
            }
        }

        if (M->name=="Rzeczywista" && L->name=="Rzeczywista")
        {

            RealNumber * RM = (RealNumber *)M;
            RealNumber *RL = (RealNumber *)L;
            int iL = round(RL->number),
                iM = round(RM->number);

            if ((iL==RL->number)&&(iM==RM->number))
            {
                if (iL<0) iL=-iL;
                if (iM<0) iM=-iM;

                int nwd =1;
                for (int d=2; d<=iL; d++)
                {
                    if ((iL%d ==0) && (iM%d==0)) nwd=d;
                };
                if (nwd!=1)
                {

                    if (RL->number == RM->number)
                    {
                        T=new RealNumber(1);
                    }
                    else
                    {
                        if (RM->number == nwd)   /// (a / 1) |-> a
                        {
                            T= new RealNumber(RL->number/nwd);
                        }
                        else
                            T=new Division( new RealNumber(RL->number/nwd),  new RealNumber(RM->number/nwd));
                    }

                    return true;
                }
            }

        }
    }

    return false;
}



bool skracanie_10(Term* &T)   /// num^2 |-> num*num
{
    if (T->name == "potega")
    {
        Power* potega = (Power*)(T);
        Term* base = potega->get_term(0);
        Term* exponent = potega->get_term(1);
        if (base->name == "Rzeczywista" && exponent->name == "Rzeczywista")
        {
            RealNumber* baseNumber = (RealNumber*)(base);
            RealNumber* exponentNumber = (RealNumber*)(exponent);
            if (exponentNumber->number == 2)
            {

                T = new RealNumber((baseNumber->number) * (baseNumber->number));
                return true;
            }
        }
    }
    return false;
}


bool skracanie_11(Term* &T)   /// a / (-b) |==> (-a) / b
{
    if (T->name == "Dzielenie")
    {
        Division* D = (Division*)(T);
        Term* licz = D->get_term(0);
        Term* mian = D->get_term(1);


        if (mian->name == "unarneodejmowanie")   /// w mianowniku jest - term
        {
            SubtractU * newL = new  SubtractU(licz->copy_term());
            T = new Division(newL, mian->get_term(0)->copy_term());
            return true;
        }
        else if (mian->name == "Rzeczywista") // w mianowniku jest ujemna rzeczywista
        {
            RealNumber* R = (RealNumber*)mian;
            if (R->number<0)
            {
                SubtractU * newL = new  SubtractU(licz->copy_term());
                T = new Division(newL, new RealNumber(-R->number));
                return true;

            }
        }
    }
    return false;
}


bool skracanie_12(Term* &T)   /// - (a+b+c+...) |=> -a + -b + -c +....
{
    if (T->name == "unarneodejmowanie")
    {
        SubtractU * U = (SubtractU*) T->get_term(0);
        if (U->name == "dodawanie")
        {

            Term * NewT = new Add();
            for (std::vector<Term*>::iterator term = U->subterms.begin(); term != U->subterms.end(); ++term)
            {
                NewT -> Add_term(new SubtractU((*term)->copy_term()));
            }
            T=NewT;
            return true;
        }
    }

    return false;
}

bool skracanie_13(Term* &T)   ///  ....* (-x) *..... |=> - (.....*x *.....)
{
    if (T->name == "mnozeniesymboliczne")
    {
        for (std::vector<Term*>::iterator term = T->subterms.begin(); term != T->subterms.end(); ++term)
        {
            if (( (*term)->name)== "unarneodejmowanie")
            {

                Term * M = new Multiplication();
                for (std::vector<Term*>::iterator t = T->subterms.begin(); t != term; t++)
                    M->Add_term((*t)->copy_term());

                M->Add_term((*term)->get_term(0)->copy_term());
                for (std::vector<Term*>::iterator t = term+1;  t!=T->subterms.end(); t++)
                    M->Add_term((*t)->copy_term());
                T=new SubtractU(M);
                return true;
            }
            if (( (*term)->name)== "Rzeczywista")
            {
                RealNumber* R = (RealNumber*)(*term);
                if (R->number<0)
                {
                    Term * M = new Multiplication();
                    for (std::vector<Term*>::iterator t = T->subterms.begin(); t != term; t++)
                        M->Add_term((*t)->copy_term());
                    M->Add_term(new RealNumber(-R->number));
                    for (std::vector<Term*>::iterator t = term+1;  t!=T->subterms.end(); t++)
                        M->Add_term((*t)->copy_term());
                    T=new SubtractU(M);
                    return true;
                }
            }
        }
    }
    return false;
}



bool skracanie_14(Term* &T)   /// - (a/b) |=> (- a) /b
{
    if (T->name =="unarneodejmowanie")
    {
        Term * U = (SubtractU*) T->get_term(0);
        if (U->name == "Dzielenie")
        {
            Division* D = (Division*)(U);
            Term* licz = D->get_term(0);
            Term* mian = D->get_term(1);
            T = new Division(new SubtractU(licz->copy_term()),mian->copy_term());
            return true;
        }
    }
    return false;
}



bool skracanie_18(Term* &T)   /// (A  /    (r/s)  ) |=> (A*s) /r
{
    if (T->name =="Dzielenie")
    {
        Term * M = T -> get_term(1);
        if (M->name == "Dzielenie")
        {
            Term * newL = new Multiplication();
            newL->Add_term(T->get_term(0)->copy_term());
            newL->Add_term(M->get_term(1)->copy_term());
            T = new Division(newL,M->get_term(0)->copy_term());
            return true;
        }
    }
    return false;
}
bool skracanie_19(Term* &T)     //// liczba * (liczba/liczba)
{

    if (T->name == "mnozeniesymboliczne")
    {
        for (int term1 = 0; term1< T->subterms.size();term1++)
        {
            if ((T->subterms[term1])->name == "Rzeczywista")
            {
               for (int term2 = 0; term2 < T->subterms.size();term2++)
                {
                   if (term1!=term2 &&
                           ((T->subterms[term2])->name == "Dzielenie" )&&( (T->subterms[term2])->get_term(0)->name== "Rzeczywista")
                            &&( (T->subterms[term2])->get_term(1)->name== "Rzeczywista"))
                    {
                        RealNumber* R1 = (RealNumber*)T->subterms[term1];
                        RealNumber * L1 = (RealNumber*)((T->subterms[term2])->get_term(0));
                        RealNumber * M1 = (RealNumber*)((T->subterms[term2])->get_term(1));


                        std::vector<Term*> newSubTerms;
                        for (int t = 0;t < T->subterms.size(); t ++)
                        {
                            if (t!=term1 && t !=term2)
                            {
                                  newSubTerms.push_back((T->subterms[t])->copy_term());
                            }
                            else
                            {
                                if (t==term2)
                                     newSubTerms.push_back(new Division(new RealNumber(R1->number * L1->number),new RealNumber(M1->number)));
                            }
                        }




                        T->subterms=  newSubTerms;
                        return true;
                    }
                }
            }
        }
    }
    return false;
}


bool skracanie_20(Term* &T)     ////...+ liczba + (liczba/liczba) +...
{

    if (T->name == "dodawanie")
    {
        for (std::vector<Term*>::iterator term1 = T->subterms.begin(); term1 != T->subterms.end(); ++term1)
        {
            if ((*term1)->name == "Rzeczywista")
            {
                for (std::vector<Term*>::iterator term2 = term1+1; term2 != T->subterms.end(); ++term2)
                {
                    if (((*term2)->name == "Dzielenie" )&&( (*term2)->get_term(0)->name== "Rzeczywista")
                            &&( (*term2)->get_term(1)->name== "Rzeczywista"))
                    {
                        RealNumber* R1 = (RealNumber*)*term1;
                        RealNumber * L1 = (RealNumber*)((*term2)->get_term(0));
                        RealNumber * M1 = (RealNumber*)((*term2)->get_term(1));


                        std::vector<Term*> newSubTerms;
                        for (std::vector<Term*>::iterator t = T->subterms.begin(); t != term1; t++)
                            newSubTerms.push_back((*t)->copy_term());

                        newSubTerms.push_back(new Division(new RealNumber(R1->number *M1->number + L1->number ),new RealNumber(M1->number)));
                        for (std::vector<Term*>::iterator t = term1+1; t != term2; t++)
                            newSubTerms.push_back((*t)->copy_term());
                        for (std::vector<Term*>::iterator t = term2+1; t!=T->subterms.end(); t++)
                            newSubTerms.push_back((*t)->copy_term());


                        T->subterms=  newSubTerms;
                        return true;
                    }
                }
            }
        }
    }
    return false;
}



bool skracanie_15(Term* &T)   /// ....+ x / liczba +y / liczba +..... |=> .....+ ((x+y) /liczba) +......
{

    if (T->name == "dodawanie")
    {
        for (std::vector<Term*>::iterator term1 = T->subterms.begin(); term1 != T->subterms.end(); ++term1)
        {
            if ((*term1)->name == "Dzielenie")
            {
                for (std::vector<Term*>::iterator term2 = term1+1; term2 != T->subterms.end(); ++term2)
                {
                    if ((*term2)->name == "Dzielenie")
                    {
                        Division* D1 = (Division*)*term1;
                        Division* D2 = (Division*)*term2;

                        if (D1 ->get_term(1)->name =="Rzeczywista" && (D2 ->get_term(1))->name=="Rzeczywista")
                        {
                            RealNumber* R1 = (RealNumber*)D1->get_term(1);
                            RealNumber* R2 = (RealNumber*)D2->get_term(1);
                            if (R1->number == R2->number)
                            {

                                Term * A = new Add();
                                for (std::vector<Term*>::iterator t = T->subterms.begin(); t != term1; t++)
                                    A->Add_term((*t)->copy_term());

                                Term * NewL = new Add();
                                NewL->Add_term(D1->get_term(0));
                                NewL->Add_term(D2->get_term(0));
                                A->Add_term(new Division(NewL,R1->copy_term()));

                                for (std::vector<Term*>::iterator t = term1+1;  t!=term2; t++)
                                    A->Add_term((*t)->copy_term());

                                for (std::vector<Term*>::iterator t = term2+1;  t!=T->subterms.end(); t++)
                                    A->Add_term((*t)->copy_term());

                                T=A;
                                return true;
                            }
                        }
                    }

                }
            }
        }
    }
    return false;
}

Term * czy_sqrt_in(Term * T, Term * temp) /// sqrt lub - sqrt lub   ...*sqrt *A...  ...*-sqrt *A...
{

    if (T->name=="pierwiastek")
    {
        if (T==temp) return NULL;
        return T;
    }
    if (T->name=="unarneodejmowanie")
    {
        Term* pierw = T->get_term(0);
        if (pierw->name=="pierwiastek")
        {
            if (pierw==temp) return NULL;
            return  pierw;
        }
        else
        {
            int ok=0;   /// ok parametr aby wyszukiwaÄ‡ pierwsze wystÄ…pienie za temp jeĹ›li temp !=NULL
            if (temp==NULL) ok =1;
            if (pierw->name=="mnozeniesymboliczne")
            {
                for (std::vector<Term*>::iterator m = pierw->subterms.begin(); m != pierw->subterms.end(); ++m)
                {
                    if ((*m)->name=="pierwiastek")
                    {
                        if ((*m) == temp) ok=1;
                        if ((*m) != temp && ok==1) return (*m);
                    }
                    if ((*m)->name=="unarneodejmowanie")
                    {
                        Term* pierw_P = (*m)->get_term(0);
                        if (pierw_P->name=="pierwiastek")
                        {
                            if (pierw_P==temp)  ok=1;
                            if (pierw_P!=temp && ok ==1)  return  pierw;
                        }
                    }
                }
            }
        }
    }
    if (T->name=="mnozeniesymboliczne")
    {
        int ok=0;   /// ok parametr aby wyszukiwaÄ‡ pierwsze wystÄ…pienie za temp jeĹ›li temp !=NULL
        if (temp==NULL) ok =1;
        for (std::vector<Term*>::iterator m = T->subterms.begin(); m != T->subterms.end(); ++m)
        {
            if ((*m)->name=="pierwiastek")
            {
                if ((*m) == temp) ok=1;
                if ((*m) != temp && ok==1) return (*m);
            }
            if ((*m)->name=="unarneodejmowanie")
            {
                Term* pierw = (*m)->get_term(0);
                if (pierw->name=="pierwiastek")
                {
                    if (pierw==temp)  ok=1;
                    if (pierw!=temp && ok==1)  return  pierw;
                }
            }
        }
    }
    return NULL;
}

Term *  Term_div_sqrt(Term * T, Term * sq)   ///tworzy new po podzieleniu T przez sq zakĹ‚adajÄ…c ĹĽe tam jest sq
{
    if (T->name=="pierwiastek")
    {
        if (T==sq) return new RealNumber(1);
        return NULL;
    }
    if (T->name=="unarneodejmowanie")
    {
        Term* pierw = T->get_term(0);
        if (pierw->name=="pierwiastek")
        {
            if (pierw==sq) return new RealNumber(-1);
            return  NULL;
        }
        else
        {

            if (pierw->name=="mnozeniesymboliczne")
            {
                Term * NewM = new Multiplication();
                for (std::vector<Term*>::iterator m = pierw->subterms.begin(); m != pierw->subterms.end(); ++m)
                {
                    if ((*m)->name=="pierwiastek")
                    {
                        if ((*m)==sq)
                        {
                            NewM->Add_term(new RealNumber(-1));
                        }
                        else
                        {
                            NewM->Add_term((*m)->copy_term());
                        }
                    }
                    else if ((*m)->name=="unarneodejmowanie")
                    {
                        Term* pierwP = (*m)->get_term(0);
                        if (pierwP->name=="pierwiastek")
                        {
                            if (pierwP==sq)
                            {
                                NewM->Add_term(new RealNumber(1));
                            }
                            else
                            {
                                NewM->Add_term((*m)->copy_term());
                            }
                        }
                        else
                        {
                            NewM->Add_term((*m)->copy_term());
                        }
                    }
                    else
                    {
                        NewM->Add_term((*m)->copy_term());
                    }
                }
                return NewM;
            }




        }
    }
    if (T->name=="mnozeniesymboliczne")
    {
        Term * NewM = new Multiplication();
        for (std::vector<Term*>::iterator m = T->subterms.begin(); m != T->subterms.end(); ++m)
        {
            if ((*m)->name=="pierwiastek")
            {
                if ((*m)==sq)
                {
                    NewM->Add_term(new RealNumber(1));
                }
                else
                {
                    NewM->Add_term((*m)->copy_term());
                }
            }
            else if ((*m)->name=="unarneodejmowanie")
            {
                Term* pierw = (*m)->get_term(0);
                if (pierw->name=="pierwiastek")
                {
                    if (pierw==sq)
                    {
                        NewM->Add_term(new RealNumber(-1));
                    }
                    else
                    {
                        NewM->Add_term((*m)->copy_term());
                    }
                }
                else
                {
                    NewM->Add_term((*m)->copy_term());
                }
            }
            else
            {
                NewM->Add_term((*m)->copy_term());
            }
        }
        return NewM;
    }
    return NULL;
}

bool skracanie_16(Term* &T)
{
    if (T->name == "dodawanie")
    {
        for (std::vector<Term*>::iterator term1 = T->subterms.begin(); term1 != T->subterms.end(); ++term1)
        {

            Term * sq1 = czy_sqrt_in(*term1, NULL);
            while (sq1)
            {
                for (std::vector<Term*>::iterator term2 = term1+1; term2 != T->subterms.end(); ++term2)
                {



                    Term * sq2 = czy_sqrt_in(*term2, NULL);

                    while ( sq2 )
                    {


                        if (*(sq1) == *(sq2))
                        {

                            Term * D1 = Term_div_sqrt(*term1,sq1);
                            Term * D2 = Term_div_sqrt(*term2,sq2);

                            if (D1==NULL ||D2==NULL)
                                std::cout <<"*****************";

                            Term *W =  new Multiplication(new Add(D1,D2),sq1->copy_term());
                            *term1 = W;
                            T->subterms.erase(term2);
                            return true;
                        }

                        sq2 = czy_sqrt_in(*term2,sq2);
                    }
                }

                Term * sq3 = sq1;
                sq1 = czy_sqrt_in(*term1,sq1);
            }


        }
    }
    return false;
}



bool skracanie_17(Term* &T)  /// 1 = sqrt 1  i sqrt (a*a*b) = a*sqrt b
{
    if (T->name == "pierwiastek")
    {
        Term* pierw = T->get_term(1);
        if (pierw->name=="Rzeczywista")
        {
            RealNumber * R = (RealNumber*)pierw;
            int i = round(R->number);
            if (R->number == i)
            {
                if (R->number==1)
                {
                    T = new RealNumber(1);
                    return true;
                }
                else
                {
                    int a = 1, dziel =1;
                    while (a*a <= i)
                    {
                        if (i%(a*a)==0) dziel = a;
                        a++;
                    }
                    if (dziel!=1)
                    {
                        Term * M = new Multiplication();
                        M->Add_term(new RealNumber(dziel));
                        M->Add_term(new Sqrt(new RealNumber(2),new RealNumber(i/(dziel*dziel))));
                        T=M;
                        return true;
                    }
                }
            }
        }
    }
    return false;
}

bool skracanie_21(Term* &T)  /// A / (.... sqrt B.....) -> (A*sqrt B) /(.....B .....)
{
    if (T->name == "Dzielenie" )
    {
        Term* L =T->get_term(0);
        Term* M = T->get_term(1);
        if (M->name=="mnozeniesymboliczne")
        {
            for (std::vector<Term*>::iterator term = M->subterms.begin(); term != M->subterms.end(); ++term)
                if ((*term)->name=="pierwiastek")
                {
                    Term * copyM=new Multiplication();
                    for (std::vector<Term*>::iterator t = M->subterms.begin(); t != term; t++)
                        copyM->Add_term((*t)->copy_term());
                    copyM ->Add_term((*term)->get_term(1)->copy_term());
                    for (std::vector<Term*>::iterator t = term+1;  t!=M->subterms.end(); t++)
                        copyM->Add_term((*t)->copy_term());
                    T = new Division( new Multiplication(L->copy_term(),(*term)->copy_term()),copyM );
                    return true;
                }
        }
        else if (M->name=="pierwiastek")
        {
                   Term * P = M->get_term(1);

                    T = new Division( new Multiplication(L->copy_term(),M->copy_term()),P->copy_term() );
                    return true;
        }

    }
    return false;
}

bool skracanie_22(Term* &T)  /// A / (C+-sqrt D) |=>
{
    if (T->name == "Dzielenie" )
    {
        Term* L =T->get_term(0);
        Term* M = T->get_term(1);
        if (M->name=="dodawanie" && M->subterms.size()==2)
        {
            Term * SS = M->get_term(1);
            Term * sq = czy_sqrt_in(SS, NULL);
            if (sq)
            {
                    Term *  dq = Term_div_sqrt(SS, sq);
                    Term *newL = new Multiplication(L->copy_term(), new Add(M->get_term(0)->copy_term(),
                                                                            new Multiplication(new SubtractU(dq->copy_term()),sq->copy_term())));
                    Term * newM = new Add(new Multiplication(M->get_term(0)->copy_term(),M->get_term(0)->copy_term()),
                                                new SubtractU(new Multiplication(new Multiplication(dq->copy_term(),dq->copy_term()),sq->get_term(1)->copy_term())));
                    T = new Division(newL,newM);
                    return true;
            }
        }
    }
return false;
}

bool skrac_lista(Term* (&T),bool only_one)
{
    int zmiana = 0;
    int cos = true;/// rĂłb
    while ( cos )
    {
        cos = false;
        if (skracanie_1(T))
        {
            cos = true;
            zmiana++;
        }
        if ( zmiana ==0 || only_one) if (skracanie_2(T))
            {
                cos = true;
                zmiana++;
            }
        if ( zmiana ==0 || only_one) if  (skracanie_3(T))
            {
                cos = true;
                zmiana++;
            };
        if ( zmiana ==0 || only_one) if  (skracanie_4(T))
            {
                cos = true;
                zmiana++;
            };
        if ( zmiana ==0 || only_one) if  (skracanie_5(T))
            {
                cos = true;
                zmiana++;
            };
        if ( zmiana ==0 || only_one) if  (skracanie_6(T))
            {
                cos = true;
                zmiana++;
            };
        if ( zmiana ==0 || only_one) if  (skracanie_7(T))
            {
                cos = true;
                zmiana++;
            };

        if ( zmiana ==0 || only_one) if  (skracanie_8(T))
            {
                cos = true;
                zmiana++;
            };
        if ( zmiana ==0 || only_one) if  (skracanie_9(T))
            {
                cos = true;
                zmiana++;
            };
        if ( zmiana ==0 || only_one) if  (skracanie_9_1(T))
            {
                cos = true;
                zmiana++;
            };

        if ( zmiana ==0 || only_one) if  (skracanie_10(T))
            {
                cos = true;
                zmiana++;
            };
        if ( zmiana ==0 || only_one) if  (skracanie_11(T))
            {
                cos = true;
                zmiana++;
            };
        if ( zmiana ==0 || only_one) if  (skracanie_12(T))
            {
                cos = true;
                zmiana++;
            };
        if ( zmiana ==0 || only_one) if  (skracanie_13(T))
            {
                cos = true;
                zmiana++;
            };

        if ( zmiana ==0 || only_one) if  (skracanie_14(T))
            {
                cos = true;
                zmiana++;
            };
        if ( zmiana ==0 || only_one) if  (skracanie_15(T))
            {
                cos = true;
                zmiana++;
            };
        if ( zmiana ==0 || only_one) if  (skracanie_16(T))
            {
                cos = true;
                zmiana++;
            };
        if ( zmiana ==0 || only_one) if  (skracanie_17(T))
            {
                cos = true;
                zmiana++;
            };
        if ( zmiana ==0 || only_one) if  (skracanie_18(T))
            {
                cos = true;
                zmiana++;
            };
        if ( zmiana ==0 || only_one) if  (skracanie_19(T))
            {
                cos = true;
                zmiana++;
            };
        if ( zmiana ==0 || only_one) if  (skracanie_20(T))
            {
                cos = true;
                zmiana++;
            };
        if ( zmiana ==0 || only_one) if  (skracanie_21(T))
            {
                cos = true;
                zmiana++;
            };
        if ( zmiana ==0 || only_one) if  (skracanie_22(T))
            {
                cos = true;
                zmiana++;
            };
        if (only_one) cos = false;
    }
    if (zmiana > 0) return true;
    return false;
}
bool skracanie_reku(Term* (&T),bool only_one)
{
    int zmiany = 0;
    for (std::vector<Term*>::iterator term = T->subterms.begin(); term != T->subterms.end(); ++term)
    {
        if (zmiany==0 || !only_one )
            if( skracanie_reku(*term,only_one)) zmiany++;
    }
    if (zmiany==0 || !only_one )
        if (skrac_lista(T,only_one)) zmiany++;

    if (zmiany>0) return true;
    return false;
}
bool skracanie(Term* &T, bool only_one)
{
    int zmiany = 0;
    if(splaszcz(T)) zmiany++;
    if (zmiany == 0 || !only_one)
    {
        if (skracanie_reku(T,only_one)) zmiany++;
    }
    if (zmiany>0) return true;
    return false;
}


#endif
