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

#define pi 3.14159265359
#define e 0.5772156649   


class Error {
public:
	std::string message;
	Error(std::string message) : message(message) {
		std::cout << "Blad : ) " << message;
	}
};

// WYNIK  
class Result {
public:
	double liczba;
	double liczba2;
	bool poprawna;
public:
	Result(double liczba) : liczba(liczba), poprawna(true) {}
	Result(double liczba , double liczba2) : liczba(liczba), liczba2(liczba2), poprawna(true) {}
	Result() : liczba(0), poprawna(false) {}
};

// klasa bazowa 
class Term {
public:
	
	std::vector<Term*> subterms;
	std::string name;
	int prioriti;     /// + -> 2 - -> 2  * 3    \frac 1   power 0     liczba - > 10  

public:
	Term() {
		this->name = "wyrazenie";
		prioriti = 0;
	
	}
	Term(const Term& other) : name(other.name) {};
	~Term() {};
	virtual void Add_dodaj(Term* t1){}
	virtual void print() { std::cout << "print"; }  ///kasacja
	virtual void print_new(std::ofstream& plik) { plik << "print"; }
	virtual Result calculate() { return Result(); };
	virtual void rename(std::string co, std::string na) {};
	virtual bool operator<(const Term& t) const { return prioriti < t.prioriti; }
	virtual void  Reduce(std::ofstream& plik) { plik << "reduce"; }



};


// Rzeczywiste 
class RealNumber : public Term {
public:
	double number;
public:
	RealNumber() : Term(), number(0) {
		name = "Rzeczywista";
		prioriti = 10;
	}

	RealNumber(double number) : Term(), number(number) {
		name = "Rzeczywista";
		prioriti = 10;
	}

	RealNumber(const RealNumber& other) : Term(other), number(other.number) {}

	RealNumber(RealNumber&& other) noexcept : Term(std::move(other)), number(std::move(other.number)) {}
	~RealNumber() {}

	double get_number(void) {
		return number;
	}
	RealNumber operator=(const RealNumber& a1) {
		number = a1.number;
	}
	RealNumber operator+(const RealNumber& a1) {
		return RealNumber(number + a1.number);
	}
	RealNumber operator-(const RealNumber& a1) {
		return RealNumber(number - a1.number);
	}
	RealNumber operator*(const RealNumber& a1) {
		return RealNumber(number * a1.number);
	}
	RealNumber operator/(const RealNumber& a1) {
		return RealNumber(number / a1.number);
	}
	void operator += (const RealNumber& a1) {
		number = number + a1.number;
	}

	void operator -= (const RealNumber& a1) {
		number = number - a1.number;
	}

	void operator *= (const RealNumber& a1) {
		number = number * a1.number;
	}

	void operator /= (const RealNumber& a1) {
		number = number / a1.number;
	}

	bool operator==(const RealNumber& a1) {
		if (number == a1.number) {
			return true;
		}
		return false;
	}
	virtual void print_new(std::ofstream& plik);
	Result calculate() {
		return Result(number);
	}
	bool operator<(const Term& t) const {
		return prioriti < t.prioriti;
	}


	virtual void Reduce(std::ofstream& plik) override {
		//plik << " Number " << number;
		plik << "Zredukowany"; 
		
	}

};

void RealNumber::print_new(std::ofstream& plik) {
	//plik << "{" << number << "}";
	plik << number;
}

// Ulamek
// licznik = numerator 
// mianownik = denominator 
class Fraction : public Term {
public:
	int numerator;
	int denominator;
public:
	Fraction(int numerator, int denominator) : Term(), numerator(numerator), denominator(denominator) {}
	int nwd(int a, int b) { if (b == 0) return a; return nwd(b, a % b); }
	void skroc() {
		if (denominator == 0) {
			throw Error("Dzielenie przez zero ");
			exit(1);
		};
		int n = nwd(numerator, denominator);
		if (n < 0) n = -n;
		numerator /= n;
		denominator /= n;
	}
	~Fraction() {
	}

	Fraction operator=(const Fraction& x)
	{
		numerator = x.numerator; denominator = x.denominator;
	};


	Fraction operator+(const Fraction& x)
	{
		return Fraction(numerator * x.denominator + denominator * x.numerator, denominator * denominator);
	};

	void operator+=(const Fraction& x)
	{
		numerator = numerator * x.denominator + denominator * x.numerator; denominator = denominator * x.denominator; skroc();
	}

	Fraction operator-(const Fraction& x)
	{
		return Fraction(numerator * x.denominator - numerator * x.denominator, denominator * x.denominator);
	};

	void operator-=(const Fraction& x) {
		numerator = numerator * x.denominator - numerator * x.denominator, denominator = denominator * x.denominator; skroc();
	};

	Fraction operator- (void) {
		return Fraction(-numerator, denominator);
	}
	Fraction operator*(const Fraction& x)
	{
		return Fraction(numerator * x.numerator, denominator * x.denominator);
	};

	void operator*=(const Fraction& x)
	{
		numerator *= x.numerator, denominator *= x.denominator; skroc();
	};

	Fraction operator/(const Fraction& x)
	{
		return Fraction(numerator * x.denominator, denominator * x.numerator);
	};

	void operator/=(const Fraction& x)
	{
		numerator = numerator * x.denominator, denominator = denominator * x.numerator; skroc();
	};

	bool operator==(const Fraction& x) {
		if (numerator * x.denominator == denominator * x.numerator)
		{
			return true;
		}
		else return false;
	}

	friend Fraction operator + (int integer, const Fraction& x) {
		return Fraction(x.numerator + x.numerator * integer, x.denominator);
	}

	void print(std::ofstream plik) {
		skroc();
		int n = numerator;
		int de = denominator;
		plik << "(";
		if (n < 0 && de < 0) { n = -n; de = -de; }
		if (n < 0) {
			plik << "-(";
			n = -n;
			plik << n << " | " << de << "))";
		}
		else if (de < 0) {
			plik << "-(";
			de = -de;
			plik << n << " | " << de << "))";
		}
		else plik << n << " | " << de << ")";
	}


	Result calculate() override {
		skroc();
		Result ww;
		double wyn = double(numerator) / double(denominator);
		ww.liczba = wyn;
		ww.poprawna = true;
		return ww;
	}
	bool operator<(const Term& t) const {
		return prioriti < t.prioriti;
	}

	
};



class SubtractU : public Term {
public:
	Term* t1;
public:

	// Konstruktor domyślny
	SubtractU() : Term(), t1(nullptr) { name = "unarneodejmowanie"; prioriti = 2; }

	// Konstruktor kopiujący
	SubtractU(const SubtractU& other) : Term(other), t1(new Term(*other.t1)) { name = "unarneodejmowanie"; prioriti = 2; }

	// Konstruktor przenoszący
	SubtractU(SubtractU&& other) noexcept : Term(std::move(other)), t1(std::move(other.t1)) { name = "unarneodejmowanie"; prioriti = 2; }

	// Konstruktor inicjalizujący pole t1
	SubtractU(Term* t1) : Term(), t1(t1) { name = "unarneodejmowanie"; prioriti = 2; }


	/// <summary>
	/// cos i = cos(5); domyslny
	/// cos i = cos(j); kopiujacy
	/// i=j; przeciazonie 
	/// </summary>





	bool operator<(const Term& t) const {
		return prioriti < t.prioriti;
	}



	virtual void print_new(std::ofstream& plik) {
		plik << "-(";
		t1->print_new(plik);
		plik << ")";

	}






	Result calculate() override {
		Result t1_result = t1->calculate();

		if (!t1_result.poprawna)
			return Result();
		return Result(-t1_result.liczba);
	}



	void Reduce(std::ofstream& plik) {
		if (t1 != nullptr) {
			t1->Reduce(plik);
		}

		// Sprawdź, czy t1 jest liczbą rzeczywistą
		bool t1_is_real = (t1 != nullptr && t1->name == "Rzeczywista");
		plik << t1->name;
		bool t1_is_sqrt = (t1 != nullptr && t1->name == "pierwiastek");
		

		if (t1_is_real) {
			// t1 jest liczbą rzeczywistą, oblicz wynik
			double result = t1->calculate().liczba;
			Term* tempoflife = new RealNumber(result);

			t1 = tempoflife;


		}
		

	}


	~SubtractU() {
		delete t1;
	}
};

// Mnozenie 

class Multiplication : public Term {
public:
	std::vector<Term*> Tvect;
public:
	Multiplication(Term* Czynnik1, Term* Czynnik2) : Term() {
		Tvect.push_back(Czynnik1);
		Tvect.push_back(Czynnik2);
		prioriti = 3;
		name = "mnozeniesymboliczne";
	}
	Multiplication(const Multiplication& other) : Term(other) {
		name = "mnozeniesymboliczne";
		prioriti = 3;
		for (auto term : other.Tvect) {
			Tvect.push_back(new Term(*term));
		}
	}
	Multiplication() : Term() {
		name = "mnozeniesymboliczne";
		prioriti = 3;
	}
	void Add_dodaj(Term* t1) {
		Tvect.push_back(t1);
	}
	void print() {
		bool first_term = true;
		for (auto& term : Tvect) {
			if (!first_term) {
				std::cout << " * ";
			}
			term->print();
			first_term = false;
		}
	}

	bool operator<(const Term& t) const {
		return prioriti < t.prioriti;
	}
	void print_new(std::ofstream& plik) {
		bool first_term = true;
		std::vector<std::unique_ptr<Term>> subterms;

		for (auto& term : Tvect) {
			if (!first_term && term->prioriti >= prioriti) {
				plik << "(";
			}
			if (term->subterms.size() > 0) { // second factor with subterms	
				plik << "(";
				bool first_subterm = true;
				for (auto& subterm : term->subterms) {
					if (!first_subterm) {
						plik << "-";
					}
					subterm->print_new(plik);
					first_subterm = false;
				}
				plik << ")";
			}
			else { // normal term
				term->print_new(plik);
			}
			if (!first_term && term->prioriti >= prioriti) {
				plik << ")";
			}
			if (&term != &Tvect.back()) {
				plik << "\\cdot";
			}
			first_term = false;
		}
	}
	void Reduce(std::ofstream& plik) {
		double product = 1.;
		std::vector<Term*> new_Tvect;
		std::vector<Term*> summands;

		for (auto term : Tvect) {

			if (term->name == "Rzeczywista") {
				product *= term->calculate().liczba;
			}
			 if (term->name == "mnozeniesymboliczne") {
				Multiplication* add = dynamic_cast<Multiplication*>(term);
				add->Reduce(plik);  // Rekurencyjnie przetwórz poddrzewo dodawania.
				product *= add->calculate().liczba;
				for (auto nested_term : add->Tvect) {
					summands.push_back(nested_term);
				}
			}
			 else if (term->name == "unarneodejmowanie") {
				 term->Reduce(plik);
				 product *= -1;
				 SubtractU* afd = dynamic_cast<SubtractU*>(term);
				 Result resultx(-1);
				 for (auto& term : Tvect) {
					 Result term_result = term->calculate();
					 if (!term_result.poprawna) {
						 //return Result();
					 }
					 if (term_result.poprawna == true) {
						 resultx.liczba *= term_result.liczba;
						 plik << term->name;
						 summands.push_back(term);
						 summands.push_back(new RealNumber(resultx.liczba));
					 }
					 else {
						 product *= term_result.liczba;
					 }
				 }
			 }





			 else {

				 summands.push_back(term);
			 }
			 }


		/*

		double suma = 1.0;
		for (auto term : summands) {
			if (term->name == "Rzeczywista") {
				suma *= term->calculate().liczba;
				plik << suma;
			}
			if (term->name == "pierwiastek") {
				new_Tvect.push_back(term);
			}
			 if (term->name == "potega") {
				Result popop = term->calculate();
				RealNumber* zxzx = new RealNumber(popop.liczba);
				suma *= zxzx->calculate().liczba;

			}
			 if (term->name == "unarneodejmowanie" ) {
				 
				
				 plik << "xxx";
				 new_Tvect.push_back(term);




			 }
			else {

				suma *= term->calculate().liczba;
				plik << term->calculate().liczba << " \\\\";

			}
		}

		*/

		if (product != 1.0) {
			RealNumber* r = new RealNumber(product);
			summands.push_back(r);
		}

		Tvect = summands;
	}


