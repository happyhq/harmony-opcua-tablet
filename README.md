# HarmonyOS OPC UA Tablet (Mock UI)

目标：HarmonyOS 4.1.1 平板端，OPC UA 读写工具 UI+逻辑骨架（模拟数据）。

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
  common/MockOpcUa.ts
  common/HistoryStore.ts
```

## 下一步（接入真实 OPC UA）
- 替换 `MockOpcUaClient` 为真实 OPC UA 客户端
- 支持匿名/用户名密码
- 增加连接状态管理与错误码提示

## 说明
当前为 UI/交互骨架，不依赖真实 OPC UA 服务端。
# harmony-opcua-tablet
