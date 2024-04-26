#pragma once

#include <memory>

// compat reason
#define CPPHTTPLIB_OPENSSL_SUPPORT
#include <cpp-httplib/httplib.h>

#define CURL_STATICLIB
#include <curl/curl.h>

#include "../../Utils/TextParse.h"

class HttpClient
{
public:
    struct HttpResult
    {
        CURLcode ErrCode;
        CURLproxycode ProxyErrCode;
        httplib::Response Response;
    };
public:
    HttpClient(std::string target_url);
    ~HttpClient() {
        curl_easy_cleanup(m_curl);
    }

    void SetSocks5(std::string ip, std::string username, std::string password) {
        m_socks_url = ip;
        m_socks_username = username;
        m_socks_password = password;

        if (!username.empty()) {
            curl_easy_setopt(m_curl, CURLOPT_PROXY, ("socks5://" + username + ":" + password + "@" + ip).c_str());
            m_socks_username = username;
            m_socks_password = password;
        }
        else if (!ip.empty()) {
            curl_easy_setopt(m_curl, CURLOPT_PROXY, ("socks5://" + ip).c_str());
        }
    }

    void SetNewUrl(std::string url) {
        m_target_url = url;
    }

    HttpResult Post(
        const std::string& url_path,
        const std::string& body,
        const httplib::Headers& header,
        const std::string& content_type
    );
    HttpResult Get(const std::string& url_path, const httplib::Headers& header);

    std::string GetSocksUrl() { return m_socks_url; }

private:
    std::string m_user_agent = "UbiServices_SDK_2022.Release.9_PC64_ansi_static";

    std::string m_target_url;

    std::string m_socks_url, m_socks_username, m_socks_password;

    CURL* m_curl;
};

