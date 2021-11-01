/*----------------------------------------*\
  assets - MATH.js
  @author Evrard Vincent (vincent@ogre.be)
  @Date:   2021-10-30 18:10:59
  @Last Modified time: 2021-10-30 19:10:13
\*----------------------------------------*/
var timeCounter = 0;
var LOGIC = {
  conf : {
    CREDIT: "by vincent evrard",
    DESCRIPTION: "basic noise",
    ISFVSN: "2",
    CATEGORIES: [
      "orage"
      ],
    INPUTS: [{
      NAME :  "A",
      TYPE :  "float",
      DEFAULT : 0,
      MIN : 0.0,
      MAX : 1.0
    },{
      NAME :  "B",
      TYPE :  "float",
      DEFAULT : 0,
      MIN : 0.0,
      MAX : 1.0
    }],
    OUTPUTS: [{
      NAME :  "ADD",
      TYPE :  "float",
      DEFAULT : 0.0,
      MIN : 0.0,
      MAX : 2.0
    },{
      NAME :  "DIF",
      TYPE :  "float",
      DEFAULT : 0.0,
      MIN : 0.0,
      MAX : 1.0
    },{
      NAME :  "MUL",
      TYPE :  "float",
      DEFAULT : 0.0,
      MIN : 0.0,
      MAX : 1.0
    },{
      NAME :  "MOD",
      TYPE :  "float",
      DEFAULT : 0.0,
      MIN : 0.0,
      MAX : 1.0
    },{
      NAME :  "MIN",
      TYPE :  "float",
      DEFAULT : 0.0,
      MIN : 0.0,
      MAX : 1.0
    },{
      NAME :  "MAX",
      TYPE :  "float",
      DEFAULT : 0.0,
      MIN : 0.0,
      MAX : 1.0
    }]
  },
  main : function(time, deltaTime, frameIndex) {
    var a = this.conf.INPUTS[this.conf.MAP_IN["A"]].VALUE;
    var b = this.conf.INPUTS[this.conf.MAP_IN["B"]].VALUE;
    this.conf.OUTPUTS[this.conf.MAP_OUT["ADD"]].VALUE = a + b;
    this.conf.OUTPUTS[this.conf.MAP_OUT["DIF"]].VALUE = a - b;
    this.conf.OUTPUTS[this.conf.MAP_OUT["MUL"]].VALUE = a * b;
    this.conf.OUTPUTS[this.conf.MAP_OUT["MOD"]].VALUE = a % b;
    this.conf.OUTPUTS[this.conf.MAP_OUT["MIN"]].VALUE = Math.min(a, b);
    this.conf.OUTPUTS[this.conf.MAP_OUT["MAX"]].VALUE = Math.max(a, b);
    
    return JSON.stringify(this.conf.OUTPUTS); 
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
LOGIC;