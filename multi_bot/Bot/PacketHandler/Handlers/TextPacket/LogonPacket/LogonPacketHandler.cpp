#include "LogonPacketHandler.h"
#include "../../../../../Utils/TextParse.h"
#include "../../../../../Utils/Klv.hpp"
#include "../../../../../Utils/Hash.hpp"

void LogonPacketHandler::Handle() {
    m_bot->GetLoggerPtr()->Debug("building login data");
    Utils::TextParse login_data{ "requestedName|MouseGar", "\n" };

    if (!m_bot->GetLoginData().growid.empty()) {
        login_data = Utils::TextParse("tankIDName|" + m_bot->GetLoginData().growid, "\n");
        login_data.Add("tankIDPass", m_bot->GetLoginData().growid_pass);
        login_data.Add("requestedName", "MouseGar");
    }

    login_data.Add("f", "1");
    login_data.Add("protocol", m_bot->GetLoginData().game_protocol);
    login_data.Add("game_version", m_bot->GetLoginData().game_version);
    login_data.Add("fz", "37836328");
    login_data.Add("cbits", "1056");
    login_data.Add("player_age", "19");
    login_data.Add("GDPR", "1");
    login_data.Add("category", "_-5000");
    login_data.Add("totalPlaytime", "0");
    login_data.Add("meta", m_bot->GetLoginData().meta);
    login_data.Add("platformID", "0,1,1");
    login_data.Add("deviceVersion", "0");
    login_data.Add("country", "us");
    login_data.Add("mac", m_bot->GetLoginData().mac);
    login_data.Add("wk", m_bot->GetLoginData().wk);
    login_data.Add("zf", "-145153251");
    login_data.Add("hash", m_bot->GetLoginData().hash);
    login_data.Add("fhash", "-716928004");
    login_data.Add("rid", m_bot->GetLoginData().rid);
    login_data.Add("lmode", "0");

    if (!m_bot->GetRedirectData().UserID.empty()) {
        login_data.Add("user", m_bot->GetRedirectData().UserID);
        login_data.Add("UUIDToken", m_bot->GetRedirectData().UUIDToken);
        login_data.Add("token", m_bot->GetRedirectData().Token);
        login_data.Add("doorID", m_bot->GetRedirectData().DoorID);
        login_data.Set("lmode", "1");
    }

    login_data.Add("hash2", std::to_string(Utils::Hash::proton(fmt::format("{}RT", m_bot->GetLoginData().mac).c_str())));
    login_data.Add("klv", Utils::generate_klv(std::stoi(m_bot->GetLoginData().game_protocol), m_bot->GetLoginData().game_version, m_bot->GetLoginData().rid));

    Packet login_pkt{ ePacketType::NET_MESSAGE_GENERIC_TEXT, login_data.GetTextRaw() };
    m_bot->SendPacket(login_pkt.CreateToENetPacket());
    m_bot->GetLoggerPtr()->Debug("Sent login data");
}