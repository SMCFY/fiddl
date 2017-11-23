# fiddl

### Authors
- Michael Castanieto
- Jonas Holfelt
- Gergely Csapo


#### Instructions:
1. Clone or download the repo for this project.
2. Open the Projucer
3. Create a new Audio Application template from Projucer with the name _FiguraTK_ and select iOS as a target platform.
4. Replace the `Source` folder in your JUCE project with the `Source` folder in the repo.
5. Add the soundtouch folder to your JUCE project
6. In `Project Settings`
&nbsp;&nbsp;Add the `soundtouch/include` path to `Header search paths` 
&nbsp;&nbsp;Change `C++ Language Standard` to `C++14
7. Add the `juce_dsp` module to your JUCE project.
8. Select desired target to export and click `Save and Open in IDE` (currently only working for iOS and OSX)
9. Build the application

#### Release notes v0.4(Moustachio):
>Lowpass and highpass filters are added to the list of audio processors, and envelopes are implemented for playback. This release is introducing two different parameter spaces with different mapping schemes. One intended to be used for sustained sound and the other for impulse sounds. The `sustained space` features an array of buttons representing discrete pitches. The `impulse space` is inspired by a cicular membrane, using the distance from center to control pitch, release time, and highpass filter cutoff.

#### Release notes v0.3(Jolene):
>Pitch shifting, utilizing the Soundtouch library. Multitouch functionality added, two finger's `Y` position controls the pitch and one finger's `Y` position is mapped to gain on a logarithmic scale.

#### Release notes v0.2(Talkboy):
>Introducing fingertracking and real-time gain control on a new graphical interface. Auto-truncation of recording implemented in order to automatically crop samples to transients. Press and hold `Record` button to start recording and play it back by pressing on the top part of the application window. Visual feedback added on playback, marking the finger position. The `Y` position of the finger determines the gain of the output.

#### Release notes v0.1(Smoke n Mirrors):
>First prototype, presenting the core functionality of the application. This prototype demonstrates recording audio from a device's microphone to a buffer, where the audio can be played back. Record audio by pressing `Record` and stop recording by pressing `Stop` on the left side of the screen. Play the audio back by pressing `Play` and stop playback by pressing `Stop` on the right side of the screen. Audio can be recorded for no more than 3 seconds. Devices have been tested successfully on iOS for both iPhone and iPad.
