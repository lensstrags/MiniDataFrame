# MiniDataFrame

MiniDataFrame is a lightweight C++ project that reads CSV files and displays them as formatted tabular data in the console. It provides dataframe-style functionality such as column alignment, row and column truncation, and head/tail viewing, while remaining simple and dependency-free.

---
##  Features

- CSV file parsing (simple comma-separated format)
- Column-based tabular data storage
- Automatic column width calculation
- Formatted console output with alignment
- Head and tail view functions
- Row truncation (for large datasets)
- Column truncation with ellipsis separators
- Optional cell truncation

---

## How It Works

- The first row of the CSV is treated as column headers
- Remaining rows are stored as data entries
- Column widths are computed based on header + data values
- Output is formatted using fixed-width console alignment
- Large datasets are condensed using ellipsis-based truncation

---

## Limitations

This project is intentionally simple and does not implement full CSV standards.

- No support for quoted fields (e.g., "a,b")
- No support for escaped commas
- No trimming of whitespace
- Only supports basic CSV structure

---

## Project Structure

```
MiniDataFrame/
├── include/
│   └── CsvReader.h
├── src/
│   ├── CsvReader.cpp
│   └── main.cpp
├── data/
│   └── sample.csv
└── README.md
```

---

## Requirements

- C++11 or newer
- Standard C++ library
- No external dependencies

## Build Instructions

### Compile (Linux / MacOS)

```bash
g++ -std=c++17 src/*.cpp -Iinclude -o bin/mini_dataframe
```

### Compile (Windows - MinGW)

```
g++ -std=c++17 src\*.cpp -Iinclude -o bin\mini_dataframe
```

## Usage Example

```cpp
#include <iostream>

int main() {
    CsvReader df("data/sample.csv");

    df.PrintDataFrame();
    df.PrintHead(5);
    df.PrintTail(5);

    return 0;
}
```

## Sample Output

```text
    Name        Age    Department    ...    Status      Region
0   Alice       24     Engineering   ...    Active      North
1   Bob         31     Marketing     ...    Active      South
2   Carol       27     Finance       ...    Active      East
..  ...         ...    ...           ...    ...         ...
18  Emma        29     Research      ...    Active      West
19  Liam        33     Sales         ...    Active      North

[20 rows x 15 columns]
```

## Future Improvements

- RFC-compliant CSV parsing
- Support for quoted fields
- Data filtering and Selection
- Column statistics

