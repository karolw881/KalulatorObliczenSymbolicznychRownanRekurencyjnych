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


#ifndef term_cpp
#define term_cpp

class Error
{
public:
    std::string message;
    Error(std::string message) : message(message)
    {
        std::cout << "Blad : ) " << message;
    }
};

class Result
{
public:
    double liczba;
    double liczba2;
    bool poprawna;
public:
    Result(double liczba) : liczba(liczba), poprawna(true) {}
    Result(double liczba, double liczba2) : liczba(liczba), liczba2(liczba2), poprawna(true) {}
    Result() : liczba(0), poprawna(false) {}
};

class Term
{
public:
    std::vector<Term*> subterms;
    std::string name;
    int prioriti;     /// + -> 2 - -> 2  * 3  \frac 1   power 0     liczba - > 10

public:
    Term(std::string Name)
    {
        this->name = Name; prioriti = 0;
    }
    virtual Term * copy_term()
    {

        return new Term(*this);
    }

    Term(const Term& other) : name(other.name)
    {
        prioriti = other.prioriti;
        for (std::vector<Term*>::const_iterator it = other.subterms.begin(); it != other.subterms.end(); ++it)
        {
            subterms.push_back((*it)->copy_term());
        }
    }

    ~Term()   //jesli dziaÂła destruktor to rozwala siĂŞ pamiĂŞĂ¦
    {
        // for (std::vector<Term*>::iterator it = subterms.begin(); it != subterms.end(); ++it) {
        //   delete *it;
        //};
    };
    virtual void Add_term(Term* t1)
    {
        subterms.push_back(t1);
    }
    virtual void print_new(std::ostream& plik)
    {
        plik << "print";
    }

    virtual Result calculate()
    {
        return Result();
    };

    Term * get_term(int nr)
    {
        if (nr<0 || subterms.size()<=nr)
        {
            std::cout << "nieprawne poszukiwanie w termie";
            exit(-10);
            return NULL;
        }
        return subterms[nr];
    }
    virtual float term_tree_size()
    {
        float sum=0;
        for (std::vector<Term*>::const_iterator it = subterms.begin(); it != subterms.end(); ++it)
        {
            sum+= (*it)->term_tree_size();
        }
        if (sum == 0)
        {
            sum=1;
        }
        return sum;
    }
    virtual void term_to_node(std::ostream& plik)
    {
        plik << "node [fact] {$"+name.substr(0, 4)+"$}\n";

        for (std::vector<Term*>::const_iterator it = subterms.begin(); it != subterms.end(); ++it)
        {
            plik << "child{\n";
            if (subterms.size()>1)
            {
                 float dist =  term_tree_size() / (subterms.size()) * (subterms.size()-1);
                 float d=round(dist*100);
                plik << "[sibling distance="<<d/100<<"cm]\n";
            }
            (*it)-> term_to_node(plik);
            plik << "}\n";
        }

    }
    void term_tree(std::ostream& plik)
    {
        plik << "\\begin{center}\n";
        plik << "\\begin{tikzpicture}[level distance=0.5cm, growth parent anchor=south]\n";
        plik << "\\node [fact] {$Term tree"<<term_tree_size()<<"$} [-latex]\n";

        plik<< "child{ ";

        if (subterms.size()>0)
        {
            float dist = term_tree_size();
            //dist = dist / (subterms.size()) * (subterms.size()-1);
            plik << "[sibling distance="<< (int)dist <<"cm]\n";
        }
        else
        {
            plik << "\n";
        }
        term_to_node(plik);
        plik << "};\n";
        plik << "\\end{tikzpicture}\n";
        plik << "\\end{center}\n";
    };


    bool operator==(const Term& rhs);
};

// Rzeczywiste
class RealNumber : public Term
{
public:
    double number;
public:
    virtual Term * copy_term()
    {
        return new RealNumber(*this);
    }
    RealNumber(double number) : Term("Rzeczywista"), number(number)
    {
        prioriti = 50;
    }
    RealNumber(const RealNumber& other) : Term(other), number(other.number) {};
    ~RealNumber() {};
    double get_number(void)
    {
        return number;
    }

    virtual void print_new(std::ostream& plik)
    {
        plik << "{" << number << "}";
    }
    Result calculate()
    {
        return Result(number);
    }


