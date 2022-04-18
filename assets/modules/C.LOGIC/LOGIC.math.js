/*----------------------------------------*\
  assets - MATH.js
  @author Evrard Vincent (vincent@ogre.be)
  @Date:   2021-10-30 18:10:59
  @Last Modified time: 2022-04-17 07:25:28
\*----------------------------------------*/
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
      MIN : -1.0,
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
    },{
      NAME :  "LGT",
      TYPE :  "float",
      DEFAULT : 0.0,
      MIN : 0.0,
      MAX : 1.0
    },{
      NAME :  "SMT",
      TYPE :  "float",
      DEFAULT : 0.0,
      MIN : 0.0,
      MAX : 1.0
    }]
  },
  main : function(time, deltaTime, frameIndex) {
    var a = this.getInput("A").VALUE;
    var b = this.getInput("B").VALUE;;
    this.setOutput("ADD", a + b);
    this.setOutput("DIF", a - b);
    this.setOutput("MUL", a * b);
    this.setOutput("MOD", a % b);
    this.setOutput("MIN", Math.min(a, b));
    this.setOutput("MAX", Math.max(a, b));
    this.setOutput("LGT", a > b);
    this.setOutput("SMT", a < b);
     return JSON.stringify([
      this.getOutput("ADD"),
      this.getOutput("DIF"),
      this.getOutput("MUL"),
      this.getOutput("MOD"),
      this.getOutput("MIN"),
      this.getOutput("MAX"),
      this.getOutput("LGT"),
      this.getOutput("SMT")
    ]);  
  }
});