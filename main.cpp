
#include<string>
using std::string;
#include<iostream>
using std::cout; using std::cin; using std::endl;
#include "class-08.h"


int main (){
    Program p1("first.txt");
    Program p2("second.txt");
    CPU my_cpu({p1,p2});
    my_cpu.run();
}
