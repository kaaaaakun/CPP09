#include "BitcoinExchange.hpp"

//-- -- --  -- BitcoinExchange -- -- --  -- //
// BitcoinExchange canonical form
BitcoinExchange::BitcoinExchange(const std::string csvDataFile)
    : dataBase_(DataBase(csvDataFile)) {}
BitcoinExchange::~BitcoinExchange() {}
BitcoinExchange::BitcoinExchange(const BitcoinExchange& other)
    : dataBase_(other.dataBase_) {}
BitcoinExchange& BitcoinExchange::operator=(const BitcoinExchange& other) {
  if (this == &other) return *this;
  this->dataBase_ = other.dataBase_;
  return *this;
}

void BitcoinExchange::PrintResult(
    const std::pair<std::string, std::string> pair,
    const std::pair<std::string, std::string> result) const {
  double exchangePrice =
      utils::StrToDouble(pair.second) * utils::StrToDouble(result.second);

  std::cout << pair.first << " => " << pair.second << " = " << exchangePrice
            << std::endl;
}

// public
void BitcoinExchange::Exchange(std::string inputFile) {
  std::ifstream input(inputFile);
  std::string line;
  std::pair<std::string, std::string> pair, result;

  if (!input)
    throw std::invalid_argument("Error: Cannot open file " + inputFile);
  std::getline(input, line);
  if (utils::IsValidHeader(line, "|", "date", "value") == false)
    throw std::invalid_argument("Error: Invalid file header : " + line);
  while (std::getline(input, line)) {
    try {
      if (utils::SetLine(line, "|", pair) == false)
        throw std::invalid_argument("Error: Invalid line format : " + line);
      if (utils::CheckData(pair) == false)
        throw std::invalid_argument("Error: too large a number : " +
                                    pair.second);
      result = dataBase_.LowerBound(pair.first);
      PrintResult(pair, result);
    } catch (std::exception& e) {
      std::cerr << e.what() << std::endl;
    }
  }
}

//-- -- --  -- DataBase -- -- --  -- //

// DataBase canonical form
DataBase::DataBase(std::string dataFile) : dataFile_(dataFile) {
  std::pair<std::string, std::string> pair;
  std::ifstream file(dataFile_);
  std::string line;

  try {
    if (!file)
      throw std::invalid_argument("Error: Cannot open file : " + dataFile_);
    std::getline(file, line);
    if (utils::IsValidHeader(line, ",", "date", "exchange_rate") == false)
      throw std::invalid_argument("Error: Invalid file header : " + line);
    while (std::getline(file, line)) {
      if (utils::SetLine(line, ",", pair) == false)
        throw std::invalid_argument("Error: Invalid file format : " + line);
      utils::CheckData(pair);
      dataBase_map_.insert(pair);
    }
    size_ = dataBase_map_.size();
  } catch (std::exception& e) {
    std::string msg = "DataBase ";
    msg.append(e.what());
    throw std::invalid_argument(msg);
  }
}
DataBase::~DataBase() {}
DataBase::DataBase(const DataBase& other) {
  if (this == &other) return;
  *this = other;
}
const DataBase& DataBase::operator=(const DataBase& other) {
  if (this == &other) return *this;
  this->dataBase_map_ = other.dataBase_map_;
  this->dataFile_ = other.dataFile_;
  return *this;
}

// https://en.cppreference.com/w/cpp/algorithm/lower_bound の実装を参考に
const std::pair<std::string, std::string> DataBase::LowerBound(
    std::string key) const {
  std::map<std::string, std::string>::const_iterator it;
  it = dataBase_map_.lower_bound(key);
  return (make_pair(it->first, it->second));



  std::map<std::string, std::string>::const_iterator first =
      dataBase_map_.begin();
  std::string comp;
  size_t step;
  size_t count = size_;

  while (0 < count) {
    it = first;
    step = count / 2;
    std::advance(it, step);
    if (it->first < key) {
      first = it;
      count -= step + 1;
    } else
      count = step;
  }
  if (key < first->first)
    throw std::invalid_argument("Error: long time ago : " + key);
  return (make_pair(first->first, first->second));
}

