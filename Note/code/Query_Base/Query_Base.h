#ifndef QUERY_BASE_H
#define QUERY_BASE_H

#include "../Query/Query.h"

class Query_base{
    friend class Query;
protected:
    using line_no = TextQuery::line_no;
    virtual ~Query_base() = default;
private:
    virtual QueryResult eval(const TextQuery&) const = 0;
    virtual std::string rep()const = 0;
};


class WordQuery: public Query_base{
    friend class Query;
    WordQuery(const std::string& str):query_word(str){ }
    QueryResult eval(const TextQuery& t)const override{\
                                        return t.query(query_word);}
    std::string rep() const override{ return query_word;}
    std::string query_word;
};

/* 对用户使用的类，提供接口， 隐藏继承体系 */
class Query{
    friend Query operator~(const Query&);
    friend Query operator|(const Query&, const Query&);
    friend Query operator&(const Query&, const Query&);
public:
    Query(const std::string& s):q(new WordQuery(s)){ }  //构建新的WordQuery对象
    QueryResult eval(const TextQuery &t) const {return q->eval(t);}
    std::string rep() const {return q->rep();}

private:
    Query(std::shared_ptr<Query_base> query):q(query){ }
    std::shared_ptr<Query_base> q;
};


class NotQuery:public Query_base{
    friend Query operator~(const Query&);
    NotQuery(const Query& q):query(q){ }
    std::string rep()const override {return "~(" + query.rep() + ")";}
    QueryResult eval(const TextQuery& t)const override;
    Query query;
};

QueryResult NotQuery::eval(const TextQuery& t)const{
    auto result = query.eval(t);
    auto ret_line = std::make_shared<std::set<line_no> >();
    //std::shared_ptr<std::set<line_no> > ret_line;
    auto beg = result.begin();
    auto end = result.end();
    auto sz = result.get_file()->size();
    for(size_t i = 0; i != sz; ++i){
        if(beg == end || i != *beg){
            ret_line->insert(i);
            std::cout << "insert" <<std::endl;
        }
        else if(beg != end){
            ++beg;
            std::cout << "increment" <<std::endl;
        }
        std::cout << "loop" << std::endl;
    }
    return QueryResult(rep(), result.get_file(), ret_line);
}
inline Query operator~(const Query& operand){
    return std::shared_ptr<Query_base>(new NotQuery(operand));
}

class BinaryQuery: public Query_base{
protected:
    BinaryQuery(const Query& l, const Query& r,std::string s): \
                                                    lhs(l),rhs(r),opSym(s){ }
    std::string rep()const override \
                            {return "(" + lhs.rep() + opSym + rhs.rep() + ")";}
    Query lhs, rhs;
    std::string opSym; //二元运算的运算符
};



class AndQuery: public BinaryQuery{
    friend Query operator&(const Query&, const Query&);
    AndQuery(const Query& left, const Query& right): \
                                            BinaryQuery(left, right, "&"){ }
    QueryResult eval(const TextQuery&) const override;
};

QueryResult AndQuery::eval(const TextQuery& t) const {
    auto left = lhs.eval(t), right = rhs.eval(t);
    auto ret_line = std::make_shared<std::set<line_no> >();
    //std::shared_ptr<std::set<line_no> > ret_line;

    set_intersection(left.begin(),left.end(),right.begin(), right.end(), inserter(*ret_line, ret_line->begin()));
    return QueryResult(rep(), left.get_file(), ret_line);
}
inline Query operator&(const Query& lhs, const Query& rhs){
    return std::shared_ptr<Query_base>(new AndQuery(lhs, rhs));
}

class OrQuery: public BinaryQuery{
    friend Query operator|(const Query&, const Query&);
    OrQuery(const Query& left, const Query& right):
                                            BinaryQuery(left,right, "|"){ }
    QueryResult eval(const TextQuery&)const override;
};

QueryResult OrQuery::eval(const TextQuery& t) const{
    auto left = lhs.eval(t);
    auto right = rhs.eval(t);
    auto ret_line = std::make_shared<std::set<line_no>>(left.begin(), left.end());
    ret_line->insert(right.begin(), right.end());
    return QueryResult(rep(), left.get_file(), ret_line);
}

inline Query operator|(const Query& lhs, const Query& rhs){
    return std::shared_ptr<Query_base>(new OrQuery(lhs, rhs));
}

#endif //QUERY_BASE_H