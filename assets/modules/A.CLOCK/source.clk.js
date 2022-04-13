/*----------------------------------------*\
  assets - clock.js
  @author Evrard Vincent (vincent@ogre.be)
  @Date:   2021-11-08 16:59:24
  @Last Modified time: 2022-04-13 23:33:45
\*----------------------------------------*/

var Base = require('base');

Base({
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
      TYPE :  "clock"
    }]
  },
  main : function(time, deltaTime) {
  	var B = Math.round(this.getInput("BPM").VALUE);
    this.setInput("BPM", B);
    this.setOutput("CLOCK", deltaTime * B * 0.0166667);
    return JSON.stringify([
      this.getOutput("CLOCK"),
      this.getInput("BPM")
    ]); 
  }
});