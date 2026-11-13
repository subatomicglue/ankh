export class HudTileText {
  constructor(context,image){this.context=context;this.image=image;}

  draw(index,x,y){this.context.drawImage(this.image,(index%32)*8,Math.floor(index/32)*8,8,8,x,y,8,8);}

  write(x,y,text,indexFor){
    let column=0;
    for(const character of text){
      if(character==='\n'){y+=8;column=0;continue;}
      const index=indexFor(character);if(index!==null)this.draw(index,x+column*8,y);
      column++;
    }
  }

  printPixels(x,y,text){this.write(x,y,text,character=>character>=' '&&character<='Z'?17+character.charCodeAt(0)-32:null);}
  printnumPixels(x,y,text){this.write(x,y,text,character=>character>='0'&&character<='9'?7+character.charCodeAt(0)-48:null);}
  print(tileX,tileY,text){this.printPixels(tileX*8,tileY*8,text);}
  printnum(tileX,tileY,text){this.printnumPixels(tileX*8,tileY*8,text);}

  label(element,text,scale=2){
    const canvas=document.createElement('canvas'),pixelRatio=devicePixelRatio||1,width=text.length*8,height=8;
    canvas.width=Math.round(width*scale*pixelRatio);canvas.height=Math.round(height*scale*pixelRatio);
    canvas.style.width=`${width*scale}px`;canvas.style.height=`${height*scale}px`;canvas.setAttribute('aria-hidden','true');
    const context=canvas.getContext('2d');context.setTransform(scale*pixelRatio,0,0,scale*pixelRatio,0,0);context.imageSmoothingEnabled=false;
    new HudTileText(context,this.image).printPixels(0,0,text);element.setAttribute('aria-label',text);element.replaceChildren(canvas);
  }
}
