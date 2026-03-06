#include "opcua_client.h"

extern "C" {
#include "third_party/open62541/open62541.h"
}

static UA_Client *g_client = nullptr;

static UA_NodeId parseNodeId(const std::string &nodeIdStr, UA_StatusCode &code) {
  UA_NodeId nodeId;
  UA_NodeId_init(&nodeId);
  UA_String s = UA_String_fromChars(nodeIdStr.c_str());
  code = UA_NodeId_parse(&nodeId, s);
  UA_String_clear(&s);
  return nodeId;
}

bool opcua_connect(const std::string &ip, const std::string &port,
                   const std::string &username, const std::string &password,
                   std::string &outMsg) {
  if (ip.empty() || port.empty()) {
    outMsg = "IP/端口不能为空";
    return false;
  }
  if (g_client) {
    UA_Client_disconnect(g_client);
    UA_Client_delete(g_client);
    g_client = nullptr;
  }

  g_client = UA_Client_new();
  UA_ClientConfig *config = UA_Client_getConfig(g_client);
  UA_ClientConfig_setDefault(config);
  if (!username.empty()) {
    UA_ClientConfig_setAuthenticationUsername(config, username.c_str(), password.c_str());
  }

  std::string endpoint = "opc.tcp://" + ip + ":" + port;
  UA_StatusCode status = UA_Client_connect(g_client, endpoint.c_str());
  if (status != UA_STATUSCODE_GOOD) {
    outMsg = "连接失败";
    return false;
  }
  outMsg = "连接成功";
  return true;
}

bool opcua_disconnect(std::string &outMsg) {
  if (!g_client) {
    outMsg = "未连接";
    return false;
  }
  UA_Client_disconnect(g_client);
  UA_Client_delete(g_client);
  g_client = nullptr;
  outMsg = "已断开";
  return true;
}

bool opcua_read(const std::string &nodeIdStr, std::string &outValue, std::string &outMsg) {
  if (!g_client) {
    outMsg = "未连接";
    return false;
  }
  if (nodeIdStr.empty()) {
    outMsg = "NodeId 不能为空";
    return false;
  }

  UA_StatusCode code;
  UA_NodeId nodeId = parseNodeId(nodeIdStr, code);
  if (code != UA_STATUSCODE_GOOD) {
    outMsg = "NodeId 解析失败";
    return false;
  }

  UA_Variant value;
  UA_Variant_init(&value);
  UA_StatusCode status = UA_Client_readValueAttribute(g_client, nodeId, &value);
  UA_NodeId_clear(&nodeId);

  if (status != UA_STATUSCODE_GOOD) {
    outMsg = "读取失败";
    UA_Variant_clear(&value);
    return false;
  }

  if (UA_Variant_isScalar(&value) && value.type) {
    if (value.type == &UA_TYPES[UA_TYPES_STRING]) {
      UA_String *s = (UA_String *)value.data;
      outValue.assign((char *)s->data, s->length);
    } else if (value.type == &UA_TYPES[UA_TYPES_BOOLEAN]) {
      UA_Boolean v = *(UA_Boolean *)value.data;
      outValue = v ? "true" : "false";
    } else if (value.type == &UA_TYPES[UA_TYPES_INT32]) {
      outValue = std::to_string(*(UA_Int32 *)value.data);
    } else if (value.type == &UA_TYPES[UA_TYPES_FLOAT]) {
      outValue = std::to_string(*(UA_Float *)value.data);
    } else if (value.type == &UA_TYPES[UA_TYPES_DOUBLE]) {
      outValue = std::to_string(*(UA_Double *)value.data);
    } else {
      outValue = "[unsupported type]";
    }
  } else {
    outValue = "[non-scalar]";
  }

  UA_Variant_clear(&value);
  outMsg = "读取成功";
  return true;
}

bool opcua_write(const std::string &nodeIdStr, const std::string &valueType,
                 const std::string &value, std::string &outMsg) {
  if (!g_client) {
    outMsg = "未连接";
    return false;
  }
  if (nodeIdStr.empty()) {
    outMsg = "NodeId 不能为空";
    return false;
  }

  UA_StatusCode code;
  UA_NodeId nodeId = parseNodeId(nodeIdStr, code);
  if (code != UA_STATUSCODE_GOOD) {
    outMsg = "NodeId 解析失败";
    return false;
  }

  UA_Variant v;
  UA_Variant_init(&v);

  if (valueType == "String") {
    UA_String s = UA_String_fromChars(value.c_str());
    UA_Variant_setScalar(&v, &s, &UA_TYPES[UA_TYPES_STRING]);
  } else if (valueType == "Int") {
    UA_Int32 i = std::stoi(value);
    UA_Variant_setScalar(&v, &i, &UA_TYPES[UA_TYPES_INT32]);
  } else if (valueType == "Float") {
    UA_Float f = std::stof(value);
    UA_Variant_setScalar(&v, &f, &UA_TYPES[UA_TYPES_FLOAT]);
  } else if (valueType == "Boolean") {
    UA_Boolean b = (value == "true" || value == "1");
    UA_Variant_setScalar(&v, &b, &UA_TYPES[UA_TYPES_BOOLEAN]);
  } else {
    outMsg = "不支持的类型";
    UA_NodeId_clear(&nodeId);
    return false;
  }

  UA_StatusCode status = UA_Client_writeValueAttribute(g_client, nodeId, &v);
  UA_NodeId_clear(&nodeId);
  UA_Variant_clear(&v);

  if (status != UA_STATUSCODE_GOOD) {
    outMsg = "写入失败";
    return false;
  }

  outMsg = "写入成功";
  return true;
}
