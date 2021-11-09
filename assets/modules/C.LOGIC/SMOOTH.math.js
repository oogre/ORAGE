/*----------------------------------------*\
  assets - MATH.js
  @author Evrard Vincent (vincent@ogre.be)
  @Date:   2021-10-30 18:10:59
  @Last Modified time: 2021-11-08 16:31:54
\*----------------------------------------*/
var timeCounter = 0;
var SMOOTH = {
  conf : {
    CREDIT: "by vincent evrard",
    DESCRIPTION: "basic noise",
    ISFVSN: "2",
    CATEGORIES: [
      "orage"
      ],
    INPUTS: [{
      NAME :  "TARGET",
      TYPE :  "float",
      DEFAULT : 1.0,
      MIN : 0.0,
      MAX : 1.0
    },{
      NAME :  "SPEED",
      TYPE :  "float",
      DEFAULT : 0.0,
      MIN : 0.0,
      MAX : 0.5
    }],
    OUTPUTS: [{
      NAME :  "VALUE",
      TYPE :  "float",
      DEFAULT : 0.0,
      VALUE : 0.0,
      MIN : 0.0,
      MAX : 1.0
    }]
  },
  main : function(time, deltaTime, frameIndex) {
    var target = this.conf.INPUTS[this.conf.MAP_IN["TARGET"]].VALUE;
    var speed = this.conf.INPUTS[this.conf.MAP_IN["SPEED"]].VALUE;
    var value = this.conf.OUTPUTS[this.conf.MAP_OUT["VALUE"]].VALUE;

    value += (target - value) * speed;

    this.conf.OUTPUTS[this.conf.MAP_OUT["VALUE"]].VALUE = value;
    
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
SMOOTH;