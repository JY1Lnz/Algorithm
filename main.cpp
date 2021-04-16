#include <iostream>
#include "AvlTree.h"

void AvlTreeTest()
{
/*
    sample
*/
    AvlTree x;
    for (int i = 1;i <= 15; ++i) x.add(i);
    x.print();
    x.remove(8);
    std::cout << "---------------" << std::endl;
    x.print();
}

int main()
{
    AvlTreeTest();

    return 0;
}