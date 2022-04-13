var timeCounter = 0;
var oldRst = 0;
var LFO = {
  conf : {
    CREDIT: "by vincent evrard",
    DESCRIPTION: "basic lfo",
    ISFVSN: "2",
    CATEGORIES: [
      "orage"
      ],
    INPUTS: [{
      NAME :  "CLOCK",
      TYPE :  "clock"
    },{
      NAME :  "RST",
      TYPE :  "float",
      DEFAULT : 0.0,
      MIN : 0.0,
      MAX : 1.0
    },{
      NAME :  "DIR",
      TYPE :  "float",
      DEFAULT : 1.0,
      MIN : -1.0,
      MAX : 1.0
    },{
      NAME :  "PHA",
      TYPE :  "float",
      DEFAULT : 0.0,
      MIN : 0.0,
      MAX : 1.0
    }],
    OUTPUTS: [{
      NAME :  "SAW",
      TYPE :  "float",
      DEFAULT : 0.0,
      MIN : 0.0,
      MAX : 1.0
    },{
      NAME :  "TRI",
      TYPE :  "float",
      DEFAULT : 0.0,
      MIN : 0.0,
      MAX : 1.0
    }]
  },
  main : function() {
    var deltaTime = this.conf.INPUTS[this.conf.MAP_IN["CLOCK"]].VALUE;
    var Z = Math.round(this.conf.INPUTS[this.conf.MAP_IN["DIR"]].VALUE);
    var R = Math.round(this.conf.INPUTS[this.conf.MAP_IN["RST"]].VALUE);
    var P = this.conf.INPUTS[this.conf.MAP_IN["PHA"]].VALUE;
    
    timeCounter += Z * deltaTime;
    timeCounter += 128;
    timeCounter = timeCounter - Math.floor(timeCounter);
    var value = timeCounter ;
    value += P;
    if(R == 0 && oldRst == 1) {
      timeCounter -= value;
      value = 0;
    }
    oldRst = R;
    value  = value - Math.floor(value);

    return JSON.stringify([{
        NAME : "SAW", TYPE :  "float", VALUE : value
      }, {
        NAME : "TRI", TYPE :  "float", VALUE : 1-Math.abs((value * 2) - 1)
      }, {
        NAME : "DIR", TYPE :  "float", VALUE : Z
      }
    ]); 
  }
};
Object.assign(BASE, LFO);

LFO;