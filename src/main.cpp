#include <iostream>
#include "CsvReader.h"

int main() {
    CsvReader df("data/sample.csv");

    df.PrintDataFrame();
    df.PrintHead(5);
    df.PrintTail(5);

    return 0;
}
