/*----------------------------------------*\
  assets - base.js
  @author Evrard Vincent (vincent@ogre.be)
  @Date:   2021-12-14 23:10:13
  @Last Modified time: 2022-04-18 13:21:40
\*----------------------------------------*/

module.exports = function(self){
	return Object.assign(self, {
		getConf : function() {
			this.conf.MAP_OUT = new Object();
			this.conf.MAP_IN = new Object();
			for(var i = 0 ; i < this.conf.OUTPUTS.length ; i++){
				this.conf.MAP_OUT[this.conf.OUTPUTS[i].NAME] = i;
			}
			for(var i = 0 ; i < this.conf.INPUTS.length ; i++){
				this.conf.MAP_IN[this.conf.INPUTS[i].NAME] = i;
			}
			return JSON.stringify(this.conf); 
		},
		getInput : function(name){
			var id = this.conf.MAP_IN[name];
			if(id !== undefined){
				return this.conf.INPUTS[id];
			}
			print("get function unable to find "+name);
		},
		getOutput : function(name){
			var id = this.conf.MAP_OUT[name];
			if(id !== undefined){
				return this.conf.OUTPUTS[id];
			}
			print("get function unable to find "+name);
		},
		setInput : function(name, value, elem){
			if(!elem)elem="VALUE";
			var id = this.conf.MAP_IN[name];
			if(id !== undefined){
				var param = this.conf.INPUTS[id];
				if(param){
					param[elem] = value;
					return;
				}
			}
			// print("function unable to find "+name);
		},
		setOutput : function(name, value, elem){
			if(!elem)elem="VALUE";
			var id = this.conf.MAP_OUT[name];
			if(id !== undefined){
				var param = this.conf.OUTPUTS[id];
				if(param){
					param[elem] = value;
					return;
				}
			}
			print("function unable to find "+name);
		},
		isMinified : function(){
			if(!this.conf.UI || !this.conf.UI[0] || !this.conf.UI[0].minified)return false;
			return this.conf.UI[0].minified;
		},
		getPosition : function(){
			if(!this.conf.UI || !this.conf.UI[0] || !this.conf.UI[0].position)return "";
				return JSON.stringify(this.conf.UI[0].position);
		},
		valueToMagneticId : function(param){
			return Math.floor(param.VALUE * (param.MAGNETIC.length-1))
		},
		valueToMagnetic : function(param){
			return param.MAGNETIC[this.valueToMagneticId(param)];
		},
		magneticToValue : function(param){
			return this.valueToMagneticId(param)/(param.MAGNETIC.length-1)
		},
		valueToLabel : function(param){
			return param.LABELS[this.valueToMagneticId(param)];
		},
		rebuild : function(rawContent, rawData){
			var data = JSON.parse(rawData);
			var inStartToken = /INPUTS\s*\:\s*/;
			var outStartToken = /OUTPUTS\s*\:\s*/;
			var UIStartToken = /UI\s*\:\s*/;
			var stopToken = /\}\s*\]/;
			var rawInputs = JSON.stringify(data.INPUTS);
			var rawOutputs = JSON.stringify(data.OUTPUTS);
			var rawUi = JSON.stringify(data.UI);
			var inject = function(raw, startToken, content, stopToken){
				var parcel = raw.split(startToken)[1].split(stopToken)[0];
				return raw.slice(0, raw.indexOf(parcel)) + content +
					raw.slice(raw.indexOf(parcel) + parcel.length+2);
			}
			var content = inject(rawContent, inStartToken, rawInputs, stopToken);
			if(!UIStartToken.test(content)){
				content = inject(content, outStartToken, rawOutputs+", \n    UI : " + rawUi + ", ", stopToken);	
			}else{
				content = inject(content, outStartToken, rawOutputs, stopToken);	
				content = inject(content, UIStartToken, rawUi, stopToken);
			}
			return content ;
		}
	});
}

