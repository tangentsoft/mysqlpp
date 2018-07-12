#include <mysql++.h>

int main()
{
    mysqlpp::String greeting("Hello, world!");
    std::cout << greeting << std::endl;
    return 0;
}