    virtual void term_to_node(std::ostream& plik)
    {
        plik << "node [fact] {$Rzecz$}\n";
        plik << "child{\n";
        plik << "node  [leaf] {$"<<number <<"$}\n";
        plik << "}\n";
    }
};



// Odejmowanie Unarne
class SubtractU : public Term
{
public:

    SubtractU(Term* t1) : Term("unarneodejmowanie")
    {
        prioriti = 2;
        Add_term(t1);
    }
     virtual Term * copy_term()
    {
        return new SubtractU(*this);
    }


    virtual void print_new(std::ostream& plik)
    {
        plik << "-";
 if (get_term(0)->prioriti >= 2)
       {
            plik << " ( ";
            get_term(0)->print_new(plik);
            plik << " ) ";
       }
        else get_term(0)->print_new(plik);
    }

    Result calculate() override
    {
        Result t1_result = get_term(0)->calculate();
        if (!t1_result.poprawna)
            return Result();
        return Result(-t1_result.liczba);
    }


};

// Mnozenie

class Multiplication : public Term
{
public:
    Multiplication() : Term("mnozeniesymboliczne")
    {
        prioriti = 10;
    }
    Multiplication(Term* Czynnik1, Term* Czynnik2) : Term("mnozeniesymboliczne")
    {
        Add_term(Czynnik1);
        Add_term(Czynnik2);
        prioriti = 10;
    }
         virtual Term * copy_term()
    {
        return new Multiplication(*this);
    }


    Multiplication(const Multiplication& other) : Term(other) {};


    void print_new(std::ostream& plik)
    {
        bool first_term = true;
        for (std::vector<Term*>::iterator it = subterms.begin(); it != subterms.end(); ++it)
        {
            if (!first_term)
                plik << "\\cdot";
            if ((*it)->prioriti <= prioriti)
            {
                plik << "(";
            }
            (*it)->print_new(plik);
            if ((*it)->prioriti <= prioriti)
            {
                plik << ")";
            }
            first_term = false;
        }
    }

    Result calculate() override
    {
        Result result(1);
        for (std::vector<Term*>::iterator it = subterms.begin(); it != subterms.end(); ++it)
        {
            Result term_result = (*it)->calculate();
            if (!term_result.poprawna)
            {
                return Result();
            }
            result = result.liczba * term_result.liczba;
        }
        return result;
    }

    ~Multiplication()
    {
        // for (std::vector<Term*>::iterator it = subterms.begin(); it != subterms.end(); ++it) {
        //   delete *it;
//   }
    }
};
class Division : public Term
{
public:
    Division(Term* t1, Term* t2) :Term("Dzielenie")
    {
        prioriti = 20;
        Add_term(t1);
        Add_term(t2);
    }

    Division(const Division& other) :Term(other){};

    void print_new(std::ostream& plik)
    {
        plik << "\\frac{ ";
        subterms[0]->print_new(plik);
        plik << " }{ ";
        subterms[1]->print_new(plik);
        plik << " } ";
    }
    virtual Term * copy_term()
    {
        return new Division(*this);
    }

    Result calculate() override
    {
        Result t1_result = subterms[0]->calculate();
        Result t2_result = subterms[1]->calculate();
        Result w;

        if (t2_result.liczba == 0)
        {
            throw Error("Dzielenie przez zero");
        }

        w.liczba = t1_result.liczba / t2_result.liczba;
        w.poprawna = true;
        return w;
    }
};


//  dodawnaie
class Add : public Term
{
public:

    Add(): Term( "dodawanie")
    {
        prioriti = 4;
    }

    Add(Term *t1,Term * t2): Term( "dodawanie")
    {
        Add_term(t1);
        Add_term(t2);
        prioriti = 4;
    }
    Add(const Add& other) :Term(other){};

 virtual Term * copy_term()
    {
        return new Add(*this);
    }

    virtual void print_new(std::ostream& plik)
    {
        for (int i = 0; i < subterms.size(); i++)
        {
            if (subterms[i]->name != "unarneodejmowanie" && i!=0)
                plik << " + ";
            if (subterms[i]->prioriti < prioriti && subterms[i]->name != "unarneodejmowanie" && i!=0 )
            {
                plik << "(";
            }

            subterms[i]->print_new(plik);

            if (subterms[i]->prioriti < prioriti &&subterms[i]->name != "unarneodejmowanie" && i!=0)
            {
                plik << ")";
            }
        }

    }



