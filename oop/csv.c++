#include <csv.h++>

namespace PROJECT_NAME {
    CSV::CSV(char delimiter, char grouper) : delimiter(delimiter), grouper(grouper) {
        if(delimiter == grouper) {
            throw std::invalid_argument("CSV parser should have different delimiter and group, but all of them is set to character: '"s + delimiter + "'");
        }
    }

    [[nodiscard]]
    std::vector<std::vector<std::string>> CSV::parse(const std::string& raw_csv) const {
        std::vector<std::vector<std::string>> csv_table;

        if(raw_csv.empty() || raw_csv == "\n")
            return csv_table;

        for(auto& line : split_string(raw_csv, "\n")) {
            if(line.empty() || line == "\n")
                continue;

            std::vector<std::string> csv_table_row;
            std::stringstream csv_table_cell;

            bool in_group = false;

            for(const auto& character : line) {
                if(character == delimiter && !in_group) {
                    csv_table_row.push_back(csv_table_cell.str());
                    csv_table_cell.str({});
                    csv_table_cell.clear();
                } else if(character == grouper) {
                    in_group = !in_group;
                } else {
                    csv_table_cell << character;
                }
            }

            if(in_group) {
                throw std::runtime_error("CSV table row '"s + line + "' has unclosed group character. Please, add character '" + grouper + "' to close it.");
            }

            if(!csv_table_cell.str().empty()) {
                csv_table_row.push_back(csv_table_cell.str());
                csv_table_cell.str({});
                csv_table_cell.clear();
            }

            csv_table.push_back(csv_table_row);
        }

        return csv_table;
    }

    [[nodiscard]]
    std::vector<std::vector<std::string>> CSV::parse_file(const std::string& filename, bool create_file_if_not_exists) const {
        if(!std::filesystem::exists(filename)) {
            if(create_file_if_not_exists) {
                std::ofstream { filename };
            } else {
                throw std::runtime_error("File '"s + filename + "' cannot be parsed, because it does not exist. If you want to create file instead automatically, please, set the second argument of 'parse_file(..., create_file_if_not_exists) to 'true'.");
            }
        }

        std::fstream csv_input_file {filename };

        if(!csv_input_file.is_open())
            throw std::runtime_error("Cannot open file '"s + filename + "' for reading CSV table for some reason");

        std::string csv_file_content((std::istreambuf_iterator<char>(csv_input_file)),
                                     std::istreambuf_iterator<char>());

        return parse(csv_file_content);
    }

    void CSV::export_file(const std::string& filename, const std::vector<std::vector<std::string>>& csv_table) const {
        std::fstream csv_output_file { filename };

        if(!csv_output_file.is_open())
            throw std::runtime_error("Cannot open file '"s + filename + "' for writing CSV table for some reason");

        for(const auto& csv_table_row : csv_table) {
            for(const auto& csv_table_cell : csv_table_row) {
                csv_output_file << grouper << csv_table_cell << grouper << delimiter;
            }
            csv_output_file << '\n';
        }

        csv_output_file.close();
    }

    [[nodiscard]]
    const char& CSV::get_delimiter() const {
        return delimiter;
    }

    [[nodiscard]]
    const char& CSV::get_grouper() const {
        return grouper;
    }
}