	Result calculate() override {
		Result result(1);
		for (auto& term : Tvect) {
			Result term_result = term->calculate();
			if (!term_result.poprawna) {
				return Result();
			}
			result = result.liczba * term_result.liczba;
		}
		return result;
	}
	Multiplication operator*(const Multiplication& other) {
		Multiplication result(*this);
		for (auto term : other.Tvect) {
			result.Tvect.push_back(new Term(*term));
		}
		return result;
	}
	~Multiplication() {
		for (auto& term : Tvect) {
			delete term;
		}
	}
};

// Dzielenie 	
class Division : public Term {
public:
	Term* t1;
	Term* t2;
public:
	Division() : Term(), t1(nullptr), t2(nullptr) {
		name = "Dzielenie";
		prioriti = 3;
	}

	Division(Term* t1, Term* t2) : Term(), t1(t1), t2(t2) {
		name = "Dzielenie";
		prioriti = 3;
	}

	Division(const Division& other) : Term(other), t1(new Term(*other.t1)), t2(new Term(*other.t2)) {
		prioriti = 3;
	}

	Division(Division&& other) noexcept : t1(std::move(other.t1)), t2(std::move(other.t2)) {
		name = std::move(other.name);
		prioriti = other.prioriti;
	}

	void print() {
		std::cout << "(";
		t1->print();
		std::cout << "|";
		t2->print();
		std::cout << ")";
	}



	void print_new(std::ofstream& plik) {
		plik << "\\frac{";
		t1->print_new(plik);
		plik << "}{";
		t2->print_new(plik);
		plik << "}";
	}
	virtual void  Reduce(std::ofstream& plik) {
		t1->Reduce(plik);
		t2->Reduce(plik);

	}

	Result calculate() override {
		Result t1_result = t1->calculate();
		Result t2_result = t2->calculate();
		Result w;

		if (t2_result.liczba == 0) {
			throw Error("Dzielenie przez zero : ( hah ");
		}

		//std::cout << t1_result.liczba;
		w.liczba = t1_result.liczba / t2_result.liczba;
		w.poprawna = true;
		return w;
	}




	~Division() {
		delete t1, t2;
	}
	bool operator<(const Term& t) const {
		return prioriti < t.prioriti;
	}
};


//  dodawnaie   
class Add : public Term {
public:
	std::vector< Term* > Tvect;


public:
	Add() :Term()
	{

		name = "dodawanie";
		prioriti = 2;
	}




	Add(const Add& other) : Term(other)
	{

	
		name = "dodawanie";
		prioriti = 2;
		for (int i = 0; i < other.Tvect.size(); i++)

			Tvect.push_back(other.Tvect[i]);

	}

	Add(Term* t1, Term* t2) : Term() {
		name = "dodawanie";
	
		prioriti = 2;
		Tvect.push_back(t1);
		Tvect.push_back(t2);
	}

	void Add_dodaj(Term* t1)
	{
		Tvect.push_back(t1);

	}


	void print() {
		std::cout << "(dodawanie)";
	}

	bool operator<(const Term& t) const {
		return prioriti < t.prioriti;
	}

	virtual void print_new(std::ofstream& plik) {

		for (int i = 0; i < Tvect.size(); i++)
		{
			if (i != 0)
			{
				if (Tvect[i]->name == "unarneodejmowanie")
				{
					/// nic bo znak rozdzielający będzie z unarnego;
				}
				else
				{
					plik << "+";
				};


			}
			if (Tvect[i]->prioriti > prioriti)
			{
				plik << "(";
			}
			Tvect[i]->print_new(plik);
			if (Tvect[i]->prioriti > prioriti)
			{
				plik << ")";
			};
		}
		/*plik << "(";
		t1->print_new(plik);
		plik << "+";
		t2->print_new(plik);
		plik << ")";*/
	}


	Result calculate() override {
		for (int i = 0; i < Tvect.size(); i++)
		{
			if (!Tvect[i]->calculate().poprawna) return Result();
		}

		double W = 0;
		for (int i = 0; i < Tvect.size(); i++)
		{

			W += Tvect[i]->calculate().liczba;
		};
		return Result(W);


		// t1_result = t1->calculate();
		/// t2_result = t2->calculate();
		///if (!t1_result.poprawna || !t2_result.poprawna)
		//	return Result();
		//return Result(t1_result.liczba + t2_result.liczba);
	}

	~Add() {
		for (int i = 0; i < Tvect.size(); i++)
		{
			delete Tvect[i];
		}
	




	}
	void Reduce(std::ofstream& plik) {
		std::vector<Term*> new_Tvect;
		std::vector<Term*> summands;
		double nested_sum = 0.;

		for (auto term : Tvect) {
			term->Reduce(plik);

			if (term->name == "Rzeczywista") {
				summands.push_back(term);
			}
			else if (term->name == "dodawanie") {
				Add* add = dynamic_cast<Add*>(term);
				add->Reduce(plik);  // Rekurencyjnie przetwórz poddrzewo dodawania.
				nested_sum += add->calculate().liczba;
				for (auto nested_term : add->Tvect) {
					summands.push_back(nested_term);
				}
			}
	
			else {
				summands.push_back(term);
			}
		}
	
		
		double suma = 0;
		for (auto term : summands) {
			if (term->name == "pierwiastek") {
				new_Tvect.push_back(term);
			}
			else if (term->name == "potega" ) {
				Result popop = term->calculate();
				RealNumber * zxzx = new RealNumber(popop.liczba);
				suma += zxzx->calculate().liczba;
				
			}
			else if (term->name == "unarneodejmowanie") {
				new_Tvect.push_back(term);
			}
			else {
				suma += term->calculate().liczba;
				plik << term->calculate().liczba << " \\\\";
			
			}
		}

		if (suma != 0) {
			RealNumber* r = new RealNumber(suma);
			
			new_Tvect.push_back(r);
		}

		Tvect = new_Tvect;
	}


	







};





// odejmowanie 

//     
class Subtrac : public Term {
public:
	Term* t1;
	Term* t2;
public:
	Subtrac(Term* t1 = nullptr, Term* t2 = nullptr) : Term(), t1(t1), t2(t2) {
		name = "dodejmowanie"; prioriti = 2;
	}
	Subtrac(const Subtrac& other) : Term(other), t1(new Term(*other.t1)), t2(new Term(*other.t2)) {}
	Subtrac() :Term() {
		name = "odejmowanie "; prioriti = 2;
	}

	// jeszcze jeden konstruktor 
	void print() {
		std::cout << "(";
		t1->print();
		std::cout << "+";
		t2->print();
		std::cout << ")";
	}

	bool operator<(const Term& t) const {
		return prioriti < t.prioriti;
	}
	void print_new(std::ofstream& plik) {
		bool needParentheses = (t1->prioriti < prioriti);
		if (needParentheses) {
			plik << "(";
		}
		t1->print_new(plik);
		if (needParentheses) {
			plik << ")";
		}
		plik << "-";
		bool needParentheses2 = (t2->prioriti <= prioriti);
		if (needParentheses2) {
			plik << "(";
		}
		t2->print_new(plik);
		if (needParentheses2) {
			plik << ")";
		}
	}
	void Reduce(std::ofstream& plik) {
		if (t1 != nullptr) {
			t1->Reduce(plik);
		}
		if (t2 != nullptr) {
			t2->Reduce(plik);
		}

	}



	Result calculate() override {
		Result t1_result = t1->calculate();
		Result t2_result = t2->calculate();
		if (!t1_result.poprawna || !t2_result.poprawna)
			return Result();
		return Result(t1_result.liczba - t2_result.liczba);
	}

	~Subtrac() {
		delete t1, t2;
	}
};




// Pierwiastek 
class Sqrt : public Term {
public:
	Term* t1;
	Term* t2;
public:
	Sqrt() : Term() { name = "pierwiastek"; prioriti = 0;  }
	Sqrt(Term* t1, Term* t2) : Term(), t1(t1), t2(t2) { name = "pierwiastek"; prioriti = 0; }
	Sqrt(const Sqrt& other) : Term(other), t1(new Term(*other.t1)), t2(new Term(*other.t2)) { prioriti = 0; }
	void print() {
		std::cout << "sqrt(";
		t1->print();
		std::cout << ",";
		t2->print();
		std::cout << ")";
	}
	bool operator<(const Term& t) const {
		return prioriti < t.prioriti;
	}
	virtual void print_new(std::ofstream& plik) {
		if (name == "pierwiastek") {
			plik << "\\sqrt[";
			if (t1) {
				t1->print_new(plik);
			}
			plik << "]{";
			if (t2->prioriti > prioriti) {
				plik << "{";
				t2->print_new(plik);
				plik << "}";
			}
			else {
				t2->print_new(plik);
			}
			plik << "}";
		}
		else {
			if (t1) {
				if (t1->prioriti > prioriti) {
					plik << "{";
					t1->print_new(plik);
					plik << "}";
				}
				else {
					t1->print_new(plik);
				}
			}
			plik << "\\text{" << name << "}";
			if (t2) {
				if (t2->prioriti > prioriti) {
					plik << "{";
					t2->print_new(plik);
					plik << "}";
				}
				else {
					t2->print_new(plik);
				}
			}
		}
	}

	void Reduce(std::ofstream& plik) {
		if (t1 != nullptr) {
			t1->Reduce(plik);
		}

		if (t1 != nullptr && t1->name == "mnozeniesymboliczne") {
			// t1 jest mnożeniem, skracaj mnożenie rekurencyjnie
			Multiplication* multiplication = dynamic_cast<Multiplication*>(t1);
			multiplication->Reduce(plik);
			t1 = multiplication;
		}
		if (t1 != nullptr && t1->name == "dodawanie") {
			// t1 jest dodawaniem, skracaj dodawanie rekurencyjnie
			Add* addition = dynamic_cast<Add*>(t1);
			addition->Reduce(plik);
			t1 = addition;
		}

	}

	


	// cos trzeba zmienic 
	Result calculate() {
		Result re1 = t2->calculate();
		Result re2 = t1->calculate();
		Result ww;
		if (re1.liczba < 0)throw Error("liczba pod pierwistkiem mniejsza od zera chyba bez urojonych zabronione");
		else {
			ww.liczba = pow(re1.liczba, 1.0 / re2.liczba);
			ww.poprawna = true;
		}
		return ww;
	}

	~Sqrt() {
		delete t1, t2;
	}
};


// potega

class Power : public Term {
public:
	Term* t1;
	Term* t2;


