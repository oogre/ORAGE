

var timeCounter = 0;
var oldRst = 0;
var LFO = {
  conf : {
    CREDIT: "by vincent evrard",
    DESCRIPTION: "basic lfo",
    ISFVSN: "2",
    CATEGORIES: [
      "orage"
      ],
    INPUTS: [{
      NAME :  "BPM",
      TYPE :  "float",
      DEFAULT : 60.0,
      MIN : 0.0,
      MAX : 300.0
    },{
      NAME :  "DIR",
      TYPE :  "float",
      DEFAULT : 1.0,
      MIN : 0.0,
      MAX : 1.0
    },{
      NAME :  "PHA",
      TYPE :  "float",
      DEFAULT : 0.0,
      MIN : 0.0,
      MAX : 1.0
    },{
      NAME :  "MUL",
      TYPE :  "float",
      DEFAULT : 1.0,
      MIN : 1.0,
      MAX : 9.0
    },{
      NAME :  "DIV",
      TYPE :  "float",
      DEFAULT : 1.0,
      MIN : 1.0,
      MAX : 9.0
    },{
      NAME :  "EXP",
      TYPE :  "float",
      DEFAULT : 5.0,
      MIN : 0.0,
      MAX : 9.0
    },{
      NAME :  "RST",
      TYPE :  "float",
      DEFAULT : 0.0,
      MIN : 0.0,
      MAX : 1.0
    }],
    OUTPUTS: [{
      NAME :  "SAW",
      TYPE :  "float",
      DEFAULT : 0.0,
      MIN : 0.0,
      MAX : 1.0
    },{
      NAME :  "SIN",
      TYPE :  "float",
      DEFAULT : 0.0,
      MIN : -1.0,
      MAX : 1.0
    },{
      NAME :  "COS",
      TYPE :  "float",
      DEFAULT : 0.0,
      MIN : -1.0,
      MAX : 1.0
    },{
      NAME :  "REC",
      TYPE :  "float",
      DEFAULT : 0.0,
      MIN : 0.0,
      MAX : 1.0
    },{
      NAME :  "TRI",
      TYPE :  "float",
      DEFAULT : 0.0,
      MIN : 0.0,
      MAX : 1.0
    }]
  },
  main : function(time, deltaTime, frameIndex) {

    var B = Math.floor(this.conf.INPUTS[this.conf.MAP_IN["BPM"]].VALUE);
    var Z = Math.round(this.conf.INPUTS[this.conf.MAP_IN["DIR"]].VALUE) * 2 -1;
    var D = Math.floor(this.conf.INPUTS[this.conf.MAP_IN["DIV"]].VALUE)-1;
    var M = Math.floor(this.conf.INPUTS[this.conf.MAP_IN["MUL"]].VALUE)-1;
    var R = Math.round(this.conf.INPUTS[this.conf.MAP_IN["RST"]].VALUE);
    var E = this.conf.INPUTS[this.conf.MAP_IN["EXP"]].VALUE;
    var P = this.conf.INPUTS[this.conf.MAP_IN["PHA"]].VALUE;
    
    var timer = deltaTime * B * 0.0166667;
    var div = 1.0 / Math.pow(2, D);
    var multi = Math.pow(2, M);
    var exp = E >= 5 ? (1.0 / (E - 4.0)) : (Math.abs(6.0 - E)) ;

    timeCounter += Z * timer * div * multi;
    timeCounter += 128;
    timeCounter = timeCounter - Math.floor(timeCounter);
    var value = timeCounter ;
    value += P;
    if(R == 0 && oldRst == 1) {
      timeCounter -= value;
      value = 0;
    }
    oldRst = R;
    value  = value - Math.floor(value);
    
    var saw = Math.pow(value, exp);
    this.conf.OUTPUTS[this.conf.MAP_OUT["SAW"]].VALUE = saw;
    this.conf.OUTPUTS[this.conf.MAP_OUT["SIN"]].VALUE = Math.sin(saw * 6.28);
    this.conf.OUTPUTS[this.conf.MAP_OUT["COS"]].VALUE = Math.cos(saw * 6.28);
    this.conf.OUTPUTS[this.conf.MAP_OUT["REC"]].VALUE = saw > 0.5;
    this.conf.OUTPUTS[this.conf.MAP_OUT["TRI"]].VALUE = Math.pow(Math.abs(value * 2 - 1), exp);
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
LFO;