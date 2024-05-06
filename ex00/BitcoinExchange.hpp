#ifndef BITCOINEXCHANGE_HPP
#define BITCOINEXCHANGE_HPP

#include <cstddef>
#include <exception>
#include <fstream>
#include <functional>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

#define DEFAULT_CSV_DATA_FILE "data.csv"

class DataBase {
 private:
  // members
  std::map<std::string, std::string> dataBase_map_;
  std::string dataFile_;
  size_t size_;
  // private functions

 public:
  // canonical
  DataBase(std::string dataFile);
  ~DataBase();
  DataBase(const DataBase& other);
  const DataBase& operator=(const DataBase& other);

  const std::pair<std::string, std::string> LowerBound(std::string key) const;
};

class BitcoinExchange {
 private:
  // members
  DataBase dataBase_;
  // private functions
  bool CheckData(const std::pair<std::string, std::string>& pair) const;
  bool SetLine(std::string line, std::pair<std::string, std::string>& pair);
  void PrintResult(const std::pair<std::string, std::string> pair,
                   const std::pair<std::string, std::string> result) const;
  bool IsValidBitcoinExchangeHeader(std::ifstream& input);

 public:
  // canonical
  BitcoinExchange(const std::string csvDataFile);
  ~BitcoinExchange();
  BitcoinExchange(const BitcoinExchange& other);
  BitcoinExchange& operator=(const BitcoinExchange& other);

  void Exchange(std::string inputFile);
};

// utils
namespace utils {
// stodの実装
double StrToDouble(std::string str);
// stodのエラー処理
bool IsValidDouble(std::string str);
bool IsFeb(int year, int day);
// 日付のエラー処理
bool IsValidDate(std::string date);
bool CheckDateFormat(std::string str);
void SetDate(const std::string& str, int& year, int& month, int& day);
// 空白をスキップする
std::string SkipSpace(std::string s);
bool SetLine(std::string line, std::string delimiter,
             std::pair<std::string, std::string>& pair);
bool IsValidHeader(std::string line, std::string delimiter, std::string key,
                   std::string value);
}  // namespace utils

#endif  // BITCOINEXCHANGE_HPP