	Power(Term* t1, Term* t2) : Term(), t1(t1), t2(t2) { name = "potega"; 
	prioriti = 0;
	}
	// wariant Power(Term * t1) jaki t1 ^ 0 = 1 
	Power(Term* t1) : Term(), t1(t1), t2(nullptr) { name = "potega"; }
	Power(const Power& other) : Term(other), t1(new Term(*other.t1)), t2(new Term(*other.t2)) {}
	Power() : Term() { name = "potega"; }
	~Power() { delete t1, t2; }
	bool operator<(const Term& t) const {
		return prioriti < t.prioriti;
	}
	void print() {
		/// <summary>
		/// if (prioritet->t1 > tr
		/// </summary>
		std::cout << "(";
		t1->print();
		std::cout << "^";
		t2->print();
		std::cout << ")";
	}

	/*

	virtual void print_new(std::ofstream& plik) {
		if (t1->prioriti > prioriti) {
			plik << "{";
			t1->print_new(plik);
			plik << "}";
		}
		else {
			t1->print_new(plik);
		}
		plik << "^{";
		if (t2->prioriti > prioriti) {
			plik << "{";
			t2->print_new(plik);
			plik << "}";
		}
		else {
			t2->print_new(plik);
		}
		plik << "}";
	}
	*/

	void print_new(std::ofstream& plik) override {
		bool need_parentheses = false;
		if (t1->prioriti > prioriti || (t1->prioriti == prioriti && dynamic_cast<Power*>(t1))) {
			need_parentheses = true;
		}

		if (need_parentheses) {
			plik << "(";
		}

		t1->print_new(plik);

		if (need_parentheses) {
			plik << ")";
		}

		plik << "^";

		if (t2->prioriti > prioriti) {
			plik << "{";
			t2->print_new(plik);
			plik << "}";
		}
		else {
			t2->print_new(plik);
		}
	}




	void rename(std::string co, std::string na) {
		t1->rename(co, na);
		t2->rename(co, na);
	}

	Result calculate() override {
		Result re1 = t1->calculate();
		Result re2 = t2->calculate();
		Result ww;
		ww.liczba = std::pow(re1.liczba, re2.liczba);
		ww.poprawna = true;
		return ww;
	}

	void Reduce(std::ofstream& plik) {
		if (t1 != nullptr) {
			t1->Reduce(plik);
		}
		if (t2 != nullptr) {
			t2->Reduce(plik);
		}

		Result t1_result = t1->calculate();
		Result t2_result = t2->calculate();

		if (t1_result.poprawna && t2_result.poprawna) {
			double result = std::pow(t1_result.liczba, t2_result.liczba);
			t1 = new RealNumber(result);
			t2 = new RealNumber(1.0);
		}
	}

};




class Var : public Term {
public:
	std::string name;

public:
	Var(std::string n) : Term() { name = n; prioriti = -1;
	}
	Var(const Var& other) : Term(other), name(other.name) { prioriti = -1; }

	void print() {
		std::cout << "(" << name << ")";
	}

	virtual void print_new(std::ofstream& plik)
	{
		plik << "{" << name << "}";
		//plik << name;
	}

	void rename(std::string co, std::string na) {
		if (name == co) name = na;
	}
	bool operator<(const Term& t) const {
		return prioriti < t.prioriti;
	}

	

};





class FormalSeries : public Term {
public:
	Term* indeks_dolny;
	Term* indeks_gorny;
	Term* wyrazenie;
	Var* napis;

public:
	FormalSeries(Term* id, Term* ig, Term* wyrazeniex, Var* nn) : Term() {
		indeks_dolny = id;
		indeks_gorny = ig;
		wyrazenie = wyrazeniex;
		napis = nn;
	}
	void print() {
		std::cout << "\\sum_{";
		napis->print();
		std::cout << " = ";
		indeks_dolny->print();
		std::cout << "}_{";
		indeks_gorny->print();
		std::cout << "} {";
		wyrazenie->print();
		std::cout << "}";
	};



	virtual void print_new(std::ofstream& plik) {
		plik << "\\sum_{";
		napis->print_new(plik);
		plik << " = ";
		indeks_dolny->print_new(plik);
		plik << "}^{";
		indeks_gorny->print_new(plik);
		plik << "}";
		wyrazenie->print_new(plik);
	};


	void rename(std::string co, std::string na) {
		indeks_dolny->rename(co, na);
		indeks_gorny->rename(co, na);
		wyrazenie->rename(co, na);
		//napis->rename(co, na);
	}
	bool operator<(const Term& t) const {
		return prioriti < t.prioriti;
	}

	void  Reduce(std::ofstream& plik) {

	}

};




class Wyznacznik : public Term {
public:
	Term * numerator1;
	Term * denominator1;
	Term * numerator2;
	Term * denominator2;
	Term* numerator3;
	Term* denominator3;


public:
	Wyznacznik(Term* num1, Term* num2, Term* num3 ,  Term * den1 , Term * den2 , Term * den3) {
		numerator1 = num1;
		denominator1 = den1;
		numerator2 = num2;
		denominator2 = den2;
		numerator3 = num3;
		denominator3 = den3;
	

	}

	void print() {
		std::cout << "det[(";
		numerator1->print();
		std::cout << ")/(";
		denominator1->print();
		std::cout << ") + (";
		numerator2->print();
		std::cout << ")/(";
		denominator2->print();
		std::cout << ")]" << std::endl;
	}

	/*
	void print_new(std::ofstream& plik ) {
		plik << "\\begin{df}" << std::endl;
		plik << "\\textit{Macierzą} $A$ nazywamy tablicę" << std::endl;
		plik << "$$A=[\\alpha_{ij}]_{ \\tiny" << std::endl;
		plik << "\\left[\\begin{array}{ccccc}" << std::endl;
		// << "\\alpha_{11}&\\dots &\\alpha_{1i}& \\dots &\\alpha _{1n}\\\\"
			//<< std::endl;
			numerator1->print_new(plik);
		plik << std::endl;
		plik << "\\alpha_{21}&\\dots &\\alpha_{2i}& \\dots &\\alpha_{2n}\\\\"
			<< std::endl;
		plik << "\\vdots & &\\vdots & &\\vdots \\\\" << std::endl;
		plik << "\\alpha_{m1}&\\dots &\\alpha_{mi}& \\dots &\\alpha_{mn}\\\\"
			<< std::endl;
		plik << "\\end{array}\\right]" << std::endl;
		plik << "$$" << std::endl;
		plik << "elementów $\\alpha_{ij}$, $i=1,\\dots ,n$, $j=1,\\dots ,n$." << std::endl;
		plik << "\\end{df}" << std::endl;
		plik << "det\\left[\\left\\{";
		numerator1->print_new(plik);
		plik << "\\right\\}/\\left\\{";
		denominator1->print_new(plik);
		plik << "\\right\\} + \\left\\{";
		numerator2->print_new(plik);
		plik << "\\right\\}/\\left\\{";
		denominator2->print_new(plik);
		plik << "\\right\\}\\right\\}]" ;
	}

	*/


	void print_new(std::ofstream& plik) {
		Term* A = new Var("A");
		Term* B = new Var("B");
		
		plik << "$$ \\\\ ";
		plik << "$$ \\\\ ";
		plik << "$$ \\\\ ";
		plik << "\\underbrace{\\left[\\begin{array}{cccc}" << std::endl;

		numerator1->print_new(plik);
		plik << "&";
		numerator2->print_new(plik);
		plik << "& = ";
		numerator3->print_new(plik);
		plik << "&\\\\\n";
		denominator1->print_new(plik);
		plik << "&";
		denominator2->print_new(plik);
		plik << "& = ";
		denominator3->print_new(plik);
		plik << "& \\\\\n";
	
		plik << "\\end{array}\\right]}_{A(Wyznacznik)} ";



		plik << "\\\\";
		plik << "Wyznacznik obliczeniow y "; 
		plik << "\\\\"; 



		Term* det = new Add();
		Term* det_1 = new Multiplication();
		Term* det_2 = new Multiplication();
		det_1->Add_dodaj(numerator1);
		det_1->Add_dodaj(denominator2);
		det_2->Add_dodaj(numerator2);
		det_2->Add_dodaj(denominator1);
		det->Add_dodaj(det_1);
		det->Add_dodaj(new SubtractU(det_2));
		//det->print_new(plik);

		Term* detx1 = new Add();
		Term* detx2 = new Multiplication();
		Term* detx3 = new Multiplication();

		detx2->Add_dodaj(numerator3);
		detx2->Add_dodaj(denominator2);


		detx3->Add_dodaj(numerator2);
		detx3->Add_dodaj(denominator3);


		detx1->Add_dodaj(detx2);
		detx1->Add_dodaj(new SubtractU(detx3));
		//detx1->Reduce(plik);
		//det -> Reduce(plik);
		//detx2->Reduce(plik);
			
		Term* detx = new Division(detx1 , det);
	


		plik << " \\\\  ";
 		detx->print_new(plik);



		
		Term* dety1 = new Add();
		Term* dety2 = new Multiplication();
		Term* dety3 = new Multiplication();


		dety2->Add_dodaj(numerator1);
		dety2->Add_dodaj(denominator3);


		dety3->Add_dodaj(numerator3);
		dety3->Add_dodaj(denominator1);


		dety1->Add_dodaj(dety2);
		dety1->Add_dodaj(new SubtractU(dety3));


		plik << " \\\\  ";

		Term* dety = new Division(dety1 , det);

		dety->Reduce(plik);
		dety->print_new(plik);


		plik << " \\\\  ";
	
	}


	Result calculate() override {
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
			std::cout << "Układ równań jest sprzeczny lub nieoznaczony." << std::endl;
		}
		else
		{
			std::cout << " c1  =  " << c1.liczba  <<  " b2   = " << b2.liczba << "  b1   = " <<  b1.liczba << "  c2   = " << c2.liczba ;
			
			
			
			
			x = (c1.liczba * b2.liczba - b1.liczba * c2.liczba) / det;
			y = (a1.liczba * c2.liczba - c1.liczba * a2.liczba) / det;
			std::cout << " x = " << x << " y = " << y;
		}
		result.liczba = x;
		result.liczba2 = y;
		return result;
	}

	void rename(std::string co, std::string na) {
		numerator1->rename(co, na);
		denominator1->rename(co, na);
		numerator2->rename(co, na);
		denominator2->rename(co, na);
		//a->rename(co, na);
		//b->rename(co, na);
	}

	~Wyznacznik() {
		delete numerator1, numerator2, denominator1, denominator2 , denominator3 , numerator3; 
	}
};

class Wyznacznik2 : public Term {
public:
	Term* c;
	Term* d;
	Term* z;
	Term* x1;
	Term* x2;
	Term* j;

	Wyznacznik2(Term* C, Term* D, Term* Z, Term* X1, Term* X2, Term* J) : c(C), d(D), z(Z), x1(X1), x2(X2), j(J) {}

	void print() {
		std::cout << "det([";
		c->print();
		std::cout << " ";
		d->print();
		std::cout << " ";
		z->print();
		std::cout << "]" << std::endl;
		std::cout << "    [";
		x1->print();
		std::cout << " ";
		x2->print();
		std::cout << " ";
		j->print();
		std::cout << "])" << std::endl;
	}


	void print_new(std::ofstream& plik) {
	
		Term* W_m1 = new Multiplication();
		W_m1->Add_dodaj(c);
		W_m1->Add_dodaj(x2);
		Term* W_m2 = new Multiplication();
		W_m2->Add_dodaj(d);
		W_m2->Add_dodaj(x1);
		Term* W = new Subtrac(W_m1 , W_m2 );
	
		

	
	}


