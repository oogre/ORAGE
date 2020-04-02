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
### Matte 
[![Matte Module](./doc/modules/Matte.jpg)](./doc/modules/Matte.jpg)<br/>
YPbPr is converted from the RGB video signal, which is split into three components: Y, PB, and PR. This color space has been choosen to have a better control on brightness and tint. 
Y : Luma (brightness or luminance) information. 
Pb : the difference between blue and luma.
Pr : the difference between red and luma.
```
    R = Y + 1.574f * Pr
    G = Y - 0.187f * Pb - 0.469f * Pr
    B = Y + 1.856f * Pb
```
### Oscillator
[![Oscillator Module](./doc/modules/Oscillator.jpg)](./doc/modules/Oscillator.jpg)<br/>
[TO DO -- description]
### Cloud
[![Oscillator Module](./doc/modules/Oscillator.jpg)](./doc/modules/Oscillator.jpg)<br/>
[TO DO -- description]
### Mosher
[![Mosher Module](./doc/modules/Mosher.jpg)](./doc/modules/Mosher.jpg)<br/>
[TO DO -- description]
### Spliter
[![Spliter Module](./doc/modules/Spliter.jpg)](./doc/modules/Spliter.jpg)<br/>
[TO DO -- description]
### Crossfader
[![Crossfader Module](./doc/modules/Crossfader.jpg)](./doc/modules/Crossfader.jpg)<br/>
[TO DO -- description]
### Tile
[![Tile Module](./doc/modules/Tile.jpg)](./doc/modules/Tile.jpg)<br/>
[TO DO -- description]
### Freezer
[![Freezer Module](./doc/modules/Freezer.jpg)](./doc/modules/Freezer.jpg)<br/>
[TO DO -- description]
### ColorAdjustement
[![ColorAdjustement Module](./doc/modules/ColorAdjustement.jpg)](./doc/modules/ColorAdjustement.jpg)<br/>
[TO DO -- description]
### TintCorrector
[![TintCorrector Module](./doc/modules/TintCorrector.jpg)](./doc/modules/TintCorrector.jpg)<br/>
[TO DO -- description]
### Delay
[![Delay Module](./doc/modules/Delay.jpg)](./doc/modules/Delay.jpg)<br/>
[TO DO -- description]
### Resize
[![Resize Module](./doc/modules/Resize.jpg)](./doc/modules/Resize.jpg)<br/>
[TO DO -- description]
### Blur
[![Blur Module](./doc/modules/Blur.jpg)](./doc/modules/Blur.jpg)<br/>
[TO DO -- description]
### Kaleidoscope
[![Kaleidoscope Module](./doc/modules/Kaleidoscope.jpg)](./doc/modules/Kaleidoscope.jpg)<br/>
[TO DO -- description]
### Lfos
[![Lfo Module](./doc/modules/Lfo.jpg)](./doc/modules/Lfo.jpg)<br/>
[TO DO -- description]
### Random
[![Random Module](./doc/modules/Random.jpg)](./doc/modules/Random.jpg)<br/>
[TO DO -- description]
### ProcessCV
[![ProcessCV Module](./doc/modules/ProcessCV.jpg)](./doc/modules/ProcessCV.jpg)<br/>
[TO DO -- description]
### SyphonInput
[![SyphonInput Module](./doc/modules/SyphonInput.jpg)](./doc/modules/SyphonInput.jpg)<br/>
[TO DO -- description]
### Player
[![Player Module](./doc/modules/Player.jpg)](./doc/modules/Player.jpg)<br/>
[TO DO -- description]
### Output
[![Output Module](./doc/modules/Output.jpg)](./doc/modules/Output.jpg)<br/>
[TO DO -- description]

## Installation
Currently available only on OSX(10.13.6). <br/>
[Download](https://github.com/oogre/ORAGE/raw/master/Orage.app.zip)
## Contributes
This software is written in C++([cinder](https://github.com/cinder/Cinder))<br/>
The user interface is build thanks to [UI](https://github.com/rezaali/Cinder-UI) cinder block from [Reza Ali](https://www.syedrezaali.com/)<br/>
Let me know your [ideas, needs or issues](https://github.com/oogre/ORAGE/issues)<br/>
Fork this repository and feel free to propose new functionalities or upgrade.<br/>
## Licence
[![License: CC BY-NC-SA 4.0](https://img.shields.io/badge/License-CC%20BY--NC--SA%204.0-lightgrey.svg)](https://creativecommons.org/licenses/by-nc-sa/4.0/)