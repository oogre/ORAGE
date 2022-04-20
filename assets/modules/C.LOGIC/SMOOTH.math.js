/*----------------------------------------*\
  assets - MATH.js
  @author Evrard Vincent (vincent@ogre.be)
  @Date:   2021-10-30 18:10:59
  @Last Modified time: 2022-04-17 07:38:48
\*----------------------------------------*/
var Base = require('base');
var oldValue= 0;
Base({
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
    var target = this.getInput("TARGET").VALUE;
    var speed  = this.getInput("SPEED").VALUE;
    var value  = this.getOutput("VALUE").VALUE;
    value += (target - value) * speed;
    this.setOutput("VALUE", value);
    return JSON.stringify([
      this.getOutput("VALUE")
    ]);  
  }
});