	Result calculate() override {
		Result r_c = c->calculate();
		Result r_d = d->calculate();
		Result r_z = z->calculate();
		Result r_x1 = x1->calculate();
		Result r_x2 = x2->calculate();
		Result r_j = j->calculate();

		Result W = r_c.liczba * r_x2.liczba  - r_d.liczba * r_x1.liczba; 
		std::cout << W.liczba << "\n";
		std::cout << r_c.liczba << " " << r_x2.liczba << " " << r_d.liczba << " " << r_x1.liczba;

		// Wyznacznik Wc
		Result Wc = r_z.liczba * r_x2.liczba - r_d.liczba * r_j.liczba;
		std::cout << Wc.liczba << "\n";
		// Wyznacznik Wd
		Result Wd = r_c.liczba * r_j.liczba - r_z.liczba * r_x1.liczba;
		std::cout << Wd.liczba << "\n";

		Result r;
		r.liczba = Wc.liczba / W.liczba;
		r.liczba2 = Wd.liczba / W.liczba;
		// Zwróć wynik
		return r;
	}

};




class VarIndex : public Term {
public:
	Term* num;
	Term* Index;
public:
	VarIndex(Term* indexTerm, Term* numTerm) : Term() {
		name = "Wyrazenie z Indeksem";
		Index = indexTerm;
		num = numTerm;
	}


	//VarIndex(const VarIndex& other) : Term(other), Index(new Term(*other.Index)), num(new Term(*other.num)) {}

	virtual void print_new(std::ofstream& plik)
	{
		plik << "{";
		Index->print_new(plik);
		plik << "}_{";
		num->print_new(plik);
		plik << "}";
	}

	void print() {
		Index->print();
		std::cout << "_{";
		num->print();
		std::cout << "}";
	}


	void printLatex(std::ofstream& plik) {
		std::string index_str;
		std::string num_str;

		// JeĹ›li wyraĹĽenie indeksowe to staĹ‚a, to przepisz wartoĹ›Ä‡ do Ĺ‚aĹ„cucha znakĂłw
		if (RealNumber* c = dynamic_cast<RealNumber*>(Index)) {
			// index_str = c->getValueAsString();
		}
		// W przeciwnym przypadku zapisz wyraĹĽenie indeksowe do Ĺ‚aĹ„cucha znakĂłw w formacie LaTeX
		else {
			index_str = "\\left(";
			// Index->printLatex(plik);
			index_str += "\\right)";
		}

		// JeĹ›li wyraĹĽenie numeryczne to staĹ‚a, to przepisz wartoĹ›Ä‡ do Ĺ‚aĹ„cucha znakĂłw
		if (RealNumber* c = dynamic_cast<RealNumber*>(num)) {
			//num_str = c->RealNumber ();
		}
		// W przeciwnym przypadku zapisz wyraĹĽenie numeryczne do Ĺ‚aĹ„cucha znakĂłw w formacie LaTeX
		else {
			num_str = "\\left(";
			//num->printLatex(plik);
			num_str += "\\right)";
		}

		// Zapisz indeks z dolnym indeksem do pliku w formacie LaTeX
		plik << "x_{" << index_str << "-" << num_str << "}";
	}

	~VarIndex() {
		delete Index, num;
	}
};
class Parentheses : public Term {
public:
	Parentheses(Term* t) : Term(), term(t) { prioriti = -1; }
	virtual void print_new(std::ofstream& plik) {
		plik << "(";
		term->print_new(plik);
		plik << ")";
	}
	void print() {
		std::cout << "(";
		term->print();
		std::cout << ")";
	}

public:
	Term* term;

	Result calculate() override {
		Result ret = term->calculate();
		return ret;
	}
};


class Equal : public Term {
public:
	Equal(Term* l, Term* r) : Term(), lhs(l), rhs(r) {}
	virtual void print_new(std::ofstream& plik)
	{
		lhs->print_new(plik);
		plik << " = ";
		rhs->print_new(plik);


	}
	void print() {
		lhs->print();
		std::cout << " = ";
		rhs->print();
	}

public:
	Term* lhs;
	Term* rhs;
};










Term* ZwrocmyGlownySZereguuzywajacstosu(FormalSeries* A) {
	std::stack<Term*> stos; // Tworzenie stosu z wskaźnikami na obiekty typu Term

	// Dodawanie elementów na stos
	stos.push(A);


	// Odczytywanie elementu z wierzchołka stosu
	Term* glownyElement = stos.top();

	// Usuwanie elementów ze stosu
	stos.pop();


	// Sprawdzanie, czy stos jest pusty
	if (stos.empty()) {
		std::cout << "Stos jest pusty." << std::endl;
	}

	return glownyElement;
}






void liczszereg(Term* A, Term* B, Term* Z, Term* J, std::ofstream& plik) {
	
	plik << " \n \n $$  Rownanie  Rekurencyjne  $$ $$ ";
	//   <<<<<<<<<<<<<  Rownanie do obliczenia >>>>>>





	Term* x_n = new VarIndex(new Var("x"), new Var("n"));
	Term* x_n_mniej_jeden = new VarIndex(new Var("x"), new Var("n-1"));
	Term* x_n_mniej_dwa = new VarIndex(new Var("x"), new Var("n-2"));
	Term* pierwszy = new Multiplication(new VarIndex(new Var("a"), new RealNumber(0)), new Power(new Var("x"), new RealNumber(0)));
	Term* drugi = new Multiplication(new VarIndex(new Var("a"), new RealNumber(1)), new Power(new Var("x"), new RealNumber(1)));
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
	azeroxzero->Add_dodaj(new VarIndex(new Var("a"), new Var("0")));
	ajedenxjeden->Add_dodaj(new Power(new Var("x"), new Var("0")));

	Term* BetterAdd = new Add();
	Term* BetterMultiply1 = new Multiplication();
	Term* BetterMultiply2 = new Multiplication();
	Term* BetterAddsmall = new Add();
	BetterMultiply1->Add_dodaj(A);
	BetterMultiply1->Add_dodaj(new Var("x"));
	BetterAddsmall->Add_dodaj(new Var("f(x)"));
	BetterAddsmall->Add_dodaj(new SubtractU(new VarIndex(new Var("a") , new Var("0"))));
	BetterMultiply1->Add_dodaj(new Parentheses(BetterAddsmall));
	BetterMultiply2->Add_dodaj(B);
	BetterMultiply2->Add_dodaj(new Power(new Var("x") , new Var("2")));
	BetterMultiply2->Add_dodaj(new Var("f(x)"));
	BetterAdd->Add_dodaj(BetterMultiply1);
	BetterAdd->Add_dodaj(BetterMultiply2);


	Term* tempg = new Multiplication();
	tempg->Add_dodaj(A);
	tempg->Add_dodaj(new Var("x"));
	tempg->Add_dodaj(new Var("f(x)"));
	



	Term* BetterAddlewe = new Add();
	BetterAddlewe->Add_dodaj(new Var("f(x)"));
	BetterAddlewe->Add_dodaj(new SubtractU(tempg));
	BetterAddlewe->Add_dodaj(new SubtractU(	BetterMultiply2));









	// Declaration and initialization of VarIndex term
	Term* varIndexTerm = new Multiplication(new VarIndex(new Var("a"), new Var("n")), new Power(new Var("x"), new Var("n")));

	// Declaration and initialization of FormalSeries object
	Term* FunkcjaTworzaca = new FormalSeries(new Var("2"), new Var("\\infty"), varIndexTerm, new Var("n"));



	Term* aa = B;
	Term* bb = new SubtractU(new Multiplication(Z, J));
	Term* cc = new RealNumber(1);



	Add* wiel = new Add();
	Term* aaa = new SubtractU(B); // Odejmowanie: A - B
	Term* bbb = new SubtractU(new Multiplication(Z, J)); // Odejmowanie: Z * J - B
	Term* ccc = new RealNumber(1); // Liczba rzeczywista: 1 
	Term* dd = new RealNumber(0); // Liczba rzeczywista: 0

	Term* ee = new Multiplication(aa, bb); // Mnożenie: (A + B) * (C * D)
	Term* ff = new Subtrac(ee, cc); // Odejmowanie: (A + B) * (C * D) - (1 - B)
	Term* gg = new Add(ff, dd); // Dodawanie: (A + B) * (C * D) - (1 - B) + 0

	Term* aaaa = new Power(A, B); // Potęgowanie: A^B
	Term* bbbb = new Multiplication(A, Z); // Mnożenie: A * C
	Term* cccc = new Subtrac(new Subtrac(aaaa, bbbb), new RealNumber(1)); // Odejmowanie: (A^B - A*C) - 1
	Term* dddd = new Add(cccc, ff); // Dodawanie: (A^B - A*C) - 1 + 0
	
	wiel->Add_dodaj(aaa);
	wiel->Add_dodaj(bbb);
	wiel->Add_dodaj(ccc);

	plik << "*******";
	wiel->print_new(plik);
	plik << "*******";
	gg->print_new(plik);
	plik << "*******";
	dddd->print_new(plik);

	Term* temptemp = new Subtrac(new Var("1"), pierwszemnozenie_a_temp_HELP);
	Term* temptemptemp = new Subtrac(temptemp, new Multiplication(J, new Var("x")));
	Term* temptemptemptemp = new Subtrac(temptemptemp, drugiemnozenie_a_temp_HELP_2);
	Term* lewe_mnozenie = new Multiplication(new Var("f(x)"), temptemptemptemp);

	//Term* prawe_dodawaie = new Add(new Multiplication(Z, new Power(new Var("x"), new Var("0"))), new Multiplication(pierwszemnozenie_a_temp_HELP, Z));
	Term* prawe_dodawanie = new Add();
	Term* Ferdek = new Multiplication();
	Term* pazdzioch = new Add();
	Term* boczek = new Multiplication();
	boczek->Add_dodaj(new SubtractU(A));
	boczek->Add_dodaj(new VarIndex(new Var("a"), new Var("0")));
	boczek->Add_dodaj(new Power(new Var("x"), new Var("0")));
	pazdzioch->Add_dodaj(new VarIndex(new Var("a") , new Var("1")));
	pazdzioch->Add_dodaj(boczek);
	Ferdek->Add_dodaj(new Var("x"));
	Ferdek->Add_dodaj(new Parentheses(pazdzioch));
	prawe_dodawanie->Add_dodaj(new VarIndex(new Var("a"), new Var("0")));
	prawe_dodawanie->Add_dodaj(Ferdek);








	Term* lewy = new Multiplication();
	Term* MagentaAdd = new Add();
	Term* MagentasmallMult1 = new Multiplication();
	Term* MagentasmallMult2 = new Multiplication();
	Term* MagentasmallMult3 = new Multiplication();
	
	MagentasmallMult1->Add_dodaj(A);
	MagentasmallMult1->Add_dodaj(new Var("x"));
	

	MagentasmallMult2->Add_dodaj(J);
	MagentasmallMult2->Add_dodaj(new Var("x"));

	MagentasmallMult3->Add_dodaj(B);
	MagentasmallMult3->Add_dodaj(new Power (new Var("x") , new Var("2")));
	

	// Dodałęm twarde nawiasy 
	MagentaAdd->Add_dodaj(new Var("1"));


	MagentaAdd->Add_dodaj(new SubtractU(MagentasmallMult1));
	//MagentaAdd->Add_dodaj(new SubtractU(MagentasmallMult2));
	MagentaAdd->Add_dodaj(new SubtractU(MagentasmallMult3));
	


	lewy->Add_dodaj(new Var("f(x)"));
	lewy->Add_dodaj(new Parentheses( MagentaAdd));


	Term* ulamek = new Division(prawe_dodawanie, lewy);
	




	plik << "$$ \n  \n \n  $$   ";

	equation->print_new(plik);
	plik << "$$ \n  \n \n  $$ ";
	plik << "$$ \n \n \n $$";


	Term* nextequation = new Add(PD, FunkcjaTworzaca);

	plik << "f(x) = ";
	nextequation->print_new(plik);
	plik << "$$ \n  \n \n  $$    ";
	plik << "$$ \n  \n \n  $$   ";

	plik << "f(x) = ";




	// rowwnanie po f(x)
	next->print_new(plik);
	plik << "$$ \n \n \n $$ ";
	// nizej 1 f(x)
	Term* next1 = new Add(PD, SILLL);
	plik << "f(x) = ";
	next1->print_new(plik);
	



	// nizej 2 f(x)
	plik << "$$ \n  \n \n  $$    ";
	// nizej 3 f(x)
	
	Term* ASDZ = new Add(PD, SILCOMBO);
	plik << "f(x) = ";
	ASDZ->print_new(plik);
	//plik << "$$ \n  \n \n  $$     ";



	
	
	//PD->print_new(plik);
	//plik << "$$ \n  \n \n  $$    ";
	//SIK->print_new(plik);
	
	
	Term* next2 = new Add(PD, SIK);
	//next2->print_new(plik);
	plik << " $$\n \n \n $$		";
	Term* next3 = new Add(PD, SIE);

	// nizej 4 ????
	plik << "f(x) = ";
	next3->print_new(plik);
	plik << " $$\n  $$		";
	//plik << "f(x) = ";
	
	//PD->print_new(plik);
	//plik << " $$  \n  $$ + \n  ";
	//SIE->print_new(plik);
	plik << "$$ \n $$  f(x) = ";
	Term* next4 = new Add(PD, BetterAdd);

	next4->print_new(plik);
	plik << "$$ \n   $$  ";
	//PD->print_new(plik);
	//plik << "$$ \n  $$  + ";

	plik << "$$ \n   $$  ";
	BetterAddlewe->print_new(plik);
	plik << "  =  "; 



	Term* tempi = new Multiplication();
	tempi->Add_dodaj(A);
	tempi->Add_dodaj(new Var("x"));
	tempi->Add_dodaj(new SubtractU(new VarIndex(new Var("a"), new Var("0"))));
	//tempi->print_new(plik);

	Term* tempv = new Add();
	tempv->Add_dodaj(PD);
	tempv->Add_dodaj(tempi);
	tempv->print_new(plik);



	plik << "$$ \n $$  ";
//	SIEWIE->print_new(plik);




	plik << "$$ \n $$  ";
	lewy->print_new(plik);

	plik << " = ";
	
	
	
	prawe_dodawanie->print_new(plik);// jest ok prawe dodawanie  
	plik << "$$ \n  \n \n  $$ f(x) =    ";
	ulamek->print_new(plik);



	delete lewe_mnozenie, temptemp, temptemptemp, temptemptemptemp, prawe_dodawanie, ulamek;
	delete pierwszemnozenie_b, drugiemnozenie_b, SIK ;
	delete pierwszemnozenie_a, drugiemnozenie_a;
	delete SIK, next3;
	//delete  SILLLLL;
	delete next1;
	delete pierwszemnozenie, drugiemnozenie, coscos;
	delete next;
	delete x_n, x_n_mniej_jeden, x_n_mniej_dwa;
	delete IL, SILLL;
	delete SIL, SILL;
	delete pierwszy, drugi, PD, cos, next2;
	delete SILLLL, SILLLL2, SILCOMBO;
	delete nextequation, ulamek, varIndexTerm, FunkcjaTworzaca;

	// pomocnicze !!!!!!!usun pamitaj
	delete pierwszemnozenie_a_temp_HELP;
	delete drugiemnozenie_a_temp_HELP;

	delete pierwszemnozenie_a_temp_HELP_2;
	delete drugiemnozenie_a_temp_HELP_2;
}





