/*
{
   "CATEGORIES" : [ "orage" ],
   "CREDIT" : "by vincent evrard",
   "DESCRIPTION" : "basic oscillator",
   "INPUTS" : [
      {
         "CURRENT" : 14.760999999999999,
         "DEFAULT" : 0,
         "MAX" : 1.7976900000000001e+308,
         "MIN" : 0,
         "NAME" : "TIME",
         "TYPE" : 4
      },
      {
         "CURRENT" : 0.0166661,
         "DEFAULT" : 0,
         "MAX" : 1.7976900000000001e+308,
         "MIN" : 0,
         "NAME" : "TIMEDELTA",
         "TYPE" : 4
      },
      {
         "CURRENT" : 0,
         "DEFAULT" : 0,
         "MAX" : 0,
         "MIN" : 0,
         "NAME" : "tex0",
         "TYPE" : 9
      },
      {
         "CURRENT" : 1,
         "DEFAULT" : 1,
         "MAX" : 10,
         "MIN" : 0,
         "NAME" : "freq",
         "TYPE" : 4
      },
      {
         "CURRENT" : 0,
         "DEFAULT" : 0,
         "MAX" : 1,
         "MIN" : 0,
         "NAME" : "sync",
         "TYPE" : 4
      },
      {
         "CURRENT" : 0,
         "DEFAULT" : 0,
         "MAX" : 1,
         "MIN" : 0,
         "NAME" : "dX",
         "TYPE" : 4
      },
      {
         "CURRENT" : 0,
         "DEFAULT" : 0,
         "MAX" : 1,
         "MIN" : -1,
         "NAME" : "dY",
         "TYPE" : 4
      },
      {
         "CURRENT" : 0,
         "DEFAULT" : 0,
         "MAX" : 1,
         "MIN" : -1,
         "NAME" : "mod",
         "TYPE" : 4
      },
      {
         "CURRENT" : 640,
         "DEFAULT" : 640,
         "MAX" : 1920,
         "MIN" : 1,
         "NAME" : "WIDTH",
         "TYPE" : 4
      },
      {
         "CURRENT" : 480,
         "DEFAULT" : 480,
         "MAX" : 1080,
         "MIN" : 1,
         "NAME" : "HEIGHT",
         "TYPE" : 4
      }
   ],
   "ISFVSN" : "",
   "NAME" : "OSC.fs",
   "OUTPUTS" : [
      {
         "CURRENT" : 0,
         "DEFAULT" : 0,
         "MAX" : 0,
         "MIN" : 0,
         "NAME" : "out0",
         "TYPE" : 9
      }
   ]
}
*/

float PI = 3.14159265359;
float TWO_PI = 6.283185306;

float phaseProcess(in vec2 fs, in float fq, in float sync, in float mod){
    float phase =  mod + mix(fs.x, fs.y, sync) * fq;
    return fract(phase);
}

float sawWave(in float phase, in float height){
    return (phase) + height;
}

float sinWave(in float phase, in float height){
    return (sin(-0.5*PI + phase * TWO_PI)*0.5+0.5) + height;
}

float nozWave(in float phase, in float height){
    return fract( sin( phase ) * 999.9)+height;
}

float triWave(in float phase, in float height){
    return 1 - abs(((phase)*2.0)-1.0)+ height;
}

float recWave(in float phase, in float height){
	phase = triWave(phase, height);
    if(phase>0.5){
        return 1.0;
    }else{
        return 0.0;
    };
}


void main()
{
  	vec3 modIntensity = IMG_NORM_PIXEL(tex0, isf_FragNormCoord.xy).rgb;
  	float modI = max(modIntensity.r, max(modIntensity.g, modIntensity.b));
	float modFactor = mix(0, modI, _tex0_sample);

	float phase = phaseProcess( isf_FragNormCoord.xy,
                                freq ,
                                sync,
                                (1.0-dX) + mod * modFactor );
    
    out0 = vec4(vec3(sawWave(phase, dY)), 1);
}
