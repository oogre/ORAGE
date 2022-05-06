/*----------------------------------------*\
  assets - clock.js
  @author Evrard Vincent (vincent@ogre.be)
  @Date:   2021-11-08 16:59:24
  @Last Modified time: 2022-04-22 09:15:31
\*----------------------------------------*/

var Base = require('base');
var oldFPS = 0;
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
      NAME :  "FPS",
      TYPE :  "float",
      DEFAULT : 0.0,
      MIN : 0.0,
      MAX : 100.0
    }, {
      NAME :  "CLOCK",
      TYPE :  "clock"
    }]
  },
  main : function(time, deltaTime) {
  	var B = Math.round(this.getInput("BPM").VALUE);
    this.setInput("BPM", B);
    this.setOutput("CLOCK", deltaTime * B * 0.0166667);
    var FPS = 1/deltaTime;

    oldFPS += (FPS - oldFPS) * 0.1;
    var oFPS = Math.floor(oldFPS);
    this.setOutput("FPS", oFPS);
    this.setOutput("FPS", oFPS, "LABEL");
    return JSON.stringify([
      this.getOutput("CLOCK"),
      this.getOutput("FPS"),
      this.getInput("BPM")
    ]); 
  }
});