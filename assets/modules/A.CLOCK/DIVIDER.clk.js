/*----------------------------------------*\
  assets - DIVIDER.js
  @author Evrard Vincent (vincent@ogre.be)
  @Date:   2021-11-08 22:13:00
  @Last Modified time: 2021-12-17 09:33:54
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
      TYPE :  "CLOCK"
    },{
      NAME :  "MUL",
      TYPE :  "float",
      DEFAULT : 1.0,
      MIN : 1.0,
      MAX : 5.0,
      MAGNETIC : [1, 2, 3, 4, 5]
    },{
      NAME :  "DIV",
      TYPE :  "float",
      DEFAULT : 1.0,
      MIN : 1.0,
      MAX : 5.0,
      MAGNETIC : [1, 2, 3, 4, 5]
    }],
    OUTPUTS: [{
      NAME :  "CLOCK_OUT",
      TYPE :  "CLOCK"
    }]
  },
  main : function() {
  	var deltaTime = this.getInput("CLOCK_IN").VALUE;
  	var D = Math.floor(this.getInput("DIV").VALUE)-1;
    var M = Math.floor(this.getInput("MUL").VALUE)-1;
    
    var div = 1.0 / Math.pow(2, D);
    var multi = Math.pow(2, M);
    
    deltaTime *= div * multi;
    
    this.setOutput("CLOCK_OUT", deltaTime);

    return JSON.stringify([
      this.getOutput("CLOCK_OUT")
    ]); 
  }
});
