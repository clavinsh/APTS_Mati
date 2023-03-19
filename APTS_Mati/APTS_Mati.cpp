#include <iostream>
#include<fstream>

int main()
{
    std::fstream File("hair.in");

    if (!File.is_open()) {
        return -1;
    }
    unsigned int barbers = 0;

    barbers << File.get();

    std::cout << barbers;



    //char c;
    //while (File.get(c))
    //    std::cout << c;
    // 
    // 
    // 
    //std::cout << "Hello World!\n";

    File.close();



    return 0;
}

