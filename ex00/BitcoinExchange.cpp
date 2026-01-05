#include "BitcoinExchange.hpp"
#include <cctype>

BitcoinExchange::BitcoinExchange() {}

BitcoinExchange::BitcoinExchange(const BitcoinExchange &other)
    : _data(other._data) {}

BitcoinExchange &BitcoinExchange::operator=(const BitcoinExchange &other) {
  if (this != &other) {
    _data = other._data;
  }
  return *this;
}

BitcoinExchange::~BitcoinExchange() {}

std::string BitcoinExchange::trimWhitespace(const std::string &str) const {
  size_t start = str.find_first_not_of(" \t\n\r");
  if (start == std::string::npos) {
    return "";
  }
  size_t end = str.find_last_not_of(" \t\n\r");
  if (start == std::string::npos || end == std::string::npos) {
    return "";
  }
  return str.substr(start, end - start + 1);
}

bool BitcoinExchange::isValidDate(const std::string &date) const {
  if (date.length() != 10 || date[4] != '-' || date[7] != '-') {
    return false;
  }

  for (size_t i = 0; i < date.size(); ++i) {
    if (i == 4 || i == 7) {
      continue;
    }
    if (!std::isdigit(static_cast<unsigned char>(date[i]))) {
      return false;
    }
  }

  int year = std::atoi(date.substr(0, 4).c_str());
  int month = std::atoi(date.substr(5, 2).c_str());
  int day = std::atoi(date.substr(8, 2).c_str());

  if (year < 0 || month < 1 || month > 12 || day < 1 || day > 31) {
    return false;
  }

  int daysInMonth[] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

  if (year % 4 == 0 && (year % 100 != 0 || year % 400 == 0)) {
    daysInMonth[2] = 29;
  }

  return (day <= daysInMonth[month]);
}

float BitcoinExchange::getExchangeRate(const std::string &date) const {
  
  if (_data.empty()) {
    throw std::runtime_error("Exchange rate data is empty.");
  }
  std::map<std::string, float>::const_iterator it = _data.upper_bound(date);
  if (it == _data.begin()) {
    throw std::runtime_error(
        "No exchange rate data available for the given date or earlier.");
  }
  --it;
  return it->second;
}

BitcoinExchange::ValueResult BitcoinExchange::isValidValue(
    const std::string &valueStr, float &value) const {
  if (valueStr.empty()) {
    return BitcoinExchange::VALUE_BAD_INPUT;
  }

  char *endPtr;
  
  double valueDouble = std::strtod(valueStr.c_str(), &endPtr);

  if (valueStr.c_str() == endPtr || *endPtr != '\0') {
    return BitcoinExchange::VALUE_BAD_INPUT;
  }

  if (valueDouble < 0) {
    return BitcoinExchange::VALUE_NEGATIVE;
  }

  if (valueDouble > 1000) {
    return BitcoinExchange::VALUE_TOO_LARGE;
  }

  value = static_cast<float>(valueDouble);
  return BitcoinExchange::VALUE_OK;
}

void BitcoinExchange::parseDataFile(const std::string &filename) {
  std::ifstream dataFile(filename.c_str());
  if (!dataFile.is_open()) {
    throw std::runtime_error("Could not open database file.");
  }
  std::string buffer;
  if (!std::getline(dataFile, buffer)) {
    throw std::runtime_error("Database file is empty.");
  }

  while (std::getline(dataFile, buffer)) {
    std::stringstream ss(buffer);
    std::string date;
    std::string valueStr;

    if (!std::getline(ss, date, ',') || !std::getline(ss, valueStr)) {
      continue;
    }
    _data[trimWhitespace(date)] = std::atof(trimWhitespace(valueStr).c_str());
  }
  dataFile.close();
}

void BitcoinExchange::processInputFile(const std::string &filename) const {
  std::ifstream inputFile(filename.c_str());
  if (!inputFile.is_open()) {
    throw std::runtime_error("could not open file.");
  }
  std::string buffer;
  if (!std::getline(inputFile, buffer)) {
    throw std::runtime_error("empty input file.");
  }
  while (std::getline(inputFile, buffer)) {
    if (buffer.empty()) {
      continue;
    }
    std::string date;
    std::string valueStr;
    size_t pipePos = buffer.find('|');

    if (pipePos == std::string::npos) {
      std::cerr << "Error: bad input => " << buffer << std::endl;
      continue;
    }

    date = trimWhitespace(buffer.substr(0, pipePos));
    valueStr = trimWhitespace(buffer.substr(pipePos + 1));

    if (!isValidDate(date)) {
      std::cerr << "Error: bad input => " << buffer << std::endl;
      continue;
    }
    float value;
    ValueResult result = isValidValue(valueStr, value);
    if (result != VALUE_OK) {
      if (result == VALUE_NEGATIVE) {
        std::cerr << "Error: not a positive number." << std::endl;
      } else if (result == VALUE_TOO_LARGE) {
        std::cerr << "Error: too large a number." << std::endl;
      } else {
        std::cerr << "Error: bad input => " << buffer << std::endl;
      }
      continue;
    }
    try {
      float rate = getExchangeRate(date);
      float res = rate * value;
      std::cout << date << " => " << value << " = " << res << std::endl;
    } catch (const std::exception &e) {
      std::cerr << "Error: " << e.what() << std::endl;
    }
  }
  inputFile.close();
}
