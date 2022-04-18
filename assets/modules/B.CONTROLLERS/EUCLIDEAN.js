var timeCounter = 0;
var oldRst = 0;
var oldSteps = 0;
var oldBeats = 0;
var oldPhase = 0;
var rythm = [];
var Base = require('base');

Base({
  conf : {
    CREDIT: "by vincent evrard",
    DESCRIPTION: "euclidean rythm generator",
    ISFVSN: "2",
    CATEGORIES: [
      "orage"
      ],
    INPUTS: [{
      NAME :  "CLOCK",
      TYPE :  "clock",
      DEFAULT : 0.0,
      MIN : 0.0,
      MAX : 1.0
    },{
      NAME :  "RST",
      TYPE :  "float",
      DEFAULT : 0.0,
      MIN : 0.0,
      MAX : 1.0
    },{
      NAME :  "STEPS",
      TYPE :  "float",
      DEFAULT : 1.0,
      MIN : 1.0,
      MAX : 32
    },{
      NAME :  "BEATS",
      TYPE :  "float",
      DEFAULT : 1.0,
      MIN : 1.0,
      MAX : 32.0
    },{
      NAME :  "PHASE",
      TYPE :  "float",
      DEFAULT : 0.0,
      MIN : 0.0,
      MAX : 1.0
    }],
    OUTPUTS: [{
      NAME :  "OUTPUT",
      TYPE :  "float",
      DEFAULT : 0.0,
      MIN : 0.0,
      MAX : 1.0
    }]
  },
  main : function(time, deltaTime) {
    timeCounter += this.getInput("CLOCK").VALUE;
    var R = Math.round(this.getInput("RST").VALUE);
    var steps = Math.round(this.getInput("STEPS").VALUE);
    var beats = Math.round(this.getInput("BEATS").VALUE);
    var phase = Math.round(this.getInput("PHASE").VALUE * steps);

    beats = Math.min(steps, beats);
    if(oldSteps != steps || oldBeats != beats || oldPhase != phase){
      rythm = buildRythm(steps, beats, phase);
      oldSteps = steps;
      oldBeats = beats; 
      oldPhase = phase;
    }

    if(R == 0 && oldRst == 1) {
      timeCounter = 0;
    }
    oldRst = R;
    var currentStep = Math.floor(timeCounter % steps);
    // var currentStep = Math.floor((time - Math.floor(time)) * steps);
   
    this.setOutput("OUTPUT", + rythm[currentStep]);
    this.setInput("BEATS", beats);
    this.setInput("STEPS", steps);
    this.setInput("PHASE", phase/steps);
    this.setInput("RST", R);

    return JSON.stringify([
      this.getInput("RST"),
      this.getInput("BEATS"),
      this.getInput("BEATS"),
      this.getInput("STEPS"),
      this.getInput("PHASE"),
      this.getOutput("OUTPUT")
    ]);  
  }
});

var buildRythm = function(steps, beats, phase){
    var rythm = [];
    if(steps == beats){
      for(var i = 0 ; i < steps ; i ++){
        rythm.push(true);
      }
      return rythm;
    }
    var pauses = steps - beats;
    var switcher = false;
    if (beats > pauses) {
      switcher = true;
      // XOR swap pauses and beats
      pauses ^= beats;
      beats ^= pauses;
      pauses ^= beats;
    }
    var per_pulse = Math.floor(pauses / beats);
    var remainder = pauses % beats;
    var noskip = (remainder == 0) ? 0 : Math.floor(beats / remainder);
    var skipXTime = (noskip == 0) ? 0 : Math.floor((beats - remainder)/noskip);
    var count = 0;
    var skipper = 0;
    for (var i = 1; i <= steps; i++) {
      if (count == 0) {
        rythm.push(!switcher);
        count = per_pulse;
        if (remainder > 0 && skipper == 0) {
          count++;
          remainder--;
          skipper = (skipXTime > 0) ? noskip : 0;
          skipXTime--;
        } else {
          skipper--;
        }
      } else {
        rythm.push(switcher);
        count--;
      }
    }
    if (switcher) {
      // XOR swap pauses and beats
      pauses ^= beats;
      beats ^= pauses;
      pauses ^= beats;
    }
    for(var i = 0 ; i < phase ; i ++){
      rythm.unshift(rythm.pop());
    }
    return rythm;
}