void liczszeregVIP (Term* A, Term* B, Term* ZERO, Term* JEDEN, std::ofstream& plik) {	
	Term* prawe_dodawanie = new Add();
	Term* Ferdek = new Multiplication();
	Term* pazdzioch = new Add();
	Term* boczek = new Multiplication();
	boczek->Add_dodaj(new SubtractU(A));
	boczek->Add_dodaj(new VarIndex(new Var("a"), new Var("0")));
	boczek->Add_dodaj(new Power(new Var("x"), new Var("0")));
	pazdzioch->Add_dodaj(new VarIndex(new Var("a"), new Var("1")));
	pazdzioch->Add_dodaj(boczek);
	Ferdek->Add_dodaj(new Var("x"));
	Ferdek->Add_dodaj(new Parentheses(pazdzioch));
	prawe_dodawanie->Add_dodaj(new VarIndex(new Var("a"), new Var("0")));
	prawe_dodawanie->Add_dodaj(Ferdek);
	plik << " f(x) =  \\n";
	Term* Ax = new Multiplication(A,    new Var("x"));
	//new VarIndex(new Var("a"), new Var("1")) 
	Term* A_jeden_razy_x = new Multiplication(JEDEN, new Var("x"));
	Term* B_razy_x_do_kwadratu = new Multiplication( B , new Power(new Var("x"), new Var("2")));
 


	// TErm up of DIVISIOn
	Term* TEMP = new Multiplication(A, new Var("x"));

	Term* up_gold_of_fract = new Add();
	up_gold_of_fract->Add_dodaj(ZERO);
	up_gold_of_fract->Add_dodaj(new Multiplication(TEMP , ZERO));


	//   DOWn of ulamek 
	Term* down_gold_of_fract = new Add();
	down_gold_of_fract->Add_dodaj(new Var("1"));
	down_gold_of_fract->Add_dodaj(new SubtractU(Ax));
	//down_gold_of_fract->Add_dodaj(new SubtractU(A_jeden_razy_x));
	down_gold_of_fract->Add_dodaj(new SubtractU(B_razy_x_do_kwadratu));
	
	
	// up , down in division to plik 
	//down_gold_of_fract->print_new(plik);


	Term* ulamek = new Division( prawe_dodawanie , down_gold_of_fract );
	ulamek->print_new(plik);

	plik << "  $$ \n \n  $$   ";


	// Term* temp = new RealNumber();
	Term* downDWA = new Add();
	downDWA->Add_dodaj(new SubtractU(B_razy_x_do_kwadratu));
	downDWA->Add_dodaj(new SubtractU(Ax));
	downDWA->Add_dodaj(new RealNumber(1));

	Term* prawe_dodawanie_u = new Add();
	Term* Ferdek_u = new Multiplication();
	Term* pazdzioch_u = new Add();
	Term* boczek_u = new Multiplication();
	boczek_u->Add_dodaj(new SubtractU(A));
	boczek_u->Add_dodaj(ZERO);
	boczek_u->Add_dodaj(new RealNumber(1));
	pazdzioch_u->Add_dodaj(JEDEN);
	pazdzioch_u->Add_dodaj(boczek_u);
	Ferdek_u->Add_dodaj(new Var("x"));
	Ferdek_u->Add_dodaj(new Parentheses(pazdzioch_u));
	prawe_dodawanie_u->Add_dodaj(ZERO);
	prawe_dodawanie_u->Add_dodaj(Ferdek_u);
	plik << " f(x) =  \\n";



	Term* uporzadkowany_ulamek = new Division(prawe_dodawanie_u, downDWA);
	uporzadkowany_ulamek->print_new(plik);



	Term* utr = new Add();
	utr->Add_dodaj(A);

	Term* aa = new SubtractU(B); 
	Term* bb = new SubtractU (utr);
	Term* cc = new RealNumber(1) ; 


	Term* MinusCzteryarazyc = new Multiplication(); 
	MinusCzteryarazyc->Add_dodaj(new SubtractU(new RealNumber(4)));
	MinusCzteryarazyc->Add_dodaj(aa);
	MinusCzteryarazyc->Add_dodaj(cc);
	
	
	Term* Delta = new Add();
	Delta->Add_dodaj(new Power(bb, new RealNumber(2)));
	Delta->Add_dodaj(MinusCzteryarazyc);
	Delta->print_new(plik);
	Delta->Reduce(plik);
	Result wynikdelty = Delta->calculate();
	plik << " $$ \\n \\n  \\Delta = " << wynikdelty.liczba;
	Term* minusBEdodacsqrtDELTA = new Add();
	minusBEdodacsqrtDELTA->Add_dodaj(new SubtractU(bb));
	minusBEdodacsqrtDELTA->Add_dodaj(new Sqrt( new RealNumber(2), Delta ));
	
	Term* x1 = new Division(minusBEdodacsqrtDELTA, new Multiplication(new RealNumber(2) , aa));

	x1->Reduce(plik);
	Result wynikx1 = x1->calculate();
	plik << " $$ $$  \\n \\n  $$ $$   $$\\n  \\\\ $$            x1  = " << wynikx1.liczba << " = ";
	


	x1->print_new(plik);
	plik << " $$ $$ ";

	Term * minusBEodjacsqrtDELTA = new Add();
	minusBEodjacsqrtDELTA->Add_dodaj(new SubtractU(bb));
	minusBEodjacsqrtDELTA->Add_dodaj(new SubtractU(new Sqrt( new RealNumber(2)  , Delta )));
	Term* x2 = new Division(minusBEodjacsqrtDELTA, new Multiplication(new RealNumber(2), aa)); 
	plik << " \\\\ x2 przed skroceniem  \\\\ ";
	x2->print_new(plik);
	x2->Reduce(plik);
	Result wynikx2 = x2->calculate();
	plik << "  $$   $$\\n  \\\\ $$  x2 =  " << wynikx2.liczba <<  "  =  " ;
	x2->print_new(plik); plik  << "   $$ $$ ";


	plik << "  $$ \n \n  $$   ";

	plik << "  $$ \n \n  $$   ";



	Term* fantasy_real_A = new Division(new Var("A"), new Add(new Var("x"), new SubtractU(x1)));
	Term* fantasy_real_B = new Division(new Var("B"), new Add(new Var("x"), new SubtractU(x2)));
	Term* VIPdodawanie = new Add();
	
	VIPdodawanie->Add_dodaj(fantasy_real_A);
	VIPdodawanie->Add_dodaj(fantasy_real_B);



	prawe_dodawanie_u->print_new(plik);
	plik << " = "; 
	VIPdodawanie->print_new(plik);


	// dotad ulamek ten z A   i    B  


	// <<< !!! tutaj aa A bb B zeby do wyswietlania widziec jak cos 
	//Term* pl = new Multiplication( new Var("aa"), new Var("A"));
	//Term* pk = new Multiplication(new Var("bb"), new Var("B"));
		Term* pl = new Multiplication( aa, A);
	Term* pk = new Multiplication(bb,B);
	Term* pz = new Add();
	pz->Add_dodaj(pl);
	pz->Add_dodaj(pk);
	Term* px  = new Multiplication();
	px->Add_dodaj(new Var("x"));
	px->Add_dodaj(pz);
	px->Add_dodaj(new Var("x"));
	plik << " $$  \\\\ $$ $$  ";
	prawe_dodawanie_u->print_new(plik);
	plik << " = ";
	px->print_new(plik);

	plik << " $$  \\\\ $$ $$  ";
	Term* pxx = new Multiplication();
	//popraw
	Term* pll = new Multiplication(aa, A);
	Term* pkk = new Multiplication(bb, B);
	Term* pzz = new Add();
	pzz->Add_dodaj(pll);
	pzz->Add_dodaj(pkk);
	pxx->Add_dodaj(pzz);
	pxx->Add_dodaj(new Var("x"));

		plik << " $$  \\
		\\n $$ $$  ";
	plik << " $$  \\\\ \\\\ \\\\ $$ $$  ";
		
	

	Term* RiskAdd = new Add();
	Term* RiskMult = new Multiplication();
	RiskMult->Add_dodaj(ZERO);
	RiskMult->Add_dodaj(A);
	RiskAdd->Add_dodaj(ZERO);
	RiskAdd->Add_dodaj(RiskMult);

	RiskAdd->print_new(plik);
	plik << " = ";
	pxx->print_new(plik);

	Term* temp = new Add();
	Term* temp1 = new Multiplication();
	temp->Add_dodaj(new Multiplication(aa , A));
	temp->Add_dodaj(new Multiplication(aa, B));
	temp1->Add_dodaj(new Var("x"));
	temp1->Add_dodaj(temp);

	plik << "\\\\";
	prawe_dodawanie_u->print_new(plik);
	plik << " = ";
	temp1->print_new(plik);




	// wyznacznik 



	Term* num1 = new Multiplication();
	num1->Add_dodaj(aa);
	//num1->Add_dodaj(new RealNumber(wynikx1.liczba));
	num1->Add_dodaj(x1);
	num1->Add_dodaj(A);
	

	Term* unum1 = new SubtractU(num1);



	Term* num2 = new Multiplication();
	num2->Add_dodaj(aa);
	num2->Add_dodaj(x2);
	num2->Add_dodaj(B);
	Term* unum2 = new SubtractU(num2);

	Term* num3 = new Multiplication();
	num3->Add_dodaj(ZERO);

	Term* den1 = new Multiplication();
	den1->Add_dodaj(aa);
	den1->Add_dodaj(new Var("A"));

	Term* den2 = new Multiplication();
	den2->Add_dodaj(aa);
	den2->Add_dodaj(new Var("B"));

	Term* den3 = new Add();
	Term* den3help = new Multiplication();
	den3help->Add_dodaj(JEDEN);
	den3help->Add_dodaj(ZERO);
	den3->Add_dodaj(JEDEN);
	den3->Add_dodaj(new SubtractU(den3help));
	


	


	Term* my_wyznacznik = new Wyznacznik(unum1, unum2 , num3 ,  den1 , den2 , den3);
	my_wyznacznik->print_new(plik);





	Term* num11 = new Multiplication();
	num11->Add_dodaj(aa);
	num11->Add_dodaj(x1);




	Term* unum11 = new SubtractU(num11);



	Term* num22 = new Multiplication();
	num22->Add_dodaj(aa);
	num22->Add_dodaj(x2);
	
	Term* unum22 = new SubtractU(num22);

	Term* num33 = new Multiplication();
	num33->Add_dodaj(ZERO);

	Term* den11 = new Multiplication();
	den11->Add_dodaj(aa);



	Term* den22 = new Multiplication();
	den22->Add_dodaj(aa);

	

	Term* den33 = new Multiplication();
	den33->Add_dodaj(A);
	den33->Add_dodaj(ZERO);






	Term* my_wyznacznik1 = new Wyznacznik(unum11, unum22, num33, den11, den22, den3);
	my_wyznacznik1->print_new(plik);

	plik << "  $$ \\n $$  W1 =  ";
	plik << my_wyznacznik1->calculate().liczba;

	plik << "  $$ \\n $$  W2  =  ";
	plik << my_wyznacznik1->calculate().liczba2;


	Term* my_wyznacznik3 = new Wyznacznik(new RealNumber(7), new RealNumber(2), new RealNumber(1), new RealNumber(3), new RealNumber(4), new RealNumber(2));
	my_wyznacznik3->print_new(plik);


	//   Wyswiwtlanie A i B 
	
	plik << "  A ";
	plik << my_wyznacznik3->calculate().liczba;

	plik << " B  ";
	plik << my_wyznacznik3->calculate().liczba2;
	

	// ostateczne wyswietlanie 


	plik << " $$ \\\\  $$ $$ \\\\$$  $$ \\\\ $$ $$\\\\ $$ " << "  $$ f(x) =  ";


	Term* ExtremTERMznanyjako_A_ = new RealNumber(my_wyznacznik1->calculate().liczba2);
	Term* ExtremTERMznanyjako_B_ = new RealNumber(my_wyznacznik1->calculate().liczba);
	Term* ExtremTermznajak_x1 = new RealNumber(wynikx1.liczba);
	Term* ExtremTermznajak_x2 = new RealNumber(wynikx2.liczba);
	


	Term* TempDodawanie = new Add();
	//TempDodawanie->Add_dodaj(new Var("x"));
	//TempDodawanie->Add_dodaj(new SubtractU(ExtremTermznajak_x1));


	Term* MonsterAdd = new Add();
	MonsterAdd->Add_dodaj(new Division(ExtremTERMznanyjako_A_,TempDodawanie ));

	//MonsterAdd->print_new(plik);






	// MonsterAddimprove  IMPROVE    







	Term* TempDodawanieimprove = new Add();
	TempDodawanieimprove->Add_dodaj(new Var("x"));
	TempDodawanieimprove->Add_dodaj(new SubtractU(new VarIndex(new Var("x") , new Var("1"))));



	Term* TempDodawanieimprove2 = new Add();
	TempDodawanieimprove2->Add_dodaj(new Var("x"));
	TempDodawanieimprove2->Add_dodaj(new SubtractU(new VarIndex(new Var("x"), new Var("2"))));


	Term* MonsterAddimprove = new Add();
	MonsterAddimprove->Add_dodaj(new Division(new Var("A"), TempDodawanieimprove));
	MonsterAddimprove->Add_dodaj(new Division(new Var("B"), TempDodawanieimprove2));


	MonsterAddimprove->print_new(plik);



	Term * XprzezX1 = new Power(new Division(new Var("x") , new VarIndex( new Var("x")  , new Var("1")))  , new Var("n"));
	Term* szeregend_ = new FormalSeries(new Var("0") , new Var("\\infty") , XprzezX1, new Var("n"));

	Term* XprzezX1_dwa = new Power(new Division(new Var("x"), new Power( new Var("x"), new Var("2"))), new Var("n"));
	Term* szeregend__dwa = new FormalSeries(new Var("0"), new Var("\\infty"), XprzezX1_dwa, new Var("n"));


	Term* MultiplyFantasy = new Multiplication();
	MultiplyFantasy->Add_dodaj(new Division(new Var("A") , new SubtractU (new VarIndex(new Var("x") ,  new Var("1")))));
	MultiplyFantasy->Add_dodaj(szeregend_);


	Term* MultiplyFantasy2 = new Multiplication();
	MultiplyFantasy2->Add_dodaj(new Division(new Var("B"), new SubtractU(new VarIndex(new Var("x"), new Var("2")))));
	MultiplyFantasy2->Add_dodaj(szeregend__dwa);

	Term* AddFinal = new Add();
	AddFinal->Add_dodaj(MultiplyFantasy);
	AddFinal->Add_dodaj(MultiplyFantasy2);


	plik << " $$  \\\\ $$ $$  ";



	plik << " $$  \\\\ $$ $$  ";
	AddFinal->print_new(plik);



	Term* RexonaMultiply = new Multiplication();
	RexonaMultiply->Add_dodaj(new Division(new Var("A" )  , new SubtractU(new VarIndex(new Var("x") , new Var("1")))));
	RexonaMultiply->Add_dodaj(new Division(new Power (new Var("x")  , new Var("n")), new SubtractU( new Power (new VarIndex(new Var("x"), new Var("1")) , new Var("n")))));



	Term* RexonaMultiply2 = new Multiplication();
	RexonaMultiply2->Add_dodaj(new Division(new Var("B"), new SubtractU(new VarIndex(new Var("x"), new Var("2")))));
	//RexonaMultiply2->Add_dodaj(new Division(new Power(new Var("x"), new Var("n")), new SubtractU(new Power(new VarIndex(new Var("x"), new Var("2")), new Var("n")))));



	Term* DarknessAdd = new Add();
	Term* DarknessMultipl1 = new Multiplication();
	Term* DarknessMultipl2 = new Multiplication();

	DarknessMultipl1->Add_dodaj(new Division(ExtremTERMznanyjako_A_ , new SubtractU(new RealNumber(wynikx1.liczba))));
	DarknessMultipl1->Add_dodaj(new Division(new Var("x") ,  new VarIndex(new Var("x"), new Var("1"))));

	DarknessMultipl2->Add_dodaj(new Division(ExtremTERMznanyjako_B_, new SubtractU(new RealNumber(wynikx2.liczba))));
	DarknessMultipl2->Add_dodaj(new Power (new Division(new VarIndex(new Var("x"), new Var("1")), new Power(new Var("x"), new Var("2"))) , new Var("n")));

	//DarknessAdd->Add_dodaj(DarknessMultipl1);
	//DarknessAdd->Add_dodaj(DarknessMultipl2);
	plik << " $$  \\\\ $$ $$  ";
	

	plik << " $$  \\\\ $$ $$  ";
	//DarknessAdd->print_new(plik);

	Term* FF1 = new FormalSeries(new Var("0") , new Var("\\infty") , new Power ( DarknessMultipl1 , new Var("n")), new Var("n"));
	Term* FF2 = new FormalSeries(new Var("0"), new Var("\\infty"), DarknessMultipl2, new Var("n"));

	plik << " $$  \\\\ $$ $$  ";

	plik << " $$  \\\\ $$ $$  ";
	DarknessAdd->Add_dodaj(FF1);
	DarknessAdd->Add_dodaj(FF2);
	DarknessAdd->print_new(plik);






	Term* FINAL_BIG_Mult = new Multiplication();
	Term* FINAL_ADD = new Add();
	Term* FINAL_SMALL_MULT_1_ = new Multiplication();
	Term* FINAL_SMALL_MULT_2_ = new Multiplication();





	FINAL_SMALL_MULT_1_->Add_dodaj(new Division(new Var("A") , new SubtractU( new VarIndex (new Var("x"), new Var("1")))));
	FINAL_SMALL_MULT_1_->Add_dodaj(new Division(new Power(new Var("x")  , new Var("n")), new Power(new VarIndex(new Var("x"), new Var("1")), new Var("n"))));




	FINAL_SMALL_MULT_2_->Add_dodaj(new Var("B"));
	FINAL_SMALL_MULT_2_->Add_dodaj(new Division(new Var("1"), new Power(new VarIndex(new Var("x"), new Var("2")), new Var("n"))));

	FINAL_ADD->Add_dodaj(FINAL_SMALL_MULT_1_);
	FINAL_ADD->Add_dodaj(FINAL_SMALL_MULT_1_);


	FINAL_BIG_Mult->Add_dodaj(FINAL_ADD);
	FINAL_BIG_Mult->Add_dodaj(new Power(new Var("x") , new Var("n")));


	Term* Wzor = new FormalSeries(new Var("0") , new Var("\\infty")   , FINAL_BIG_Mult , new Var("n"));
	plik << " $$  \\\\ $$ $$  ";

	plik << " $$  \\\\ $$ $$  ";
	Wzor->print_new(plik);
	



	// Ostateczny wynik 
//	Result resultpo = ptga->calculate();
//	std::cout << resultpo.liczba;

	Term* AprzezX1 = new Division(ExtremTERMznanyjako_A_ , new SubtractU(new RealNumber(wynikx1.liczba)));
	Result result = AprzezX1->calculate();


	Term* AprzezX2 =  new Division(ExtremTERMznanyjako_B_, new SubtractU(new RealNumber(wynikx2.liczba)));
	Result result2 = AprzezX2->calculate();



	Term* FINAL_BIG_Mult_f = new Multiplication();
	Term* FINAL_ADD_f = new Add();
	Term* FINAL_SMALL_MULT_1_f = new Multiplication();
	Term* FINAL_SMALL_MULT_2_f = new Multiplication();





	FINAL_SMALL_MULT_1_f->Add_dodaj(new RealNumber(result.liczba));
	FINAL_SMALL_MULT_1_f->Add_dodaj(new Division(new Var("1"), new Power(new VarIndex(new Var("x"), new Var("1")), new Var("n"))));




	FINAL_SMALL_MULT_2_f->Add_dodaj(new RealNumber(result2.liczba));
	FINAL_SMALL_MULT_2_f->Add_dodaj(new Division(new Var("1"), new Power(new VarIndex(new Var("x"), new Var("2")), new Var("n"))));

	FINAL_ADD_f->Add_dodaj(FINAL_SMALL_MULT_1_f);
	FINAL_ADD_f->Add_dodaj(FINAL_SMALL_MULT_2_f);


	FINAL_BIG_Mult_f->Add_dodaj(FINAL_ADD_f);
	FINAL_BIG_Mult_f->Add_dodaj(new Power(new Var("x"), new Var("n")));


	Term* Wzor_f = new FormalSeries(new Var("0"), new Var("\\infty"), FINAL_BIG_Mult_f, new Var("n"));
	plik << " $$  \\\\ $$ $$  ";

	plik << " $$  \\\\ $$ $$  ";
	Wzor_f->print_new(plik);



		 

	delete TempDodawanieimprove, MonsterAddimprove;
	delete MonsterAdd, ExtremTermznajak_x1, ExtremTermznajak_x2, ExtremTERMznanyjako_A_, ExtremTERMznanyjako_B_;

	delete temp , temp1 , Ax, pl  , pk , pz , px ,  down_gold_of_fract , A_jeden_razy_x , B_razy_x_do_kwadratu , up_gold_of_fract , ulamek , aa , bb , cc , Delta , MinusCzteryarazyc;
	delete x1 , minusBEdodacsqrtDELTA , x2 , minusBEdodacsqrtDELTA , fantasy_real_A ,fantasy_real_B, VIPdodawanie;
}