    Result calculate() override
    {
        for (int i = 0; i < subterms.size(); i++)
        {
            if (!subterms[i]->calculate().poprawna) return Result();
        }

        double W = 0;
        for (int i = 0; i < subterms.size(); i++)
        {
            W += subterms[i]->calculate().liczba;
        };
        return Result(W);



    }

    ~Add()
    {


    }










};


// potega
class Power : public Term
{
public:

    Power(Term* t1,Term* t2) : Term("potega")
    {
        prioriti = 40;
        Add_term(t1);
        Add_term(t2);
    }

    Power(const Power& other) : Term(other) {};

    ~Power()
    {

    }
     virtual Term * copy_term()
    {
        return new Power(*this);
    }

    void print_new(std::ostream& plik) override
    {
        bool need_parentheses = false;    /// wyjÄ…tek aby liczba ujemna byĹ‚a w nawiasie
        if (get_term(0)->prioriti < prioriti)
        {
            need_parentheses = true;
        }
        if (get_term(0)->name == "Rzeczywista")/// wyjÄ…tek aby liczba ujemna byĹ‚a w nawiasie
        {
            RealNumber  * R = (RealNumber*) get_term(0);
            if (R->number<0) need_parentheses = true;

        }


        if (need_parentheses)
        {
            plik << "\\left(";
        }
        get_term(0)->print_new(plik);

        if (need_parentheses)
        {
            plik << "\\right)";
        }

        plik << "^ {";
        get_term(1)->print_new(plik);
        plik << "}";

    }

    Result calculate() override
    {
        Result re1 = subterms[0]->calculate();
        Result re2 = subterms[1]->calculate();
        Result ww;
        ww.liczba = std::pow(re1.liczba, re2.liczba);
        ww.poprawna = true;
        return ww;
    }
};

// Pierwiastek
class Sqrt : public Term
{

public:
    Sqrt(Term* t1, Term* t2) : Term("pierwiastek")
    {
        name = "pierwiastek";
        prioriti = 100;
        Add_term(t1);
        Add_term(t2);
    }

    Sqrt(const Sqrt& other) : Term(other) {};
     virtual Term * copy_term()
    {
        return new Sqrt(*this);
    }

    virtual void print_new(std::ostream& plik)
    {
        plik << "\\sqrt[";
        get_term(0)->print_new(plik);
        plik << "]{";
        get_term(1)->print_new(plik);
        plik << "}";
    }

    Result calculate() override
    {
        Result re1 = subterms[1]->calculate();
        Result re2 = subterms[0]->calculate();
        Result ww;
        if (re1.liczba < 0)
            throw Error("liczba pod pierwistkiem mniejsza od zera chyba bez urojonych zabronione");
        else
        {
            ww.liczba = pow(re1.liczba, 1.0 / re2.liczba);
            ww.poprawna = true;
        }
        return ww;
    }

    ~Sqrt()
    {
        for (std::vector<Term*>::iterator it = subterms.begin(); it != subterms.end(); ++it)
        {
            delete *it;
        }
    }
};
class Var : public Term
{
public:
    std::string Vname;

public:
    Var(std::string n) : Term("var")
    {
        Vname=n;
        prioriti = 50;
    }
    Var(const Var& other) : Term(other), Vname(other.Vname)
    {
        prioriti = 50;
    }
   virtual Term * copy_term()
    {
        return new Var(*this);
    }

    virtual void print_new(std::ostream& plik)
    {
        plik << "{" << Vname << "}";
    }

    void rename(std::string co, std::string na)
    {
        if (Vname == co) Vname = na;
    }

    virtual void term_to_node(std::ostream& plik)
    {
        plik << "node [fact] {$Var$}\n";
        plik << "child{\n";
        plik << "node  [leaf] {$"<<Vname <<"$}\n";
        plik << "}\n";
    }

};
class FormalSeries : public Term
{



public:
    FormalSeries(Term* id, Term* ig, Term* wyrazeniex, Var* nn) : Term("FormalSeries")
    {
        prioriti = 30;
        Add_term(id);
        Add_term(ig);
        Add_term(wyrazeniex);
        Add_term(nn);
    }


 virtual Term * copy_term()
    {
        return new FormalSeries(*this);
    }

