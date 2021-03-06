# 指针与引用

## 引用

引用是指一个变量的别名，对引用的操作就是对其引用对象的操作。 引用在其定义的时候就必须初
始化。 在一些场合能够减少对象的拷贝，从而提升效率。引用分为左值引用和右值引用，分别代表
对左值和右值的引用。

### 左值和右值

当一个对象被用作左值的时候使用的对象的身份，即对象在内存中的位置；当一个对象被当做右值的时候使用的是对象的值(内容)。使用右值的地方可以使用左值来代替，但是反过来不行。

一个判断是否是左值的简便办法是，将值取地址，可以取地址就是左值，不能取地址就是右值。一般来说，临时对象、表达式值、字面值都是右值。

返回左值引用的函数，连同赋值、下标、以及前置递增递减运算符(返回引用)都是返回左值；返回非引用类型的函数，连同算数、关系、位、以及后置递增递减运算符都是右值。

**左值持久，右值短暂**:左值是持久状态的，右值是要么是字面值常量，要么是在表达式求值过程中的临时对象，或者将亡值。

### 引用使用 

+   左值引用就是一般意义上的引用，使用`&`来表示，右值引用是对右值的引用，使用`&&`表示
    。

+   左值引用不能引用字面值以及表达式的结果，但是常量左值引用以及右值引用可以。

+   常量引用可以引用左值和右值，但不能改变其值，右值引用只能引用右值，可以改变右值的值
    。
+   不能定义指向引用的指针，但是存在指针的引用


### 引用限定符

成员函数可以在成员函数后面添加引用限定符来指定`this`可以指向一个左值还是右值，其中`&`表示`this`可以指向左值，申明了`&`的成员函数只能用于左值，`&&`表示`this`可以指向右值，申明了`&&`的函数只能用于右值。

引用限定符只能对(非static)成员函数使用，并且同时出现在申明和定义里面。

对于具有相同函数名和参数列表的两个函数(例如，一个函数的的`const`和非`sonst`版本)，当其中一个使用了引用限定符，则其他的一个必须使用引用限定符；
```C++
class Foo{
public:
    Foo sorted() &&;     //非const版本使用了引用限定符
    Foo sorted() const &; //const版本就必须使用引用限定符，const必须在&之前
    Foo sorted(Cmp*);     //参数列表不一样，不受之前影响
    Foo sorted(Cmp*) const;
}
```

## 指针和引用差别 ##

1. 指针是一个指向对象的对象。但，引用是一个对象的别名，不是一个对象。

2. 指针申明的时候可以不初始化，之后也可以改变。但，引用在申明的时候必须初始化，而且初始化之后不能够修改。

3. 指针的值可以为空，但是引用的值不能为NULL。

4. 可以有const指针，但是没有const引用。

5. 指针和引用的自增(++)运算意义不一样。

6. sizeof引用"得到的是所指向的变量(对象)的大小，而"sizeof指针"得到的是指针本身的大小

7. 指针可以有多级，但是引用只能是一级(没有引用的引用，但是对引用再次应用是合法的，两个引用都是对原始对象的引用)；

8. 作为参数的时候，指针实际上是传递指针的值拷贝，但是引用传递的是对象本身。



## const ##

1. const对象在初始化的时候可以使用非const对象初始化；

2. const对象只在文件内有效。如果想在多个文件中共享const对象，在定义得时候使用如下`extern const val = ？？;`。

    + 其他文件要使用的时候，使用引用申明`extern const val`;


### const 和引用 ###

1. `const int &ref`等同于 `int const &ref`;但是`const int* &ref` 不等同于`int* const &ref`,而是等同于`int const * &ref`。

1. 非const的引用只能引用非const的同类型变量。`type val = 10; type &r = val;`的语义是引用r可以改变其绑定的值。

2. `const type &r1`语义：只是限定了r1是一个对象的const引用，且是不能修改对象，并没有指定对象是否是const的。因此在初始化的时候，这个对象可以是同类型的普通变量、const对象、字面值，表达式解结果，右值。
    ```C
    int i = 32;
    const int j = 21;
    const int &r1 = i;
    const int &r2 = j;
    const int &r3 = 33;
    const int &r4 = i * 2;
    ```
