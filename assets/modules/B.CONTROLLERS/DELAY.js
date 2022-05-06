var timeCounter = 0;
var Base = require('base');
var buffer = [];
var maxBufferDuration = 10;//second
var oldDuration;
var cursor;
Base({
  conf : {
    CREDIT: "by vincent evrard",
    DESCRIPTION: "experimentation delay signal",
    ISFVSN: "2",
    CATEGORIES: [
      "orage"
  	],
    INPUTS: [
    {
      NAME :  "SIGNAL",
      TYPE :  "float",
      DEFAULT : 0.0,
      MIN : 0.0,
      MAX : 1.0
    },{
      NAME :  "DURATION",
      TYPE :  "float",
      DEFAULT : 0.0,
      MIN : 0.0,
      MAX : 1.0
    }
    ],
    OUTPUTS: [{
      NAME :  "CURSOR",
      TYPE :  "float",
      DEFAULT : 0.0,
      MIN : 0.0,
      MAX : 1.0
    },{
      NAME :  "SIG_OUT",
      TYPE :  "float",
      DEFAULT : 0.0,
      VALUE : 0.0,
      MIN : 0.0,
      MAX : 1.0
    }]
  },
  main : function(time, deltaTime) {
  	if(buffer.length == 0){
  		var length = maxBufferDuration/deltaTime;
  		for(var i = 0 ; i < length ; i ++){
  			buffer.push(0);
  		}
  	}
    var sig = this.getInput("SIGNAL").VALUE;
    var duration = this.getInput("DURATION").VALUE * maxBufferDuration;
    this.setInput("DURATION", duration.toFixed(2) +" sec", "LABEL");
    buffer.unshift(sig);
    if(oldDuration != duration){
    	cursor = (duration / deltaTime) % buffer.length;
    	oldDuration = duration;
    }
    this.setOutput("SIG_OUT", buffer.splice(cursor, 1)[0]);
    this.setOutput("CURSOR", (time / duration)%1);
    return JSON.stringify([
      this.getOutput("CURSOR"), 
      this.getOutput("SIG_OUT"), 
      this.getInput("DURATION")
    ]);
  }
});