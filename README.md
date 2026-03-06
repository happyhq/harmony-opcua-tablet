# OpenHarmony OPC UA Tablet (NAPI + open62541)

目标：OpenHarmony 4.1.1 平板端，OPC UA 读写工具 UI+逻辑骨架（NAPI + open62541 单文件）。

## 功能
- 表单：IP / 端口 / 用户名 / 密码 / NodeId
- 类型选择：String / Int / Float / Boolean
- 读/写：手动按钮
- 结果展示：状态 + 读取值
- 历史记录：默认 200 条，可清空

## 结构
```
entry/src/main/ets/
  entryability/EntryAbility.ets
  pages/Index.ets
  common/HistoryStore.ts

entry/src/main/cpp/
  CMakeLists.txt
  napi_init.cpp
  opcua_client.cpp
  opcua_client.h
  third_party/open62541/open62541.c
  third_party/open62541/open62541.h
```

## NAPI 接口
- connect(ip, port, username, password) -> { ok, message }
- read(nodeId) -> { ok, message, value }
- write(nodeId, valueType, value) -> { ok, message }

## 说明
- 已接入 open62541 单文件（v1.4）
- 当前为 UI/交互骨架 + NAPI 框架，需在真实环境下联调服务端
# harmony-opcua-tablet
