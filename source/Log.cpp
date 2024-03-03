#include "Log.h"

#include <chrono>
#include <fstream>
#include <iostream>
#include <ctime>
#include <sstream>
#include <iomanip>


namespace Log
{
  std::ofstream logfile;

  void Init(const std::string &logfilepath)
  {
    logfile = std::ofstream(logfilepath);
  }

  std::string TimeStamp()
  {
    auto now = std::chrono::system_clock::now();
    auto in_time_t = std::chrono::system_clock::to_time_t(now);

    std::stringstream ss;
    ss << std::put_time(std::localtime(&in_time_t), "%Y-%m-%d %X");
    return ss.str();
  }

  void Status(const std::string &message)
  {
    const std::string formattedMessage = "[" + TimeStamp() + "]  STATUS: " + message + "\n";
    logfile<<formattedMessage;
    std::cout<<formattedMessage;
  }

  void Warning(const std::string &message)
  {
    const std::string formattedMessage = "[" + TimeStamp() + "] WARNING: " + message + "\n";
    logfile<<formattedMessage;
    std::cout<<formattedMessage;
  }

  void Error(const std::string &message)
  {
    const std::string formattedMessage = "[" + TimeStamp() + "]   ERROR: " + message + "\n";
    logfile<<formattedMessage;
    std::cout<<formattedMessage;
  }

  void Exit()
  {
    logfile.close();
  }
};