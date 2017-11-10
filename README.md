# Semester project at Sound and Music Computing, AAU Copenhagen

### Authors
- Michael Castanieto
- Jonas Holfelt
- Gergely Csapo


#### Instructions:
1. Create a new Audio Application template from Projucer with the name _FiguraTK_
2. Clone or download the repo
3. Replace the Source folder in your Projucer project with the new Source folder
4. Open the Projucer project
5. Select desired target and click `Save and Open in IDE` (currently only working for iOS and OSX)
6. Build the application

#### Release notes v0.3(Jolene):
>Pitch shifting, utilizing the Soundtouch library. Multitouch functionality added, two finger's `Y` position controls the pitch and one finger's `Y` position is mapped to gain on a logarithmic scale.

#### Release notes v0.2(Talkboy):
>Introducing fingertracking and real-time gain control on a new graphical interface. Auto-truncation of recording implemented in order to automatically crop samples to transients. Press and hold `Record` button to start recording and play it back by pressing on the top part of the application window. Visual feedback added on playback, marking the finger position. The `Y` position of the finger determines the gain of the output.

#### Release notes v0.1(Smoke n Mirrors):
>First prototype, presenting the core functionality of the application. This prototype demonstrates recording audio from a device's microphone to a buffer, where the audio can be played back. Record audio by pressing `Record` and stop recording by pressing `Stop` on the left side of the screen. Play the audio back by pressing `Play` and stop playback by pressing `Stop` on the right side of the screen. Audio can be recorded for no more than 3 seconds. Devices have been tested successfully on iOS for both iPhone and iPad.