//-- -- --  -- utils -- -- --  -- //
namespace utils {
// stodの実装
double StrToDouble(std::string str) {
  std::stringstream ss(str);
  double ret;
  ss >> ret;
  return ret;
}
// stodのエラー処理
bool IsValidDouble(std::string str) {
  int count = 0;

  while (str[count] != '\0' && str[count] != '.') {
    if (!isdigit(str[count])) return false;
    count++;
  }
  if (str[count] == '\0')
    return true;
  else if (str[++count] == '\0')
    return false;
  else {
    while (str[count] != '\0') {
      if (!isdigit(str[count])) return false;
      count++;
    }
  }
  return true;
}
// 日付のエラー処理
bool IsValidDate(std::string dateStr) {
  int year, month, day;

  if (CheckDateFormat(dateStr) == false) return false;
  SetDate(dateStr, year, month, day);
  switch (month) {
    case 1:
    case 3:
    case 5:
    case 7:
    case 8:
    case 10:
    case 12:
      if (1 <= day && day <= 31) return true;
      break;
    case 4:
    case 6:
    case 9:
    case 11:
      if (1 <= day && day <= 30) return true;
      break;
    case 2:
      return IsFeb(year, day);
  }
  return false;
}

bool CheckDateFormat(std::string str) {
  for (int i = 0; i < 4; i++) {
    if (!isdigit(str[i])) return false;
  }
  if (str[4] != '-') return false;
  for (int i = 5; i < 7; i++) {
    if (!isdigit(str[i])) return false;
  }
  if (str[7] != '-') return false;
  for (int i = 8; i < 10; i++) {
    if (!isdigit(str[i])) return false;
  }
  if (str[10] != '\0') return false;
  return true;
}

void SetDate(const std::string& dateStr, int& year, int& month, int& day) {
  std::stringstream ssYear(dateStr.substr(0, 4));
  ssYear >> year;
  std::stringstream ssMonth(dateStr.substr(5, 2));
  ssMonth >> month;
  std::stringstream ssDay(dateStr.substr(8, 2));
  ssDay >> day;
}

bool IsFeb(int year, int day) {
  if (1 <= day && day <= 28)
    return true;
  else if (29 < day)
    return false;
  // うるう年の判定
  if ((year % 4 == 0 && year % 100 != 0) || year % 400 == 0) return true;
  return false;
}

// 空白をスキップする
std::string SkipSpace(std::string s) {
  const std::string whitespace = " \t\f\v\n\r";
  size_t pos_first = s.find_first_not_of(whitespace);
  size_t pos_last = s.find_last_not_of(whitespace);

  if (pos_first == std::string::npos || pos_last == std::string::npos)
    return "";
  return s.substr(pos_first, pos_last - pos_first + 1);
}

bool SetLine(std::string line, std::string delimiter,
             std::pair<std::string, std::string>& pair) {
  size_t pos = line.find(delimiter);
  if (pos == std::string::npos) return false;
  pair.first = utils::SkipSpace(line.substr(0, pos));
  pair.second = utils::SkipSpace(line.substr(pos + 1));
  if (pair.second == "") return false;
  return true;
}

bool IsValidHeader(std::string line, std::string delimiter, std::string key,
                   std::string value) {
  std::pair<std::string, std::string> header;
  if (SetLine(line, delimiter, header) == false) return false;
  if (header.first != key || header.second != value) return false;
  return true;
}

bool CheckData(const std::pair<std::string, std::string>& pair) {
  double price;

  if (IsValidDate(pair.first) == false)
    throw std::invalid_argument("Error: Invalid date : " + pair.first);
  if (IsValidDouble(pair.second) == false)
    throw std::invalid_argument("Error: Invalid price : " + pair.second);
  price = StrToDouble(pair.second);
  if (1000 < price) return false;
  return true;
}

}  // namespace utils
