#include "TextParse.h"

using namespace Utils;

std::vector<std::string> TextParse::StringTokenize(std::string text, std::string delim) {
    std::vector<std::string> ret{};
    size_t cur_pos = 0;
    size_t found = text.length();

    // if the delimeter is not found, just return back the whole string.
    if (text.find(delim) == std::string::npos) {
        ret.push_back(text);
        return ret;
    }

    while ((found = text.find(delim, cur_pos)) != std::string::npos) {
        ret.push_back(text.substr(cur_pos, found - cur_pos));
        cur_pos = found + 1;
    }

    if (cur_pos <= text.length() - 1) {
        ret.push_back(text.substr(cur_pos, cur_pos - text.length()));
    }

    // a quick hack to fix trailing delim
    if (text.substr(text.length() - delim.length(), delim.length()) == delim) {
        ret.push_back("");
    }

    return ret;
}

std::vector<std::string> Utils::TextParse::StringTokenizeFormatted(std::string text, std::string delim) {
    std::vector<std::string> tokenized = StringTokenize(text, delim);
    for (size_t i = 0; i < tokenized.size(); i++) {
        tokenized[i] = "[" + std::to_string(i) + "] " + tokenized[i];
    }

    return tokenized;
}

std::string Utils::TextParse::StringVectorToRaw(std::vector<std::string> str_vector, std::string seperator) {
    std::string ret {};

    for (const std::string& str : str_vector) {
        ret += str + seperator;
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
                str = StringVectorToRaw(tokenized_str, seperator);
                return;
            }
        }
    }

    return;
}

void Utils::TextParse::AddKey(std::string key, std::string seperator) {
    m_parsed_str.push_back(key + seperator);
}

void Utils::TextParse::Add(std::string key, std::string value, size_t value_offset, std::string seperator) {
    AddKey(key, seperator);
    Set(key, value, value_offset, seperator);
}