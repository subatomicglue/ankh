export class InputBuffer {
  constructor(){this.commands=[];}
  push(command){this.commands.push(command);}
  peek(){return this.commands.slice();}
  drain(){return this.commands.splice(0);}
}
