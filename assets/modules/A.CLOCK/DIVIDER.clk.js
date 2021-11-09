/*----------------------------------------*\
  assets - DIVIDER.js
  @author Evrard Vincent (vincent@ogre.be)
  @Date:   2021-11-08 22:13:00
  @Last Modified time: 2021-11-09 21:42:30
\*----------------------------------------*/
var DIVIDER = {
  conf : {
    CREDIT: "by vincent evrard",
    DESCRIPTION: "basic CLOCK DIVIDER",
    ISFVSN: "2",
    CATEGORIES: [
      "orage"
      ],
    INPUTS: [{
      NAME :  "CLOCK_IN",
      TYPE :  "CLOCK"
    },{
      NAME :  "MUL",
      TYPE :  "float",
      DEFAULT : 1.0,
      MIN : 1.0,
      MAX : 5.0
    },{
      NAME :  "DIV",
      TYPE :  "float",
      DEFAULT : 1.0,
      MIN : 1.0,
      MAX : 5.0
    }],
    OUTPUTS: [{
      NAME :  "CLOCK_OUT",
      TYPE :  "CLOCK"
    }]
  },
  main : function() {
  	var deltaTime = this.conf.INPUTS[this.conf.MAP_IN["CLOCK_IN"]].VALUE;
  	var D = Math.floor(this.conf.INPUTS[this.conf.MAP_IN["DIV"]].VALUE)-1;
    var M = Math.floor(this.conf.INPUTS[this.conf.MAP_IN["MUL"]].VALUE)-1;
    
    var div = 1.0 / Math.pow(2, D);
    var multi = Math.pow(2, M);
    
    deltaTime *= div * multi;
    
    return JSON.stringify([{
        NAME : "CLOCK_OUT", 
        TYPE :  "CLOCK", 
        VALUE : deltaTime
      },{
        NAME : "DIV", TYPE : "float", VALUE : D + 1
      }, {
        NAME : "MUL", TYPE : "float", VALUE : M + 1
      }
    ]); 
  },
  getConf : function() {
    this.conf.MAP_OUT = new Object();
    this.conf.MAP_IN = new Object();
    for(var i = 0 ; i < this.conf.OUTPUTS.length ; i++){
      this.conf.MAP_OUT[this.conf.OUTPUTS[i].NAME] = i;
    }
    for(var i = 0 ; i < this.conf.INPUTS.length ; i++){
      this.conf.MAP_IN[this.conf.INPUTS[i].NAME] = i;
    }
    return JSON.stringify(this.conf); 
  },
  setInput : function (name, value){
    this.conf.INPUTS[this.conf.MAP_IN[name]].VALUE = value;
  }
};
DIVIDER;