/*----------------------------------------*\
  assets - Easeing.js
  @author Evrard Vincent (vincent@ogre.be)
  @Date:   2021-11-08 15:27:53
  @Last Modified time: 2022-04-17 07:22:24
\*----------------------------------------*/
var Base = require('base');

Base({
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
    var x = this.getInput("TARGET").VALUE;
    this.setOutput("Sin", Math.cos((0.5-x) * Math.PI * 2) * 0.5 + 0.5);
    this.setOutput("Rec", x < 0.5 ? 0 : 1);
    this.setOutput("InQuint", x * x * x * x * x);
    this.setOutput("OutQuint", 1 - Math.pow(1 - x, 5));
    this.setOutput("InOutCirc", x < 0.5 ? 16 * x * x * x * x * x : 1 - Math.pow(-2 * x + 2, 5) / 2);
    
    return JSON.stringify([
      this.getOutput("Sin"),
      this.getOutput("Rec"),
      this.getOutput("InQuint"),
      this.getOutput("OutQuint"),
      this.getOutput("InOutCirc")
    ]);  
  }
});
