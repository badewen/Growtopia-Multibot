#pragma once

#include <string>
#include <vector>

namespace Utils
{
    class TextParse
    {
    public:
        explicit TextParse(std::string text, std::string delim)
            : m_parsed_str{ std::move(StringTokenize(text, delim)) }
        {}

        static inline TextParse Parse(std::string text, std::string delim) { return TextParse{ text, delim }; }

        static std::vector<std::string> StringTokenize(std::string text, std::string delim);
        static std::vector<std::string> StringTokenizeFormatted(std::string text, std::string delim);
        static std::string StringVectorToRaw(std::vector<std::string> vector, std::string seperator);
        
        // value offset is relative to the key index found.
        std::string Get(std::string key, size_t value_offset = 1, std::string seperator = "|");
        void Set(std::string key, std::string value, size_t value_offset = 1, std::string seperator = "|");

        void AddKey(std::string key, std::string seperator);
        void Add(std::string key, std::string value, size_t value_offset = 1, std::string seperator = "|");
    
        // return whole text as a string and seperated by a newline
        std::string GetTextRaw() { return StringVectorToRaw(m_parsed_str, "\n"); }

        std::vector<std::string> GetTextTokenized() { return m_parsed_str; }

    private:
        std::vector<std::string> m_parsed_str;
    };
}
