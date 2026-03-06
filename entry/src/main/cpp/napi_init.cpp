#include <string>
#include <napi/native_api.h>
#include "opcua_client.h"

static napi_value makeString(napi_env env, const std::string &s) {
  napi_value result;
  napi_create_string_utf8(env, s.c_str(), s.length(), &result);
  return result;
}

static std::string getString(napi_env env, napi_value v) {
  size_t len = 0;
  napi_get_value_string_utf8(env, v, nullptr, 0, &len);
  std::string out;
  out.resize(len);
  napi_get_value_string_utf8(env, v, out.data(), len + 1, &len);
  return out;
}

static napi_value Connect(napi_env env, napi_callback_info info) {
  size_t argc = 4;
  napi_value argv[4];
  napi_get_cb_info(env, info, &argc, argv, nullptr, nullptr);

  std::string ip = argc > 0 ? getString(env, argv[0]) : "";
  std::string port = argc > 1 ? getString(env, argv[1]) : "";
  std::string username = argc > 2 ? getString(env, argv[2]) : "";
  std::string password = argc > 3 ? getString(env, argv[3]) : "";

  std::string msg;
  bool ok = opcua_connect(ip, port, username, password, msg);

  napi_value ret;
  napi_create_object(env, &ret);
  napi_value okVal;
  napi_get_boolean(env, ok, &okVal);
  napi_set_named_property(env, ret, "ok", okVal);
  napi_set_named_property(env, ret, "message", makeString(env, msg));
  return ret;
}

static napi_value Disconnect(napi_env env, napi_callback_info info) {
  std::string msg;
  bool ok = opcua_disconnect(msg);
  napi_value ret;
  napi_create_object(env, &ret);
  napi_value okVal;
  napi_get_boolean(env, ok, &okVal);
  napi_set_named_property(env, ret, "ok", okVal);
  napi_set_named_property(env, ret, "message", makeString(env, msg));
  return ret;
}

static napi_value Read(napi_env env, napi_callback_info info) {
  size_t argc = 1;
  napi_value argv[1];
  napi_get_cb_info(env, info, &argc, argv, nullptr, nullptr);

  std::string nodeId = argc > 0 ? getString(env, argv[0]) : "";
  std::string value;
  std::string msg;
  bool ok = opcua_read(nodeId, value, msg);

  napi_value ret;
  napi_create_object(env, &ret);
  napi_value okVal;
  napi_get_boolean(env, ok, &okVal);
  napi_set_named_property(env, ret, "ok", okVal);
  napi_set_named_property(env, ret, "message", makeString(env, msg));
  napi_set_named_property(env, ret, "value", makeString(env, value));
  return ret;
}

static napi_value Write(napi_env env, napi_callback_info info) {
  size_t argc = 3;
  napi_value argv[3];
  napi_get_cb_info(env, info, &argc, argv, nullptr, nullptr);

  std::string nodeId = argc > 0 ? getString(env, argv[0]) : "";
  std::string valueType = argc > 1 ? getString(env, argv[1]) : "";
  std::string value = argc > 2 ? getString(env, argv[2]) : "";

  std::string msg;
  bool ok = opcua_write(nodeId, valueType, value, msg);

  napi_value ret;
  napi_create_object(env, &ret);
  napi_value okVal;
  napi_get_boolean(env, ok, &okVal);
  napi_set_named_property(env, ret, "ok", okVal);
  napi_set_named_property(env, ret, "message", makeString(env, msg));
  return ret;
}

static napi_value Init(napi_env env, napi_value exports) {
  napi_property_descriptor desc[] = {
    {"connect", 0, Connect, 0, 0, 0, napi_default, 0},
    {"disconnect", 0, Disconnect, 0, 0, 0, napi_default, 0},
    {"read", 0, Read, 0, 0, 0, napi_default, 0},
    {"write", 0, Write, 0, 0, 0, napi_default, 0}
  };
  napi_define_properties(env, exports, sizeof(desc) / sizeof(desc[0]), desc);
  return exports;
}

NAPI_MODULE(opcua_napi, Init)
