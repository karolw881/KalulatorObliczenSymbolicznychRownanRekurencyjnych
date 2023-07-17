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

#ifndef parse_cpp
#define parse_cpp

Term * get_first_term(std::vector <std::string> &tokens,int &from,int &to);

void Parse_Error(std::string msg,int pos,std::vector <std::string> &tokens)
{
    std::cout << "Blad: " << msg <<" ";
    for (;pos <tokens.size()-1;pos++)
        std::cout << tokens[pos];
    std::cout<<std::endl;


}


Term * get_term_up_to_prior(std::vector <std::string> &tokens,int &from,int &to,std::string Prior)
{
 int f=from,t=tokens.size();
 Term *T = get_first_term(tokens,f,t);
  if (T==NULL) {return NULL;}
 f=t+1;
 if (f < tokens.size())
 {
     bool test_priori = false;
     if (Prior=="+" && ((tokens[f]=="*") || (tokens[f]=="/")))  test_priori =  true;
     while (test_priori)
     {
           std::string OpOrg = tokens[f];
           f++;
          Term *G = get_first_term(tokens,f,t);
          if (G==NULL)  {Parse_Error("Nieudane pobranie prawego argumentu",f-1,tokens);return NULL;}
          if (OpOrg=="*"){ T= new Multiplication(T,G);}
         else {T= new Division(T,G);};

         test_priori =  false;
         f=t+1;
         if((f < tokens.size()) && ((tokens[f]=="*") || (tokens[f]=="/"))) test_priori =  true;
     }
 }
 to=t; return T;
};

Term * get_term_up_to_token(std::vector <std::string> &tokens,int &from,int &to,std::string Token)   //+ "end"  ///to ustawia siê na pozycji Token
{
    int f=from,t=tokens.size();
    Term *T = get_first_term(tokens,f,t);
     if (T==NULL) { return NULL;}
    f = t+1; t=tokens.size();
    while (f < tokens.size() && tokens[f]!=Token)
    {
         std::string Op="", OpOrg=tokens[f];
         if (tokens[f]=="+" || tokens[f]=="-"){Op="+";}
         else
              if (tokens[f]=="*" || tokens[f]=="/"){Op="*";}
         else {Parse_Error("Nieoczekiwany symbol:"+tokens[f]+" zamist symbolu operacji" ,f,tokens) ;return NULL;}

         f ++;
         if (f >=  tokens.size()) {Parse_Error("Symbol operacja na koñcu tekstu",f,tokens);return NULL;}
         Term  *G = get_term_up_to_prior(tokens,f,t,Op);
         if (G==NULL) {return NULL;}
         f=t;  t=tokens.size();
         f++;
         ///skladanie termu
         if (OpOrg=="+"){ T= new Add(T,G);}
         else if (OpOrg=="-") {T= new Add(T,new SubtractU(G));}
         else if (OpOrg=="*"){ T= new Multiplication(T,G);}
         else if (OpOrg=="/"){T= new Division(T,G);}; ///<- tu nie ma else
    }
    to=f;
    if (f >= tokens.size()) {return NULL;}
    else return T;
}



    Term * get_first_term(std::vector <std::string> &tokens,int &from,int &to)
    {
         if (tokens[from]=="(")
         {
              int f=from+1,t =  tokens.size();
              if (f>= tokens.size()) {Parse_Error("Nie domkniete wyrazenie (... ",from,tokens); return NULL;}
              Term * P = get_term_up_to_token(tokens,f,t,")");
              f=t;
              if (P==NULL) { return NULL;}
              if (f>= tokens.size()) {Parse_Error("nie udane pobranie termu (...) ",from,tokens);  return NULL;}
              to = f;
              return P;
         }
         else if (tokens[from]=="-")
         {
              int f=from+1, t = tokens.size();
              if (f>= tokens.size()) {Parse_Error("Nie domkniete wyrazenie -... ",from,tokens); return NULL;}
              Term * U = get_first_term(tokens,f,t);
              if  (U==NULL ) { return NULL;}
              to=t;
              return new SubtractU(U);
         }
         else if (tokens[from]=="sqrt")
         {
              if (from +1 >= tokens.size()){Parse_Error("Symbol sqrt na koñcu tekstu",from,tokens); return NULL;}
              int f = from+1;int t=tokens.size();
              if (tokens[f]!="(")  {Parse_Error("Oczekiwany sqrt(",f,tokens); return NULL;}
              f++;
              if (f >= tokens.size()){return NULL;}
              Term * P=get_term_up_to_token(tokens,f,t,",");
                if (P==NULL ) {return NULL;}
              f=t;
              f++;
              t=tokens.size();
              if (f >= tokens.size() ) {return NULL;}
              Term * S=get_term_up_to_token(tokens,f,t,")");
              f=t;
              if (S==NULL ) { return NULL;}
              to = f;
              return new Sqrt(P,S);
         }
         else if(tokens[from]=="+" || tokens[from]=="*" || tokens[from]=="/")
         {
             Parse_Error("nieoczekiwany symbol "+tokens[from]+"zamiast termu",from,tokens);  return NULL;
         }
         else if(tokens[from]==",")
         {
             Parse_Error("nieoczekiwany symbol "+tokens[from]+"zamiast termu",from,tokens); return NULL;
         }
         else
         {
             to=from;
             return new RealNumber(atoi(tokens[from].c_str()));
         }
    }

Term * parse(std::string text)
{
    std::vector <std::string> tokens;
    int pos=0;
    int nawias=0;
    ///wstepne parsowanie rozpoznaje dozwolone znaki +,-,*,\,0-9,sqrt i sprawdza czy podomykane nawiasy
    while (pos < text.length())
    {
        switch (text[pos])
        {
            case '+': tokens.push_back("+");pos++; break;
            case '-': tokens.push_back("-");pos++; break;
            case '*': tokens.push_back("*");pos++; break;
            case '/': tokens.push_back("/");pos++; break;

            case 's': if ((pos + 3 <  text.length())  &&
                             (text[pos+1]=='q') && (text[pos+2]=='r') && (text[pos+3]='t' ))
                             {
                                 tokens.push_back("sqrt");pos+= 4;

                             }
                         else
                         {
                             std::cout << "nieznany napis: " << text.substr(pos, text.length());
                             return NULL;
                         }
                         break;
              case ',': tokens.push_back(",");   pos++; break;//przecinek w sqrt
             case '(': tokens.push_back("(");  nawias++; pos++; break;
             case ')': tokens.push_back(")");  nawias--;
                         if (nawias<0) {std::cout << "nadmiar domkniec nawiasow: " << text.substr(0,pos+1);return NULL;}
                        pos++;
                        break;
            default: if ('0'<=text[pos]  && text[pos]<='9')
            {
                std::string liczba="";
                liczba.push_back(text[pos]);
                 pos++;
                while ('0'<=text[pos]  && text[pos]<='9' && pos < text.length())
                {
                    liczba+=text[pos];
                    pos++;
                }
                tokens.push_back(liczba);
            }
            else {std::cout << "nieoczekiwany symbol: " << text[pos];return NULL;}
        }
    }
   if (nawias>0) {std::cout<<"niedomkniete"<<nawias<<"-nawias(y)";return NULL;}
   tokens.push_back("end");
   int f=0,t;
   return get_term_up_to_token(tokens,f,t,"end");
}



#endif
