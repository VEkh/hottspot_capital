#if !defined ETRADE__CLIENT_H
#define ETRADE__CLIENT_H

#include "lib/formatted.cpp" // Formatted::stream, Formatted::fmt_stream_t
#include <map>               // std::map
#include <string>            // std::string

namespace ETrade {
class Client {
public:
  enum debug_t { OFF, ON };

  struct props_t {
    debug_t debug_flag;
  };

  std::string fetch_quote(char *);
  std::string fetch_quote(std::string);
  void fetch_access_token();
  void refresh_token();

  Client();
  Client(props_t);

private:
  struct client_config_t {
    std::string account_id;
  } client_config;

  struct oauth_params_t {
    std::string consumer_key;
    std::string consumer_secret;
    std::string token;
    std::string token_secret;
    std::string verifier;
  } oauth;

  Formatted::fmt_stream_t stream_format = Formatted::stream();
  const char *CONFIG_PATH = "./config/etrade/credentials.json";
  const char *TOKENS_PATH = "./config/etrade/tokens.json";

  props_t props = {
      .debug_flag = debug_t::OFF,
  };

  std::string build_request_header(std::string,
                                   std::map<std::string, std::string>);
  std::string fetch_token(std::string);

  void fetch_request_token();
  void load_client_config();
  void load_token();
  void write_token(std::string);
};
} // namespace ETrade

#endif
