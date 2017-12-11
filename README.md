#fiddl

Description: [fiddl](https://itunes.apple.com/dk/app/fiddl/id1320021639?mt=8&ign-itsct=1320021639-1320021639&ign-itscg=0176&ign-mpt=uo%3D4) is love, fiddl is life

<a href="https://itunes.apple.com/dk/app/fiddl/id1320021639?mt=8&ign-itsct=1320021639-1320021639&ign-itscg=0176&ign-mpt=uo%3D4">
<img src="https://github.com/SMCFY/fiddl/blob/develop/Resources/Images/fiddl.png?raw=true" width="250" href="https://itunes.apple.com/dk/app/fiddl/id1320021639?mt=8&ign-itsct=1320021639-1320021639&ign-itscg=0176&ign-mpt=uo%3D4">
</a>

### Synopsis

At the top of the file there should be a short introduction and/ or overview that explains **what** the project is. This description should match descriptions added for package managers (Gemspec, package.json, etc.)

### Motivation

A short description of the motivation behind the creation and maintenance of the project. This should explain **why** the project exists.

## Getting Started
Try out fiddl on the [App Store](https://itunes.apple.com/dk/app/fiddl/id1320021639?mt=8&ign-itsct=1320021639-1320021639&ign-itscg=0176&ign-mpt=uo%3D4) if you would like to run the latest stable version on iPhone or iPad.

The following installation guide will help you to get this project running on your local macOS computer for development and testing purposes. 

### Prerequisities

This project was developed on macOS 10.12 and above and has been tested on iOS 11.0 and above.

The following software must be installed prior to building fiddl:

```
* JUCE (5.2.0)
* Xcode (11.2)
```

### Installing

The following instructions will guide you in setting up the development environment. It assumes that you are familiar with JUCE and know how to use the Projucer. you know how to use the JUCE Projuce

#### Instructions

1. Clone or download the repo for this project.
2. Open the Projucer
3. Create a new Audio Application template from Projucer with the name _FiguraTK_ and select iOS as a target platform.
4. Replace the `Source` folder in your JUCE project with the `Source` folder in the repo.
5. Add the `soundtouch` folder to your JUCE project
6. In `Project Settings` add the `soundtouch/include` path to `Header search paths` and change `C++ Language Standard` to `C++14`
7. Add the `juce_dsp` module to your JUCE project.
8. Select `Xcode (iOS)` as the `Selected exporter` and click `Save and Open in IDE`
9. Build the application


## Built With

* macOS (version 10.12 and above)
* [JUCE](https://juce.com) (version 5.2)
* [Xcode](https://developer.apple.com/xcode/) (version 11.2)
* [SoundTouch Audio Processing Library](https://www.surina.net/soundtouch/) (version 1.9.2)

## Authors

* **Jonas Holfelt** - *Initial work* - [jholfelt](https://github.com/jholfelt)
* **Geri Csapo** - *Initial work* - [gericsapo](https://github.com/gericsapo)
* **Michael Castanieto** - *Initial work* - [umnum](https://github.com/umnum)

## License

This project is licensed under the **BSD License** - see the [LICENSE.md](LICENSE.md) file for details.

## Acknowledgments

* Hat tip to anyone who's code was used
* Inspiration
* etc
