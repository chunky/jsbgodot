#include "jsbgodot.h"
#include "FGFDMExec.h"
#include <InputEvent.hpp>
#include <unistd.h>
#include "models/FGFCS.h"
#include "models/FGAircraft.h"
#include "models/FGPropulsion.h"

/**
 * Simple script to connect JSBSim to a Godot 3D Spatial body
 *
 * On JSBSim: http://jsbsim.sourceforge.net/JSBSimReferenceManual.pdf
 *      https://github.com/JSBSim-Team/jsbsim/issues/396
 *
 * On Godot: https://docs.godotengine.org/en/stable/tutorials/plugins/gdnative/gdnative-cpp-example.html
 */
using namespace godot;

void JSBGodot::_register_methods() {
    register_method("_process", &JSBGodot::_process);
    register_method("_physics_process", &JSBGodot::_physics_process);
    register_method("_input", &JSBGodot::_input);
    register_property<JSBGodot, float>("input_pitch", &JSBGodot::input_pitch, 0.0);
    register_property<JSBGodot, float>("input_roll", &JSBGodot::input_roll, 0.0);
    register_property<JSBGodot, float>("input_rudder", &JSBGodot::input_rudder, 0.0);
    register_property<JSBGodot, float>("input_throttle", &JSBGodot::input_throttle, 1.0);
}

JSBGodot::JSBGodot() {
}

JSBGodot::~JSBGodot() {
    delete FDMExec;
}

void JSBGodot::_init() {
    FDMExec = new JSBSim::FGFDMExec();
    char cwd[1024];
    char *thing = getcwd(cwd, 1024);
    printf("cwd: %s\n", thing);
    FDMExec->SetRootDir(SGPath("../jsbsim"));
    FDMExec->SetAircraftPath(SGPath("aircraft"));
    FDMExec->SetEnginePath(SGPath("engine"));
    FDMExec->SetSystemsPath(SGPath("systems"));
    do_scripted = true;
    if (do_scripted) {
        FDMExec->LoadScript(SGPath("scripts/Short_S23_4.xml"));
    } else {
        FDMExec->LoadModel(SGPath("aircraft"),
                           SGPath("engine"),
                           SGPath("systems"),
                           "C130");
        initialise_loc();
        copy_inputs_to_JSBSim();
    }
//    std::shared_ptr<JSBSim::FGAircraft> ac = FDMExec->GetAircraft();
//    std::shared_ptr<JSBSim::FGPropulsion> prop = FDMExec->GetPropulsion();
//    std::shared_ptr<JSBSim::FGEngine> engine = prop->GetEngine(-1);
    FDMExec->RunIC();
//    FDMExec->DoTrim((JSBSim::TrimMode));
}

void JSBGodot::initialise_loc() {
    std::shared_ptr<JSBSim::FGPropagate> Propagate = FDMExec->GetPropagate();
    Propagate->SetLongitude(0.0);
    Propagate->SetLatitude(0.0);
    Propagate->SetAltitudeASLmeters(9150);
    JSBSim::FGInitialCondition initial_cond;
    Propagate->SetInitialState()
//    FDMExec->DoTrim(0);
}

void JSBGodot::copy_inputs_to_JSBSim() {
    if(do_scripted) {
        return;
    }
//    return;
    std::shared_ptr<JSBSim::FGFCS> FCS = FDMExec->GetFCS();

    FCS->SetDeCmd(input_pitch);
    FCS->SetDaCmd(input_roll);
    FCS->SetDrCmd(input_rudder);
    FCS->SetThrottleCmd(-1, input_throttle);
}

void JSBGodot::copy_outputs_from_JSBSim() {

    std::shared_ptr<JSBSim::FGPropagate> Propagate = FDMExec->GetPropagate();
//    std::shared_ptr<JSBSim::FGAuxiliary> Auxiliary = FDMExec->GetAuxiliary();
//    std::shared_ptr<JSBSim::FGAtmosphere> Atmosphere = FDMExec->GetAtmosphere();
//    std::shared_ptr<JSBSim::FGAccelerations> Accelerations = FDMExec->GetAccelerations();

    float altitude = Propagate->GetAltitudeASL();
    float latitude = Propagate->GetLocation().GetLatitudeDeg();
    float longitude = Propagate->GetLocation().GetLongitudeDeg();

    float bank = Propagate->GetEuler(JSBSim::FGForce::ePhi) * -1.0;
    float pitch = Propagate->GetEuler(JSBSim::FGForce::eTht) * -1.0;
    float heading = Propagate->GetEuler(JSBSim::FGForce::ePsi);

    Vector3 newRot = Vector3(pitch, heading, bank);
    set_rotation(newRot);

    float x = longitude;
    float y = altitude;
    float z = latitude;
    Vector3 newPos = Vector3(x, y, z);
    set_translation(newPos);
    printf("LLA: %f,%f,%f\n", latitude, longitude, altitude);
}

void JSBGodot::_input(const Ref<InputEvent> event) {
}

void JSBGodot::_physics_process(const real_t delta) {
    copy_inputs_to_JSBSim();
    FDMExec->Run();
    copy_outputs_from_JSBSim();
}

void JSBGodot::_process(float delta) {
}

