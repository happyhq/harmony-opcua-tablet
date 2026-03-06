#pragma once

#include <string>

bool opcua_connect(const std::string &ip, const std::string &port,
                   const std::string &username, const std::string &password,
                   std::string &outMsg);

bool opcua_disconnect(std::string &outMsg);

bool opcua_read(const std::string &nodeId, std::string &outValue, std::string &outMsg);

bool opcua_write(const std::string &nodeId, const std::string &valueType,
                 const std::string &value, std::string &outMsg);
