#ifndef STRVEC_H
#define STRVEC_H

#include <memory>
#include <utility>

class StrVec{
public:
    StrVec():elements(nullptr), first_free(nullptr), cap(nullptr){ }
    StrVec(const StrVec&);
    StrVec(StrVec&&) noexcept;
    StrVec& operator=(const StrVec&);
    StrVec& operator=(StrVec&&) noexcept;
    ~StrVec();

    void push_back(const std::string&);
    void push_back(std::string&&);
    size_t size() const { return first_free - elements;}
    size_t capacity() const{return cap - elements;}
    std::string* begin() const{return elements;}
    std::string* end() const {return first_free;}
private:
    static std::allocator<std::string> alloc; //分配元素
    std::string *elements;
    std::string *first_free;
    std::string *cap;

    void free();
    void reallocate();
    void chk_n_alloc(){if(size() == capacity()) reallocate();}
    std::pair<std::string*, std::string*> alloc_n_copy(const std::string*,\
                                                        const std::string*);
};

void StrVec::push_back(const std::string &str){
    chk_n_alloc();
    alloc.construct(first_free++,str);
}

void StrVec::push_back(std::string &&str){
    chk_n_alloc();
    alloc.construct(first_free++,std::move(str));
}

std::pair<std::string*, std::string*> StrVec::alloc_n_copy(\
                    const std::string* b, const std::string* e){
    auto data = alloc.allocate(e - b);
    return {data, uninitialized_copy(b, e, data)};
}

void StrVec::free(){
    if(elements){
        for(auto p = first_free; p != elements;){
            alloc.destroy(--p);
        }
        alloc.deallocate(elements, cap - elements);
    }
}

StrVec::StrVec(const StrVec& strv){
    auto newdata = alloc_n_copy(strv.begin(), strv.end() );
    elements = newdata.first;
    first_free = cap = newdata.second;
}

StrVec::StrVec(StrVec&& strv) noexcept: 
            elements(strv.elements), first_free(strv.first_free), cap(strv.cap){
    // 为了使是对str调用析构安全
    strv.elements = strv.cap = strv.first_free = nullptr;
}

StrVec::~StrVec(){
    free();
}

StrVec& StrVec::operator=(const StrVec& rhs){
    if( this != &rhs){
        auto newdata = alloc_n_copy(rhs.begin(), rhs.end() );
        free();
        elements = newdata.first;
        cap = first_free = newdata.second;
    }
    return *this;
}

StrVec& StrVec::operator=(StrVec&& rhs) noexcept{
    if(this != &rhs){
        free();
        elements = rhs.elements;
        first_free = rhs.first_free;
        cap = rhs.cap;

        rhs.elements = rhs.first_free = rhs.cap = nullptr;
    }
    return *this;
}

// void StrVec::reallocate(){
//     auto newcapacity = size() ? 2* size(): 1;
//     auto newdata = alloc.allocate(newcapacity);
//     auto dest = newdata;
//     auto elem = elements;
//     for(size_t i = 0; i < size(); ++i){
//         alloc.construct(dest++, std::move(*elem++));
//     } 
//     free();
//     elements = newdata;
//     cap = elements + newcapacity;
//     first_free = dest;
// }


void StrVec::reallocate(){
    auto newcapacity = size() ? 2* size(): 1;
    auto newdata = alloc.allocate(newcapacity);

    auto last = uninitialized_copy(make_move_iterator(begin()),make_move_iterator(end()), newdata);
    free();
    elements = newdata;
    cap = elements + newcapacity;
    first_free = last;
}

#endif //STRVEC_H
