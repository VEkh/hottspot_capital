#if !defined ETRADE__CLIENT_get_access_token
#define ETRADE__CLIENT_get_access_token

#include "client.h"          // ETrade::Client, client_config, stream_format
#include "lib/formatted.cpp" // Formatted
#include <iostream>          // std::cin, std::cout, std::endl
#include <sstream>           // std::stringstream
#include <string>            // std::string

void ETrade::Client::get_access_token() {
  Formatted::fmt_stream_t fmt = stream_format;
  std::string verifier;
  std::stringstream url;

  url << "https://us.etrade.com/e/t/etws/authorize?"
      << "key=" << client_config.oauth_consumer_key << "&token=";

  std::cout << fmt.bold << fmt.cyan;
  std::cout << "Your authorization URL: " << fmt.reset << url.str()
            << std::endl;

  std::cout << fmt.bold << fmt.yellow << std::endl;
  std::cout << "Enter the response verifier: ";
  std::cout << fmt.reset;

  std::cin >> verifier;
  std::cout << std::endl;
}
#endif