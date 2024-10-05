#include <pre.h>
#include <iostream>

int main()
{
    std::vector<data_point_t> d;

    parse12(d);
    std::cout << "Size1: " << d.size() << "\n";

    d.clear();
    parse12(d);

    std::cout << "Size2: " << d.size() << "\n";
}