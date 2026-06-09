/**
 * @class CsvReader
 * @brief Loads and stores CSV data in a tabular structure for formatted output.
 * 
 * The CsvReader class reads a CSV file from disk and stores its content in
 * an internal table representation. The first row is treated as the header
 * row and defines the column labels. All subsequent rows are treated as data.
 * 
 * Each row is stored as a vector of strings, and the dataset is represented
 * as a 2D vector of strings.
 * 
 * The class supports formatted console output with optional truncation of
 * rows and columns for large datasets.
 * 
 * @features
 * - Separates column labels from data rows.
 * - Computes column widths for aligned tabular printing.
 * - Supports truncated "head/tail" style display for large datasets.
 * - Maintains consistent row structure via padding.
 * 
 * @Limitations
 * - Only supports simple comma-separated values (no RFC-compliant CSV parsing).
 * - Does not support quoted fields or escaped delimiters.
 * - Whitespace is preserved exactly as read (no trimming).
 * 
 * @invariant
 * - columnLabels.size() defines the number of columns.
 * - All rows in dataTable are padded to match columnLabels.size().
 * - columnWidths is derived from columnLabels and dataTable and must be synchronized.
 * 
 * @error_handling
 * - Throws std::ios_base::failure if the file cannot be opened.
 * - Throws std::runtime_error if a row contains more fields than the header row.
 * - Rows with fewer fields are treated as incomplete data and are padded with
 *      placeholder values ("NaN").
 */

 #ifndef CSVREADER_H
 #define CSVREADER_H

 #include <vector>
 #include <string>

 class CsvReader {
    public:
        /**
         * @brief Constructs a CsvReader object and loads data from a CSV file.
         * 
         * Opens and parses a CSV file from the given file path. The first row
         * of the file is treated as the header row and stored as column labels.
         * All subsequent rows are treated as data rows and stored in an internal
         * 2D table structure.
         * 
         * Each row is parsed by splitting on commas. Rows with fewer fields than
         * the header are automatically padded during normalization. Rows with more
         * fields than the header are considered invalid and will cause an exception.
         * 
         * After loading the data, column display widths are computed based on the
         * header and data contents to support aligned table output.
         * 
         * @param filePath Path to the CSV file to load.
         * 
         * @throws std::ios_base::failure if the file cannot be opened.
         * @throws std::runtime_error If a row has more fields than expected.
         */
        CsvReader(const std::string& filePath);

        /**
         * @brief Prints the dataframe in a formatted tabular layout.
         * 
         * Outputs the stored CSV data to standard output using the precomputed
         * column widths for alignment. Column labels are printed first, followed
         * by each data row and a summary of the dataframe dimensions.
         * 
         * When row truncation is enabled and the number of rows exceeds the
         * configured display limit, only the first and last portions of the
         * dataset are shown with an ellipsis separator between them.
         * 
         * When column truncation is enabled and the number of columns exceeds
         * the configured display limit, only the first and last portions of 
         * each row are shown with an ellipsis separator between them.
         * 
         * When cell truncation is enabled and cell values are longer than the
         * configured display limit, values are shortened and displayed with
         * a trailing ellipsis ("..."). This affects only the displayed output
         * and does not modify the underlying stored data.
         * 
         * Example output:
         * 
         * @code
         *     Name     Age     ...     Occupation      Description
         * 0   Alice    24      ...     Engineer        Lorem ipsum dolor...
         * 1   Bob      31      ...     Designer        Lorem ipsum dolor...
         * ..  ...      ...     ...     ...             ...
         * 98  Emma     27      ...     Analyst         Lorem ipsum dolor...
         * 99  Liam     29      ...     Manager         Lorem ipsum dolor...
         * 
         * [100 rows x 25 columns]
         * 
         * @endcode
         * 
         * @param truncateRows
         * If true, row output may be abbreviated when the dataframe exceeds
         * the configured row display limit.
         * 
         * @param truncateCols
         * If true, column output may be abbreviated when the dataframe exceeds
         * the configured column display limit.
         * 
         * @param truncateCells
         * If true, individual cell values may be abbreviated when their length
         * exceeds the configured maximum cell display width.
         * 
         * @assumptions
         * - columnLabels has been initialialized and contains valid column headers.
         * - columnWidths has been computed via SetColumnWidths().
         * - All rows in dataTable have been normalized to match columnLabels.size().
         * 
         * @side_effects
         * - Writes formatted dataframe output to std::cout.
         */
        void PrintDataFrame(bool truncateRows = true, bool truncateCols = true, bool truncateCells = true) const;

        /**
         * @brief Prints the first rows of the dataframe in formatted table form.
         * 
         * Outputs the column labels followed by the first numRows rows of the
         * dataset using precomputed column widths for aligned tabular display.
         * Row indices are printed alongside each row to indicate their position
         * within the dataframe.
         * 
         * If the requested number of rows exceeds the number of available rows,
         * all rows in the dataframe are printed.
         * 
         * When column truncation is enabled and the number of column exceeds
         * the configured display limit, only the first and last portions of
         * each row are displayed with an ellipsis separator between them.
         * 
         * When cell truncation is enabled and cell values are longer than the
         * configured display limit, values are shortened and displayed with
         * a trailling ellipsis ("..."). This affects only the displayed output
         * and does not modify the underlying stored data.
         * 
         * Example output:
         * 
         * @code
         *      Name    Age     ...     Occupation      Description
         * 0    Alice   24      ...     Engineer        Lorem ipsum dolor...
         * 1    Bob     31      ...     Analyst         Lorem ipsum dolor...
         * 2    Carol   27      ...     Designer        Lorem ipsum dolor...
         * 3    David   29      ...     Manager         Lorem ipsum dolor...
         * 4    Emma    26      ...     Scientist       Lorem ipsum dolor...
         * 
         * @endcode
         * 
         * @param numRows
         * Number of rows to print from the beginning of the dataframe.
         * If numRows exceeds the number of available rows, all rows are printed.
         * 
         * @param truncateCols
         * If true, column output may be abbreviated when the dataframe exceeds
         * the configured column display limit.
         * 
         * @param truncateCells
         * If true, individual cell values may be abbreviated when their length
         * exceeds the configured maximum cell diplay width.
         * 
         * @assumptions
         * - columnLabels has been initialized and contains valid headers
         * - columnWidths has been computed via SetColumnWidths().
         * - All rows in dataTable are normalized to match columnLabels.size().
         * 
         * @side_effects
         * - Writes formatted output to std::cout.
         * 
         * @Limitations
         * - Does not print datafrmae dimensions after output.
         * - Does not perform row truncation beyond the numRows limit.
         */
        void PrintHead(size_t numRows = 5, bool truncateCols = true, bool truncateCells = true) const;

        /**
         * @brief Prints the last rows of the dataframe in formatted table form.
         * 
         * Outputs the column labels followed by the last numRows rows of the
         * dataset using precomputed column widths for aligned tabular display.
         * Row indices are printed alongside each row to indicate their position
         * within the dataframe.
         * 
         * If the requested number of rows exceeds the number of available rows,
         * all rows in the dataframe are printed.
         * 
         * When column truncation is enabled and the number of columns exceeds
         * the configured display limit, only the first and last portions of
         * each row are displayed with an ellipsis separator between them.
         * 
         * When cell truncation is enabled and cell values are longer than the
         * configured display limit, values are shortened and displayed with
         * a trailling ellipsis ("..."). This affects only the displayed output
         * and does not modify the underlying stored data.
         * 
         * Example output:
         * 
         * @code
         *      Name    Age     ...     Occupation      Description
         * 95   Noah    32      ...     Engineer        Lorem ipsum dolor...
         * 96   Emma    27      ...     Designer        Lorem ipsum dolor...
         * 97   Sophia  30      ...     Designer        Lorem ipsum dolor...
         * 98   Liam    29      ...     Manager         Lorem ipsum dolor...
         * 99   Olivia  26      ...     Scientist       Lorem ipsum dolor...
         * 
         * @endcode
         * 
         * @param numRows
         * Number of rows to print from the end of the dataframe.
         * If numRows exceeds the number of available rows, all rows are printed.
         * 
         * @param truncateCols
         * If true, column output may be abbreviated when the dataframe exceeds
         * the configured column display limit.
         * 
         * @param truncateCells
         * If true, individual cell values may be abbreviated when their length
         * exceeds the configured maximum cell display width.
         * 
         * @assumptions
         * - columnLabels has been initialized and contains valid headers.
         * - columnWidths has been computed via SetColumnWidths().
         * - All rows in dataTable are normalized to match columnLabels.size().
         * 
         * @side_effects
         * - Writes formatted output to std::cout.
         * 
         * @limitations
         * - Does not print dataframe dimensions after output.
         * - Does not perform row truncation beyond the numRows limit.
         */
        void PrintTail(size_t numRows = 5, bool truncateCols = true, bool truncateCells = true) const;

    private:
        // Data storage
        std::vector<std::string> columnLabels;
        std::vector<std::vector<std::string>> dataTable;
        std::vector<size_t> columnWidths;

        // Static members
        static const size_t INDEX_PADDING;      // Padding added to each row index
        static const size_t COLUMN_PADDING;     // Additional spacing added to each printed column
        static const size_t ELLIPSIS_WIDTH;     // Reserved for the ellipsis separator

        // Holds the maximum number of rows displayed when truncation is enabled
        static const size_t MAX_ROWS;

        // Holds the maximum number of columns displayed when truncation is enabled
        static const size_t MAX_COLUMNS;

        // Holds the maximum cell width when cells truncation is enabled
        static const size_t MAX_CELL_WIDTH;

        // Helper methods

        /**
         * @brief Parses a CSV row into individual fields
         * 
         * Splits a single line of text using comma delimiters and extracts each
         * field into a vector of strings. This parser performs simple delimiter-
         * based splitting and does not support advanced CSV features.
         * 
         * @param row A single line from a CSV file.
         * @return A vector containing the parsed fields from the row.
         * 
         * @assumptions
         * - Fields are separated strictly by commas.
         * - No support for quoted fields (e.g., "a,b" is not treated as one field).
         * - No support for escaped delimiters or special characters.
         * - Input represents a single logical row (no embedded newlines).
         * 
         * @Limitations
         * - Cannot parse RFC-compliant CSV format.
         * - Whitespace is preserved as-is and not trimmed.
         */
        std::vector<std::string> ParseRow(const std::string& row) const;

        /**
         * @brief Normalizes a CSV row to match the expected column count.
         * 
         * Ensures that the input row has the same number of fields as the
         * header row by appending placeholder values ("NaN") for missing entries.
         * This allows all rows in the dataset to maintain consistent structure
         * for table formatting and display.
         * 
         * @param rowToClean Row data to be normalized in-place.
         * 
         * @assumptions
         * - columnLabels has already been initialized and is valid.
         * - The expected number of columns is defined by columnLabels.size().
         * - rowToClean represents a parsed CSV row.
         * - Missing values are acceptable and can be represented as placeholders.
         * 
         * @side_effects
         * - Modifies rowToClean in-place by appending "NaN" values.
         * 
         * @Limitations
         * - Does not handle case where row has more fields than expected.
         *      (This is assumed to be validated elsewhere.)
         */
        void CleanData(std::vector<std::string>& rowToClean) const;

        /**
         * @brief Computes the display width of each column.
         * 
         * Calculates the maximum string width required for each column based on
         * both column labels and all data values. These widths are used to align
         * tabular output when printing the dataframe.
         * 
         * The computed widths are stored in the columnWidths vector.
         * 
         * @assumptions
         * - columnLabels has been initialized and its size defines the number of columns.
         * - Every row in dataTable has at least columnLabels.size() elements.
         * - dataTable and columnLabels are structurally consistent (same column count).
         * 
         * @side_effects
         * - Appends values to columnWidths (does not clear existing contents).
         * 
         * @Limitations
         * - Does not validate consistency between rows and column count.
         * - Assumes data has already been normalized by earlier steps (e.g., CleanData).
         */
        void SetColumnWidths();

        /**
         * @brief Retrieves the display width of a column.
         * 
         * Returns the precomputed width for the specified column index. These
         * widths are calculated in SetColumnWidths() and used for aligning
         * tabular output.
         * 
         * @param index Index of the column.
         * @return The stored display width for the specified column.
         * 
         * @assumptions
         * - SetColumnWidths() has already been called successfully.
         * - index is within the valid range [0, columnWidths.size()].
         * - columnWidths is synchronized with columnLabels.
         * 
         * @Limitations
         * - No bounds checking is performed.
         * - Passing an invalid index results in undefined behavior.
         */
        size_t GetColumnWidth(size_t index) const;

        /** 
         * @brief Prints the column header row.
         * 
         * Outputs column labels using precomputed column widths for aligned
         * tabular formatting. A leading padding column is printed to align
         * headers with row indices.
         * 
         * If the number of columns does not exceed the configured display limit,
         * truncation is disabled automatically.
         * 
         * @param truncateCols
         * Controls whether column truncation is attempted. This value may be
         * modified by the function if truncation is not required.
         * 
         * @param truncateCells
         * If true, individual cell values may be abbreviated when their length
         * exceeds the configured  maximum cell display width.
         * 
         * @assumptions
         * - columnLabels has been initialized and contains valid headers.
         * - MAX_COLUMNS defines the maximum number of columns for truncated display.
         * - PrintRow() correctly formats a full row of column labels.
         * 
         * @side_effects
         * - May modify truncateCols to false.
         * - Writes formatted output to std::cout.
        */
        void PrintColumnLabels(bool& truncateCols, bool truncateCells) const;

        /**
         * @brief Prints a single row of the dataframe in formatted table style.
         * 
         * Outputs each field in the row using precomputed column widths to ensure
         * aligned tabular formatting. If column truncation is enabled and the
         * number of columns exceeds the display limit, only the first and last
         * portions of the row are displayed with an ellipsis separator.
         * 
         * When cell truncation is enabled and cell values are longer than the
         * configured display limit, values are shortened and displayed with
         * a trailling ellipsis ("..."). This affects only the displayed output
         * and does not modify the underlying stored data.
         * 
         * This function creates a local copy of the input row when cell truncation
         * is enabled in order to safely modify display values without affecting the
         * underlying dataset.
         * 
         * @param rowToPrint Row data to print.
         * 
         * @param truncateCols
         * If true, the row may be truncated when it exceeds the configured
         * column display limit.
         * 
         * @param truncateCells
         * If true, individual cell values may be abbreviated when their length
         * exceeds the configured maximum cell display width.
         * 
         * @assumptions
         * - rowToPrint.size() is consistent with columnLabels.size().
         * - GetColumnWidth(i) is valid for all indices accessed.
         * - columnWidths has already been initialized via SetColumnWidths().
         * - MAX_COLUMNS defines an even split-able display limit.
         * 
         * @side_effects
         * - Writes formatted output to std::cout.
         * 
         * @Limitations
         * - No bounds checking is performed on rowToPrint indexing.
         * - Assumes row data is already normalized (no missing/extra columns).
         * - Does not validate consistency between row size and column metadata.
         * 
         */
        void PrintRow(const std::vector<std::string>& rowToPrint, bool truncateCols, bool truncateCells) const;

        /**
         * @brief Applies display truncation to cells in a row.
         * 
         * Iterates through each cell in the provided row and shortens any string
         * that exceeds the configured maximum cell width. Truncated values preserve
         * the beginning of the string and append an ellipsis ("...") to indicate
         * omitted content.
         * 
         * This function modifies the input row in-place and is intended for use
         * during formatted output only. It does not affect the underlying dataset
         * stored in the dataframe.
         * 
         * Truncation behavior:
         * - If a cell's length is less than or equal to MAX_CELL_WIDTH, it is
         *   left unchanged.
         * - If a cell's length exceeds MAX_CELL_WIDTH, it is shortened to fit
         *   within the limit and an ellipsis is inserted at the end.
         * 
         * @param row
         * Row of string values to be formatted. Modified in-place.
         * 
         * @assumptions
         * - MAX_CELL_WIDTH is at least 3 to allow space for the ellipsis.
         * - Each element in row is a valid string.
         * 
         * @side_effects
         * - Modifies the contents of row directly.
         * 
         * @Limitations
         * - Does not account for multibyte or Unicode display width.
         * - Assumes ASCII-style character width equivalence.
         */
        void FormatCells(std::vector<std::string>& row) const;

        /**
         * @brief Prints a separator row for truncated dataframe output.
         * 
         * Outputs a visual separator used to indicate omitted rows or columns.
         * When printing a truncated dataframe. The separator mirrors the column
         * structure using ellipsis placeholders ("...") aligned to column widths.
         * 
         * When column truncation is enabled, the separator prints:
         * - a leading portion of columns,
         * - an ellipsis gap,
         * - and a trailing portion of columns.
         * 
         * When truncation is disabled, the separator spans the full column range
         * using placeholder values.
         * 
         * @param truncateCols
         * If true, the separator is formatted using column truncation rules.
         * 
         * @assumptions
         * - columnLabels has been initialized and defines the column count.
         * - GetColumnWidth(i) is valid for all accessed indices.
         * - MAX_COLUMNS defines the truncation split point.
         * - Output structure matches PrintRow() formatting logic.
         * 
         * @side_effects
         * - Writes formatted separator output to std::cout.
         */
        void PrintSeparator(bool truncateCols) const;
 };

 #endif