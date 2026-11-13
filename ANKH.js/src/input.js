const DIRECTIONS={w:[0,-1],a:[-1,0],s:[0,1],d:[1,0],i:[0,-1],j:[-1,0],k:[0,1],l:[1,0]};

export class InputBuffer {
  constructor(){this.commands=[];}
  push(command){this.commands.push(command);}
  peek(){return this.commands.slice();}
  drain(){return this.commands.splice(0);}
}

export class KeyboardControls {
  constructor(target,input){this.input=input;this.padMode=null;target.addEventListener('keydown',event=>this.keyDown(event));}
  setPadMode(mode){this.padMode=mode;}
  keyDown(event){
    if(event.repeat)return;const key=event.key.toLowerCase(),direction=DIRECTIONS[key];
    if(direction){event.preventDefault();const movement='ijkl'.includes(key),modified=event.shiftKey||event.ctrlKey;this.input.push({type:movement?(this.padMode??(modified?'step':'run')):(modified?'touch':'shoot'),direction});return;}
    const commands={' ':{type:'stop'},escape:{type:'pause'},p:{type:'pause'},r:{type:'reset'},f2:{type:'debug-flyover'},'`':{type:'debug-flyover'},enter:{type:'start'},'1':{type:'drop',slot:0},'2':{type:'drop',slot:1},'3':{type:'drop',slot:2},'4':{type:'drop',slot:3}};
    if(commands[key]){event.preventDefault();this.input.push(commands[key]);}
  }
}
