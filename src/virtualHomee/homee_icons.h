#pragma once

// Node icon strings ("image" field of node::setImage()). homee does NOT
// derive this automatically from the profile - it must be set explicitly,
// otherwise the app shows a generic question-mark icon for less common
// profiles (confirmed e.g. for CANodeProfileWaterMeter). The app's icon
// picker is disabled for some profiles, but the field is still respected
// when set programmatically.
//
// Unlike homee_defines.h (1:1 conversion of the official SDK enums), no
// official list of these identifiers is published by homee. The values
// below were compiled from two sources, both accessed through an
// ordinary web browser against the author's own homee cube:
//
//   1. Live "image" field values observed in the cube's own WebSocket
//      JSON output for real, already-configured devices (e.g.
//      "nodeicon_radiator", "nodeicon_energymeter") - i.e. the plain
//      interface/protocol data a homee cube sends to any client that
//      talks to it, not homee's application code.
//   2. A naming pattern noticed in strings shipped inside the web app's
//      client-side bundle ("nodeicon_<name>_cubetype_<N>_value_<M>[_unknown]"),
//      used only to infer additional identifier names by the same
//      pattern as (1). No app code, logic, markup, styling, or icon
//      artwork was copied, reproduced, or redistributed - only the bare
//      "<name>" identifier strings, i.e. plain data values for the
//      documented "image" field, are listed below.
//
// This list exists purely for interoperability with the "image" field of
// homee's own published node/attribute protocol (see homee_defines.h and
// https://github.com/stfnhmplr/homee-api) and contains no homee icon
// graphics, trademarks-as-branding, or original application source.
// It is unofficial and unaffiliated with homee; identifiers may change
// or be incomplete. Extend as more are discovered.

