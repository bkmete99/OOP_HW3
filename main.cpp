/* @Author
 * Student Name: Burak Karamete
 * Student ID : 150170014 *
 * Date: 09.05.2019 */

///////////////////
///////C++11///////
///////////////////

//Please compile with std=c11

#include <iostream>
#include <fstream>
#include <typeinfo>
#include <sstream>
#include <list>
#include <iterator>
#include <queue>
#include <algorithm>
#include <map>

using namespace std;


//Wish class means every order in a table
//For example 2 massala chicken is a wish
class wish {
    string name_of_wish;
    int num_of_wish;
public:
    wish() {};

    wish(string name, int num) {
        this->set_name_of_wish(name);
        this->set_num_of_wish(num);
    }

    void set_name_of_wish(string x) { name_of_wish = x; }

    string get_name_of_wish() { return name_of_wish; };

    void set_num_of_wish(int x) { num_of_wish = x; }

    int get_num_of_wish() { return num_of_wish; };
};


//Order class means every table
//For example table 1's wishes  2 m.chicken and 1 coke
//I stored these in a queue in an order object
class Order {
    string table;
    queue<wish> wishes;
    double tablecost;
    double totalprice;
    double tip;
    double taxrate;

public:
    Order(string nameoftable) {//New table , new order
        setTable(nameoftable);
        tablecost = 0;//Initially cost 0
    }

    const queue<wish> &getWishes() const {
        return wishes;
    }

    const string &getTable() const {
        return table;
    }

    void setTable(const string &table) {
        Order::table = table;
    }

    queue<wish> get_wish_list() { return wishes; }

    int get_tablecost() { return tablecost; }

    double get_tip() {
        tip = (tablecost * 15.0) / 100.0;
        return tip;
    }

    double get_totalprice() {
        taxrate = (tablecost * 8.0) / 100.0;
        totalprice = tip + tablecost + taxrate;
        return totalprice;
    }

    void add_wish(string namewish, int numwish) {//In every wish i add this wish to my queue
        wish newwish(namewish, numwish);
        wishes.push(newwish);
    }

    void addcost(int curcost) { tablecost += curcost; }//In every wish i add new wishes cost

};

class Ingredient {
private:
    string name;
    int itemcount;
    double price;
public:
    Ingredient() {
    }

    void set_price(double inprice) { price = inprice; }

    virtual double get_price() { return price; }

    virtual void set_itemcount(int Itemcount) { itemcount = Itemcount; }

    virtual int get_itemcount() { return itemcount; }

    void setname(string Name) { name = Name; }

    string get_name() { return name; }

};


class Type1 : public Ingredient {
    int gram;
public:
    Type1(double price, int Itemcount, string name) {
        set_price(price);
        set_itemcount(Itemcount);
        setname(name);
    }


    virtual bool is_enough(int desired) { if (desired <= gram)return true; else return false; }//if stock is enough for this ingredient

    virtual double get_full_price(int desired) { return desired * get_price(); }

    virtual int get_itemcount() { return gram; }

    virtual void set_itemcount(int x) { gram=x; }

    void virtual consume(int consumed) {//if we can serve this wish so we can change its stock information
        this->set_itemcount(get_itemcount() - consumed);
    }


};


class Type2 : public Ingredient {
    int number;
public:
    Type2(double price, int Itemcount, string name) : Ingredient() {
        set_price(price);
        set_itemcount(Itemcount);
        setname(name);
    }
    virtual int get_itemcount(){ return number;}

    virtual bool is_enough(int desired) { if (desired <= number) { return true; } else return false; }

    virtual void set_itemcount(int x) { number = x; }

    virtual double get_full_price(int desired) { return desired * get_price(); }

    void virtual consume(int consumed) {
        this->set_itemcount(get_itemcount() - consumed);
    }
};

class Type3 : public Ingredient {
    //list<Type3> t3list;
    int ml;
public:
    Type3(double price, int Itemcount, string name) : Ingredient() {
        set_price(price);
        set_itemcount(Itemcount);
        setname(name);
    }

    virtual bool is_enough(int desired) { if (desired <= ml)return true; else return false; }

    virtual void set_itemcount(int x) { ml = x; }

    virtual int get_itemcount(){return ml;}

    virtual double get_full_price(int desired) { return desired * get_price(); }

    void virtual consume(int consumed) {
        this->set_itemcount(get_itemcount() - consumed);
    }
};


