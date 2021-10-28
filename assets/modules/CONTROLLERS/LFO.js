var testObj = {
  conf : {
    CREDIT: "by vincent evrard",
    DESCRIPTION: "basic lfo",
    ISFVSN: "2",
    CATEGORIES: [
      "orage"
      ],
    INPUTS: [{
      NAME :  "freq",
      TYPE :  "float",
      DEFAULT : 60.0,
      MIN : 0.0,
      MAX : 600.0
    }],
    OUTPUTS: [{
      NAME :  "sine",
      TYPE :  "float",
      DEFAULT : 0.0,
      MIN : -1.0,
      MAX : 1.0
    }]
  },
  main : function(time) { 
    for(var i = 0 ; i < this.conf.OUTPUTS.length ; i++){
      if(this.conf.OUTPUTS[i].NAME == "sine"){
        this.conf.OUTPUTS[i].value = Math.sin(time);
      }
    }
    return JSON.stringify(this.conf.OUTPUTS); 
  },
  getConf : function() { 
    return JSON.stringify(this.conf); 
  }
};
testObj;