#if !defined ETRADE__STOCK_BOT_log_quote
#define ETRADE__STOCK_BOT_log_quote

/*
 * ETrade::StockBot
 * etrade_client
 * fmt
 * quote_t
 * quotes
 * sma_t
 * symbol
 */
#include "stock_bot.h"

#include "lib/formatted.cpp"     // Formatted
#include "lib/utils/float.cpp"   // utils::float
#include "lib/utils/integer.cpp" // utils::integer
#include "parse_quote.cpp"       // parse_quote
#include <iomanip>               // std::setprecision
#include <iostream>              // std::cout, std::endl, std::fixed
#include <math.h>                // INFINITY, abs
#include <string>                // std::string
#include <utility>               // std::pair

void ETrade::StockBot::log_quote() {
  Formatted::Stream log_color = fmt.yellow;
  const int ticks = quotes.size();

  if (!ticks) {
    return;
  }

  const quote_t *previous_quote = ticks > 1 ? &(quotes.at(ticks - 2)) : nullptr;
  const quote_t current_quote = quotes.back();
  const sma_t simple_moving_average = current_quote.simple_moving_average;
  const double buy_to_sell_delta_ratio =
      !simple_moving_average.buy_delta && !simple_moving_average.sell_delta
          ? INFINITY
          : abs(simple_moving_average.buy_delta /
                simple_moving_average.sell_delta);

  if (previous_quote) {
    if (current_quote.current_price > previous_quote->current_price) {
      log_color = fmt.green;
    } else if (current_quote.current_price < previous_quote->current_price) {
      log_color = fmt.red;
    }
  }

  std::cout << std::setprecision(2) << std::fixed;
  std::cout << fmt.bold << fmt.underline << log_color;
  std::cout << symbol << " Quote:" << std::endl;
  std::cout << fmt.reset;
  std::cout << fmt.bold << log_color;
  std::cout << "Current Price: "
            << utils::float_::to_currency(current_quote.current_price)
            << std::endl;
  std::cout << "High: " << utils::float_::to_currency(current_quote.high)
            << std::endl;
  std::cout << "Low: " << utils::float_::to_currency(current_quote.low)
            << std::endl;
  std::cout << "Simple Moving Average ("
            << utils::integer_::seconds_to_clock(simple_moving_average.seconds)
            << "): " << utils::float_::to_currency(simple_moving_average.price)
            << std::endl;
  std::cout << "Buy Delta: "
            << utils::float_::to_currency(simple_moving_average.buy_delta)
            << std::endl;
  std::cout << "Sell Delta: "
            << utils::float_::to_currency(simple_moving_average.sell_delta)
            << std::endl;
  std::cout << "Buy to Sell Delta Ratio: " << buy_to_sell_delta_ratio
            << std::endl;
  std::cout << fmt.reset << std::endl;
}

#endif
