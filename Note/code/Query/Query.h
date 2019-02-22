#include <iostream>
#include <map>
#include <string>
#include <fstream>
#include <memory>
#include <vector>
#include <set>
#include <sstream>
#include <algorithm>

/*
1. 没有去掉标点符号
2. 没有区分大小写
3.使用shared_ptr 指向vector 会不会析构的时候会不会导致内存泄漏
 */
class QueryResult;

class TextQuery{
public:
    using line_no = std::vector<std::string>::size_type;
    explicit TextQuery(std::ifstream& is);
    const QueryResult query(const std::string& word) const;
private:
    std::shared_ptr<std::vector<std::string> > _spfile;
    std::map<std::string, std::shared_ptr<std::set<line_no> > > _dict;
};

class QueryResult{
friend std::ostream& print(std::ostream& out, const QueryResult& result);
public:
    explicit QueryResult(std::string str, 
                std::shared_ptr<std::vector<std::string> > file,
                std::shared_ptr<std::set<TextQuery::line_no> > lines):
    _sought(str), _spfile(file),_splines(lines){ }
    std::shared_ptr<std::vector<std::string> > get_file() const 
                                                    { return _spfile;}

    std::set<TextQuery::line_no>::iterator begin(){return _splines->begin();}
    std::set<TextQuery::line_no>::iterator end(){return _splines->end();}
private:
    std::string _sought;
    std::shared_ptr<std::vector<std::string> > _spfile;
    std::shared_ptr<std::set<TextQuery::line_no> > _splines;
};


TextQuery::TextQuery(std::ifstream& is):_spfile(new std::vector<std::string>){
    std::string text;
    while(getline(is, text)){
        _spfile->push_back(text);
        int lineno = _spfile->size() - 1;
        std::istringstream line(text);
        std::string word;
        while(line >> word){
            auto &lines = _dict[word];
            if(!lines){
                lines.reset(new std::set<line_no>);
            }
            lines->insert(lineno);
        } 
    }
}

const QueryResult TextQuery::query(const std::string& word) const{
    static std::shared_ptr<std::set<line_no> > nodata(new std::set<line_no>);
    auto loc = _dict.find(word);
    if(loc == _dict.end() ){
        return QueryResult(word, _spfile, nodata);
    }
    else{
        return QueryResult(word, _spfile, loc->second);
    }
}

std::ostream& print(std::ostream& out, const QueryResult& result){
    out << result._sought << " occurs " << (*result._splines).size() << " times: " << std::endl;
    for(auto line : *(result._splines)){
    out << "\t(line " << line << ") " << (*result._spfile)[line] << std::endl;
    }
    return out;
}