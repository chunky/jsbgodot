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
* Open demo/project in Godot

## Implementation Notes

Fundamentally, what this does is extend Godot's "Spatial" node. This
GDNative script has no understanding of how inputs are handled; it
exposes some input members, then you plumb them in script. [This is so
that inputs could come from an AI, or from HID, or elsewhere; this code
does not have an opinion about that]

## Messing with it

As I write this, this barely qualifies as a "test to see if this can be
done". Turns out, it can be done. When I was googling for "JSBSim Godot",
this is what I was looking for... except it didn't exist at the time.

## License

MIT License.


Gary <chunky@icculus.org>
