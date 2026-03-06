import { ValueType } from './HistoryStore';

export interface OpcUaConfig {
  ip: string;
  port: string;
  username: string;
  password: string;
  nodeId: string;
  valueType: ValueType;
  writeValue: string;
}

export class MockOpcUaClient {
  connected: boolean = false;

  async connect(cfg: OpcUaConfig): Promise<string> {
    await this.delay(300);
    if (!cfg.ip || !cfg.port) {
      throw new Error('IP/端口不能为空');
    }
    this.connected = true;
    return `已连接 ${cfg.ip}:${cfg.port}`;
  }

  async read(cfg: OpcUaConfig): Promise<string> {
    await this.delay(200);
    if (!this.connected) throw new Error('未连接');
    if (!cfg.nodeId) throw new Error('NodeId 不能为空');
    // mock: generate value by type
    switch (cfg.valueType) {
      case 'Int': return String(Math.floor(Math.random() * 100));
      case 'Float': return (Math.random() * 100).toFixed(2);
      case 'Boolean': return Math.random() > 0.5 ? 'true' : 'false';
      default: return 'mock-value';
    }
  }

  async write(cfg: OpcUaConfig): Promise<string> {
    await this.delay(200);
    if (!this.connected) throw new Error('未连接');
    if (!cfg.nodeId) throw new Error('NodeId 不能为空');
    if (!cfg.writeValue) throw new Error('写入值不能为空');
    return `写入成功: ${cfg.writeValue}`;
  }

  private delay(ms: number) {
    return new Promise((resolve) => setTimeout(resolve, ms));
  }
}