int main() {
    list<Type1> t1list;
    list<Type2> t2list;
    list<Type3> t3list;

    //Reading stock
    fstream infile("stock.txt");
    if (!infile) {
        cerr << "Unable to open file stock.txt";
        exit(1);
    }
    string a, b;
    int i = 0;
    int type, itemcount;
    double price;
    while (i < 4) {
        i++;
        infile >> a;
    }
    i = 0;
    while (i < 27) {
        infile >> a;
        while (1) {
            infile >> b;
            if (b != "1" && b != "2" && b != "3") { a = a + " " + b; }
            else {
                stringstream geek(b);
                geek >> type;
                break;
            }
        }
        infile >> itemcount >> price;
        i++;

        if (type == 1) {
            Type1 ba(price, itemcount, a);
            t1list.push_back(ba);
        }
        if (type == 2) {
            Type2 ba(price, itemcount, a);
            t2list.push_back(ba);
        }
        if (type == 3) {
            Type3 ba(price, itemcount, a);
            t3list.push_back(ba);
        }
    }


    //Reading menu and i used map
    ifstream infiles("menu.txt");
    if (!infiles) {
        cerr << "Unable to open file menu.txt";
        return 1;
    }   // if file cant found
    map<string, string> fullmenu;
    string line, token;
    getline(infiles, line);
    string menuline, foodname, ingred;
    while (!infiles.eof()) {
        getline(infiles, menuline);
        istringstream bk(menuline);
        getline(bk, foodname, '\t');
        getline(bk, ingred);
        fullmenu.insert(pair<string, string>(foodname, ingred));

    }

    //Reading order.txt
    int x, y;
    string line2;
    ifstream infileo("order.txt");
    if (!infiles) {
        cerr << "Unable to open file order.txt";
        return 1;
    }   // if file cant found

    while (!infileo.eof()) {
        getline(infileo, line);
        Order newo(line);//line is table name like table1 or table2
        infileo >> x;
        getline(infileo, line);
        while (x != 0) {//every time means a table
            getline(infileo, line);
            string newline = "", newword;
            stringstream ordertoline(line);
            ordertoline >> y;
            while (ordertoline >> newword) { newline += newword + " "; }
            newline.pop_back();
            newo.add_wish(newline, y);
            x--;
        }
        string tok, toka;
        cout << newo.getTable() << " ordered:" << endl;
        queue<wish> order_wishes = newo.getWishes();
        while (!order_wishes.empty()) {//Every order of a table

            wish current_wish = order_wishes.front();
            int num_of_unit = current_wish.get_num_of_wish();
            string name_of_wish = current_wish.get_name_of_wish();
            string ing_of_wish = fullmenu.at(name_of_wish);
            int count = 0;
            bool serve = true,atleast=true;int servedunit=0;
            double ordercost = 0;
            while (count != num_of_unit && serve == true &&atleast) {

                istringstream ord(ing_of_wish);
                istringstream ord2(ing_of_wish);
                while (getline(ord, tok, ',') && serve == true) {//every ingredient
                    istringstream ingoford(tok);
                    int numofing;
                    string unit_or_not, others, otherword;
                    ingoford >> numofing;
                    ingoford >> unit_or_not;
                    if (unit_or_not == "gram" || unit_or_not == "ml") {//type1 or type3

                    } else {
                        others = unit_or_not + " ";
                        unit_or_not = "";
                    }
                    while (ingoford >> otherword) {
                        others += otherword;
                        others += " ";
                    }
                    others.pop_back();

                    if (unit_or_not == "gram") {//type1
                        list<Type1>::iterator it;
                        for (it = t1list.begin(); it != t1list.end(); ++it) {
                            if ((*it).get_name() == others) {
                                Type1 temp((*it).get_price(), (*it).get_itemcount(), (*it).get_name());
                                if (!temp.is_enough(numofing)) { serve = false; }else{atleast=true;}

                            }
                        }
                    }

                    if (unit_or_not == "" or ing_of_wish == "N/A") {
                        if (ing_of_wish == "N/A") {
                            others = name_of_wish;
                            numofing = 1;
                        }
                        list<Type2>::iterator ita;
                        for (ita = t2list.begin(); ita != t2list.end(); ++ita) {
                            if ((*ita).get_name() == others) {
                                Type2 temp((*ita).get_price(), (*ita).get_itemcount(), (*ita).get_name());

                                if (!temp.is_enough(numofing)) {
                                    serve = false;
                                }else{atleast=true;}

                            }
                        }
                    }

                    if (unit_or_not == "ml") {


                        list<Type3>::iterator itar;

                        for (itar = t3list.begin(); itar != t3list.end(); ++itar) {
                            if ((*itar).get_name() == others) {
                                Type3 temp((*itar).get_price(), (*itar).get_itemcount(), (*itar).get_name());
                                if (!temp.is_enough(numofing)) { serve = false; }else{atleast=true;}

                            }
                        }
                    }

                }
                int remain = -1,current;float myprice;string mytype;
                if (serve == true) {
                    while (getline(ord2, tok, ',')) {//yes we can serve it so we can calculate its price
                        //cout << "aaaa" << tok;
                        istringstream ingoford(tok);
                        int numofing;
                        string unit_or_not, others, otherword;
                        ingoford >> numofing;
                        ingoford >> unit_or_not;
                        if (unit_or_not == "gram" || unit_or_not == "ml") {//type1 or type3

                        } else {
                            others = unit_or_not + " ";
                            unit_or_not = "";
                        }
                        while (ingoford >> otherword) {
                            others += otherword;
                            others += " ";
                        }
                        others.pop_back();
                        if (unit_or_not == "gram") {//type1
                            list<Type1>::iterator it;
                            for (it = t1list.begin(); it != t1list.end(); ++it) {
                                if ((*it).get_name() == others) {
                                    Type1 temp((*it).get_price(), (*it).get_itemcount(), (*it).get_name());
                                    ordercost += temp.get_full_price(numofing);
                                    (*it).consume(numofing);
                                    remain = (*it).get_itemcount() - numofing;
                                    current=(*it).get_itemcount();
                                    mytype="1";
                                    myprice=(*it).get_price();
                                }
                            }
                        }

                        if (unit_or_not == "" or ing_of_wish == "N/A") {
                            if (ing_of_wish == "N/A") {
                                others = name_of_wish;
                                numofing = 1;
                            }
                            list<Type2>::iterator ita;
                            for (ita = t2list.begin(); ita != t2list.end(); ++ita) {
                                if ((*ita).get_name() == others) {
                                    Type2 temp((*ita).get_price(), (*ita).get_itemcount(), (*ita).get_name());
                                    ordercost += temp.get_full_price(numofing);
                                    (*ita).consume(numofing);

                                    remain = (*ita).get_itemcount() - numofing;
                                    current=(*ita).get_itemcount();
                                    mytype="2";

                                    myprice=(*ita).get_price();
                                }
                            }
                        }

                        if (unit_or_not == "ml") {

                            mytype="3";
                            list<Type3>::iterator itar;


                            for (itar = t3list.begin(); itar != t3list.end(); ++itar) {
                                if ((*itar).get_name() == others) {
                                    //cout<<"This is type3"<<others<<"price is "<<(*itar).get_price()<<endl;
                                    Type3 temp((*itar).get_price(), (*itar).get_itemcount(), (*itar).get_name());
                                    ordercost += temp.get_full_price(numofing);
                                    (*itar).consume(numofing);
                                    remain = (*itar).get_itemcount() - numofing;
                                     current=(*itar).get_itemcount();

                                    myprice=(*itar).get_price();
                                }
                            }
                        }
                        //We have update stock.txt

                        ifstream in_File("stock.txt");
                        string fileline, tokenfile;
                        vector<string> lines;
                        string input;

                        while (getline(in_File, fileline)) {
                            lines.push_back(fileline);
                        }
string findd=others;
                        stringstream ss;
                        ss<<myprice;
                        string pri=ss.str();


       string stra=to_string(current);
string strab=to_string(remain);
if(remain<0)remain=0;
                        for (int j = 0; j < lines.size(); ++j) {
                            if(!lines[j].find(findd)){
                                lines[j]=findd+ "\t"+mytype+"\t"+stra+"\t"+pri ;

                            }
                        }
                        in_File.close();
                        ofstream output("stock.txt");
                        for (string l:lines) { output << l << '\n'; }
                        output.close();


                    }
                    servedunit++;
                } else {
                    if(count==num_of_unit || !serve){atleast=false;}
                    cout << "We don't have enough " << name_of_wish << endl;
                    serve = false;
                }

count++;
                if(!atleast){current_wish.set_num_of_wish(current_wish.get_num_of_wish()-1);}
            }

                if (servedunit!=0) {
                    cout << current_wish.get_num_of_wish() << " " << name_of_wish << " cost: " << ordercost << endl;
                    newo.addcost(ordercost);
                }

            order_wishes.pop();;
        }

        cout << "Tip is " << newo.get_tip() << endl << "Total cost: " << newo.get_totalprice() << " TL" << endl;
    }

    return 0;
}