constexpr const char* NodeIconDefault                = "default";
constexpr const char* NodeIconArmchair                = "nodeicon_armchair";
constexpr const char* NodeIconAwning                  = "nodeicon_awning";
constexpr const char* NodeIconBed                     = "nodeicon_bed";
constexpr const char* NodeIconBinaryInput             = "nodeicon_binaryinput";
constexpr const char* NodeIconBolt                    = "nodeicon_bolt";
constexpr const char* NodeIconBrightnessSensor        = "nodeicon_brightnesssensor";
constexpr const char* NodeIconBulb                    = "nodeicon_bulb";
constexpr const char* NodeIconCamera                  = "nodeicon_camera";
constexpr const char* NodeIconCo2                     = "nodeicon_co2";
constexpr const char* NodeIconCositherm               = "nodeicon_cositherm";
constexpr const char* NodeIconDimmableBulb            = "nodeicon_dimmablebulb";
constexpr const char* NodeIconDimmablePlug            = "nodeicon_dimmableplug";
constexpr const char* NodeIconDoorbell                = "nodeicon_doorbell";
constexpr const char* NodeIconDoorSensor              = "nodeicon_doorsensor";
constexpr const char* NodeIconDoubleBulb              = "nodeicon_doublebulb";
constexpr const char* NodeIconDoublePlug              = "nodeicon_doubleplug";
constexpr const char* NodeIconDryer                   = "nodeicon_dryer";
constexpr const char* NodeIconEnergyMeter             = "nodeicon_energymeter";
constexpr const char* NodeIconEntranceDoor            = "nodeicon_entrancedoor";
constexpr const char* NodeIconFloodDetector           = "nodeicon_flooddetector";
constexpr const char* NodeIconGarageDoor              = "nodeicon_garagedoor";
constexpr const char* NodeIconGate                    = "nodeicon_gate";
constexpr const char* NodeIconGestureControl          = "nodeicon_gesturecontrol";
constexpr const char* NodeIconGroup                   = "nodeicon_group";
constexpr const char* NodeIconHeatingSystem           = "nodeicon_heatingsystem";
constexpr const char* NodeIconHomeegram               = "nodeicon_homeegram";
constexpr const char* NodeIconHorizontalSectionalDoor = "nodeicon_horizontalsectionaldoor";
constexpr const char* NodeIconLock                    = "nodeicon_lock";
constexpr const char* NodeIconManometer               = "nodeicon_manometer";
constexpr const char* NodeIconMaximumDetector         = "nodeicon_maximumdetector";
constexpr const char* NodeIconMinimumDetector         = "nodeicon_minimumdetector";
constexpr const char* NodeIconMotionDetector          = "nodeicon_motiondetector";
constexpr const char* NodeIconNetatmoIndoorModule     = "nodeicon_netatmoindoormodule";
constexpr const char* NodeIconNetatmoMainModule       = "nodeicon_netatmomainmodule";
constexpr const char* NodeIconNetatmoOutdoorModule    = "nodeicon_netatmooutdoormodule";
constexpr const char* NodeIconNetatmoRainModule       = "nodeicon_netatmorainmodule";
constexpr const char* NodeIconNetatmoWindModule       = "nodeicon_netatmowindmodule";
constexpr const char* NodeIconNode                    = "nodeicon_node";
constexpr const char* NodeIconNoProfile               = "nodeicon_noprofile";
constexpr const char* NodeIconOneButtonRemote         = "nodeicon_onebuttonremote";
constexpr const char* NodeIconPatioDoubleDoor         = "nodeicon_patiodoubledoor";
constexpr const char* NodeIconPlan                    = "nodeicon_plan";
constexpr const char* NodeIconPlug                    = "nodeicon_plug";
constexpr const char* NodeIconPoller                  = "nodeicon_poller";
constexpr const char* NodeIconPot                     = "nodeicon_pot";
constexpr const char* NodeIconPresence                = "nodeicon_presence";
constexpr const char* NodeIconRadiator                = "nodeicon_radiator";
constexpr const char* NodeIconRemote                  = "nodeicon_remote";
constexpr const char* NodeIconRepeater                = "nodeicon_repeater";
constexpr const char* NodeIconRoadblocker             = "nodeicon_roadblocker";
constexpr const char* NodeIconSectionalDoor           = "nodeicon_sectionaldoor";
constexpr const char* NodeIconShield                  = "nodeicon_shield";
constexpr const char* NodeIconShutter                 = "nodeicon_shutter";
constexpr const char* NodeIconSiren                   = "nodeicon_siren";
constexpr const char* NodeIconSlidingGate             = "nodeicon_slidinggate";
constexpr const char* NodeIconSmartKey                = "nodeicon_smartkey";
constexpr const char* NodeIconSmokeDetector           = "nodeicon_smokedetector";
constexpr const char* NodeIconSpeaker                 = "nodeicon_speaker";
constexpr const char* NodeIconStar                    = "nodeicon_star";
constexpr const char* NodeIconSuitcase                = "nodeicon_suitcase";
constexpr const char* NodeIconSun                     = "nodeicon_sun";
constexpr const char* NodeIconSunSail                 = "nodeicon_sunsail";
constexpr const char* NodeIconSwingGateDouble         = "nodeicon_swinggatedouble";
constexpr const char* NodeIconSwingGateSingle         = "nodeicon_swinggatesingle";
constexpr const char* NodeIconTeddy                   = "nodeicon_teddy";
constexpr const char* NodeIconTemperature             = "nodeicon_temperature";
constexpr const char* NodeIconTiltGate                = "nodeicon_tiltgate";
constexpr const char* NodeIconTurtle                  = "nodeicon_turtle";
constexpr const char* NodeIconTurtle2                 = "nodeicon_turtle2";
constexpr const char* NodeIconTv                      = "nodeicon_tv";
constexpr const char* NodeIconTwoButtonRemote         = "nodeicon_twobuttonremote";
constexpr const char* NodeIconVentilationSystem       = "nodeicon_ventilationsystem";
constexpr const char* NodeIconWarmWater               = "nodeicon_warmwater";
constexpr const char* NodeIconWashingMachine          = "nodeicon_washingmachine";
constexpr const char* NodeIconWaterValve              = "nodeicon_watervalve";
constexpr const char* NodeIconWeatherStation          = "nodeicon_weatherstation";
constexpr const char* NodeIconWindowHandle            = "nodeicon_windowhandle";
constexpr const char* NodeIconWindWheel               = "nodeicon_windwheel";
constexpr const char* NodeIconXmasTree                = "nodeicon_xmastree";