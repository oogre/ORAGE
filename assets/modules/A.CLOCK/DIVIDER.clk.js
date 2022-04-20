/*----------------------------------------*\
  assets - DIVIDER.js
  @author Evrard Vincent (vincent@ogre.be)
  @Date:   2021-11-08 22:13:00
  @Last Modified time: 2022-04-18 13:24:10
\*----------------------------------------*/
var Base = require('base');

Base({
  conf : {
    CREDIT: "by vincent evrard",
    DESCRIPTION: "basic CLOCK DIVIDER",
    ISFVSN: "2",
    CATEGORIES: [
      "orage"
      ],
    INPUTS: [{
      NAME :  "CLOCK_IN",
      TYPE :  "clock"
    },{
      NAME :  "MUL",
      TYPE :  "float",
      DEFAULT : 0.0,
      MIN : 0.0,
      MAX : 1.0,
      MAGNETIC : [1, 2, 3, 4, 5, 8, 16, 32, 64],
      LABELS : ["1", "2", "3", "4", "5", "8", "16", "32", "64"]
    },{
      NAME :  "DIV",
      TYPE :  "float",
      DEFAULT : 0.0,
      MIN : 0.0,
      MAX : 1.0,
      MAGNETIC : [1, 1/2, 1/3, 1/4, 1/5, 1/8, 1/16, 1/32, 1/64],
      LABELS : ["1", "1/2", "1/3", "1/4", "1/5", "1/8", "1/16", "1/32", "1/64"]
    }],
    OUTPUTS: [{
      NAME :  "CLOCK_OUT",
      TYPE :  "clock"
    }]
  },
  main : function() {
  	var deltaTime = this.getInput("CLOCK_IN").VALUE;
  	var DIV = this.getInput("DIV");
    var MUL = this.getInput("MUL");
    var D = this.valueToMagnetic(DIV);
    var M = this.valueToMagnetic(MUL);
   
    deltaTime *= D * M;
    
    this.setInput("DIV", this.valueToLabel(DIV), "LABEL");
    this.setInput("MUL", this.valueToLabel(MUL), "LABEL");
    this.setOutput("CLOCK_OUT", deltaTime);
    this.setInput("DIV", this.magneticToValue(DIV));
    this.setInput("MUL", this.magneticToValue(MUL));

    return JSON.stringify([
      this.getOutput("CLOCK_OUT"),
      this.getInput("DIV"),
      this.getInput("MUL")
    ]); 
  }
});
