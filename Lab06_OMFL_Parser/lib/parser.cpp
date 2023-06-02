#include <regex>
#include "parser.h"

using namespace omfl;

const std::regex KEY_REGEX(R"([a-zA-z0-9-_]+)");
const std::regex SECTION_REGEX(R"(^[^.][a-zA-z0-9-_.]+[^.]$)");
const std::regex ARRAY_REGEX(R"(^[[\[\]0-9a-zA-Z, "+-.]+]$)");
const std::regex STRING_REGEX(R"("[^"]+"\s*)");
const std::regex FLOAT_REGEX(R"([+-]?\d+\.\d+)");
const std::regex INT_REGEX(R"([+-]?\d+)");

std::vector<std::string> SplitStringDots(const std::string& str) {
    std::istringstream iss(str);
    std::vector<std::string> tokens;
    std::string token;
    while (std::getline(iss, token, '.')) {
        if (!token.empty())
            tokens.push_back(token);
    }

    return tokens;
}

std::vector<Object> ParseArray(Parser& parser, const std::string& data) {
    std::vector<Object> arr;
    std::stack<std::vector<Object>> validator;

    std::string value;

    for (char c: data) {
        // New Array started
        if (c == '[' && value.empty()) {
            std::vector<Object> temp_arr;
            validator.push(temp_arr);
            continue;
        }

        if (value.empty() && c == ' ') continue;

        // Value ended
        if (c == ',') {
            if (!value.empty()) {
                value = value.substr(0, value.find_last_not_of(" \t") + 1);
                Types t;
                if (std::regex_match(value, INT_REGEX)) t = integer;
                else if (std::regex_match(value, FLOAT_REGEX)) t = floating;
                else if (std::regex_match(value, STRING_REGEX)) t = string;
                else if (std::regex_match(value, ARRAY_REGEX)) t = array;
                else if (value == "true" || value == "false") t = boolean;
                else {
                    parser.is_valid = false;
                    return {};
                }
                if (t == string) value = value.substr(1, value.size() - 2);

                Object obj(t, value);
                validator.top().emplace_back(obj);
                value.clear();
            }

            continue;
        }

        // Array ended
        if (c == ']' && value.find('[') == std::string::npos) {
            if (!value.empty()) {
                value = value.substr(0, value.find_last_not_of(" \t") + 1);
                Types t;
                if (std::regex_match(value, INT_REGEX)) t = integer;
                else if (std::regex_match(value, FLOAT_REGEX)) t = floating;
                else if (std::regex_match(value, STRING_REGEX)) t = string;
                else if (std::regex_match(value, ARRAY_REGEX)) t = array;
                else if (value == "true" || value == "false") t = boolean;
                else return {};

                if (t == string) value = value.substr(1, value.size() - 2);

                Object obj(t, value);
                validator.top().emplace_back(obj);
                value.clear();
            }
            std::vector<Object> temp = validator.top();
            validator.pop();
            if (validator.empty()) arr = temp;
            else validator.top().emplace_back(Object(array, temp));

            continue;
        }

        value += c;
    }
    if (!validator.empty()) {
        parser.is_valid = false;
        return {};
    }

    return arr;
}

Union omfl::parse(const std::filesystem::path& path) {
    std::ifstream file(path, std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "Failed to open file " << path;
        exit(1);
    }

    Parser parser;

    Section last_section = *parser.root;

    while (file.good()) {
        std::string line;
        std::getline(file, line);

        std::string key;
        std::string value;

        bool is_key = true;
        bool is_string_value = false;
        bool is_ready_string = false;
        bool is_array = false;
        bool is_section = false;

        for (char ch: line) {
            // # Comment started
            if ((!is_string_value || is_ready_string) && ch == '#') break;

            // Skip spaces
            if (!is_string_value && ch == ' ') {
                if (!value.empty()) value += ch;
                continue;
            }

            // "STRING" begin or ends
            if (ch == '\"' && !is_array) {
                if (is_string_value) is_ready_string = true;
                is_string_value = true;
                value += ch;
                continue;
            }

            // Splitter Key/Value
            if (ch == '=') {
                is_key = false;
                continue;
            }

            // Section or Array
            if (ch == '[') {
                if (is_key) {
                    is_section = true;
                    is_key = false;
                    continue;
                } else if (is_string_value) {
                    value += ch;
                    continue;
                } else {
                    is_array = true;
                }
            }
            if (ch == ']') {
                if (is_section) {
                    break;
                }
            }

            // Scanning KEY
            if (is_key || is_section) {
                key += ch;
            } else {
                value += ch;
            }
        }

        if (!(key.empty() && value.empty() && !is_section) &&
            !std::regex_match(key, KEY_REGEX) &&
            !(is_section && std::regex_match(key, SECTION_REGEX))) {
            parser.is_valid = false;
        }

        if (!key.empty()) {

            if (is_section) {

                std::vector<std::string> section_path = SplitStringDots(key);

                Section last_ptr = *parser.root;
                for (auto& sect: section_path) {
                    auto iter = last_ptr.m_Sections.find(sect);
                    if (iter != last_ptr.m_Sections.end()) {
                        last_ptr = last_ptr.m_Sections.at(sect);
                    } else {
                        last_ptr.m_Sections[sect] = Section();
                        last_ptr = last_ptr.m_Sections[sect];
                    }
                    /*
                    try {
                        last_ptr = last_ptr.m_Sections.at(sect);
                    } catch (std::out_of_range& exception) {
                        last_ptr.m_Sections[sect] = Section();
                        last_ptr = last_ptr.m_Sections[sect];
                    }
                     */
                }
                last_section = last_ptr;

            } else {
                Types t;
                value = value.substr(0, value.find_last_not_of(" \t") + 1);

                if (is_array) {
                    if (!std::regex_match(value, ARRAY_REGEX)) parser.is_valid = false;
                    std::vector<Object> arr = ParseArray(parser, value);
                    t = array;
                    //last_section->m_KeyValue[key] = Object(t, value);
                    last_section.m_KeyValue[key] = Object(t, arr);
                    //last_section->m_KeyValue[key] = {t, arr};
                    continue;

                } else if (is_string_value) {
                    if (!std::regex_match(value, STRING_REGEX)) parser.is_valid = false;
                    value = value.substr(1, value.rfind('\"') - 1);
                    t = string;

                } else {
                    if (value == "false" || value == "true") {
                        t = boolean;

                    } else if (value.find('.') != std::string::npos) {
                        if (!std::regex_match(value, FLOAT_REGEX)) parser.is_valid = false;
                        t = floating;

                    } else {
                        if (!std::regex_match(value, INT_REGEX)) parser.is_valid = false;
                        t = integer;
                    }
                }


                auto iter = last_section.m_KeyValue.find(key);
                if (iter == last_section.m_KeyValue.end()) {
                    last_section.m_KeyValue[key] = Object(t, value);
                } else {
                    // Redefinition Key
                    parser.is_valid = false;
                }
            }
        }
    }

    Union u(Object(null_obj, ""), *parser.root);
    u.is_valid = parser.is_valid;
    return u;
}


Union omfl::parse(const std::string& str) {
    std::string filename = "sample.omfl";
    std::ofstream file(filename, std::ios::binary);
    file << str;
    file.close();
    std::filesystem::path path = filename;

    return parse(path);
}
