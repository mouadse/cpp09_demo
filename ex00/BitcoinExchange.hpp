#ifndef BITCOINEXCHANGE_HPP
#define BITCOINEXCHANGE_HPP

#include <cstdlib>
#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <string>

class BitcoinExchange {
public:
  enum ValueResult { VALUE_OK, VALUE_BAD_INPUT, VALUE_NEGATIVE, VALUE_TOO_LARGE };

private:
  std::map<std::string, float> _data;

  std::string trimWhitespace(const std::string &str) const;
  bool isValidDate(const std::string &date) const;
  ValueResult isValidValue(const std::string &valueStr, float &value) const;
  float getExchangeRate(const std::string &date) const;

public:
  //
  BitcoinExchange();
  BitcoinExchange(const BitcoinExchange &other);
  BitcoinExchange &operator=(const BitcoinExchange &other);
  ~BitcoinExchange();
  
  void parseDataFile(const std::string &filename);
  void processInputFile(const std::string &filename) const;
};

#endif // BITCOINEXCHANGE_HPP
