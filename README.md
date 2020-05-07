[![IMAGE ALT TEXT HERE](./doc/pictures/pic.01.jpg)](./doc/pictures/pic.01.jpg)

# ORAGE

Here is the source code and documentation of ORAGE.

* [Introduction](#introduction)
* [Inspiration](#inspiration)
* [State of art](#state-of-art)
* [Installation](#installation)
* [Contributes](#contributes)
* [Licence](#licence)

## Introduction

__ORAGE is a live visual modular synthetizer__</br></br>
As visual artist and developper, I think a part of my creativity comes from my shortcomings. Therefore, build my own visual softwares has always been obvious.
This tool is in fact the «whathousand» iteration of that idea to create that kind of software.
</br></br>
[TO DO -- description] c++(cinder)
</br></br>
Take a look at [ORAGE TV](https://www.youtube.com/channel/UCZ0oXcxj3lE8kQSSsniIZ6w) to see it in action.

## Inspiration

I'm inspired by analogic video synth as [LZX](https://lzxindustries.net/),<br/>
node programming visual generators as [cables.gl](https://cables.gl/home),<br/>digital video synth as [Lumen](https://lumen-app.com/),<br/>
glitch production softwares as [signal culture](http://signalculture.org/),<br/> 
this amazing tutorial [the book of shaders](https://thebookofshaders.com)<br/> and many others... <br/> Last but not least, I've been mainly inspired by my bandmate [Harold Osica](https://www.facebook.com/osicamusic).

## State of art

### Synthetizers

1. #### Matte
   
   [![Matte Module](./doc/modules/Matte.jpg)](./doc/modules/Matte.jpg)<br/>
   YPbPr is converted from the RGB video signal, which is split into three components: Y, PB, and PR. <br/>This color space has been choosen to have a better granularity control on brightness and tint. <br/>
   
   - __Input__ : None
   - __Output__ : Unicolor video output signal.
   - __Y__ : Luma (brightness or luminance) information. 
   - __Pb__ : the difference between blue and luma.
   - __Pr__ : the difference between red and luma.
     
     ```
     R = Y + 1.574f * Pr
     G = Y - 0.187f * Pb - 0.469f * Pr
     B = Y + 1.856f * Pb
     ```

2. #### Oscillator
   
   [![Oscillator Module](./doc/modules/Oscillator.jpg)](./doc/modules/Oscillator.jpg)<br/>
   
   - __Input__ : 
     - __A__ : __Mod__ intensity video input signal.
     - __B__ : Color video input signal.
   - __Output__ : 
     - __A__ : Oscillator video output signal.
     - __B__ : Trailed video output signal.
   - __Hz__ : Frequency of the oscillation.
   - __Sync__ : Synchronisation of the frequency on Horizontal/Vertical axes.
   - __Dx__ : Offset the phase of the oscillation.
   - __Dy__ : Offset the Intensity of the oscillation.
   - __Mod__ : Offset the phase of the oscillation depending on __Input A__.
   - __Trail__ : A curious way to keep trace of lasts states of the oscillator.
   - __Sine__ : Intensity of the Sinus wave.
   - __Saw__ : Intensity of the Saw wave.
   - __Tri__ : Intensity of the Triangular wave.
   - __Noise__ : Intensity of the Noise wave.
   - __Exp__ : A curious way to contrast the signal of the oscillator.

3. #### Cloud
   
   [![Cloud Module](./doc/modules/Cloud.jpg)](./doc/modules/Cloud.jpg)<br/>
   [TO DO -- description]

### Filters

4. #### Mosher
   
   [![Mosher Module](./doc/modules/Mosher.jpg)](./doc/modules/Mosher.jpg)<br/>
   
   - __Input__ : 
     - __A__ : video input signal to filter.
     - __B__ : video input signal to callibrate the filter.
   - __Output__ : Scaled video output signal.
   - __Gain__ : Scale factor.
   - __X__ : Offset the horizontal position of the anchor point.
   - __Y__ : Offset the vertical position of the anchor point.

5. #### Spliter
   
   [![Spliter Module](./doc/modules/Spliter.jpg)](./doc/modules/Spliter.jpg)<br/>
   
   - __Input__ : 
     - __A__ : video input signal to filter.
     - __B__ : video input signal to filter.
   - __Output__ : 
     - __A__ : max(Input A - Input B, 0).
     - __B__ : max(Input B - Input A, 0).

6. #### Crossfader
   
   [![Crossfader Module](./doc/modules/Crossfader.jpg)](./doc/modules/Crossfader.jpg)<br/>
   
   - __Input__ : 
     - __A__ : video input signal to filter.
     - __B__ : video input signal to filter.
   - __Output__ : Mixed video output signal.
   - __Blend__ : Select filters : 
     - Add : min(A + B, 1)
     - Normal : A
     - Lighten : max(A, B)
     - Darken : min(A, B)
     - Multiply : A x B
     - Average : (A + B) / 2
     - Substract : max(A + B - 1, 0)
     - Difference : abs(A - B)
     - Negation : (1 - abs(1 - A - B))
     - Exclusion : A + B - 2.0 * A * B
     - Phoenix : (min(A, B) - max(A, B) + 1)
     - Overlay : (A < 0.5 ? (2.0 * A * B) : (1.0 - 2.0 * (1.0 - A) * (1.0 - B)))
     - HardMix : ((BlendVividLight(A, B) < 0.5) ? 0.0 : 1.0)
       - BlendVividLight : ((B < 0.5) ? BlendColorBurnf(A, (2.0 * B)) : BlendColorDodgef(A, (2.0 * (B - 0.5))))
       - BlendColorBurnf : ((B == 0.0) ? B : max((1.0 - ((1.0 - A) / B)), 0.0))
       - BlendColorDodgef : ((B == 1.0) ? B : min(A / (1.0 - B), 1.0))
   - __Crossfade__ : Mix Inputs __A__ · __B__

7. #### Tile
   
   [![Tile Module](./doc/modules/Tile.jpg)](./doc/modules/Tile.jpg)<br/>
   [TO DO -- description]

8. #### Freezer
   
   [![Freezer Module](./doc/modules/Freezer.jpg)](./doc/modules/Freezer.jpg)<br/>
   [TO DO -- description]

9. #### Color Adjustement
   
   [![ColorAdjustement Module](./doc/modules/ColorAdjustement.jpg)](./doc/modules/ColorAdjustement.jpg)<br/>
   [TO DO -- description]

10. #### Tint Corrector
    
    [![TintCorrector Module](./doc/modules/TintCorrector.jpg)](./doc/modules/TintCorrector.jpg)<br/>
    [TO DO -- description]

11. #### Delay
    
    [![Delay Module](./doc/modules/Delay.jpg)](./doc/modules/Delay.jpg)<br/>
    [TO DO -- description]

12. #### Resize
    
    [![Resize Module](./doc/modules/Resize.jpg)](./doc/modules/Resize.jpg)<br/>
    [TO DO -- description]

13. #### Blur
    
    [![Blur Module](./doc/modules/Blur.jpg)](./doc/modules/Blur.jpg)<br/>
    [TO DO -- description]

14. #### Kaleidoscope
    
    [![Kaleidoscope Module](./doc/modules/Kaleidoscope.jpg)](./doc/modules/Kaleidoscope.jpg)<br/>
    [TO DO -- description]

### Controllers

15. #### Lfos
    
    [![Lfo Module](./doc/modules/Lfo.jpg)](./doc/modules/Lfo.jpg)<br/>
    [TO DO -- description]
16. #### Random
    
    [![Random Module](./doc/modules/Random.jpg)](./doc/modules/Random.jpg)<br/>
    [TO DO -- description]
17. #### ProcessCV
    
    [![ProcessCV Module](./doc/modules/ProcessCV.jpg)](./doc/modules/ProcessCV.jpg)<br/>
    [TO DO -- description]

### Players

18. #### SyphonInput
    
    [![loading-ag-840](./doc/modules/Syphon.jpg)](./doc/modules/SyphonInput.jpg)<br/>
    [TO DO -- description]
19. #### Player
    
    [![Player Module](./doc/modules/Player.jpg)](./doc/modules/Player.jpg)<br/>
    [TO DO -- description]

### Outputs

20. #### Output
    
    [![Output Module](./doc/modules/Output.jpg)](./doc/modules/Output.jpg)<br/>
    [TO DO -- description]

## Installation

Currently available only on OSX(10.13.6). <br/>
[Download](https://github.com/oogre/ORAGE/releases)

## Contributes

This software is written in C++([cinder](https://github.com/cinder/Cinder))<br/>
The user interface is build thanks to [UI](https://github.com/rezaali/Cinder-UI) cinder block from [Reza Ali](https://www.syedrezaali.com/)<br/>
Let me know your [ideas, needs or issues](https://github.com/oogre/ORAGE/issues)<br/>
Fork this repository and feel free to propose new functionalities or upgrade.<br/>

## Licence

[![License: CC BY-NC-SA 4.0](https://img.shields.io/badge/License-CC%20BY--NC--SA%204.0-lightgrey.svg)](https://creativecommons.org/licenses/by-nc-sa/4.0/)