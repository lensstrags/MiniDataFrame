#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>

#include "CsvReader.h"

// Static members
const size_t CsvReader::INDEX_PADDING = 3;
const size_t CsvReader::COLUMN_PADDING = 2;
const size_t CsvReader::ELLIPSIS_WIDTH = 5;
const size_t CsvReader::MAX_ROWS = 10;
const size_t CsvReader::MAX_COLUMNS = 10;
const size_t CsvReader::MAX_CELL_WIDTH = 20;


CsvReader::CsvReader(const std::string& filePath) {
    std::string lineText;       // Stores the current line read from the file
    bool firstRow = true;
    std::vector<std::string> dataHolder;    // Holds a list of each field from lineText
    size_t lineCount = 0;       // Tracks the current line number from filePath

    // Attempts to open file
    std::ifstream inFS(filePath);
    if (!inFS) {
        throw std::ios_base::failure("Error opening file: " + filePath);
    }

    while (std::getline(inFS, lineText)) {
        ++lineCount;
        dataHolder = ParseRow(lineText);

        // Sets column labels
        if (firstRow) {
            columnLabels = dataHolder;      
            firstRow = false;
        }

        // Adds parsed row data to the table
        else {
            // Terminates program if lineText has more fields than column labels
            if (dataHolder.size() > columnLabels.size()) {
                throw std::runtime_error(
                    "Row " + std::to_string(lineCount) +
                    " contains more fields than header row."
                );
            }
            
            // Normalizes each row before adding to dataTable
            CleanData(dataHolder);

            dataTable.push_back(dataHolder);
        }

    }

    SetColumnWidths();
}

void CsvReader::PrintDataFrame(bool truncateRows, bool truncateCols, bool truncateCells) const {
    
    // Defines row printing range
    size_t startRowIndex = 0;
    size_t endRowIndex = dataTable.size();

    // Reassign truncateCols to true or false base on PrintColumnLables logic
    PrintColumnLabels(truncateCols, truncateCells);
    
    // Print all rows when row truncation is disabled
    // or dataTable size is small enough
    if (!truncateRows || dataTable.size() <= MAX_ROWS) {
        for (size_t row = startRowIndex; row < endRowIndex; ++row) {
            std::cout << std::setw(INDEX_PADDING) << row;
            PrintRow(dataTable[row], truncateCols, truncateCells);
            std::cout << "\n";
        }
    }

    // Truncate rows
    else {
        endRowIndex = MAX_ROWS / 2;

        // Prints first five rows
        for (size_t row = startRowIndex; row < endRowIndex; ++row) {
            std::cout << std::setw(INDEX_PADDING) << row;
            PrintRow(dataTable[row], truncateCols, truncateCells);
            std::cout << "\n";
        }

        // Prints ellipsis separator
        PrintSeparator(truncateCols);
        std::cout << "\n";

        // Prints last 5 rows
        startRowIndex = dataTable.size() - endRowIndex;
        endRowIndex = dataTable.size();

        for (size_t row = startRowIndex; row < endRowIndex; ++row) {
            std::cout << std::setw(INDEX_PADDING) << row;
            PrintRow(dataTable[row], truncateCols, truncateCells);
            std::cout << "\n";
        }

    }
   
    std::cout << "\n";
    std::cout << "[" << dataTable.size() << " rows x " << columnLabels.size() << " columns]" << std::endl;
}

void CsvReader::PrintHead(size_t numRows, bool truncateCols, bool truncateCells) const {
    size_t startRowIndex = 0;
    size_t endRowIndex = numRows;

    if (numRows > dataTable.size()) {
        endRowIndex = dataTable.size();
    }

    PrintColumnLabels(truncateCols, truncateCells);

    for (size_t row = startRowIndex; row < endRowIndex; ++row) {
        std::cout << std::setw(INDEX_PADDING) << row;
        PrintRow(dataTable[row], truncateCols, truncateCells);
        std::cout << "\n";
    }
}

void CsvReader::PrintTail(size_t numRows, bool truncateCols, bool truncateCells) const {
    size_t endRowIndex = dataTable.size();

    size_t startRowIndex = 
        (numRows >= dataTable.size())
        ? 0
        : dataTable.size() - numRows;

    PrintColumnLabels(truncateCols, truncateCells);

    for (size_t row = startRowIndex; row < endRowIndex; ++row) {
        std::cout << std::setw(INDEX_PADDING) << row;
        PrintRow(dataTable[row], truncateCols, truncateCells);
        std::cout << "\n";
    }
}

// Private helpers definitions

std::vector<std::string> CsvReader::ParseRow(const std::string& row) const {
    // Stores the current field being extracted
    std::string field;
    
    // Holds all parsed values from the row
    std::vector<std::string> dataHolder;

    // Input stream used to tokenize the row
    std::istringstream inSS(row);

    // Extracts fields separated by commas
    while (std::getline(inSS, field, ',')) {
        dataHolder.push_back(field);
    }

    return dataHolder;
}

