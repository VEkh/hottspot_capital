#ifndef ETRADE__STOCK_BOT_initialize
#define ETRADE__STOCK_BOT_initialize

#include "fetch_account_balance.cpp" // fetch_account_balance
#include "lib/formatted.cpp"         // Formatted::error_message
#include "stock_bot.h"               // ETrade::StockBot, quantity, symbol
#include <locale.h>                  // setlocale
#include <stdexcept>                 // std::invalid_argument

void ETrade::StockBot::initialize(char *symbol_, int quantity_mulitiplier_,
                                  std::map<std::string, std::string> &flags) {
  if (symbol_ == nullptr) {
    std::string message = Formatted::error_message("Must provide a symbol");
    throw std::invalid_argument(message);
  }

  if (!quantity_mulitiplier_) {
    std::string message =
        Formatted::error_message("Quantity must be at least 1");

    throw std::invalid_argument(message);
  }

  // Support comma separation in print output
  setlocale(LC_NUMERIC, "");

  this->FLAG_MARTINGALE = flags["martingale"] == "1";
  this->FLAG_NORMALIZE_QUANTITY = flags["normalize-quantity"] == "1";
  this->account_balance = this->original_account_balance =
      fetch_account_balance();
  this->symbol = symbol_;
  this->quantity_mulitiplier = quantity_mulitiplier_;
  this->quantity = quantity_mulitiplier_;
}

#endif
