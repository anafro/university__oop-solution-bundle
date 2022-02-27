/**
 * -----------------------------------------------
 * CSV Parser
 * -----------------------------------------------
 * Use this module to parse CSV tables
 * into STL containers.
 *
 * @since OOP 1.0.0.0
 * @author Anatoly Frolov - contact@anafro.ru
 */

#pragma once

#include <string>
#include <vector>
#include <utility>
#include <filesystem>
#include <fstream>
#include <streambuf>
#include <utils/strings.h++>

using namespace std::string_literals;

namespace PROJECT_NAME {
    class CSV {
        static inline char DEFAULT_DELIMITER = ',', DEFAULT_GROUPER = '"';
        char delimiter, grouper;
    public:
        /**
         * Creates a new CSV parser with delimiter and grouper symbols (, and " by default)
         * @param delimiter The CSV cell delimiter
         * @param grouper The CSV cell grouper
         */
        CSV(char delimiter = DEFAULT_DELIMITER, char grouper = DEFAULT_GROUPER);

        /**
         * Parses the CSV table stored in the passed string.
         * @param raw_csv Raw CSV table
         * @return The CSV table stored in 2D-vector
         */
        [[nodiscard]]
        std::vector<std::vector<std::string>> parse(const std::string& raw_csv) const;

        /**
         * Parses the CSV table stored in the file.
         * @throws std::runtime_error When file not found and 'create_file_if_not_exists' is set to 'false'
         * @param filename The CSV table file
         * @param create_file_if_not_exists The flag of creating the file if not found
         * @return The CSV table stored in 2D-vector
         */
        [[nodiscard]]
        std::vector<std::vector<std::string>> parse_file(const std::string& filename, bool create_file_if_not_exists = false) const;

        /**
         * Writes the CSV table to the file.
         * @param filename The file name, where the CSV table will be written to
         * @param csv_table The CSV table
         */
        void export_file(const std::string& filename, const std::vector<std::vector<std::string>>& csv_table) const;

        /**
         * Returns the CSV delimiter of this parser.
         * @return The CSV delimiter
         */
        [[nodiscard]]
        const char& get_delimiter() const;

        /**
         * Returns the CSV grouper of this parser.
         * @return The CSV grouper
         */
        [[nodiscard]]
        const char& get_grouper() const;
    };
}