3. 对const对象的引用，只能只用const的引用；const的引用可以引用几乎所有的合法引用。对象是const的不能被修改，引用也不能修改const对象，因此只能是const的引用。但是const的引用只表明引用不能修改被引用的对象，即，该对象可以是任何的合法值。使用一个非常量引用 引用一个常量对象/值是错误的。
    ```c
    const int ci = 1024; //const对象
    const int &r1 = ci; //正确。const引用 引用const对象
    int &r2 = ci;       //错误。非const引用 引用const对象
    ```

4. `const type &r1`绑定的是一个和type类型可以强制转换的类型或者是字面值的时候，引用绑定的是一个临时变量。
    ```C
    double dval = 3.4;
    const int &r = dval;//---> const int temp = dval; const int &r = temp;
    const double &dr = dval + 1.0;//---> const double tmp = dval + 0.1; const double &dr = tmp;
    ```
5. **使用对象地址初始化引用时，引用的类型应该为指针类型(需要主要指针是否为const指针)**
    ```C++
    int val1 = 10;
    const int val2 = 20;

    int &rv1 = &val1;        //错误，使用int类型引用int* 对象
    int const &rv1 = &val1;  //错误，使用int类型引用int* 对象
    int*  &rv1 = &val1;      //错误。使用非const引用 引用右值(不能改变的 变量，近似常量)
    int* const &rv1 = &val1; //正确。int* 的const引用 引用int*的右值。
    /*
    1. const int* 引用 引用 const int*对象； const int * &rv2 = &val2;
    2. const的引用 引用右值；     int * const &rv2 = &val2;
    */
    const int* const &rv2 = &val2; //正确
    ```

### const和指针 ###

1. 顶层const表示指针本身是一个常量，是对指针权限的规定；底层const表示指针所指对象是一个const对象，是对所指对象的的规定。**在指针赋值的时候底层const类型要相同**。 

1. 指向常量的指针(pointer to const),`const type *ptr`：不能用来修改指向对象的值，但是可以修改指针的值，即可以改变指针指向的对象(不一定是const对象)。这里的`const`只是限定了`ptr`的权限，不能修改他所指向的对象，对象但不一定就必须是const的常量，而且指针的值也可以修改(改变指针指向的对象)。

2. 常量指针(const pointer)，`type * const ptr`：里面的const限定了ptr是一个常量，一旦初始化确定之后就不能修改。

3. 在进行对象的拷贝的时候，对象必须有相同的底层const资格，或者两者能够进行换换，一帮非常量可以转换成常量。


## constexpr

1.`constexpr`的目的只是在确认其所申明的变量是常量表达式，需要在编译之前就知道，而且不能修改，而`const`的目的在于限制变量、指针、引用的读写权限。

1. 申明为constexpr的变量，将有编译器来检测变量的值是否是一个常量表达式。P59

2. 一个constexpr的指针的初始值必须是nullptr或者是0，或者是存储在某个固定地址中的对象；

3. 一般来说定义在函数体之外的变量存储的地址固定不变。

3. constexpr仅仅限定一个指针，并不限定指针所指的对象。
    ```C
    const int *p = nullptr; //p是一个指向整形常量的指针-->p做指向的对象，p没有修改权限,这个对象可以是常量，也可以不是
    constexpr int *q = nullptr;//q是一个指向整型变量的常量指针-->q的内容必须在编译之前就知道，也不能修改
    ```


## volatile 《C++Primer》 P757##

`volatile`关键字的意义是，对于申明为`volatile`的变量每次读该变量都从内存中读取，防止编译器优化将其值存在CPU寄存器中导致在编译不能检测变量改变的情况下直接从寄存器读入错误的值。编译不能检测变量改变的情况通常有：

1. 多线程环境下，线程会修改的共享变量标志需要加volatile，当一个线程改变之后其他线程无法知道。

2. 存储器映射的硬件寄存器通常也要加volatile说明，因为每次对它的读写都可能由不同意义。

3. 中断服务程序中修改的供其它程序检测的变量需要加volatile； 






## auto ##

1. 使用auto的时候，顶层const会被忽略，但是底层const会保留。对于指针，推到的时候不会改变指针的权限，但是指针本身变得可变。

## decltype ##

## 强制类型转换 ##

`static_cast`：只要不包含底层const都可以使用；

`dynamic_cast`：

`const_cast`：只能改变对象的底层const，不能改变对象类型。

`reinterpret_cast`