    virtual void print_new(std::ostream& plik)
    {
        plik << "\\sum_{";
        get_term(3)->print_new(plik);
        plik << " = ";
        get_term(0)->print_new(plik);
        plik << "}^{";
        get_term(1)->print_new(plik);
        plik << "}";
        get_term(2)->print_new(plik);
    };


};


class VarIndex : public Term
{
//public:
  //  Term* num;
  //  Term* Index;
public:
    VarIndex(Term* indexTerm, Term* numTerm) : Term("VarIndex")
    {
        name = "Wyrazenie z Indeksem";
        Add_term(indexTerm);
         Add_term(numTerm);
        prioriti = 50;

    }


    //VarIndex(const VarIndex& other) : Term(other), Index(new Term(*other.Index)), num(new Term(*other.num)) {}

    virtual void print_new(std::ostream& plik)
    {
        plik << "{";
        get_term(0)->print_new(plik);
        plik << "}_{";
        get_term(1)->print_new(plik);
        plik << "}";
    }

    virtual Term * copy_term()
    {
        return new VarIndex(*this);
    }



    void printLatex1(std::ostream& plik)
    {
        std::string index_str;
        std::string num_str;

        // JeĂ„ÂąĂ˘â‚¬Âşli wyraĂ„ÂąĂ„Â˝enie indeksowe to staĂ„ÂąĂ˘â‚¬Ĺˇa, to przepisz wartoĂ„ÂąĂ˘â‚¬ÂşĂâ€žĂ˘â‚¬Âˇ do Ă„ÂąĂ˘â‚¬ĹˇaĂ„ÂąĂ˘â‚¬Ĺľcucha znakĂ„â€šĂ…â€šw
        if (RealNumber* c = dynamic_cast<RealNumber*>(get_term(1)))
        {
            // index_str = c->getValueAsString();
        }
        // W przeciwnym przypadku zapisz wyraĂ„ÂąĂ„Â˝enie indeksowe do Ă„ÂąĂ˘â‚¬ĹˇaĂ„ÂąĂ˘â‚¬Ĺľcucha znakĂ„â€šĂ…â€šw w formacie LaTeX
        else
        {
            index_str = "\\left(";
            // Index->printLatex(plik);
            index_str += "\\right)";
        }

        // JeĂ„ÂąĂ˘â‚¬Âşli wyraĂ„ÂąĂ„Â˝enie numeryczne to staĂ„ÂąĂ˘â‚¬Ĺˇa, to przepisz wartoĂ„ÂąĂ˘â‚¬ÂşĂâ€žĂ˘â‚¬Âˇ do Ă„ÂąĂ˘â‚¬ĹˇaĂ„ÂąĂ˘â‚¬Ĺľcucha znakĂ„â€šĂ…â€šw
        if (RealNumber* c = dynamic_cast<RealNumber*>(get_term(0)))
        {
            //num_str = c->RealNumber ();
        }
        // W przeciwnym przypadku zapisz wyraĂ„ÂąĂ„Â˝enie numeryczne do Ă„ÂąĂ˘â‚¬ĹˇaĂ„ÂąĂ˘â‚¬Ĺľcucha znakĂ„â€šĂ…â€šw w formacie LaTeX
        else
        {
            num_str = "\\left(";
            //num->printLatex(plik);
            num_str += "\\right)";
        }

        // Zapisz indeks z dolnym indeksem do pliku w formacie LaTeX
        plik << "x_{" << index_str << "-" << num_str << "}";
    }

    ~VarIndex()
    {
       // delete Index, num;
    }
};





bool Term::operator==(const Term& rhs)
{
    if (name == rhs.name && subterms.size() == rhs.subterms.size())
    {
        for (int i=0; i<subterms.size() ; i++)
        {
            if (!( (*subterms[i])  ==  *(rhs.subterms[i]) )) return false;
        }
        //var i RealNumber trzeba sprawdzic niezaleznie
        if (name == "var")
        {
              if  (((Var *)this)->Vname != ((Var *)&rhs)->Vname) return false;
        }
         if (name == "Rzeczywista")
        {
              if  (((RealNumber *)this)->number != ((RealNumber *)&rhs)->number) return false;
        }
        return true;

    }
    else return false;
    //std::vector<Term*> subterms;
    // std::string name;
}


#endif