void liczMetodePrzewidywan(Term* A, Term* B, Term* Z, Term* J, std::ofstream& plik) {
	plik << "\ $$  Metoda Przewidywan  $$ ";
	//   <<<<<<<<<<<<<  Rownanie do obliczenia >>>>>>
	Term* x_potega_n = new Power(new Var("x"), new Var("n"));
	Term* FancyAdd = new Add();
	Term* FancyMult1 = new Multiplication();
	Term* FancyMult2 = new Multiplication();
	FancyMult1->Add_dodaj(A);
	FancyMult1->Add_dodaj(new Power(new Var("x"), new Var("{n - 1}")));
	FancyMult2->Add_dodaj(B);
	FancyMult2->Add_dodaj(new Power(new Var("x"), new Var("{n -1}")));
	FancyAdd->Add_dodaj(FancyMult1);
	FancyAdd->Add_dodaj(FancyMult2);


	x_potega_n->print_new(plik);
	plik << " = ";
	FancyAdd->print_new(plik);

	Term* x_potega_dwa_a = new Power(new Var("x"), new RealNumber(2));
	Term* FancyAdd_a = new Add();
	Term* FancyMult1_a = new Multiplication();
	FancyMult1_a->Add_dodaj(A);
	FancyMult1_a->Add_dodaj(new Var("x"));
	FancyAdd_a->Add_dodaj(FancyMult1_a);
	FancyAdd_a->Add_dodaj(B);

	plik << "$$ \\n \\n \\n $$";
	x_potega_dwa_a->print_new(plik);
	plik << " = ";
	FancyAdd_a->print_new(plik);
	plik << "$$ \\n \\n \\n $$";

	Term* ChesterfieldAdd = new Add();
	ChesterfieldAdd->Add_dodaj(new Power(new Var("x"), new RealNumber(2)));
	ChesterfieldAdd->Add_dodaj(new SubtractU(new Multiplication(A, new Var("x"))));
	ChesterfieldAdd->Add_dodaj(new SubtractU(B));
	ChesterfieldAdd->print_new(plik);
	plik << " = 0 ";
	plik << "$$ \\n \\n \\n $$";
	plik << "\\Delta = ";



	// help
	Term* Mn = new Multiplication();
	Mn->Add_dodaj(new RealNumber(4));
	Mn->Add_dodaj(new RealNumber(1));
	Mn->Add_dodaj(new SubtractU(B));


	// Delta
	Term* delta = new Subtrac(new Power(new SubtractU(A), new RealNumber(2)), Mn);
	
	

	plik << "$$ \\Delta = ";
	delta->print_new(plik);
	Result resultDelta = delta->calculate();
	plik << " = " << resultDelta.liczba << " $$";

	// Obliczenie pierwiastka delty
	Term* pierwiastek =  new Sqrt( new  RealNumber(resultDelta.liczba) , new RealNumber(2));
	plik << "$$ \\sqrt{\\Delta} = ";
	Result resultPierwiastek = pierwiastek->calculate();
	plik  << resultPierwiastek.liczba << " $$";
	
	





	// Czy tutaj minus w subtractU(A) i w nastepnym 
	Term* gora1 = new Subtrac(A , new Sqrt( new RealNumber(2)   , new RealNumber(resultDelta.liczba)));
	Term* gora2 = new Add(A, new Sqrt(new RealNumber(2), new RealNumber(resultDelta.liczba)));
	Term* x1 = new Division(gora1 , new RealNumber(2));
	Term* x2 = new Division(gora2, new RealNumber(2));

	plik << " \\ \\ \\\\ x1 =     "; 

	x1->print_new(plik);

	plik << " \\ \\ \\\\ x2 =     ";

	x2->print_new(plik);


	plik << " \\ \\ \\\\   ";


	Term* vi = new VarIndex(new Var("d"), new Var("n"));
	vi->print_new(plik);
	plik << "   =    "; 

	Term* bigAdd = new Add();
	Term* mul1 = new Multiplication();
	Term* mul2 = new Multiplication();
	mul1->Add_dodaj(new Var("C"));
	mul1->Add_dodaj(new Power (x1  , new Var("n")));
	mul2->Add_dodaj(new Var("C"));
	mul2->Add_dodaj(new Power(x2, new Var("n")));
	bigAdd->Add_dodaj(mul1);
	bigAdd->Add_dodaj(mul2);


	Z->print_new(plik);
	plik << " = ";
	bigAdd->print_new(plik);

	

	plik << " \\ \\ \\\\   ";

	Term* bigAdd1 = new Add();
	Term* mul11 = new Multiplication();
	Term* mul22 = new Multiplication();
	mul11->Add_dodaj(new VarIndex(new Var("C"), new Var("1")));
	mul11->Add_dodaj(new Power(x1, new Var("0")));
	mul22->Add_dodaj(new VarIndex(new Var("C"), new Var("2")));
	mul22->Add_dodaj(new Power(x2, new Var("0")));
	bigAdd1->Add_dodaj(mul11);
	bigAdd1->Add_dodaj(mul22);
	bigAdd1->print_new(plik);


	plik << " \\ \\ \\\\   ";

	Term* bigAdd11 = new Add();
	Term* mul111 = new Multiplication();
	Term* mul222 = new Multiplication();
	mul111->Add_dodaj(new VarIndex ( new Var("C") , new Var("1")));
	mul111->Add_dodaj(new Power(x1, new Var("1")));
	mul222->Add_dodaj(new VarIndex (new Var("C") , new Var("2")));
	mul222->Add_dodaj(new Power(x2, new Var("1")));
	bigAdd11->Add_dodaj(mul111);
	bigAdd11->Add_dodaj(mul222);
	bigAdd11->print_new(plik);



	plik << "$$ \\\\ ";
	plik << "$$ \\\\ ";
	plik << "$$ \\\\ ";
	
	plik << "Macierz \\n";

	Term* WyznacznikPrzewidywan = new Wyznacznik(new VarIndex(new Var("C"), new Var("1")), new VarIndex(new Var("C"), new Var("2")), new Var("Z"), new VarIndex(new Var("x"), new Var("1")), new VarIndex(new Var("x"), new Var("2")), new Var("J"));

	WyznacznikPrzewidywan->print_new(plik);
	//Result ww = WyznacznikPrzewidywan->calculate();
	//plik << ww.liczba << "  w2 " << ww.liczba2;


	Term* WyznacznikPrzewidywan1 = new Wyznacznik2(new RealNumber(1), new RealNumber(1), Z, x1, x2, J);

	WyznacznikPrzewidywan1->print_new(plik);
	Result ww1 = WyznacznikPrzewidywan1->calculate();
	plik << "w1 = " << ww1.liczba << "  w2 =  " << ww1.liczba2;



	plik << " \\ \\ \\\\   ";

	Term* bigAdd11z = new Add();
	Term* mul111z = new Multiplication();
	Term* mul222z = new Multiplication();
	mul111z->Add_dodaj(new RealNumber(ww1.liczba));
	mul111z->Add_dodaj(new Power(x1, new Var("n")));
	mul222z->Add_dodaj(new RealNumber(ww1.liczba2));
	mul222z->Add_dodaj(new Power(x2, new Var("n")));
	bigAdd11z->Add_dodaj(mul111z);
	bigAdd11z->Add_dodaj(mul222z);
	bigAdd11z->print_new(plik);





	Term* testx = new Add();
	Term* testxx = new Add();
	Term* testxxx = new Add();
	Term* testxxxx = new Multiplication();
	testx->Add_dodaj(new RealNumber(1));
	testx->Add_dodaj(new RealNumber(2));
	testx->Add_dodaj(new RealNumber(4));



	testxx->Add_dodaj(new RealNumber(3));
	testxx->Add_dodaj(new RealNumber(7));


	testxxxx->Add_dodaj(new RealNumber(34));
	testxxxx->Add_dodaj(new RealNumber(75));

	testxxx->Add_dodaj(testx);
	testxxx->Add_dodaj(testxx);
	testxxx->Add_dodaj(new Sqrt(new RealNumber(3) , new RealNumber(5)));


	Term* klkl = new Add();
	klkl->Add_dodaj(testx);
	klkl->Add_dodaj(testxxx);
	klkl->Add_dodaj(new SubtractU(testxxxx));
	Term* odej = new SubtractU(klkl);
	
	//testyy->print_new(plik);


	plik << "wyglad pelnego symbolu $$ \\\\ \\\\$$ ";
	klkl->print_new(plik);
	plik << " TEST SKROTU $$ \\\\ \\\\$$ ";
	//klkl->Reduce(plik);
	//klkl->print_new(plik);

	odej->Reduce(plik);
	odej->print_new(plik);
	


	// TEST next 



	Term* Delta = new Add();
	Delta->Add_dodaj(new Power(new RealNumber(2), new RealNumber(2)));
	Delta->Add_dodaj(new RealNumber(2));
	Delta->print_new(plik);
	Delta->Reduce(plik);
	Delta->print_new(plik);

	Term* minusBEdodacsqrtDELTA = new Add();

	minusBEdodacsqrtDELTA->Add_dodaj(new Sqrt(new RealNumber(2), Delta));
	minusBEdodacsqrtDELTA->Reduce(plik);
	plik << " $$ $$  \\n \\n  $$ $$   $$\\n  \\\\ $$  TESTTTTTTTTTT           x1  = ";


	minusBEdodacsqrtDELTA->print_new(plik);


	plik << "TEST monozenia "; 

	/*
	Term* multf = new Multiplication();
	Term* multg = new Multiplication();
	Term* multh = new Multiplication();
	multf->Add_dodaj(new RealNumber(3));
	multf->Add_dodaj(new RealNumber(4));
	multg->Add_dodaj(new RealNumber(5));
	multg->Add_dodaj(new RealNumber(6));
	multh->Add_dodaj(multf);
	multh->Add_dodaj(multg);
	multh->print_new(plik);
	Term* multp = new Multiplication();
	multp->Add_dodaj(new SubtractU(multh));
	multp->Add_dodaj(new RealNumber(222));
	multp->print_new(plik);
	plik << " \\\\";
	multp->Reduce(plik);
	plik << " x\\\\ "; 
	multp->print_new(plik);
	*/

	Term* multf = new Multiplication();
	multf->Add_dodaj(new RealNumber(3));
	multf->Add_dodaj(new SubtractU(new RealNumber(4)));

	multf->print_new(plik);
	plik << " \\\\";
	multf->Reduce(plik);
	plik << " \\\\";
	multf->print_new(plik);


	// Zwolnienie pamięci
	delete A;
	delete B;
	delete delta;
	delete pierwiastek;
	
	delete x_potega_n;
	delete FancyAdd;
	delete FancyMult1;
	delete FancyMult2;
	delete x_potega_dwa_a;
	delete FancyAdd_a;
	delete ChesterfieldAdd;







}




