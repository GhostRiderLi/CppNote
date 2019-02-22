//类的继承
//
#ifndef QUOTE_H
#define QUOTE_H

#include <iostream>

class Quote{
public:
    Quote() = default;
    Quote(const std::string& book, double sales_price):
                        bookNo(book),price(sales_price){}
    virtual Quote* clone() const & {return new Quote(*this);}
    virtual Quote* clone() && {return new Quote(std::move(*this));}
    std::string isbn() const {return bookNo;}
    virtual double net_price(std::size_t n) const {return price *n;}
    virtual ~Quote() = default;
private:
    std::string bookNo;
protected:
    double price = 0.0;
};


class Bulk_quote: public Quote{
public:
    Bulk_quote() = default;
    Bulk_quote(const std::string&, double, std::size_t, double);
    double net_price(std::size_t) const override;

    Bulk_quote* clone() const &  override {return new Bulk_quote(*this);}
    Bulk_quote* clone() &&  override {return new Bulk_quote(std::move(*this));}
private:
    std::size_t min_qty = 0;
    double discount = 0.0;
};

Bulk_quote::Bulk_quote(const std::string& book, double p, std::size_t qty, 
                        double dis):
                        Quote(book, p), min_qty(qty), discount(dis){ }

double Bulk_quote::net_price(std::size_t cnt) const{
    if(cnt >= min_qty)
        return cnt * (1- discount) * price;
    else
        return cnt* price;
}
#endif//QUOTE_H
