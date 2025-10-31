<h1 align="center">
<img align="center" src="https://github.com/memory-hunter/hill-climb-racing-vita/blob/master/extras/livearea/pic0.png?raw=true" width="50%"><br>
Hill Climb Racing · PSVita Port
</h1>
<p align="center">
  <a href="#setup-instructions-for-end-users">How to install</a> •
  <a href="#controls">Controls</a> •
  <a href="#build-instructions-for-developers">How to compile</a> •
  <a href="#credits">Credits</a> •
  <a href="#license">License</a>
</p>


Hill Climb Racing is a game made by Fingersoft, a Finnish game engine where you can control a vehicle and try to survive as far as possible to get more coins for more upgrades, vehicles and stages! Each stage has a different layout, and different obstacles to overcome. Each vehicle has their own strengths and weaknesses, making certain vehicles excel in certain stages, and their vehicles have overalls. 

This repository contains a loader of **the Android release of Hill Climb Racing**, based
on the [Android SO Loader by TheFloW](https://github.com/TheOfficialFloW/gtasa_vita).
The loader provides a tailored, minimalistic Android-like environment to run
the official ARMv7 game executables on the PS Vita.

**This software does not contain the original code, executables, assets, or other
not redistributable parts of the game. The authors do not promote or condone piracy
in any way. To launch and play the game on their PS Vita device, users must provide
their own legally obtained copy of the game in form of an .apk file.**

### Minor notes
- When finishing a play session, don't quit the game immediately. Wait for around 3 seconds and then quit, otherwise your game may crash or your save data may be corrupted.
- When selecting levels or cars, wait until it responds, because it's loading things in the background.

This is currently in its **BETA** version. Improvements may be made in the future. See [Issues](https://github.com/memory-hunter/hill-climb-racing-vita/issues) for currently know problems.

## Setup Instructions (For End Users)

In order to properly install the game, you'll have to follow these steps precisely:

- Install [kubridge](https://github.com/TheOfficialFloW/kubridge/releases/) and [FdFix](https://github.com/TheOfficialFloW/FdFix/releases/) by copying `kubridge.skprx` and `fd_fix.skprx` to your taiHEN plugins folder (usually `ur0:tai`) and adding two entries to your `config.txt` under `*KERNEL`:
  
```
  *KERNEL
  ur0:tai/kubridge.skprx
  ur0:tai/fd_fix.skprx
```

**Note** Don't install fd_fix.skprx if you're using rePatch plugin!
- Make sure you have `libshacccg.suprx` in the `ur0:/data/` folder on your console. If you don't, follow [this guide](https://samilops2.gitbook.io/vita-troubleshooting-guide/shader-compiler/extract-libshacccg.suprx) to extract it.
- <u>Legally</u> obtain your copy of [Hill Climb Racing](https://play.google.com/store/apps/details?id=com.fingersoft.hillclimb&hl=en)
version 1.18.0 for Android in form of an `.apk` file.
- Open the `.apk` with any zip explorer (like [7-Zip](https://www.7-zip.org/)) and extract `assets` folder from the `.apk` into `ux0:data/hcr`. Also, extract `libgame.so` from `lib/armeabi-v7a/` in the same directory.
- Place the `.apk` itself inside `ux0:data/hcr` and rename to `base.apk`.

  Your final folder layout should look like:
  ```
   └── hcr/
    ├── assets/
    ├── libgame.so
    └── base.apk
  ```
- Install `hcr.vpk` (from [Releases](https://github.com/memory-hunter/hill-climb-racing-vita/releases/latest)).

Controls
-----------------

|             Button             | Action        |
|:------------------------------:|:-------------------|
|      ![trigl] / ![trigr]       |  Brake/Accelerate  |
|            ![circl]            |        Back        |

and the touch functionality fully works (if you don't break it somehow).

## Build Instructions (For Developers)

In order to build the loader, you'll need a [vitasdk](https://github.com/vitasdk) build fully compiled with softfp usage.
You can get your environment started nicely by following [Rocroverss's Vita .so porting guide](https://github.com/Rocroverss/vitasoguide?tab=readme-ov-file#section2).
Everything that is required should be available after installing your environment by following the instructions in the README.md of the guide.

- Compile [vitaGL](https://github.com/Rinnegatamante/vitaGL)
  ````bash
  make HAVE_GLSL_SUPPORT=1 CIRCULAR_VERTEX_POOL=2 USE_SCRATCH_MEMORY=1 SOFTFP_ABI=1 HAVE_SHADER_CACHE=1 install -j`nproc`
  ````

After all these requirements are met, you can compile the loader with the following commands:

```bash
cmake -B build .
cmake --build build
```

## Credits
- [Andy "The FloW" Nguyen](https://github.com/TheOfficialFloW/) for the original .so loader.
- [Rinnegatamante](https://github.com/Rinnegatamante/) for VitaGL and lots of help with understanding and debugging the loader.
- [PatnosD](https://github.com/PatnosDD) for giving me the motivation to do this game as my first .so loader port.
- [gl33ntwine](https://github.com/v-atamanenko/) for the [SoLoBoP (.**so** **lo**ader **bo**iler**p**late)](https://github.com/v-atamanenko/soloader-boilerplate/), help with the SharedPreferences parsing functions, this README.md as a template copied from [Baba is You! port](https://github.com/v-atamanenko/baba-is-you-vita/) and overall help.
- Everybody who has helped me in Vita Nuova discord while asking questions.

## License
This software may be modified and distributed under the terms of
the MIT license. See the [LICENSE](LICENSE) file for details.

[cross]: https://raw.githubusercontent.com/v-atamanenko/sdl2sand/master/img/cross.svg "Cross"
[circl]: https://raw.githubusercontent.com/v-atamanenko/sdl2sand/master/img/circle.svg "Circle"
[squar]: https://raw.githubusercontent.com/v-atamanenko/sdl2sand/master/img/square.svg "Square"
[trian]: https://raw.githubusercontent.com/v-atamanenko/sdl2sand/master/img/triangle.svg "Triangle"
[joysl]: https://raw.githubusercontent.com/v-atamanenko/sdl2sand/master/img/joystick-left.svg "Left Joystick"
[dpadh]: https://raw.githubusercontent.com/v-atamanenko/sdl2sand/master/img/dpad-left-right.svg "D-Pad Left/Right"
[dpadv]: https://raw.githubusercontent.com/v-atamanenko/sdl2sand/master/img/dpad-top-down.svg "D-Pad Up/Down"
[selec]: https://raw.githubusercontent.com/v-atamanenko/sdl2sand/master/img/dpad-select.svg "Select"
[start]: https://raw.githubusercontent.com/v-atamanenko/sdl2sand/master/img/dpad-start.svg "Start"
[trigl]: https://raw.githubusercontent.com/v-atamanenko/sdl2sand/master/img/trigger-left.svg "Left Trigger"
[trigr]: https://raw.githubusercontent.com/v-atamanenko/sdl2sand/master/img/trigger-right.svg "Right Trigger"
