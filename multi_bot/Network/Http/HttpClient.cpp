#include "HttpClient.h"

#include <chrono>

using namespace std::chrono_literals;

size_t writefunc(void* ptr, size_t size, size_t nmemb, std::string* s)
{
    size_t realsize = size * nmemb;

    char* buffer = (char*)malloc(realsize + 1);
    
    assert(buffer);

    if (buffer) {
        memcpy(buffer, ptr, realsize);

        buffer[realsize] = 0;

        s->append(buffer);

        free(buffer);
    }
    return realsize;
}

HttpClient::HttpClient(std::string target_url)
    : m_target_url{ target_url }
{
    m_curl = curl_easy_init();
    //curl_easy_setopt(m_curl, CURLOPT_VERBOSE, true);
    curl_easy_setopt(m_curl, CURLOPT_HEADER, 1);
    curl_easy_setopt(m_curl, CURLOPT_SSL_VERIFYPEER, false);
    //curl_easy_setopt(m_curl, CURLOPT_SSL_VERIFYSTATUS, false);
    curl_easy_setopt(m_curl, CURLOPT_SSL_VERIFYHOST, false);
    curl_easy_setopt(m_curl, CURLOPT_PROXY_SSL_VERIFYPEER, false);
    //curl_easy_setopt(m_curl, CURLOPT_USERAGENT, m_user_agent);
    curl_easy_setopt(m_curl, CURLOPT_WRITEFUNCTION, &writefunc);
}

HttpClient::HttpResult HttpClient::Post(
    const std::string& url_path,
    const std::string& body,
    const httplib::Headers& header,
    const std::string& content_type
) {
    httplib::Headers final_header{ header };

    std::string response{};

    final_header.emplace("User-Agent", m_user_agent);

    final_header.emplace("Content-Type", content_type);

    curl_slist* header_list = nullptr;

    for (auto header : final_header) {
        header_list = curl_slist_append(header_list, (header.first + ": " + header.second).c_str());
    }

    curl_easy_setopt(m_curl, CURLOPT_HTTPHEADER, header_list);
    curl_easy_setopt(m_curl, CURLOPT_POST, 1);
    curl_easy_setopt(m_curl, CURLOPT_POSTFIELDSIZE, body.size());
    curl_easy_setopt(m_curl, CURLOPT_POSTFIELDS, body.c_str());

    curl_easy_setopt(m_curl, CURLOPT_URL, (m_target_url + url_path).c_str());
    curl_easy_setopt(m_curl, CURLOPT_WRITEDATA, &response);

    HttpClient::HttpResult ret_result{};


    ret_result.ErrCode = curl_easy_perform(m_curl);

    if (ret_result.ErrCode != CURLE_OK) {
        curl_slist_free_all(header_list);

        if (ret_result.ErrCode == CURLE_PROXY) {
            curl_easy_getinfo(m_curl, CURLINFO_PROXY_ERROR, &ret_result.ProxyErrCode);
        }
        return ret_result;
    }

    std::vector<std::string> parsed_response = Utils::TextParse::StringTokenize(response, "\r\n");

    httplib::Response ret_response = {};

    std::vector<std::string> http_status_code_reason = Utils::TextParse::StringTokenize(parsed_response[0], " ");
    ret_response.version = http_status_code_reason[0];
    ret_response.status = std::stoi(http_status_code_reason[1]);
    ret_response.reason = Utils::TextParse::StringVectorToRaw({ http_status_code_reason.begin() + 2, http_status_code_reason.end() - 1 }, " ");

    bool is_header = true;
    std::vector<std::string> res_body_vec = {};

    for (uint64_t i = 1; i < parsed_response.size(); i++) {
        // set is_header to false if it encounters the first new empty line.
        if (is_header && parsed_response.at(i).empty()) {
            is_header = false;
            continue;
        }
        if (is_header) {
            // not very safe
            std::vector<std::string> header = Utils::TextParse::StringTokenize(parsed_response[i], ": ");

            ret_response.set_header(header[0], header[1]);
        }
        else {
            res_body_vec.push_back(parsed_response[i]);
        }
    }

    ret_response.body = Utils::TextParse::StringVectorToRaw(res_body_vec, "\r\n");
    ret_response.content_length_ = std::stoi(ret_response.get_header_value("Content-Length").empty() ? "0" : ret_response.get_header_value("Content-Length"));

    ret_result.Response = ret_response;

    curl_slist_free_all(header_list);

    return ret_result;
}

HttpClient::HttpResult HttpClient::Get(const std::string& url_path, const httplib::Headers& header) {
    httplib::Headers final_header{ header };

    std::string response{};

    final_header.emplace("User-Agent", m_user_agent);

    curl_slist* header_list = nullptr;

    for (auto header : final_header) {
        header_list = curl_slist_append(header_list, (header.first + ": " + header.second).c_str());
    }

    curl_easy_setopt(m_curl, CURLOPT_HTTPHEADER, header_list);
    curl_easy_setopt(m_curl, CURLOPT_HTTPGET, 1);

    curl_easy_setopt(m_curl, CURLOPT_URL, m_target_url + url_path);
    curl_easy_setopt(m_curl, CURLOPT_WRITEDATA, &response);

    HttpClient::HttpResult ret_result{};

    ret_result.ErrCode = curl_easy_perform(m_curl);

    if (ret_result.ErrCode != CURLE_OK) {
        curl_slist_free_all(header_list);

        if (ret_result.ErrCode == CURLE_PROXY) {
            curl_easy_getinfo(m_curl, CURLINFO_PROXY_ERROR, &ret_result.ProxyErrCode);
        }
        return ret_result;
    }

    std::vector<std::string> parsed_response = Utils::TextParse::StringTokenize(response, "\r\n");

    httplib::Response ret_response = {};

    std::vector<std::string> http_status_code_reason = Utils::TextParse::StringTokenize(parsed_response[0], " ");
    ret_response.version = http_status_code_reason[0];
    ret_response.status = std::stoi(http_status_code_reason[1]);
    ret_response.reason = Utils::TextParse::StringVectorToRaw({ http_status_code_reason.begin() + 2, http_status_code_reason.end() - 1 }, " ");

    bool is_header = true;
    std::vector<std::string> res_body_vec = {};

    for (uint64_t i = 1; i < parsed_response.size(); i++) {
        if (is_header && parsed_response.at(i).empty()) {
            is_header = false;
            continue;
        }
        if (is_header) {
            // not very safe
            std::vector<std::string> header = Utils::TextParse::StringTokenize(parsed_response[i], ": ");

            ret_response.set_header(header[0], header[1]);
        }
        else {
            res_body_vec.push_back(parsed_response[i]);
        }
    }

    ret_response.body = Utils::TextParse::StringVectorToRaw(res_body_vec, "\r\n");
    ret_response.content_length_ = std::stoi(ret_response.get_header_value("Content-Length"));

    ret_result.Response = ret_response;

    return ret_result;
}