#include "TextParse.h"

using namespace Utils;

std::vector<std::string> TextParse::StringTokenize(std::string text, std::string delim) {
    std::vector<std::string> ret{};
    size_t prev_found = 0;
    size_t found = text.length();

    // if the delimeter is not found, just return back the whole string.
    if (text.find(delim) == std::string::npos) {
        ret.push_back(text);
        return ret;
    }

    while ((found = text.find(delim)) != std::string::npos) {
        ret.push_back(text.substr(prev_found, found - prev_found));
    }

    return ret;
}

std::string Utils::TextParse::StringVectorToRaw(std::vector<std::string> str_vector) {
    std::string ret {};

    for (const std::string& str : str_vector) {
        ret += str + "\n";
    }

    // remove trailing newline
    ret.pop_back();

    return ret;
}

std::string Utils::TextParse::Get(std::string key, size_t value_offset, std::string seperator) {
    if (key.empty() || !m_parsed_str.size()) {
        return {};
    }

    for (const std::string& str : m_parsed_str) {
        if (str.empty()) {
            continue;
        }

        std::vector<std::string> tokenized_str = StringTokenize(str, seperator);

        for (size_t i = 0; i < tokenized_str.size(); i++) {
            if (tokenized_str.at(i) == key) {
                if (i + value_offset > tokenized_str.size()) {
                    return {};
                }

                return tokenized_str.at(i + value_offset);
            }
        }
    }

    return {};
}

void Utils::TextParse::Set(std::string key, std::string value, size_t value_offset, std::string seperator) {
    if (key.empty() || !m_parsed_str.size()) {
        return;
    }

    for (std::string& str : m_parsed_str) {
        if (str.empty()) {
            continue;
        }

        std::vector<std::string> tokenized_str = StringTokenize(str, seperator);

        for (size_t i = 0; i < tokenized_str.size(); i++) {
            if (tokenized_str.at(i) == key) {
                if (i + value_offset > tokenized_str.size()) {
                    return;
                }

                tokenized_str.at(i + value_offset) = value;
                str = StringVectorToRaw(tokenized_str);
                return;
            }
        }
    }

    return;
}