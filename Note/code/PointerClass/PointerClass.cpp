//行为像指针的类，实现了引用计数

/*
使用动态内存实现了引用计数：
    1. 构造函数为引用计数分配动态内存，并且设置为1；
    2. 拷贝构造函数值拷贝成员值，并且增加引用计数；
    3. 拷贝复制函数将左边对象引用计数减1，为 0 则销毁内存(不能调用析构函数)，
    拷贝右边参数的值，并且增加引用计数。
 */
#include <iostream>
#include <string>

class PointerClass{
    friend void swap(PointerClass &lhs, PointerClass &rhs);
    friend void print(std::ostream &os,const PointerClass &rhs);
public:
    explicit PointerClass(int iv = 0, std::string str = std::string()):\
                    i(iv), ps(new std::string(str)), use(new std::size_t(1)){}
    PointerClass(const PointerClass & rhs):i(rhs.i), ps(rhs.ps), use(rhs.use){
        ++*use;
    }
    ~PointerClass(){
        do_delete();
    }
    PointerClass& operator=(const PointerClass &rhs);
private:
    inline void do_delete();
private:
    int i;
    std::string *ps;
    std::size_t *use;
};

inline void PointerClass::do_delete(){
    if(--*use == 0){
        delete ps;  ps = nullptr;
        delete use; use = nullptr;
    }   
}

void print(std::ostream &os,const PointerClass &rhs){
    os << "\ti: " << rhs.i << std::endl;
    os << "\t*ps: " << *rhs.ps << std::endl;
    os << "\t*use: " << *rhs.use << std::endl;
}

void swap(PointerClass &lhs, PointerClass &rhs){
    using std::swap;
    swap(lhs.ps,rhs.ps);
    swap(lhs.i, rhs.i);
    swap(lhs.use, rhs.use);
}
PointerClass& PointerClass::operator=(const PointerClass &rhs){
    do_delete();
    i = rhs.i;
    ps = rhs.ps;
    use = rhs.use;
    ++*use;

    return *this;
}

int main(){
    PointerClass val1(2,"sssssssss");
    PointerClass val2(3,"hahah^_^");
    PointerClass pval1(val1);

    std::cout << "Before operator=\n" << std::endl;
    std::cout << "Val1: "<< std::endl;
    print(std::cout, val1);

    std::cout << "Pval1: "<< std::endl;
    print(std::cout, pval1);

    std::cout << "Val2: "<< std::endl;
    print(std::cout, val2);

    val1 = val2;

    std::cout << "\nAfter operator=\n" << std::endl;
    std::cout << "Val1: "<< std::endl;
    print(std::cout, val1);

    std::cout << "Pval1: "<< std::endl;
    print(std::cout, pval1);

    std::cout << "Val2: "<< std::endl;
    print(std::cout, val2);

    std::cout << "After Swap" <<std::endl;
    std::cout << "Pval1" <<std::endl;
    swap(pval1, val2);
    print(std::cout, val1);
    print(std::cout, pval1);
    print(std::cout, val2);


    return 0;
}