void skroc(FormalSeries* szereg) {
	// Pobierz granice sumowania i wyrażenie
	Term* id = szereg->indeks_dolny;
	Term* ig = szereg->indeks_gorny;
	Term* wyrazenie = szereg->wyrazenie;

	// Sprawdź, czy granice sumowania są stałe
	if (id->name == ig->name) {
		// Oblicz wartość wyrażenia dla stałych granic sumowania
		Result val = wyrazenie->calculate();

		// Zastąp szereg jednym wyrażeniem
		delete szereg->indeks_dolny;
		delete szereg->indeks_gorny;
		delete szereg->wyrazenie;
		szereg->indeks_dolny = new VarIndex(new Var(id->name), new RealNumber(0));
		szereg->indeks_gorny = new RealNumber(0);
		szereg->wyrazenie = new Multiplication(new RealNumber(val.liczba), new Power(new Var("x"), new Var(id->name)));

		// Zaktualizuj nazwę szeregu
		szereg->name = szereg->napis->name + "(" + id->name + ")";
	}
}


//  <<<<<< !!! Zapisz  do latexa !!! >>>>>>>






void zapiszDoLatex(Term* A, Term* B, Term* Z, Term* J) {
	std::ofstream plik("rownanie.tex"); // otwĂłrz plik do zapisu
	// zapisujemy wynik do pliku
	try {
		std::ofstream plik("wynik.tex");
		if (plik.is_open()) {
			plik << "\\documentclass[12pt]{article}\n";
			plik << "\\begin{document}\n";
			liczszereg(A, B, Z, J, plik);
			plik << "$$\n  \n \n \n    $$ ";
			//	rownanie->printTex(plik);
			//zapiszDoLatex(A, B, Z ,  J , plik);
			liczszeregVIP( A, B, Z, J, plik );

			plik << "$$\n \n \n $$ ";

			liczMetodePrzewidywan(A, B, Z, J, plik);
			plik << "$$\n $$ ";
			plik << "\\end{document}\n";
			plik.close();
			std::cout << "XWynik zapisano do pliku wynik.tex\n";
		}
		else {
			std::cerr << "Blad nie mozna otworzyc : ( pliku wynik.tex\n";
		}
	}
	catch (std::exception& E) {
		std::cerr << "BlAD !!!! : " << E.what() << "\n";
	}
	plik.close(); // zamknij plik
}


