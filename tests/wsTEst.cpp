#include <websocketpp/config/asio_client.hpp>
#include <websocketpp/client.hpp>
#include <iostream>
#include <nlohmann/json.hpp>
#include <fstream>
#include <chrono>

typedef websocketpp::client<websocketpp::config::asio_tls_client> client;
typedef websocketpp::lib::shared_ptr<websocketpp::lib::asio::ssl::context> context_ptr;
using namespace std::placeholders;
using json = nlohmann::json;

std::string getCurrentTimeInMillis() {
    auto now = std::chrono::system_clock::now();
    auto duration = now.time_since_epoch();
    auto millis = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();
    return std::to_string(millis);
}

context_ptr on_tls_init(const char*, websocketpp::connection_hdl) {
    context_ptr ctx = websocketpp::lib::make_shared<boost::asio::ssl::context>(boost::asio::ssl::context::sslv23);
    ctx->set_options(boost::asio::ssl::context::default_workarounds |
                     boost::asio::ssl::context::no_sslv2 |
                     boost::asio::ssl::context::no_sslv3 |
                     boost::asio::ssl::context::single_dh_use);
    return ctx;
}
void parse_s_and_c(const std::string& input) {
    size_t pos = 0;
    while (pos < input.size()) {
        // Find next '{' and '}'
        size_t start = input.find('{', pos);
        if (start == std::string::npos) break;
        size_t end = input.find('}', start);
        if (end == std::string::npos) break;

        // Extract one JSON block
        std::string block = input.substr(start, end - start + 1);

        // Find "s" in block
        size_t s_pos = block.find("\"s\"");
        if (s_pos != std::string::npos) {
            size_t s_colon = block.find(':', s_pos);
            size_t s_quote_start = block.find('"', s_colon);
            size_t s_quote_end = block.find('"', s_quote_start + 1);
            std::string s_value = block.substr(s_quote_start + 1, s_quote_end - s_quote_start - 1);

            // Find "c" in block
            size_t c_pos = block.find("\"c\"");
            size_t c_colon = block.find(':', c_pos);
            size_t c_quote_start = block.find('"', c_colon);
            size_t c_quote_end = block.find('"', c_quote_start + 1);
            std::string c_value = block.substr(c_quote_start + 1, c_quote_end - c_quote_start - 1);
            
            // std::cout << "Symbol: " << s_value << ", Close Price: " << c_value << std::endl;
        }

        pos = end + 1; // move to next block
    }
}

void on_message(websocketpp::connection_hdl, client::message_ptr msg) {
    std::cout << "Received: " << msg->get_payload() << std::endl;
}

void on_open(websocketpp::connection_hdl hdl, client* c) {
    json subscribe_message = {
        {"method", "SUBSCRIBE"},
        {"params", {"!miniTicker@arr"}},
        {"id", 1}
    };
    // <symbol>@miniTicker
    std::string payload = subscribe_message.dump();
    websocketpp::lib::error_code ec;
    client::connection_ptr con = c->get_con_from_hdl(hdl, ec);
    if (!ec) c->send(con, payload, websocketpp::frame::opcode::text);
}

int main() {
    client c;
    std::string uri = "wss://stream.binance.com:9443/ws";

    c.init_asio();
    c.set_tls_init_handler(bind(&on_tls_init, "", ::_1));
    c.set_open_handler(bind(&on_open, ::_1, &c));
    c.set_message_handler(&on_message);

    websocketpp::lib::error_code ec;
    client::connection_ptr con = c.get_connection(uri, ec);

    if (ec) {
        std::cout << "could not create connection because: " << ec.message() << std::endl;
        return 0;
    }

    c.connect(con);
    c.run();
}