void CsvReader::CleanData(std::vector<std::string>& rowToClean) const {
    // Holds the current number of values in the row
    size_t rowSize = rowToClean.size();       

    // Holds the expected number of columns based on header labels
    size_t expectedSize = columnLabels.size();   

    // Appends placeholder values until the row matches the expected size
    while (rowSize < expectedSize) {
        rowToClean.push_back("NaN");
        ++rowSize;
    }
    
}

void CsvReader::SetColumnWidths() {
    // Stores the calculated width for the current column
    size_t currColumnWidth;
    columnWidths.clear();    

    for (size_t i = 0; i < columnLabels.size(); ++i) {
        currColumnWidth = columnLabels[i].size();

        columnWidths.push_back(currColumnWidth);       
        
    }

    for (size_t row = 0; row < dataTable.size(); ++row) {
        for (size_t col = 0; col < dataTable[row].size(); ++col) {
            if (dataTable[row][col].size() > columnWidths[col]) {
                columnWidths[col] = dataTable[row][col].size();
            }
        }
    }
}

size_t CsvReader::GetColumnWidth(size_t index) const {
    // TODO: Add bounds validation
    return columnWidths[index];
}

void CsvReader::PrintColumnLabels(bool& truncateCols, bool truncateCells) const {
        
    // Adds spacing before column labels for table alignment
    std::cout << std::setw(INDEX_PADDING) << "";

    // Disable truncation when columnLabels is small enough
    // and return truncatecols   
    if (columnLabels.size() <= MAX_COLUMNS) {
        truncateCols = false;

        PrintRow(columnLabels, truncateCols, truncateCells);
    }
    
    else {
        PrintRow(columnLabels, truncateCols, truncateCells);
    }
    std::cout << "\n";
}

void CsvReader::PrintRow(const std::vector<std::string>& rowToPrint, bool truncateCols, bool truncateCells) const {
    std::vector<std::string> displayRow = rowToPrint;

    // Defines the printing range
    size_t startColIndex = 0;
    size_t endColIndex = rowToPrint.size();

    if (truncateCells) {
        FormatCells(displayRow);
    }

    if (truncateCols) {
        endColIndex = MAX_COLUMNS / 2;

        // Prints the first five columns
        for (size_t i = startColIndex; i < endColIndex; ++i) {
            std::cout << std::setw(GetColumnWidth(i) + COLUMN_PADDING) << std::left << displayRow[i];
        }

        // Prints ellipsis
        std::cout << std::setw(ELLIPSIS_WIDTH) << "...";

        //Prints the last five columns
        startColIndex = displayRow.size() - endColIndex;
        endColIndex = displayRow.size();

        for(size_t i = startColIndex; i < endColIndex; ++i) {
            std::cout << std::setw(GetColumnWidth(i) + COLUMN_PADDING) << std::left << displayRow[i];
        }
    }

    // Prints all columns
    else {
        for (size_t i = startColIndex; i < endColIndex; ++i) {
            std::cout << std::setw(GetColumnWidth(i) + COLUMN_PADDING) << std::left << displayRow[i];
        }
    }

}

void CsvReader::FormatCells(std::vector<std::string>& row) const {
    size_t numCharToRemove;
    size_t startIndex;

    for (size_t i = 0; i < row.size(); ++i) {

        if (row[i].size() > MAX_CELL_WIDTH) {
            numCharToRemove = row[i].size() - MAX_CELL_WIDTH;

            row[i].erase(row[i].size() - numCharToRemove);            // Removes extra characters
            startIndex = row[i].size() - 3;
            row[i].replace(startIndex, 3, "...");


        }
    }
}

void CsvReader::PrintSeparator(bool truncateCols) const {
    
    size_t i;
    size_t startColIndex = 0;
    size_t endColIndex = MAX_COLUMNS / 2;

    std::cout << std::setw(INDEX_PADDING) << "..";

    if (truncateCols) {

        for (i = startColIndex; i < endColIndex; ++i) {
            std::cout << std::setw(GetColumnWidth(i) + COLUMN_PADDING) << "...";
        }

        std::cout << std::setw(ELLIPSIS_WIDTH) << "...";

        startColIndex = columnLabels.size() - endColIndex;
        endColIndex = columnLabels.size();

        for (i = startColIndex; i < endColIndex; ++i) {
            std::cout << std::setw(GetColumnWidth(i) + COLUMN_PADDING) << "...";
        }
    }
    else {
        endColIndex = columnLabels.size();

        for (i = startColIndex; i < endColIndex; ++i) {
            std::cout << std::setw(GetColumnWidth(i) + COLUMN_PADDING) << "...";
        }
    }
}