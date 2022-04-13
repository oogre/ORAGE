var timeCounter = 0;
var oldRst = 0;
var Base = require('base');

Base({
  conf : {
    CREDIT: "by vincent evrard",
    DESCRIPTION: "basic lfo",
    ISFVSN: "2",
    CATEGORIES: [
      "orage"
      ],
    INPUTS: [{
      NAME :  "CLOCK",
      TYPE :  "clock"
    },{
      NAME :  "RST",
      TYPE :  "float",
      DEFAULT : 0.0,
      MIN : 0.0,
      MAX : 1.0
    },{
      NAME :  "DIR",
      TYPE :  "float",
      DEFAULT : 1.0,
      MIN : -1.0,
      MAX : 1.0
    },{
      NAME :  "PHA",
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
      NAME :  "TRI",
      TYPE :  "float",
      DEFAULT : 0.0,
      MIN : 0.0,
      MAX : 1.0
    }]
  },
  main : function() {
    var deltaTime = this.getInput("CLOCK").VALUE;
    var Z = Math.round(this.getInput("DIR").VALUE);
    var R = Math.round(this.getInput("RST").VALUE);
    var P = this.getInput("PHA").VALUE;
    timeCounter += Z * deltaTime;
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

    this.setOutput("SAW", value);
    this.setOutput("TRI", 1-Math.abs((value * 2) - 1));
    this.setInput("DIR", Z);

    return JSON.stringify([
      this.getOutput("SAW"), 
      this.getOutput("TRI"),
      this.getInput("DIR")
    ]); 
  }
});