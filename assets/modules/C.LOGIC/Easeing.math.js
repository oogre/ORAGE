/*----------------------------------------*\
  assets - Easeing.js
  @author Evrard Vincent (vincent@ogre.be)
  @Date:   2021-11-08 15:27:53
  @Last Modified time: 2021-11-08 16:34:45
\*----------------------------------------*/
var timeCounter = 0;
var EASEING = {
  conf : {
    CREDIT: "by vincent evrard",
    DESCRIPTION: "basic easeing",
    ISFVSN: "2",
    CATEGORIES: [
      "orage"
      ],
    INPUTS: [{
      NAME :  "TARGET",
      TYPE :  "float",
      DEFAULT : 0.0,
      MIN : 0.0,
      MAX : 1.0
    }],
    OUTPUTS: [{
      NAME :  "Sin",
      TYPE :  "float",
      DEFAULT : 0.0,
      VALUE : 0.0,
      MIN : 0.0,
      MAX : 1.0
    },{
      NAME :  "Rec",
      TYPE :  "float",
      DEFAULT : 0.0,
      VALUE : 0.0,
      MIN : 0.0,
      MAX : 1.0
    },{
      NAME :  "InQuint",
      TYPE :  "float",
      DEFAULT : 0.0,
      VALUE : 0.0,
      MIN : 0.0,
      MAX : 1.0
    },{
      NAME :  "OutQuint",
      TYPE :  "float",
      DEFAULT : 0.0,
      VALUE : 0.0,
      MIN : 0.0,
      MAX : 1.0
    },{
      NAME :  "InOutCirc",
      TYPE :  "float",
      DEFAULT : 0.0,
      VALUE : 0.0,
      MIN : 0.0,
      MAX : 1.0
    }]
  },
  main : function(time, deltaTime, frameIndex) {
    var x = this.conf.INPUTS[this.conf.MAP_IN["TARGET"]].VALUE;
    this.conf.OUTPUTS[this.conf.MAP_OUT["Sin"]].VALUE = Math.cos((0.5-x) * Math.PI * 2) * 0.5 + 0.5;
    this.conf.OUTPUTS[this.conf.MAP_OUT["Rec"]].VALUE = x < 0.5 ? 0 : 1;
    this.conf.OUTPUTS[this.conf.MAP_OUT["InQuint"]].VALUE = x * x * x * x * x;
    this.conf.OUTPUTS[this.conf.MAP_OUT["OutQuint"]].VALUE = 1 - Math.pow(1 - x, 5);
    this.conf.OUTPUTS[this.conf.MAP_OUT["InOutCirc"]].VALUE = x < 0.5 ? 16 * x * x * x * x * x : 1 - Math.pow(-2 * x + 2, 5) / 2;
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
EASEING;