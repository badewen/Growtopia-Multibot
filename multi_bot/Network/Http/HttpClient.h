#pragma once

#include <memory>

#define CPPHTTPLIB_OPENSSL_SUPPORT
#include <cpp-httplib/httplib.h>

class HttpClient
{
public:
    HttpClient(std::string url)
        : m_client_obj {std::move(std::make_unique<httplib::Client>(url))},
        m_current_url {url}
    {
        m_client_obj->enable_server_certificate_verification(false);
    }

    std::string SetNewUrl(std::string url) { 
        m_client_obj = std::make_unique<httplib::Client>( url );
    }

    httplib::Result Post(
        const std::string& url_path,
        const std::string& body,
        const httplib::Headers& header,
        const std::string& content_type
    );
    httplib::Result Get(const std::string& url_path, const httplib::Headers& header);

private:
    std::string m_user_agent = "UbiServices_SDK_2022.Release.9_PC64_ansi_static";
    
    std::string m_current_url;

    std::unique_ptr<httplib::Client> m_client_obj;
};

