export type ValueType = 'String' | 'Int' | 'Float' | 'Boolean';

export interface HistoryItem {
  time: string;
  action: 'CONNECT' | 'READ' | 'WRITE' | 'ERROR';
  nodeId?: string;
  value?: string;
  message: string;
}

export class HistoryStore {
  private max: number;
  items: HistoryItem[] = [];

  constructor(max = 200) {
    this.max = max;
  }

  add(item: HistoryItem) {
    this.items.unshift(item);
    if (this.items.length > this.max) {
      this.items = this.items.slice(0, this.max);
    }
  }

  clear() {
    this.items = [];
  }
}
