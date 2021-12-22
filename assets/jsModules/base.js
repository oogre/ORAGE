/*----------------------------------------*\
  assets - base.js
  @author Evrard Vincent (vincent@ogre.be)
  @Date:   2021-12-14 23:10:13
  @Last Modified time: 2021-12-17 09:26:36
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
				var param = this.conf.INPUTS[id];
				if(param){
					return param;
				}
			}
			print("get function unable to find "+name);
		},
		getOutput : function(name){
			var id = this.conf.MAP_OUT[name];
			if(id !== undefined){
				var param = this.conf.OUTPUTS[id]
				if(param){
					return param;
				}
			}
			print("get function unable to find "+name);
		},
		setInput : function(name, value){
			var id = this.conf.MAP_IN[name];
			if(id !== undefined){
				var param = this.conf.INPUTS[id];
				if(param){
					param.VALUE = value;
					return;
				}
			}
			print("function unable to find "+name);
		},
		setOutput : function(name, value){
			var id = this.conf.MAP_OUT[name];
			if(id !== undefined){
				var param = this.conf.OUTPUTS[id];
				if(param){
					param.VALUE = value;
					return;
				}
			}
			print("function unable to find "+name);
		}
	});
}

