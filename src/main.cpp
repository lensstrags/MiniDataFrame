#include <iostream>

#include "csvReader.h"

int main() {
    const std::string FILE_PATH = "C:\\Users\\noc-user.SYSTEM\\Desktop\\csv-reader\\data\\sample.csv";
    CsvReader df(FILE_PATH);

    std::cout << "DataFrame truncated: " << std::endl;
    df.PrintDataFrame();
    std::cout << "\n";

    std::cout << "Head truncated: " << std::endl;
    df.PrintHead();
    std::cout << "\n";

    std::cout << "Tail truncated: " << std::endl;
    df.PrintTail();
    std::cout << "\n";

    return 0;
}