class Test {
public:
public:
	Test(){}
	void testu() {

		// TEST ULAMEK 
		std::cout << "---------------------";
		std::cout << "\n";
		std::cout << "Test ulamek   -1 / 2 , 1/-4 , 4/5 , -4/-8 ";
		std::cout << "\n";
		Term* ff1 = new Fraction(-1, 2);
		Term* ff2 = new Fraction(1, -4);
		Term* ff3 = new Fraction(4, 5);
		Term* ff4 = new Fraction(-4, -8);
		ff1->print();
		ff2->print();
		ff3->print();
		ff4->print();
		std::cout << "\n";
		std::cout << "wynik calculate dla ff1 : \n";
		Result qw = ff1->calculate();
		std::cout << qw.liczba;

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
		rn1->print();
		rn2->print();
		rrr4.print();
		rrr5.print();

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
		ss1->print();
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

		Fraction* f1 = new Fraction(3, 5);
		//f1->print();
		RealNumber* r1 = new  RealNumber(4);
		Term* t1 = r1;
		//t1->print() ;  
		Term* bazowa = new RealNumber(5);
		//bazowa->print(); 
		Term* s1 = new Division(bazowa, bazowa);
		//s1->print() ; 

		Term* ss2 = new Division(f1, bazowa);
		std::cout << "\n";

		ss2->print();
		Result resux = ss2->calculate();
		std::cout << resux.liczba;

		// Test mnozenia 

		Fraction* ala1 = new Fraction(1, 3);
		Fraction* ala2 = new Fraction(7, 2);
		std::cout << "\n";
		std::cout << "------------------";
		std::cout << "\n";
		std::cout << "Test mnozenia ";
		std::cout << "\n";
		Term* ss4 = new Multiplication(ala1, ala2);
		Result resu = ss4->calculate();
		ss4->print();
		std::cout << resu.liczba;


		// Test potegi 
		std::cout << "\n";
		std::cout << "------------------";
		std::cout << "\n";
		std::cout << "Test potegi 5 do potego 5  ";
		std::cout << "\n";



		// POTEGA 5 do potegi 5 
		Power* ptga = new Power(bazowa, bazowa);
		ptga->print();
		Result resultpo = ptga->calculate();
		std::cout << resultpo.liczba;




		// Test dodawania 
		std::cout << "\n";
		std::cout << "------------------";
		std::cout << "\n";
		std::cout << "Test dodawania  ";
		std::cout << "\n";
		Term* dd1 = new Add(ss1, ptga);
		dd1->print();


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
		mm1->print();

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
		Sz->print();
		std::cout << "\n";
		std::cout << "Test Rename\n";
		std::cout << "-------------\n";
		Sz->rename("n", "k");
		Sz->print();

		// TEST  OBLICZENIA COMPLICATED

		std::cout << "\n";
		std::cout << "------------------";
		std::cout << "\n";
		std::cout << "Copplicated obliczenie ";
		std::cout << "\n";
		Term* trzy = new RealNumber(3.000);
		Term* dwa = new RealNumber(2.000);
		Term* jednadruga = new Fraction(1, 2);
		Term* trzydopotegidrugiej = new Power(trzy, dwa);
		Term* jednadrugaplustrzydopotegidrugiej = new Add(jednadruga, trzydopotegidrugiej);
		//jednadrugaplustrzydopotegidrugiej->print();
		Term* cztery = new RealNumber(4.000);
		Term* pierwiastekzdwoch = new Sqrt(dwa, dwa);
		Term* pierwiastekczwartegostopniaztrzech = new Sqrt(trzy, cztery);
		//pierwiastekczwartegostopniaztrzech->print();
		Term* mnozenie = new Multiplication(pierwiastekzdwoch, pierwiastekczwartegostopniaztrzech);
		Term* wynik = new Division(jednadrugaplustrzydopotegidrugiej, mnozenie);
		wynik->print();
		std::cout << "\n";

		Result resultcom = wynik->calculate();
		std::cout << resultcom.liczba;


		// TEST 10 kopia 

		// cos jest zle z kopiujacymi : )
		// dodaje plus na koncu nie wiem czemu : ) 
		//Add jednadrugaplustrzydopotegidrugiej_copy(jednadrugaplustrzydopotegidrugiej);
		//std::cout << "\n";
		//jednadrugaplustrzydopotegidrugiej_copy.print();

		// nie dziala : ) 
		//Sqrt pierwiastekczwartegostopniaztrzech_copy(pierwiastekczwartegostopniaztrzech);
		//Sqrt sssss1(pierwiastekczwartegostopniaztrzech);


		std::cout << "TUUUUUUUUUUUUUuuu\n";
		//Term* x = new Var("x");
		//Term* jed = new RealNumber(1);


		// Var napis

		Term* vvtest = new VarIndex(new Var("x"), new RealNumber(1));
		vvtest->print();






		delete vvtest;

		// Metoda przewidywan 

		Var* a = new Var("a");
		Var* b = new Var("b");


		delete a, b;

		// VAR 
		 // Stworzenie zmiennych dla wyraĹĽeĹ„ formalnych
		Var* n = new Var("n");
		Var* k = new Var("k");

		// Stworzenie wyraĹĽeĹ„ formalnych
		//Term* id = new RealNumber(0);
		//Term* ig = new Var("n");
		//Term* wyrazenie1 = new Fraction(new Power(new Var(k), new RealNumber(2)), new Silnia(new Var(k)));
		//Term* wyrazenie2 = new Fraction(new RealNumber(1), new Silnia(new Var(k"")));
		//FormalSeries* fs1 = new FormalSeries(id, ig, wyrazenie1, n);
		//FormalSeries* fs2 = new FormalSeries(id, ig, wyrazenie2, n);

		// Stworzenie wyznacznika
		//Wyznacznik* det = new Wyznacznik(fs1, fs2, fs2, fs1, k, n);

		// WyĹ›wietlenie wyznacznika
		//det->print();





		//Term* wynikk = new Division(new Subtrac(new RealNumber(1), new Sqrt(new RealNumber(2), new RealNumber(5)))  , new RealNumber(2));
		Term* wynikk = new Sqrt(new RealNumber(5), new RealNumber(2));
		Result xx = wynikk->calculate();
		std::cout << " \n xx = " << xx.liczba;




		delete  pierwiastekzdwoch, pierwiastekzdwoch, pierwiastekczwartegostopniaztrzech;
		delete  jednadruga, trzydopotegidrugiej;
		delete t1, r1;
		delete dd1;
		delete s1, bazowa, f1, ss2;
		delete mm1;
		delete ff1, ff2;
		delete rn1, rn2;

		delete mnozenie, wynik, jednadrugaplustrzydopotegidrugiej;
	}

};







int main(int argc, char** argv) {


	// Te co bylly na poczatku 
	/*
	
	
	Term* A = new RealNumber(2.1);//  Var("A");
	Term* B1 = new RealNumber(3);
	Term* B2 = new RealNumber(7);

	Term* B = new Power(B1, B2);//   Var("B");
	Term* Z = new RealNumber(7);
	Term* J = new RealNumber(1);

	*/

	
	
	
//	Term* A = new RealNumber(1);//  Var("A");
	Term* A = new   Var("A");

	Term* B1 = new RealNumber(1);
	
	
	Term* B2 = new RealNumber(4);
	Term* B = new Power(B1, B2);//   Var("B");
	
	Term* Z = new RealNumber(0);
	Term* J = new RealNumber(1);
	
	

	zapiszDoLatex(A, B1, Z, J);



	delete A;
	delete B;
	delete Z;
	delete J;








	std::cout << "Program zakonczyl dzialanie." << std::endl;
	system("pause");
	system("PAUSE");

	return 0;
}
