#include "HttpClient.h"

httplib::Result HttpClient::Post(
    const std::string& url_path,
    const std::string& body,
    const httplib::Headers& header,
    const std::string& content_type
) {
    httplib::Headers final_header{ header };

    final_header.emplace("User-Agent", m_user_agent);

    return m_client_obj->Post(url_path, final_header);
}

httplib::Result HttpClient::Get(const std::string& url_path, const httplib::Headers& header) {
    httplib::Headers final_header { header };
    
    final_header.emplace("User-Agent", m_user_agent);

    return m_client_obj->Get(url_path, final_header);
}