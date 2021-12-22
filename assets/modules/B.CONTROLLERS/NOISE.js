
var timeCounterX = 0;
var timeCounterY = 0;
var timeCounterZ = 0;

var Noise = require('noise');
var Base = require('base');

Base({
  conf : {
    CREDIT: "by vincent evrard",
    DESCRIPTION: "basic noise",
    ISFVSN: "2",
    CATEGORIES: [
      "orage"
      ],
    INPUTS: [{
      NAME :  "CLOCK_X",
      TYPE :  "CLOCK"
    },{
      NAME :  "CLOCK_Y",
      TYPE :  "CLOCK"
    },{
      NAME :  "CLOCK_Z",
      TYPE :  "CLOCK"
    },{
      NAME :  "SEED",
      TYPE :  "float",
      DEFAULT : Math.random(),
      MIN : 0.0,
      MAX : 1.0
    },{
      NAME :  "SPEED",
      TYPE :  "float",
      DEFAULT : 1,
      MIN : -1.0,
      MAX : 1.0
    }],
    OUTPUTS: [{
      NAME :  "SIMPLEX3",
      TYPE :  "float",
      DEFAULT : 0.0,
      MIN : -1.0,
      MAX : 1.0
    },{
      NAME :  "PERLIN3",
      TYPE :  "float",
      DEFAULT : 0.0,
      MIN : -1.0,
      MAX : 1.0
    }]
  },
  main : function() {
    var deltaTimeX = this.getInput("CLOCK_X").VALUE;
    var deltaTimeY = this.getInput("CLOCK_Y").VALUE;
    var deltaTimeZ = this.getInput("CLOCK_Z").VALUE;
    var s = this.getInput("SEED").VALUE;
    Noise.seed(s);
    var dir = this.getInput("SPEED").VALUE;
    timeCounterX += deltaTimeX * dir;
    timeCounterY += deltaTimeY * dir;
    timeCounterZ += deltaTimeZ * dir;
    this.setOutput("SIMPLEX3", Noise.simplex3(timeCounterX, timeCounterY, timeCounterZ));
    this.setOutput("PERLIN3",  Noise.perlin3(timeCounterX, timeCounterY, timeCounterZ));
    return JSON.stringify([
      this.getInput("SPEED"),
      this.getOutput("SIMPLEX3"), 
      this.getOutput("PERLIN3")
    ]); 
  }
});