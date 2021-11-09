/*----------------------------------------*\
  assets - clock.js
  @author Evrard Vincent (vincent@ogre.be)
  @Date:   2021-11-08 16:59:24
  @Last Modified time: 2021-11-09 08:43:50
\*----------------------------------------*/
var CLOCK = {
  conf : {
    CREDIT: "by vincent evrard",
    DESCRIPTION: "basic CLOCK",
    ISFVSN: "2",
    CATEGORIES: [
      "orage"
      ],
    INPUTS: [{
      NAME :  "BPM",
      TYPE :  "float",
      DEFAULT : 60.0,
      MIN : 1.0,
      MAX : 200.0
    }],
    OUTPUTS: [{
      NAME :  "CLOCK",
      TYPE :  "CLOCK"
    }]
  },
  main : function(time, deltaTime, frameIndex) {
  	var B = Math.floor(this.conf.INPUTS[this.conf.MAP_IN["BPM"]].VALUE);
    return JSON.stringify([{
        NAME : "CLOCK", 
        TYPE :  "CLOCK", 
        VALUE : deltaTime * B * 0.0166667
      }, {
        NAME : "BPM", 
        TYPE :  "float", 
        VALUE : B
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
CLOCK;