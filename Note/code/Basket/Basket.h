#ifndef BASKET_H
#define BASKET_H

#include <iostream>
#include <memory>
#include <set>
#include "../Quote/Quote.h"

class Basket{
public:
    void add_item(const std::shared_ptr<Quote> &sale){items.insert(sale);}
    void add_item(const Quote& sale); //拷贝版本
    void add_item(Quote && sale);   //移动版本
    double total_receipt(std::ostream&) const;
private:
    static bool compare(const std::shared_ptr<Quote> &lhs,const std::shared_ptr<Quote> &rhs){return lhs->isbn() < rhs->isbn();}
    std::multiset<std::shared_ptr<Quote>,decltype(compare)*> items{compare};
}; 

void Basket::add_item(const Quote& sale){
    items.insert(std::shared_ptr<Quote>(sale.clone()));
}

void Basket::add_item(Quote&& sale){
    items.insert(std::shared_ptr<Quote>(std::move(sale).clone()));
}

double Basket::total_receipt(std::ostream& os) const{
    double sum = 0.0;
    for(auto iter = items.cbegin(); iter != items.cend(); \ 
                                            iter = items.upper_bound(*iter)){
        sum += print_total(os, **iter, items.count(*iter));
    }
    os << "Total Sale: " << sum << endl;
    reutrn sum;
}

#endif //BASKET_H