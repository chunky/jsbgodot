#ifndef JSBGODOT_H
#define JSBGODOT_H

#include <Godot.hpp>
#include <Sprite.hpp>
#include <Spatial.hpp>
#include "FGFDMExec.h"
#include <math.h>

namespace godot {

class JSBGodot : public Spatial {
    GODOT_CLASS(JSBGodot, Spatial)

private:
    JSBSim::FGFDMExec *FDMExec;
    bool do_scripted;

    float input_pitch = 0.0;
    float input_roll = 0.0;
    float input_rudder = 0.0;
    float input_throttle = 1.0;

    void copy_inputs_to_JSBSim();

    void copy_outputs_from_JSBSim();

public:
    static void _register_methods();

    JSBGodot();
    ~JSBGodot();

    void _init(); // our initializer called by Godot

    void _process(float delta);

    void _input(const Ref<InputEvent> event);

    void _physics_process(const real_t delta);

    void initialise_loc();
};

}

#endif

