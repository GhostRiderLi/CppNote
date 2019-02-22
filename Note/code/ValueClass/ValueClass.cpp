//示例一个行为像值的类的几个重要函数
#include <iostream>
#include <string>

class ValueClass{
    friend inline void swap(ValueClass&, ValueClass&);
    friend void print(std::ostream &os, const ValueClass& val);
public:
    explicit ValueClass(int iv = 0, const std::string &s = std::string() ): \
    i(iv),str(new std::string(s)){}

    //调用复制构造函数的是新产生的对象，不存在删除原有的*str 的问题，不同于operator=
    ValueClass(const ValueClass &v): i(v.i),str(new std::string(*v.str)){}

    ~ValueClass(){delete str;}

    ValueClass& operator=(const ValueClass &v);
private:
    int i ;
    std::string *str;
};
/*
优点：
    1. 检测了自赋值的情况，防止自赋值，避免不必要的操作；
    2. 异常安全：如果分配空间不够，拷贝构造函数会抛异常(bad_alloc),但是不会影响类；
    3. 局部类在作用域之外会调用析构函数，不用自己删除。在析构之前交换了tmp.str 和this.str，最终析构的是没有赋值之前的str指向的空间；
缺点：
    1. 直接调用了拷贝构造函数，拷贝一整个类，当指针类型较少的时候，浪费空间太大，可以考虑只拷贝其中的指针成员。
 */
// ValueClass& ValueClass::operator=(const ValueClass &v){

//     if(this != &v){
//         ValueClass tmp(v);
//         std::string *pstr = str;
//         str = tmp.str;
//         tmp.str = pstr;
//         i = v.i;
//     }
//     return *this;
// }

/*
优点：
    1. 检测自赋值；
    2. 异常安全，内存不够new抛异常，但是类没有改变，不会处于不确定状态；
    3. 值赋值指针类型，内存消耗小；
缺点：
    1. 不知道
 */
ValueClass& ValueClass::operator=(const ValueClass &v){

    if(this != &v){
        std::string *pstr = new std::string(*v.str);
        delete str;
        str = pstr;
        i = v.i;
    }

    return *this;
}

void print(std::ostream &os,const ValueClass &val){
    os << "\ti: " << val.i << std::endl;
    os << "\t*str: " << *val.str << std::endl;

}
inline void swap(ValueClass &lhs, ValueClass &rhs){
    using std::swap;
    swap(lhs.str,rhs.str);
    swap(lhs.i, rhs.i);
}
int main(){

    ValueClass val1(2,"sssssssss");
    ValueClass val2(3,"hahah^_^");
    std::cout << "Val1: "<< std::endl;
    print(std::cout, val1);

    std::cout << "Val2: "<< std::endl;
    print(std::cout, val2);

    val1 = val2;
    std::cout << "Val1: "<< std::endl;
    print(std::cout, val1);

    std::cout << "After Swap " << std::endl;
    swap(val1,val2);
    print(std::cout, val1);
    print(std::cout, val2);

    return 0;

}
