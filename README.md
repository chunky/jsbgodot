# Godot JSBSim Module

## Introduction

I needed a high quality FDM attched to a game engine, without all the
usual game fluff and trappings.

Godot is open source, and JSBSim is open source. The rest is history

## Building

* Make sure to clone with submodules
```script
git clone this_repo jsbgodot
cd jsbgodot
git submodule update --init --recursive
```
* Build JSBSim
```script
cd jsbsim
cmake .
make
```
* Build Godot Bindings
```script
cd godot-cpp
# Replace <platform> with your platform
scons platform=<platform> generate_bindings=yes -j4
```
* Build this
```script
# Replace <platform> with your platform. Defaults to Linux
scons platform=<platform>
```

Gary <chunky@icculus.org>
