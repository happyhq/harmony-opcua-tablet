import { HistoryStore, HistoryItem, ValueType } from '../common/HistoryStore';
import { MockOpcUaClient, OpcUaConfig } from '../common/MockOpcUa';

@Entry
@Component
struct Index {
  @State ip: string = '';
  @State port: string = '4840';
  @State username: string = '';
  @State password: string = '';
  @State nodeId: string = '';
  @State valueType: ValueType = 'String';
  @State writeValue: string = '';
  @State readValue: string = '';
  @State status: string = '未连接';

  private history = new HistoryStore(200);
  private client = new MockOpcUaClient();

  private addHistory(item: HistoryItem) {
    this.history.add(item);
  }

  private getCfg(): OpcUaConfig {
    return {
      ip: this.ip.trim(),
      port: this.port.trim(),
      username: this.username.trim(),
      password: this.password,
      nodeId: this.nodeId.trim(),
      valueType: this.valueType,
      writeValue: this.writeValue
    };
  }

  private now() {
    const d = new Date();
    return `${d.getHours()}:${d.getMinutes()}:${d.getSeconds()}`;
  }

  async onConnect() {
    try {
      const msg = await this.client.connect(this.getCfg());
      this.status = msg;
      this.addHistory({ time: this.now(), action: 'CONNECT', message: msg });
    } catch (e) {
      const msg = String(e?.message ?? e);
      this.status = `连接失败：${msg}`;
      this.addHistory({ time: this.now(), action: 'ERROR', message: this.status });
    }
  }

  async onRead() {
    try {
      const value = await this.client.read(this.getCfg());
      this.readValue = value;
      this.addHistory({ time: this.now(), action: 'READ', nodeId: this.nodeId, value, message: `读取: ${value}` });
    } catch (e) {
      const msg = String(e?.message ?? e);
      this.addHistory({ time: this.now(), action: 'ERROR', message: `读取失败：${msg}` });
    }
  }

  async onWrite() {
    try {
      const msg = await this.client.write(this.getCfg());
      this.addHistory({ time: this.now(), action: 'WRITE', nodeId: this.nodeId, value: this.writeValue, message: msg });
    } catch (e) {
      const msg = String(e?.message ?? e);
      this.addHistory({ time: this.now(), action: 'ERROR', message: `写入失败：${msg}` });
    }
  }

  build() {
    Column() {
      Text('OPC UA 读写工具（Mock）')
        .fontSize(22)
        .fontWeight(FontWeight.Bold)
        .margin({ bottom: 12 });

      // Form
      Column() {
        Row() {
          Text('IP').width(80);
          TextInput({ text: this.ip, placeholder: '如 192.168.1.10' })
            .onChange(v => this.ip = v)
            .width('70%');
        }.margin({ bottom: 8 });

        Row() {
          Text('端口').width(80);
          TextInput({ text: this.port, placeholder: '4840' })
            .onChange(v => this.port = v)
            .width('70%');
        }.margin({ bottom: 8 });

        Row() {
          Text('用户名').width(80);
          TextInput({ text: this.username, placeholder: '可选' })
            .onChange(v => this.username = v)
            .width('70%');
        }.margin({ bottom: 8 });

        Row() {
          Text('密码').width(80);
          TextInput({ text: this.password, placeholder: '可选' })
            .onChange(v => this.password = v)
            .type(InputType.Password)
            .width('70%');
        }.margin({ bottom: 8 });

        Row() {
          Text('NodeId').width(80);
          TextInput({ text: this.nodeId, placeholder: 'ns=2;s=xxx' })
            .onChange(v => this.nodeId = v)
            .width('70%');
        }.margin({ bottom: 8 });

        Row() {
          Text('类型').width(80);
          Select([{ value: 'String' }, { value: 'Int' }, { value: 'Float' }, { value: 'Boolean' }])
            .selected(this.valueType)
            .onSelect((_, v) => this.valueType = v as ValueType)
            .width('70%');
        }.margin({ bottom: 8 });

        Row() {
          Text('写入值').width(80);
          TextInput({ text: this.writeValue, placeholder: '统一文本输入' })
            .onChange(v => this.writeValue = v)
            .width('70%');
        }.margin({ bottom: 8 });

        Row() {
          Button('连接').onClick(() => this.onConnect()).margin({ right: 8 });
          Button('读取').onClick(() => this.onRead()).margin({ right: 8 });
          Button('写入').onClick(() => this.onWrite());
        }.margin({ top: 6, bottom: 12 });

        Text(`状态：${this.status}`).fontColor(Color.Gray).margin({ bottom: 12 });

        Row() {
          Text('读取值').width(80);
          Text(this.readValue || '-')
            .fontWeight(FontWeight.Medium)
            .width('70%');
        }.margin({ bottom: 12 });
      }
      .padding(12)
      .backgroundColor('#F7F7F7')
      .borderRadius(8)
      .width('100%');

      // History
      Row() {
        Text('历史记录').fontSize(18).fontWeight(FontWeight.Medium).layoutWeight(1);
        Button('清空').onClick(() => this.history.clear());
      }.margin({ top: 16, bottom: 8 });

      List() {
        ForEach(this.history.items, (item: HistoryItem) => {
          ListItem() {
            Column() {
              Text(`${item.time}  ${item.action}`)
                .fontSize(12)
                .fontColor(Color.Gray);
              Text(item.message).fontSize(14);
              if (item.nodeId) {
                Text(`NodeId: ${item.nodeId}`).fontSize(12).fontColor(Color.Gray);
              }
              if (item.value) {
                Text(`值: ${item.value}`).fontSize(12).fontColor(Color.Gray);
              }
            }.padding(8);
          }
        });
      }
      .height('40%')
      .backgroundColor('#FFFFFF')
      .borderRadius(8)
      .width('100%');
    }
    .padding(16)
    .width('100%')
    .height('100%')
    .backgroundColor('#FFFFFF');
  }
}
