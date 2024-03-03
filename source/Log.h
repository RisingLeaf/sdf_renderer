#ifndef PTREE_LOG_H
#define PTREE_LOG_H

#include <string>



namespace Log
{
  void Init(const std::string &logfilepath);

  void Status(const std::string &message);
  void Warning(const std::string &message);
  void Error(const std::string &message);

  void Exit();
};


#endif //PTREE_LOG_H