#include "xg.hpp"
#include <iostream>

int main(){
    auto g = xg::newGuid();
    std::cout << "Here is a guid: " << g << std::endl;

    return 0;
}
