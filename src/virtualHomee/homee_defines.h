#pragma once
#include <stdint.h>

// ============================================================================
// homee_defines.h
//
// homee API constants as C++ enums, for use with node() and nodeAttributes().
//
// ---------------------------------------------------------------------------
// Provenance
// ---------------------------------------------------------------------------
// This file has two sources, and they cover different parts of it:
//
//   1. Structure, group selection and naming convention originate from
//      https://github.com/Oxi75/homee-api-esp32 (PR #5 against this repo),
//      which converted the enums of the official homee JS SDK into C++:
//
//        https://github.com/stfnhmplr/homee-api/blob/main/lib/enums.js
//        (enums.js as of v2.40.1)
//
//      Original copyright and license of that SDK (MIT):
//        Copyright (c) Stefan Himpler <info@himpler.com> (https://himpler.com)
//
//        Permission is hereby granted, free of charge, to any person obtaining a copy
//        of this software and associated documentation files (the "Software"), to deal
//        in the Software without restriction, including without limitation the rights
//        to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
//        copies of the Software, and to permit persons to whom the Software is
//        furnished to do so, subject to the following conditions:
//
//        The above copyright notice and this permission notice shall be included in all
//        copies or substantial portions of the Software.
//
//        THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//        IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//        FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
//
//   2. The values below were then verified and completed against the `ENUMS`
//      object of the homee web application (my.hom.ee, scripts/app.min.js),
//      retrieved 2026-11-16. Only the bare name/number pairs of that object
//      were used - plain protocol constants, no application code, markup or
//      assets.
//
//      Result of the most recent comparison (2026-11-16): one constant had
//      changed its value — CANotificationClassWarning moved from 2 to 11, and
//      the newly added CANotificationClassDeviceAlarm took over the value 2.
//      Seven further constants had been added, in CAAttributeType (293, 411)
//      and CAWarningCode (149, 414-416, 1504).
//
//      The file was first assembled on 2026-08-16 from enums.js v2.40.1, which
//      was then short 28 constants, mainly in CAAttributeType (394-408) and
//      CANodeProtocol (29-34). CAWLANMode was added as a group because
//      virtualHomee::getSettings() reports a wlan_mode.
//
// To re-check on future homee updates, compare against the same `ENUMS`
// object; it is the authoritative and most current list.
//
// ---------------------------------------------------------------------------
// Conventions
// ---------------------------------------------------------------------------
// All enums use plain C-style syntax (no "enum class") so values convert
// implicitly to/from the uint16_t/uint32_t parameters of node() and
// nodeAttributes() without static_cast noise at every call site. An explicit
// underlying type is still given per group to keep the storage footprint
// minimal - valid C++11, and does not require scoped enums.
//
// Enumerator names are prefixed per group to avoid collisions in the
// resulting flat (unscoped) namespace. Four groups carry bare member names in
// the source - CATimeOfDay ("Day", "Night", ...), CAGroupCategory,
// CADiaryEventType and CADiaryCauseType - and are prefixed here for that
// reason. This is the only intentional naming deviation from the source.
//
// One constant is a local addition and does not exist in the homee enums:
// CANodeProfileUnknown (0xFFFFFFFF), a sentinel for "profile not set".
// It is marked as such at its definition.
//
// Node icon strings (the "image" field of node::setImage()) live in the
// separate homee_icons.h - different provenance, see the notes in that file.
// ============================================================================

enum CANodeProfile : uint32_t
{
    CANodeProfileUnknown                             = 0xFFFFFFFF,   // local sentinel, not part of the homee enums
    CANodeProfileNone                                = 0,
    CANodeProfileHomee                               = 1,
    CANodeProfileOnOffPlug                           = 10,
    CANodeProfileDimmableMeteringSwitch              = 11,
    CANodeProfileMeteringSwitch                      = 12,
    CANodeProfileMeteringPlug                        = 13,
    CANodeProfileDimmablePlug                        = 14,
    CANodeProfileDimmableSwitch                      = 15,
    CANodeProfileOnOffSwitch                         = 16,
    CANodeProfileDoubleOnOffSwitch                   = 18,
    CANodeProfileDimmableMeteringPlug                = 19,
    CANodeProfileOneButtonRemote                     = 20,
    CANodeProfileBinaryInput                         = 21,
    CANodeProfileDimmableColorMeteringPlug           = 22,
    CANodeProfileDoubleBinaryInput                   = 23,
    CANodeProfileTwoButtonRemote                     = 24,
    CANodeProfileThreeButtonRemote                   = 25,
    CANodeProfileFourButtonRemote                    = 26,
    CANodeProfileAlarmSensor                         = 27,
    CANodeProfileDoubleOnOffPlug                     = 28,
    CANodeProfileOnOffSwitchWithBinaryInput          = 29,
    CANodeProfileWatchDogWithPressureAndTemperatures = 30,
    CANodeProfileFibaroButton                        = 31,
    CANodeProfileEnergyMeter                         = 32,
    CANodeProfileDoubleMeteringSwitch                = 33,
    CANodeProfileFibaroSwipe                         = 34,
    CANodeProfileEnergyManager                       = 38,
    CANodeProfileLevelMeter                          = 39,
    CANodeProfileRangeExtender                       = 40,
    CANodeProfileRemote                              = 41,
    CANodeProfileImpulsePlug                         = 42,
    CANodeProfileImpulseRelay                        = 43,
    CANodeProfileWallMote                            = 44,
    CANodeProfileWifiOneButtonRemote                 = 45,
    CANodeProfileWifiMeteringSwitch                  = 46,
    CANodeProfileWifiOnOffMeteringSwitch             = 47,
    CANodeProfileWifiOnOffMeteringPlug               = 48,
    CANodeProfileWifiOnOffDimmableMeteringSwitch     = 49,
    CANodeProfileWifiOnOffSwitch                     = 50,
    CANodeProfileWifiEnergyMeter                     = 51,
    CANodeProfileBrightnessSensor                    = 1000,
    CANodeProfileDimmableColorLight                  = 1001,
    CANodeProfileDimmableExtendedColorLight          = 1002,
    CANodeProfileDimmableColorTemperatureLight       = 1003,
    CANodeProfileDimmableLight                       = 1004,
    CANodeProfileDimmableLightWithBrightnessSensor   = 1005,
    CANodeProfileDimmableLightWithBrightnessAndPresenceSensor = 1006,
    CANodeProfileDimmableLightWithPresenceSensor     = 1007,
    CANodeProfileDimmableRGBWLight                   = 1008,
    CANodeProfileWifiDimmableRGBWLight               = 1009,
    CANodeProfileWifiDimmableLight                   = 1010,
    CANodeProfileOpenCloseSensor                     = 2000,
    CANodeProfileWindowHandle                        = 2001,
    CANodeProfileShutterPositionSwitch               = 2002,
    CANodeProfileOpenCloseAndTemperatureSensor       = 2003,
    CANodeProfileElectricMotorMeteringSwitch         = 2004,
    CANodeProfileOpenCloseWithTemperatureAndBrightnessSensor = 2005,
    CANodeProfileElectricMotorMeteringSwitchWithoutSlatPosition = 2006,
    CANodeProfileLock                                = 2007,
    CANodeProfileWindowHandleWithButtons             = 2008,
    CANodeProfileWindowHandleWithButtonsAndTemperatureAndHumiditySensor = 2009,
    CANodeProfileEntranceGateOperator                = 2010,
    CANodeProfilePerimeterProtectionSystem           = 2011,
    CANodeProfileGarageDoorOperator                  = 2012,
    CANodeProfileGateOperator                        = 2013,
    CANodeProfileInnerDoorOperator                   = 2014,
    CANodeProfileGarageDoorImpulseOperator           = 2015,
    CANodeProfileEntranceDoor                        = 2016,
    CANodeProfileWindowMotorSwitch                   = 2017,
    CANodeProfileWifiOpenCloseSensor                 = 2018,
    CANodeProfileWifiElctricMotorMeteringSwitch      = 2019,
    CANodeProfileTemperatureAndHumiditySensor        = 3001,
    CANodeProfileCO2Sensor                           = 3002,
    CANodeProfileRoomThermostat                      = 3003,
    CANodeProfileRoomThermostatWithHumiditySensor    = 3004,
    CANodeProfileBinaryInputWithTemperatureSensor    = 3005,
    CANodeProfileRadiatorThermostat                  = 3006,
    CANodeProfileTemperatureSensor                   = 3009,
    CANodeProfileHumiditySensor                      = 3010,
    CANodeProfileWaterValve                          = 3011,
    CANodeProfileWaterMeter                          = 3012,
    CANodeProfileWeatherStation                      = 3013,
    CANodeProfileNetatmoMainModule                   = 3014,
    CANodeProfileNetatmoOutdoorModule                = 3015,
    CANodeProfileNetatmoIndoorModule                 = 3016,
    CANodeProfileNetatmoRainModule                   = 3017,
    CANodeProfileCosiThermChannel                    = 3018,
    CANodeProfileVentilationControl                  = 3019,
    CANodeProfileThermostatWithHeatingAndCooling     = 3022,
    CANodeProfileNetatmoWindModule                   = 3023,
    CANodeProfileElectricalHeating                   = 3024,
    CANodeProfileValveDrive                          = 3025,
    CANodeProfileCamera                              = 3026,
    CANodeProfileCameraWithFloodlight                = 3027,
    CANodeProfileHeatingSystem                       = 3028,
    CANodeProfileWarmWaterCircuit                    = 3029,
    CANodeProfileHeatingCircuit                      = 3030,
    CANodeProfileAirQualitySensor                    = 3031,
    CANodeProfileWifiRoomThermostat                  = 3032,
    CANodeProfileWifiRadiatorThermostat              = 3033,
    CANodeProfileMotionDetectorWithTemperatureBrightnessAndHumiditySensor = 4010,
    CANodeProfileMotionDetector                      = 4011,
    CANodeProfileSmokeDetector                       = 4012,
    CANodeProfileFloodDetector                       = 4013,
    CANodeProfilePresenceDetector                    = 4014,
    CANodeProfileMotionDetectorWithTemperatureAndBrightnessSensor = 4015,
    CANodeProfileSmokeDetectorWithTemperatureSensor  = 4016,
    CANodeProfileFloodDetectorWithTemperatureSensor  = 4017,
    CANodeProfileWatchDogDevice                      = 4018,
    CANodeProfileLAG                                 = 4019,
    CANodeProfileOWU                                 = 4020,
    CANodeProfileEurovac                             = 4021,
    CANodeProfileOWWG3                               = 4022,
    CANodeProfileEuropress                           = 4023,
    CANodeProfileMinimumDetector                     = 4024,
    CANodeProfileMaximumDetector                     = 4025,
    CANodeProfileSmokeDetectorAndCODetector          = 4026,
    CANodeProfileSiren                               = 4027,
    CANodeProfileMotionDetectorWithOpenCloseTemperatureAndBrightnessSensor = 4028,
    CANodeProfileMotionDetectorWithBrightness        = 4029,
    CANodeProfileDoorbell                            = 4030,
    CANodeProfileSmokeDetectorAndSiren               = 4031,
    CANodeProfileFloodDetectorWithTemperatureAndHumiditySensor = 4032,
    CANodeProfileMinimumDetectorWithTemperatureSensor = 4033,
    CANodeProfileMaximumDetectorWithTemperatureSensor = 4034,
    CANodeProfilePresenceDetectorWithTemperatureAndBrightnessSensor = 4035,
    CANodeProfileCODetector                          = 4036,
    CANodeProfileTracker                             = 4037,
    CANodeProfileWifiFloodDetector                   = 4038,
    CANodeProfileWifiMotionDetector                  = 4039,
    CANodeProfileWifiGasDetector                     = 4040,
    CANodeProfileInovaAlarmSystem                    = 5000,
    CANodeProfileInovaDetector                       = 5001,
    CANodeProfileInovaSiren                          = 5002,
    CANodeProfileInovaCommand                        = 5003,
    CANodeProfileInovaTransmitter                    = 5004,
    CANodeProfileInovaReciever                       = 5005,
    CANodeProfileInovaKoala                          = 5006,
    CANodeProfileInovaInternalTransmitter            = 5007,
    CANodeProfileInovaControlPanel                   = 5008,
    CANodeProfileInovaInputOutputExtension           = 5009,
    CANodeProfileInovaMotionDetectorWithVOD          = 5010,
    CANodeProfileInovaMotionDetector                 = 5011,
    CANodeProfileWashingMachine                      = 6000,
    CANodeProfileTumbleDryer                         = 6001,
    CANodeProfileDishwasher                          = 6002,
};

enum CAAttributeType : uint16_t
{
    CAAttributeTypeNone                           = 0,
    CAAttributeTypeOnOff                          = 1,
    CAAttributeTypeDimmingLevel                   = 2,
    CAAttributeTypeCurrentEnergyUse               = 3,
    CAAttributeTypeAccumulatedEnergyUse           = 4,
    CAAttributeTypeTemperature                    = 5,
    CAAttributeTypeTargetTemperature              = 6,
    CAAttributeTypeRelativeHumidity               = 7,
    CAAttributeTypeBatteryLevel                   = 8,
    CAAttributeTypeStatusLED                      = 9,
    CAAttributeTypeWindowPosition                 = 10,
    CAAttributeTypeBrightness                     = 11,
    CAAttributeTypeFloodAlarm                     = 12,
    CAAttributeTypeSiren                          = 13,
    CAAttributeTypeOpenClose                      = 14,
    CAAttributeTypePosition                       = 15,
    CAAttributeTypeSmokeAlarm                     = 16,
    CAAttributeTypeBlackoutAlarm                  = 17,
    CAAttributeTypeCurrentValvePosition           = 18,
    CAAttributeTypeBinaryInput                    = 19,
    CAAttributeTypeCO2Level                       = 20,
    CAAttributeTypePressure                       = 21,
    CAAttributeTypeLevel                          = 22,
    CAAttributeTypeColor                          = 23,
    CAAttributeTypeSaturation                     = 24,
    CAAttributeTypeMotionAlarm                    = 25,
    CAAttributeTypeMotionSensitivity              = 26,
    CAAttributeTypeMotionInsensitivity            = 27,
    CAAttributeTypeMotionAlarmCancelationDelay    = 28,
    CAAttributeTypeWakeUpInterval                 = 29,
    CAAttributeTypeTamperAlarm                    = 30,
    CAAttributeTypeWarmWaterTemperature           = 31,
    CAAttributeTypeWarmWaterTargetTemperature     = 32,
    CAAttributeTypeLinkQuality                    = 33,
    CAAttributeTypeInovaAlarmSystemState          = 34,
    CAAttributeTypeInovaAlarmGroupState           = 35,
    CAAttributeTypeInovaAlarmIntrusionState       = 36,
    CAAttributeTypeInovaAlarmErrorState           = 37,
    CAAttributeTypeInovaAlarmDoorState            = 38,
    CAAttributeTypeInovaAlarmExternalSensor       = 39,
    CAAttributeTypeButtonState                    = 40,
    CAAttributeTypeHue                            = 41,
    CAAttributeTypeColorTemperature               = 42,
    CAAttributeTypeHardwareRevision               = 43,
    CAAttributeTypeFirmwareRevision               = 44,
    CAAttributeTypeSoftwareRevision               = 45,
    CAAttributeTypeLEDState                       = 46,
    CAAttributeTypeLEDStateWhenOn                 = 47,
    CAAttributeTypeLEDStateWhenOff                = 48,
    CAAttributeTypeTargetTemperatureOffset        = 49,
    CAAttributeTypeSavingFactor                   = 50,
    CAAttributeTypeTotalVoltage                   = 51,
    CAAttributeTypeHighTemperatureAlarm           = 52,
    CAAttributeTypeHighTemperatureAlarmTreshold   = 53,
    CAAttributeTypeLowTemperatureAlarm            = 54,
    CAAttributeTypeLowTemperatureAlarmTreshold    = 55,
    CAAttributeTypeTamperSensitivity              = 56,
    CAAttributeTypeTamperAlarmCancelationDelay    = 57,
    CAAttributeTypeBrightnessReportInterval       = 58,
    CAAttributeTypeTemperatureReportInterval      = 59,
    CAAttributeTypeMotionAlarmIndicationMode      = 60,
    CAAttributeTypeLEDBrightness                  = 61,
    CAAttributeTypeTamperAlarmIndicationMode      = 62,
    CAAttributeTypeSwitchType                     = 63,
    CAAttributeTypeTemperatureOffset              = 64,
    CAAttributeTypeAccumulatedWaterUse            = 65,
    CAAttributeTypeAccumulatedWaterUseLastMonth   = 66,
    CAAttributeTypeCurrentDate                    = 67,
    CAAttributeTypeLeakAlarm                      = 68,
    CAAttributeTypeBatteryLowAlarm                = 69,
    CAAttributeTypeMalfunctionAlarm               = 70,
    CAAttributeTypeLinkQualityAlarm               = 71,
    CAAttributeTypeMode                           = 72,
    CAAttributeTypeHeatingProgram                 = 73,
    CAAttributeTypeWarmWaterProgram               = 74,
    CAAttributeTypeCalibration                    = 75,
    CAAttributeTypePresenceAlarm                  = 76,
    CAAttributeTypeMinimumAlarm                   = 77,
    CAAttributeTypeMaximumAlarm                   = 78,
    CAAttributeTypeOilAlarm                       = 79,
    CAAttributeTypeWaterAlarm                     = 80,
    CAAttributeTypeInovaAlarmInhibition           = 81,
    CAAttributeTypeInovaAlarmEjection             = 82,
    CAAttributeTypeInovaAlarmCommercialRef        = 83,
    CAAttributeTypeSerialNumber                   = 84,
    CAAttributeTypeRadiatorThermostatSummerMode   = 85,
    CAAttributeTypeInovaAlarmOperationMode        = 86,
    CAAttributeTypeAutomaticMode                  = 87,
    CAAttributeTypePollingInterval                = 88,
    CAAttributeTypeFeedTemperature                = 89,
    CAAttributeTypeDisplayOrientation             = 90,
    CAAttributeTypeManualOperation                = 91,
    CAAttributeTypeDeviceTemperature              = 92,
    CAAttributeTypeSonometer                      = 93,
    CAAttributeTypeAirPressure                    = 94,
    CAAttributeTypeOutdoorRelativeHumidity        = 95,
    CAAttributeTypeIndoorRelativeHumidity         = 96,
    CAAttributeTypeOutdoorTemperature             = 97,
    CAAttributeTypeIndoorTemperature              = 98,
    CAAttributeTypeVentilationLevel               = 99,
    CAAttributeTypeVentilationMode                = 100,
    CAAttributeTypeRainFall                       = 101,
    CAAttributeTypeIntakeMotorRevs                = 102,
    CAAttributeTypeExhaustMotorRevs               = 103,
    CAAttributeTypeOperatingHours                 = 104,
    CAAttributeTypeInovaAlarmSilentAlert          = 105,
    CAAttributeTypeInovaAlarmPreAlarm             = 106,
    CAAttributeTypeInovaAlarmDeterrenceAlarm      = 107,
    CAAttributeTypeInovaAlarmWarning              = 108,
    CAAttributeTypeInovaAlarmFireAlarm            = 109,
    CAAttributeTypeUpTime                         = 110,
    CAAttributeTypeDownTime                       = 111,
    CAAttributeTypeShutterBlindMode               = 112,
    CAAttributeTypeShutterSlatPosition            = 113,
    CAAttributeTypeShutterSlatTime                = 114,
    CAAttributeTypeRestartDevice                  = 115,
    CAAttributeTypeSoilMoisture                   = 116,
    CAAttributeTypeWaterPlantAlarm                = 117,
    CAAttributeTypeMistPlantAlarm                 = 118,
    CAAttributeTypeFertilizePlantAlarm            = 119,
    CAAttributeTypeCoolPlantAlarm                 = 120,
    CAAttributeTypeHeatPlantAlarm                 = 121,
    CAAttributeTypePutPlantIntoLightAlarm         = 122,
    CAAttributeTypePutPlantIntoShadeAlarm         = 123,
    CAAttributeTypeColorMode                      = 124,
    CAAttributeTypeTargetTemperatureLow           = 125,
    CAAttributeTypeTargetTemperatureHigh          = 126,
    CAAttributeTypeHVACMode                       = 127,
    CAAttributeTypeAway                           = 128,
    CAAttributeTypeHVACState                      = 129,
    CAAttributeTypeHasLeaf                        = 130,
    CAAttributeTypeSetEnergyConsumption           = 131,
    CAAttributeTypeCOAlarm                        = 132,
    CAAttributeTypeRestoreLastKnownState          = 133,
    CAAttributeTypeLastImageReceived              = 134,
    CAAttributeTypeUpDown                         = 135,
    CAAttributeTypeRequestVOD                     = 136,
    CAAttributeTypeInovaDetectorHistory           = 137,
    CAAttributeTypeSurgeAlarm                     = 138,
    CAAttributeTypeLoadAlarm                      = 139,
    CAAttributeTypeOverloadAlarm                  = 140,
    CAAttributeTypeVoltageDropAlarm               = 141,
    CAAttributeTypeShutterOrientation             = 142,
    CAAttributeTypeOverCurrentAlarm               = 143,
    CAAttributeTypeSirenMode                      = 144,
    CAAttributeTypeAlarmAutoStopTime              = 145,
    CAAttributeTypeWindSpeed                      = 146,
    CAAttributeTypeWindDirection                  = 147,
    CAAttributeTypeComfortTemperature             = 148,
    CAAttributeTypeEcoTemperature                 = 149,
    CAAttributeTypeReduceTemperature              = 150,
    CAAttributeTypeProtectTemperature             = 151,
    CAAttributeTypeInovaSystemTime                = 152,
    CAAttributeTypeInovaCorrespondentProtocol     = 153,
    CAAttributeTypeInovaCorrespondentID           = 154,
    CAAttributeTypeInovaCorrespondentListen       = 155,
    CAAttributeTypeInovaCorrespondentNumber       = 156,
    CAAttributeTypeInovaCallCycleFireProtection   = 157,
    CAAttributeTypeInovaCallCycleIntrusion        = 158,
    CAAttributeTypeInovaCallCycleTechnicalProtect = 159,
    CAAttributeTypeInovaCallCycleFaults           = 160,
    CAAttributeTypeInovaCallCycleDeterrence       = 161,
    CAAttributeTypeInovaCallCyclePrealarm         = 162,
    CAAttributeTypeInovaPSTNRings                 = 163,
    CAAttributeTypeInovaDoubleCallRings           = 164,
    CAAttributeTypeInovaPIN                       = 165,
    CAAttributeTypeInovaPUK                       = 166,
    CAAttributeTypeInovaMainMediaSelection        = 167,
    CAAttributeTypeRainFallLastHour               = 168,
    CAAttributeTypeRainFallToday                  = 169,
    CAAttributeTypeIdentificationMode             = 170,
    CAAttributeTypeButtonDoubleClick              = 171,
    CAAttributeTypeSirenTriggerMode               = 172,
    CAAttributeTypeUV                             = 173,
    CAAttributeTypeSlatSteps                      = 174,
    CAAttributeTypeEcoModeConfig                  = 175,
    CAAttributeTypeButtonLongRelease              = 176,
    CAAttributeTypeVisualGong                     = 177,
    CAAttributeTypeAcousticGong                   = 178,
    CAAttributeTypeSurveillanceOnOff              = 179,
    CAAttributeTypeStorageAlarm                   = 181,
    CAAttributeTypePowerSupplyAlarm               = 182,
    CAAttributeTypeNetatmoHome                    = 183,
    CAAttributeTypeNetatmoPerson                  = 184,
    CAAttributeTypeNetatmoLastEventPersonId       = 185,
    CAAttributeTypeNetatmoLastEventTime           = 186,
    CAAttributeTypeNetatmoLastEventType           = 187,
    CAAttributeTypeNetatmoLastEventIsKnownPerson  = 188,
    CAAttributeTypeNetatmoLastEventIsArrival      = 189,
    CAAttributeTypePresenceTimeout                = 190,
    CAAttributeTypeKnownPersonPresence            = 191,
    CAAttributeTypeUnknownPersonPresence          = 192,
    CAAttributeTypeCurrent                        = 193,
    CAAttributeTypeFrequency                      = 194,
    CAAttributeTypeVoltage                        = 195,
    CAAttributeTypePresenceAlarmCancelationDelay  = 196,
    CAAttributeTypePresenceAlarmDetectionDelay    = 197,
    CAAttributeTypePresenceAlarmThreshold         = 198,
    CAAttributeTypeNetatmoThermostatMode          = 199,
    CAAttributeTypeNetatmoRelayBoilerConnected    = 200,
    CAAttributeTypeNetatmoRelayMac                = 201,
    CAAttributeTypeNetatmoThermostatModeTimeout   = 202,
    CAAttributeTypeNetatmoThermostatNextChange    = 203,
    CAAttributeTypeNetatmoThermostatPrograms      = 204,
    CAAttributeTypeHomeeMode                      = 205,
    CAAttributeTypeColorWhite                     = 206,
    CAAttributeTypeMovementAlarm                  = 207,
    CAAttributeTypeMovementSensitivity            = 208,
    CAAttributeTypeVibrationAlarm                 = 209,
    CAAttributeTypeVibrationSensitivity           = 210,
    CAAttributeTypeAverageEnergyUse               = 211,
    CAAttributeTypeBinaryInputMode                = 212,
    CAAttributeTypeDeviceStatus                   = 213,
    CAAttributeTypeDeviceRemainingTime            = 214,
    CAAttributeTypeDeviceStartTime                = 215,
    CAAttributeTypeDeviceProgram                  = 216,
    CAAttributeTypeMultilevelInput                = 222,
    CAAttributeTypeButtonPressed3Times            = 223,
    CAAttributeTypeButtonPressed4Times            = 224,
    CAAttributeTypeButtonPressed5Times            = 225,
    CAAttributeTypeRepeaterMode                   = 226,
    CAAttributeTypeAutoOffTime                    = 227,
    CAAttributeTypeCO2Alarm                       = 228,
    CAAttributeTypeInputEndpointConfiguration     = 229,
    CAAttributeTypeGustSpeed                      = 230,
    CAAttributeTypeGustDirection                  = 231,
    CAAttributeTypeLockState                      = 232,
    CAAttributeTypeAeotecSmartPlugLEDState        = 233,
    CAAttributeTypeAlarmDuration                  = 234,
    CAAttributeTypeDewPoint                       = 235,
    CAAttributeTypeGesture                        = 236,
    CAAttributeTypeGestureSequenceLearningMode    = 237,
    CAAttributeTypeGestureSequence                = 238,
    CAAttributeTypeTotalCurrentEnergyUse          = 239,
    CAAttributeTypeTotalAccumulatedEnergyUse      = 240,
    CAAttributeTypeSunsetTime                     = 241,
    CAAttributeTypeSunriseTime                    = 242,
    CAAttributeTypeCurrentLocalWeatherCondition   = 243,
    CAAttributeTypeCurrentLocalTemperature        = 244,
    CAAttributeTypeCurrentLocalHumidity           = 245,
    CAAttributeTypeForecastLocalWeatherCondition  = 246,
    CAAttributeTypeForecastLocalTempMin           = 247,
    CAAttributeTypeForecastLocalTempMax           = 248,
    CAAttributeTypeArmed                          = 249,
    CAAttributeTypeFloodlight                     = 250,
    CAAttributeTypeHumanDetected                  = 251,
    CAAttributeTypeVehicleDetected                = 252,
    CAAttributeTypeAnimalDetected                 = 253,
    CAAttributeTypeVacationMode                   = 254,
    CAAttributeTypeBlinkInterval                  = 255,
    CAAttributeTypeOtherMotionDetected            = 256,
    CAAttributeTypeIRCodeNumber                   = 257,
    CAAttributeTypeHeatingMode                    = 258,
    CAAttributeTypeDisplayAutoOffTime             = 259,
    CAAttributeTypeBacklight                      = 260,
    CAAttributeTypeOpenWindowDetectionSensibility = 261,
    CAAttributeTypeCurrentLocalWindSpeed          = 262,
    CAAttributeTypeCurrentLocalGustSpeed          = 263,
    CAAttributeTypePowerOutputGrid                = 264,
    CAAttributeTypePowerInputGrid                 = 265,
    CAAttributeTypePowerPV                        = 266,
    CAAttributeTypePowerLoad                      = 267,
    CAAttributeTypePowerOutputBattery             = 268,
    CAAttributeTypePowerInputBattery              = 269,
    CAAttributeTypeRelativeAutonomy               = 270,
    CAAttributeTypeRelativeSelfConsumption        = 271,
    CAAttributeTypeTotalCurrent                   = 272,
    CAAttributeTypeEnergyStorageLevel             = 273,
    CAAttributeTypeTotalEnergyLoad                = 274,
    CAAttributeTypeTotalEnergyProduction          = 275,
    CAAttributeTypeTotalEnergyOutputGrid          = 276,
    CAAttributeTypeTotalEnergyInputGrid           = 277,
    CAAttributeTypeTotalEnergyStored              = 278,
    CAAttributeTypeTotalEnergyRestored            = 279,
    CAAttributeTypeReplaceFilterAlarm             = 289,
    CAAttributeTypeUnknownNotification            = 291,
    CAAttributeTypeFlowAlarm                      = 293,
    CAAttributeTypeRFProtection                   = 297,
    CAAttributeTypeExpertMode                     = 299,
    CAAttributeTypeUpDownRemote                   = 300,
    CAAttributeTypeDawn                           = 301,
    CAAttributeTypeRainfallIntensity              = 302,
    CAAttributeTypeImpulse                        = 304,
    CAAttributeTypeLightImpulse                   = 305,
    CAAttributeTypeOpenPartialImpulse             = 306,
    CAAttributeTypeExternalBinaryInput            = 309,
    CAAttributeTypeButtonPressedLong              = 310,
    CAAttributeTypeMemoryUsage                    = 311,
    CAAttributeTypeProcessorUtilization           = 312,
    CAAttributeTypeUsedStorage                    = 313,
    CAAttributeTypeExternalTemperature            = 315,
    CAAttributeTypeFloorTemperature               = 316,
    CAAttributeTypeCurrentElectricityPrice        = 317,
    CAAttributeTypeHighestElectricityPrice        = 318,
    CAAttributeTypeLowestElectricityPrice         = 319,
    CAAttributeTypeCurrentEnergyConsumption       = 320,
    CAAttributeTypeCurrentCosts                   = 321,
    CAAttributeTypeSlatMinAngle                   = 322,
    CAAttributeTypeSlatMaxAngle                   = 323,
    CAAttributeTypeMotorRotation                  = 324,
    CAAttributeTypeEndPositionConfiguration       = 325,
    CAAttributeTypeAutomaticModeImpulse           = 326,
    CAAttributeTypeBrieflyOpenImpulse             = 327,
    CAAttributeTypePermanentlyOpenImpulse         = 328,
    CAAttributeTypeWindThreshold                  = 329,
    CAAttributeTypeMotorBlockedAlarm              = 330,
    CAAttributeTypeFreezeProtection               = 331,
    CAAttributeTypeFlyScreen                      = 332,
    CAAttributeTypeSunProtection                  = 333,
    CAAttributeTypeMemo                           = 334,
    CAAttributeTypeSlatRotationImpulse            = 337,
    CAAttributeWindMonitoringState                = 338,
    CAAttributeTypeHVACOperatingState             = 339,
    CAAttributeTypeExternalTemperatureOffset      = 340,
    CAAttributeTypeFloorTemperatureOffset         = 341,
    CAAttributeTypeDisplayBrightnessActive        = 342,
    CAAttributeTypeDisplayBrightnessDimmed        = 343,
    CAAttributeTypeButtonBrightnessActive         = 344,
    CAAttributeTypeButtonBrightnessDimmed         = 345,
    CAAttributeTypeDisplayTemperatureSelection    = 346,
    CAAttributeTypeResetMeter                     = 347,
    CAAttributeTypeAutomaticRuntimeCalculation    = 348,
    CAAttributeTypeDownPosition                   = 349,
    CAAttributeTypeDownSlatPosition               = 350,
    CAAttributeTypeFirmwareUpdateable             = 351,
    CAAttributeTypeCOValue                        = 354,
    CAAttributeTypeTargetTemperatureType          = 355,
    CAAttributeTypeSetBackMode                    = 356,
    CAAttributeTypeSystemAlarm                    = 357,
    CAAttributeTypeHealthAlarm                    = 358,
    CAAttributeTypeAccessControlAlarm             = 359,
    CAAttributeTypeSecurityAlarm                  = 360,
    CAAttributeTypeEmergencyAlarm                 = 361,
    CAAttributeTypeClockAlarm                     = 362,
    CAAttributeTypeApplianceAlarm                 = 363,
    CAAttributeTypeWaterValveAlarm                = 364,
    CAAttributeTypeWeatherAlarm                   = 365,
    CAAttributeTypeIrrigationAlarm                = 366,
    CAAttributeTypeGasAlarm                       = 367,
    CAAttributeTypePestAlarm                      = 368,
    CAAttributeTypeLightAlarm                     = 369,
    CAAttributeTypeWaterQualityAlarm              = 370,
    CAAttributeTypeAntiTheftRestricted            = 371,
    CAAttributeTypeUnknownMeteringValue           = 372,
    CAAttributeTypeColorExpert                    = 373,
    CAAttributeTypeSwitchMultilevelExpert         = 375,
    CAAttributeTypeUnknownSensorValue             = 376,
    CAAttributeTypeDoorLockExpert                 = 377,
    CAAttributeTypeVOCLevel                       = 352,
    CAAttributeTypeVOCValue                       = 353,
    CAAttributeTypeVentilateImpulse               = 378,
    CAAttributeTypeVibrationOnOff                 = 379,
    CAAttributeTypeSoundOnOff                     = 380,
    CAAttributeTypeVirtualVolantPostion           = 381,
    CAAttributeTypeFabricUntensioning             = 382,
    CAAttributeTypeAutomaticReversing             = 383,
    CAAttributeTypeLocked                         = 384,
    CAAttributeTypeWatchdogOnOff                  = 385,
    CAAttributeTypeIPAddress                      = 386,
    CAAttributeTypeGasAlarmState                  = 387,
    CAAttributeTypeGasConcentration               = 388,
    CAAttributeTypeHostname                       = 389,
    CAAttributeTypeBrightnessLevel                = 390,
    CAAttributeTypeForecastLocalHumidity          = 391,
    CAAttributeTypeForecastLocalWindSpeed         = 392,
    CAAttributeTypeForecastLocalGustSpeed         = 393,
    CAAttributeTypeText                           = 394,
    CAAttributeTypeIometerStreamingMode           = 395,
    CAAttributeTypeTotalConsumptionNow            = 396,
    CAAttributeTypeConsumptionLimitPeak           = 397,
    CAAttributeTypeConsumptionLimitDaily          = 398,
    CAAttributeTypeDailyConsumptionAccumulated    = 399,
    CAAttributeTypeEnergyManagerSwitchRule        = 400,
    CAAttributeTypeCurrentMarketPrice             = 401,
    CAAttributeTypeConnectedDevices               = 402,
    CAAttributeTypeMeshPartners                   = 403,
    CAAttributeTypeEnergyManagerRuleActive        = 404,
    CAAttributeTypeEnergyManagerRuleOnThreshold   = 405,
    CAAttributeTypeEnergyManagerRuleOffThreshold  = 406,
    CAAttributeTypeEnergyManagerRuleLockTime      = 407,
    CAAttributeTypeEnergyManagerRuleDevices       = 408,
    CAAttributeTypeSignalStrength                 = 411,
};

enum CANodeProtocol : uint8_t
{
    CANodeProtocolNone              = 0,
    CANodeProtocolZWave             = 1,
    CANodeProtocolZigBee            = 2,
    CANodeProtocolEnOcean           = 3,
    CANodeProtocolWMBus             = 4,
    CANodeProtocolHomematic         = 5,
    CANodeProtocolKNXRF             = 6,
    CANodeProtocolInova             = 7,
    CANodeProtocolHTTPAVM           = 8,
    CANodeProtocolHTTPNetatmo       = 9,
    CANodeProtocolHTTPKoubachi      = 10,
    CANodeProtocolHTTPNest          = 11,
    CANodeProtocolIOCube            = 12,
    CANodeProtocolHTTPCCU2          = 13,
    CANodeProtocolHTTPUPnP          = 14,
    CANodeProtocolHTTPNuki          = 15,
    CANodeProtocolHTTPSEMS          = 16,
    CANodeProtocolZWaveV3           = 17,
    CANodeProtocolLoRa              = 18,
    CANodeProtocolBiSecur           = 19,
    CANodeProtocolHTTPWolf          = 20,
    CANodeProtocolExternalHomee     = 21,
    CANodeProtocolCentronicPlus     = 22,
    CANodeProtocolHTTPMyStrom       = 24,
    CANodeProtocolWMS               = 23,
    CANodeProtocolHTTPSteca         = 25,
    CANodeProtocolMQTTShelly        = 27,
    CANodeProtocolVirtual           = 28,
    CANodeProtocolMQTTHomeAssistant = 29,
    CANodeProtocolHTTPSwitchBot     = 30,
    CANodeProtocolHTTPiCal          = 31,
    CANodeProtocolHTTPIoMeter       = 32,
    CANodeProtocolHTTPDreame        = 33,
    CANodeProtocolMQTTRoomba        = 34,
    CANodeProtocolAll               = 100,
};

enum CANodeState : uint8_t
{
    CANodeStateNone                         = 0,
    CANodeStateAvailable                    = 1,
    CANodeStateUnavailable                  = 2,
    CANodeStateUpdateInProgress             = 3,
    CANodeStateWaitingForAttributes         = 4,
    CANodeStateInitializing                 = 5,
    CANodeStateUserInteractionRequired      = 6,
    CANodeStatePasswordRequired             = 7,
    CANodeStateHostUnavailable              = 8,
    CANodeStateDeleteInProgress             = 9,
    CANodeStateCosiConnected                = 10,
    CANodeStateLocked                       = 11,
    CANodeStateWaitingForWakeup             = 12,
    CANodeStateRemoteNodeDeleted            = 13,
    CANodeStateNodeFirmwareUpdateInProgress = 14,
};

enum CANodeSecurityLevel : uint8_t
{
    CANodeSecurityLevelUnsecure          = 0,
    CANodeSecurityLevelS0                = 1,
    CANodeSecurityLevelS2Unauthenticated = 2,
    CANodeSecurityLevelS2Authenticated   = 3,
    CANodeSecurityLevelS2Access          = 4,
    CANodeSecurityLevelS0Possible        = 16,
    CANodeSecurityLevelS2Possible        = 17,
    CANodeSecurityLevelSecurityOK        = 31,
};

enum CAAttributeState : uint8_t
{
    CAAttributeStateNone                  = 0,
    CAAttributeStateNormal                = 1,
    CAAttributeStateWaitingForWakeUp      = 2,
    CAAttributeStateWaitingForValue       = 3,
    CAAttributeStateWaitingForAcknowledge = 4,
    CAAttributeStateInactive              = 5,
    CAAttributeStateIgnored               = 6,
    CAAttributeStateUnavailable           = 7,
    CAAttributeStateUnlisted              = 9,
};

enum CAAttributeChangedBy : uint8_t
{
    CAAttributeChangedByNone          = 0,
    CAAttributeChangedByItself        = 1,
    CAAttributeChangedByUser          = 2,
    CAAttributeChangedByHomeegram     = 3,
    CAAttributeChangedByAI            = 6,
    CAAttributeChangedByEnergyManager = 7,
};

enum CAAttributeBasedOn : uint8_t
{
    CAAttributeBasedOnEvents   = 1,
    CAAttributeBasedOnInterval = 2,
    CAAttributeBasedOnPolling  = 4,
    CAAttributeBasedOnManual   = 8,
};

enum CAHomeegramState : uint8_t
{
    CAHomeegramStateNone       = 0,
    CAHomeegramStateNormal     = 1,
    CAHomeegramStateExecuting  = 2,
    CAHomeegramStateNoTriggers = 3,
    CAHomeegramStateNoActions  = 4,
};

enum CAWarningCode : uint16_t
{
    CAWarningCodeZWaveJsInstallationStarted         = 0x000005DD,   // local sentinel, not part of the homee enums
    CAWarningCodeZWaveJsInstallationSuccessful      = 0x000005DE,   // local sentinel, not part of the homee enums
    CAWarningCodeZWaveJsInstallationFailed          = 0x000005DF,   // local sentinel, not part of the homee enums
    CAWarningCodeCubeAdded                          = 100,
    CAWarningCodeCubeRemoved                        = 101,
    CAWarningCodeCubeIsMissing                      = 102,
    CAWarningCodeCubeInLearnMode                    = 103,
    CAWarningCodeCubeLearnModeStarted               = 104,
    CAWarningCodeCubeLearnModeSuccessful            = 105,
    CAWarningCodeCubeLearnModeTimeout               = 106,
    CAWarningCodeCubeLearnModeNodeAlreadyAdded      = 107,
    CAWarningCodeCubeLearnModeFailed                = 108,
    CAWarningCodeCubeInRemoveMode                   = 109,
    CAWarningCodeCubeRemoveModeStarted              = 110,
    CAWarningCodeCubeRemoveModeSuccessful           = 111,
    CAWarningCodeCubeRemoveModeTimeout              = 112,
    CAWarningCodeCubeRemoveModeNodeAlreadyDeleted   = 113,
    CAWarningCodeCubeRemoveModeFailed               = 114,
    CAWarningCodeCubeScannedNodes                   = 115,
    CAWarningCodeCubeUpdateInProgess                = 116,
    CAWarningCodeCubeUpdateStarted                  = 117,
    CAWarningCodeCubeUpdateFinished                 = 118,
    CAWarningCodeCubeUpdateFailed                   = 119,
    CAWarningCodeCubeRemoveModeRequiresAction       = 120,
    CAWarningCodeCubeRemoveModeCanceled             = 121,
    CAWarningCodeCubeLearnModeCanceled              = 122,
    CAWarningCodeCubeAuthorizationRequired          = 123,
    CAWarningCodeCubeLearnModeInitializing          = 124,
    CAWarningCodeCubeRemoveModeForbidden            = 125,
    CAWarningCodeCubeResetStarted                   = 126,
    CAWarningCodeCubeResetSuccessful                = 127,
    CAWarningCodeCubeResetFailed                    = 128,
    CAWarningCodeCubeResetTimeout                   = 129,
    CAWarningCodeCubeResetNotSupported              = 130,
    CAWarningCodeCubeHealProgress                   = 131,
    CAWarningCodeCubeReplaceNodeStarted             = 136,
    CAWarningCodeCubeReplaceNodeSuccessful          = 137,
    CAWarningCodeCubeReplaceNodeFailed              = 138,
    CAWarningCodeCubeReplaceNodeInitializing        = 139,
    CAWarningCodeCubeReplaceNodeTimeout             = 140,
    CAWarningCodeCubeLearnModeDisplayCSA            = 143,
    CAWarningCodeCubeServerStarted                  = 144,
    CAWarningCodeNoNodesToAdd                       = 145,
    CAWarningCodeCubeSmartStartInitializing         = 146,
    CAWarningCodeCubeReadonlyDataPartition          = 147,
    CAWarningCodeCubeStorageDegraded                = 148,
    CAWarningCodeCubeStorageFull                    = 149,
    CAWarningCodeNodeBadLinkQuality                 = 200,
    CAWarningCodeNodeIsMissing                      = 201,
    CAWarningCodeNodeWaterDetected                  = 202,
    CAWarningCodeNodeSmokeDetected                  = 203,
    CAWarningCodeNodeBatteryLow                     = 204,
    CAWarningCodeNodeLocked                         = 205,
    CAWarningCodeNodeNotCompatible                  = 206,
    CAWarningCodeNodeResetSuccessful                = 207,
    CAWarningCodeNodeResetStarted                   = 208,
    CAWarningCodeNodeResetFailed                    = 209,
    CAWarningCodeNodeResetTimeout                   = 210,
    CAWarningCodeNodeWrongHVACMode                  = 211,
    CAWarningCodeNodeRangeError                     = 212,
    CAWarningCodeNodeBlocked                        = 213,
    CAWarningCodeNodeWrongAwayMode                  = 214,
    CAWarningCodeNodeResetCanceled                  = 215,
    CAWarningCodeNodeRemoveMissingStarted           = 216,
    CAWarningCodeNodeRemoveMissingDone              = 217,
    CAWarningCodeNodeCalibrationNeeded              = 218,
    CAWarningCodeNodeValueOutOfRange                = 219,
    CAWarningCodeSettingRemoteAccessActivated       = 300,
    CAWarningCodeSettingRemoteAccessDeactivated     = 301,
    CAWarningCodeSettingOnline                      = 302,
    CAWarningCodeSettingOffline                     = 303,
    CAWarningCodeSettingNetworkUninitialized        = 304,
    CAWarningCodeSettingNetworkInitializing         = 305,
    CAWarningCodeSettingNetworkInitialized          = 306,
    CAWarningCodeSettingGeneratingCertificate       = 307,
    CAWarningCodeUpdateAvailable                    = 400,
    CAWarningCodeUpdateDownloading                  = 401,
    CAWarningCodeUpdateInstalling                   = 402,
    CAWarningCodeUpdateInProgress                   = 403,
    CAWarningCodeUpdateSuccessful                   = 404,
    CAWarningCodeUpdateConnectionFailed             = 405,
    CAWarningCodeUpdateDownloadFailed               = 406,
    CAWarningCodeUpdateInstallationFailed           = 407,
    CAWarningCodeUpdatePreparing                    = 408,
    CAWarningCodeNoUpdateAvailable                  = 409,
    CAWarningCodeUSBUpdateAvailable                 = 410,
    CAWarningCodeNoUSBUpdateAvailable               = 411,
    CAWarningCodeUSBUpdateCanceled                  = 412,
    CAWarningCodeUpdateCheckFailed                  = 413,
    CAWarningCodeUpdateFailed                       = 414,
    CAWarningCodeUpdateNotEnoughMemory              = 415,
    CAWarningCodeUpdateNotEnoughSpace               = 416,
    CAWarningCodeBackupAvailable                    = 450,
    CAWarningCodeBackupCreationFailed               = 451,
    CAWarningCodeRestoreAvailable                   = 452,
    CAWarningCodeNoRestoreAvailable                 = 453,
    CAWarningCodeRestoreFailed                      = 454,
    CAWarningCodeRestoreRestarting                  = 455,
    CAWarningCodeRestoreCanceled                    = 456,
    CAWarningCodePermissionDenied                   = 500,
    CAWarningCodeTeachInForbidden                   = 501,
    CAWarningCodePermissionGranted                  = 502,
    CAWarningCodeVideocodeWrong                     = 503,
    CAWarningCodePasscodeWrong                      = 504,
    CAWarningCodeChangeVideocodeSuccessful          = 505,
    CAWarningCodeChangePasscodeSuccessful           = 506,
    CAWarningCodeChangeVideocodeFailed              = 507,
    CAWarningCodeChangePasscodeFailed               = 508,
    CAWarningCodeDeviceRemoved                      = 600,
    CAWarningCodeDeviceAdded                        = 601,
    CAWarningCodeUserRemoved                        = 700,
    CAWarningCodeAllUsersRemoved                    = 701,
    CAWarningCodeUserPasswordChangeRequired         = 702,
    CAWarningCodeUserPasswordChangeFailed           = 703,
    CAWarningCodeUserPasswordChangeSuccessful       = 704,
    CAWarningCodeUserAlreadyExists                  = 705,
    CAWarningCodeGoogleAssistantSyncSuccess         = 800,
    CAWarningCodeGoogleAssistantSyncError           = 801,
    CAWarningCodeAppleHomeKitPairingSuccess         = 802,
    CAWarningCodeAppleHomeKitPairingError           = 803,
    CAWarningCodeAppleHomeKitSyncSuccess            = 804,
    CAWarningCodeAppleHomeKitSyncError              = 805,
    CAWarningCodeFTPTestSuccess                     = 806,
    CAWarningCodeFTPTestError                       = 807,
    CAWarningCodeManualBackupExportSuccess          = 808,
    CAWarningCodeManualBackupExportError            = 809,
    CAWarningCodeQuarZEnableError                   = 810,
    CAWarningCodeQuarZEnableSuccess                 = 811,
    CAWarningCodeHomeegramCouldNotDownloadTTS       = 900,
    CAWarningCodeHomeegramTestResults               = 901,
    CAWarningCodeEmailVerificationSuccess           = 1100,
    CAWarningCodeEmailVerificationCodeExpired       = 1101,
    CAWarningCodeEmailVerificationWrongCode         = 1102,
    CAWarningCodeEmailVerificationNeeded            = 1103,
    CAWarningCodeEmailVerificationAlreadyVerified   = 1104,
    CAWarningCodeEmailVerificationMailSent          = 1105,
    CAWarningCodeFirmwareUpdateStarted              = 1300,
    CAWarningCodeFirmwareUpdateUploading            = 1301,
    CAWarningCodeFirmwareUpdateFileUploadComplete   = 1302,
    CAWarningCodeFirmwareUpdateFailed               = 1303,
    CAWarningCodeFirmwareUpdateSuccessful           = 1304,
    CAWarningCodeFirmwareUpdateWaitingForActivation = 1305,
    CAWarningCodeFirmwareUpdateDeviceRestarted      = 1306,
    CAWarningCodeUnlockFailed                       = 1400,
    CAWarningCodePlanCopyNotAllowed                 = 1200,
    CAWarningCodePlanNodeAlreadyInOtherPlan         = 1201,
    CAWarningCodePlanDeleteFailed                   = 1202,
    CAWarningCodePlanRemoveNodeFailed               = 1203,
    CAWarningCodePlanAddNodeFailed                  = 1204,
    CAWarningCodeZWaveControllerBusy                = 1504,
};

enum CAErrorCode : uint8_t
{
    CAErrorCodeMissingParameter        = 1,
    CAErrorCodeInvalidFormat           = 2,
    CAErrorCodeInvalidPath             = 3,
    CAErrorCodeInvalidVersion          = 4,
    CAErrorCodeObjectNotFound          = 5,
    CAErrorCodeInvalidResourceID       = 6,
    CAErrorCodeInternalServerException = 7,
    CAErrorCodeTooManyParameters       = 8,
    CAErrorCodeInvalidParameters       = 9,
    CAErrorCodeConflictingParameters   = 10,
};

enum CAWLANMode : uint8_t
{
    CAWLANModeNone    = 0,
    CAWLANModeHotspot = 1,
    CAWLANModeClient  = 2,
    CAWLANModeBridge  = 3,
};

enum CAUserRole : uint8_t
{
    CAUserRoleService        = 1,
    CAUserRoleAdmin          = 2,
    CAUserRoleStandard       = 3,
    CAUserRoleLimited        = 4,
    CAUseroleExternalService = 5,
};

enum CATriggerType : uint8_t
{
    CATriggerTypeNone      = 0,
    CATriggerTypeTime      = 1,
    CATriggerTypeAttribute = 2,
    CATriggerTypeSwitch    = 3,
    CATriggerTypeWebhook   = 4,
    CATriggerTypeHomeegram = 5,
    CATriggerTypeCelestial = 6,
    CATriggerTypePlan      = 7,
    CATriggerTypeGroup     = 8,
    CATriggerTypeUser      = 9,
};

enum CATriggerOperator : uint8_t
{
    CATriggerOperatorNone                 = 0,
    CATriggerOperatorRiseAbove            = 1,
    CATriggerOperatorFallBelow            = 2,
    CATriggerOperatorBecomeEqual          = 3,
    CATriggerOperatorAnyChangeGreaterThan = 4,
};

enum CATriggerOperand : uint8_t
{
    CATriggerOperandNone      = 0,
    CATriggerOperandValue     = 1,
    CATriggerOperandAttribute = 2,
};

enum CATriggerHomeegramEvent : uint8_t
{
    CATriggerHomeegramEventNone     = 0,
    CATriggerHomeegramEventEnabled  = 1,
    CATriggerHomeegramEventDisabled = 2,
    CATriggerHomeegramEventPlayed   = 3,
    CATriggerHomeegramEventStopped  = 4,
};

enum CATriggerPlan : uint8_t
{
    CATriggerPlanNone           = 0,
    CATriggerPlanEnabled        = 1,
    CATriggerPlanDisabled       = 2,
    CATriggerPlanVariableActive = 3,
};

enum CAConditionType : uint8_t
{
    CAConditionTypeNone      = 0,
    CAConditionTypeTime      = 1,
    CAConditionTypeAttribute = 2,
    CAConditionTypeHomeegram = 3,
    CAConditionTypeCelestial = 4,
    CAConditionTypePlan      = 5,
    CAConditionTypeGroup     = 6,
    CAConditionTypeUser      = 7,
};

enum CAConditionOperator : uint8_t
{
    CAConditionOperatorNone         = 0,
    CAConditionOperatorEqual        = 1,
    CAConditionOperatorLessEqual    = 2,
    CAConditionOperatorGreaterEqual = 3,
    CAConditionOperatorLessThan     = 4,
    CAConditionOperatorGreaterThan  = 5,
    CAConditionOperatorNotEqual     = 6,
    CAConditionOperatorTextEqual    = 7,
    CAConditionOperatorTextNotEqual = 8,
};

enum CAConditionOperand : uint8_t
{
    CAConditionOperandNone      = 0,
    CAConditionOperandValue     = 1,
    CAConditionOperandAttribute = 2,
};

enum CAConditionCheckMoment : uint8_t
{
    CAConditionCheckMomentNone        = 0,
    CAConditionCheckMomentStart       = 1,
    CAConditionCheckMomentEnd         = 2,
    CAConditionCheckMomentStartAndEnd = 3,
};

enum CAConditionHomeegramEvent : uint8_t
{
    CAConditionHomeegramEventNone     = 0,
    CAConditionHomeegramEventEnabled  = 1,
    CAConditionHomeegramEventDisabled = 2,
    CAConditionHomeegramEventPlaying  = 3,
    CAConditionHomeegramEventStopped  = 4,
};

enum CAConditionPlan : uint8_t
{
    CAConditionPlanNone           = 0,
    CAConditionPlanEnabled        = 1,
    CAConditionPlanDisabled       = 2,
    CAConditionPlanVariableActive = 3,
};

enum CAActionType : uint8_t
{
    CAActionTypeNone         = 0,
    CAActionTypeAttribute    = 1,
    CAActionTypeTTS          = 2,
    CAActionTypeNotification = 3,
    CAActionTypeGroup        = 4,
    CAActionTypeWebhook      = 5,
    CAActionTypeHomeegram    = 6,
    CAActionTypePlan         = 7,
    CAActionTypeUser         = 8,
};

enum CANotificationStyle : uint8_t
{
    CANotificationStyleNone  = 0,
    CANotificationStylePush  = 1,
    CANotificationStyleSMS   = 2,
    CANotificationStyleEmail = 3,
};

enum CANotificationClass : uint8_t
{
    CANotificationClassNone        = 0,
    CANotificationClassCubes       = 1,
    CANotificationClassDeviceAlarm = 2,
    CANotificationClassNode        = 3,
    CANotificationClassUpdate      = 4,
    CANotificationClassHomeegram   = 5,
    CANotificationClassApi         = 6,
    CANotificationClassPlan        = 8,
    CANotificationClassWarning     = 11,
};

enum CAActionHomeegramEvent : uint8_t
{
    CAActionHomeegramEventNone    = 0,
    CAActionHomeegramEventEnable  = 1,
    CAActionHomeegramEventDisable = 2,
    CAActionHomeegramEventPlay    = 3,
    CAActionHomeegramEventStop    = 4,
};

enum CAActionPlan : uint8_t
{
    CAActionPlanNone     = 0,
    CAActionPlanEnable   = 1,
    CAActionPlanDisable  = 2,
    CAActionPlanOverride = 3,
};

enum CACelestialType : uint8_t
{
    CACelestialTypeSunrise = 1,
    CACelestialTypeSunset  = 2,
};

enum CATimeOfDay : uint8_t
{
    CATimeOfDayDay       = 1,
    CATimeOfDayNight     = 2,
    CATimeOfDayMorning   = 3,
    CATimeOfDayNoon      = 4,
    CATimeOfDayAfternoon = 5,
    CATimeOfDayEvening   = 6,
    CATimeOfDayMidnight  = 7,
};

enum CADeviceType : uint8_t
{
    CADeviceTypeNone    = 0,
    CADeviceTypePhone   = 1,
    CADeviceTypeTablet  = 2,
    CADeviceTypeDesktop = 3,
    CADeviceTypeBrowser = 4,
};

enum CADeviceOS : uint8_t
{
    CADeviceOSNone         = 0,
    CADeviceOSiOS          = 1,
    CADeviceOSAndroid      = 2,
    CADeviceOSWindows      = 3,
    CADeviceOSWindowsPhone = 4,
    CADeviceOSLinux        = 5,
    CADeviceOSMacOS        = 6,
};

enum CADeviceApp : uint8_t
{
    CADeviceAppNone         = 0,
    CADeviceAppHomee        = 1,
    CADeviceAppAFRISOhome   = 2,
    CADeviceAppESTMK        = 3,
    CADeviceAppCoviva       = 4,
    CADeviceAppPuM          = 5,
    CADeviceAppCovivaBerker = 6,
    CADeviceAppNVB          = 7,
    CADeviceAppTEN          = 9,
    CADeviceAppHoermann     = 11,
    CADeviceAppQuarZ        = 12,
    CADeviceAppVARIA3       = 13,
};

enum CAAttributeHistoryError : uint8_t
{
    CAAttributeHistoryErrorTimeout         = 1,
    CAAttributeHistoryErrorNoConnection    = 2,
    CAAttributeHistoryErrorWrongHostname   = 3,
    CAAttributeHistoryErrorDisabled        = 4,
    CAAttributeHistoryErrorUnknownCurlCode = 5,
    CAAttributeHistoryErrorEmpty           = 6,
};

enum CAGroupCategory : uint8_t
{
    GroupCategoryNone       = 0,
    GroupCategoryLight      = 10,
    GroupCategoryClimate    = 11,
    GroupCategoryDoorWindow = 12,
    GroupCategoryAlarm      = 13,
    GroupCategoryEnergy     = 14,
    GroupCategoryHeating    = 15,
    GroupCategoryShutter    = 16,
};

enum HomeeMode : uint8_t
{
    HomeeModeHome     = 0,
    HomeeModeSleeping = 1,
    HomeeModeAway     = 2,
    HomeeModeVacation = 3,
};

enum DashboardGreeting : uint8_t
{
    DashboardGreetingPrefix    = 0,
    DashboardGreetingMorning   = 1,
    DashboardGreetingAfternoon = 2,
    DashboardGreetingEvening   = 3,
    DashboardGreetingMidnight  = 4,
};

enum HomeeUpdate : uint8_t
{
    HomeeUpdateStarting         = 0,
    HomeeUpdateDownloading      = 1,
    HomeeUpdateErrorDownloading = 2,
    HomeeUpdatePrepairing       = 3,
    HomeeUpdateErrorPrepairing  = 4,
    HomeeUpdateInstalling       = 5,
    HomeeUpdateErrorInstalling  = 6,
    HomeeUpdateSuccessful       = 7,
};

enum HomeeService : uint8_t
{
    ServiceAppleHomeKit    = 5,
    ServiceGoogleAssistant = 6,
    ServiceAmazonAlexa     = 7,
    ServiceQuarZ           = 8,
};

enum BackupDestination : uint8_t
{
    BackupDestinationNone    = 0,
    BackupDestinationFTP     = 1,
    BackupDestinationDropbox = 2,
};

enum BackupDestinationBitmask : uint8_t
{
    BackupDestinationBitmaskDropbox = 6,
    BackupDestinationBitmaskFTP     = 7,
};

enum HistoryDestination : uint8_t
{
    HistoryDestinationNone = 0,
    HistoryDestinationFTP  = 1,
};

enum CADiaryEventType : uint16_t
{
    CADiaryEventTypeNone                             = 0,
    CADiaryEventTypeNodeStateChanged                 = 1,
    CADiaryEventTypeNodeAttributeChangeSent          = 2,
    CADiaryEventTypeNodeAttributeChangeReceived      = 3,
    CADiaryEventTypeHomeegramActionNotExecuted       = 100,
    CADiaryEventTypeHomeegramTriggered               = 101,
    CADiaryEventTypeHomeegramCanceled                = 102,
    CADiaryEventTypeHomeegramActivated               = 103,
    CADiaryEventTypeHomeegramDeactivated             = 104,
    CADiaryEventTypeGroupSwitched                    = 200,
    CADiaryEventTypeSystemCubeAdded                  = 300,
    CADiaryEventTypeSystemCubeRemoved                = 301,
    CADiaryEventTypeSystemUserLoginSuccessful        = 302,
    CADiaryEventTypeSystemUserLoginFailed            = 303,
    CADiaryEventTypeSystemReboot                     = 304,
    CADiaryEventTypeSystemShutdown                   = 305,
    CADiaryEventTypeSystemStart                      = 306,
    CADiaryEventTypeSystemUpdateStarted              = 307,
    CADiaryEventTypeSystemUpdateSuccessful           = 308,
    CADiaryEventTypeSystemUpdateFailed               = 309,
    CADiaryEventTypeSystemInternetConnectionEstablished = 310,
    CADiaryEventTypeSystemInternetConnectionLost     = 311,
    CADiaryEventTypeSystemProxyConnectionEstablished = 312,
    CADiaryEventTypeSystemProxyConnectionLost        = 313,
    CADiaryEventTypeSystemWeatherUpdateSuccessful    = 314,
    CADiaryEventTypeSystemWeatherUpdateFailed        = 315,
    CADiaryEventTypeWebhookSent                      = 400,
    CADiaryEventTypePushSent                         = 500,
    CADiaryEventTypePushSentFailed                   = 501,
    CADiaryEventTypeMailSent                         = 510,
    CADiaryEventTypeMailSentFailed                   = 511,
    CADiaryEventTypeSystemBackupCreationSuccessful   = 316,
    CADiaryEventTypeSystemBackupCreationFailed       = 317,
    CADiaryEventTypeSystemBackupExportSuccessful     = 318,
    CADiaryEventTypeSystemBackupExportFailed         = 319,
    CADiaryEventTypeSystemHistoryExportSuccessful    = 320,
    CADiaryEventTypeSystemHistoryExportFailed        = 321,
    CADiaryEventTypeSystemCubeUpdateStarted          = 322,
    CADiaryEventTypeSystemCubeUpdateSuccessful       = 323,
    CADiaryEventTypeSystemCubeUpdateFailed           = 324,
    CADiaryEventTypeElectricityPriceUpdateSuccessful = 325,
    CADiaryEventTypeElectricityPriceUpdateFailed     = 326,
    CADiaryEventTypeEnergyConsumptionUpdateSuccessful = 327,
    CADiaryEventTypeEnergyConsumptionUpdateFailed    = 328,
    CADiaryEventTypeSystemUserPresenceChanged        = 329,
    CADiaryEventTypePlanActivated                    = 600,
    CADiaryEventTypePlanDeactivated                  = 601,
    CADiaryEventTypePlanScheduleTriggered            = 602,
    CADiaryEventTypePlanScheduleRestored             = 603,
    CADiaryEventTypePlanScheduleSkipped              = 604,
    CADiaryEventTypePlanEventTriggered               = 605,
    CADiaryEventTypePlanTemporaryOverriden           = 606,
    CADiaryEventTypeWindMonitoringLinkSuccessful     = 650,
    CADiaryEventTypeWindMonitoringLinkFailed         = 651,
    CADiaryEventTypeWindMonitoringUnlinkSuccessful   = 652,
    CADiaryEventTypeWindMonitoringUnlinkFailed       = 653,
};

enum CADiaryCauseType : uint8_t
{
    CADiaryCauseTypeNone               = 0,
    CADiaryCauseTypeNodeAttribute      = 1,
    CADiaryCauseTypeHomeegramTrigger   = 10,
    CADiaryCauseTypeHomeegramCondition = 11,
    CADiaryCauseTypeHomeegramAction    = 12,
    CADiaryCauseTypeGroup              = 20,
    CADiaryCauseTypeSystem             = 30,
    CADiaryCauseTypeHighRam            = 31,
    CADiaryCauseTypeUpdate             = 32,
    CADiaryCauseTypeDownloadFailed     = 33,
    CADiaryCauseTypeInstallationFailed = 34,
    CADiaryCauseTypeUser               = 40,
    CADiaryCauseTypeWrongPassword      = 41,
    CADiaryCauseTypeUnknownUserName    = 42,
    CADiaryCauseTypePlan               = 50,
    CADiaryCauseTypePlanSchedule       = 51,
    CADiaryCauseTypePlanEvent          = 52,
    CADiaryCauseTypeGlobalWatchdog     = 60,
    CADiaryCauseTypeEmailLimitReached  = 70,
    CADiaryCauseTypeEmailUnverified    = 71,
    CADiaryCauseTypeEmailVerify        = 72,
};

enum CAPlanType : uint8_t
{
    CAPlanTypeNone               = 0,
    CAPlanTypeHeating            = 1,
    CAPlanTypeEnergy             = 2,
    CAPlanTypeOnOff              = 3,
    CAPlanTypePresenceSimulation = 4,
    CAPlanTypeElectricalHeating  = 5,
    CAPlanTypeWindMonitoring     = 50,
    CAPlanTypeSunAutomation      = 51,
    CAPlanTypeDawnAutomation     = 52,
};

enum CAPlanEventType : uint8_t
{
    CAPlanEventTypeNone                = 0,
    CAPlanEventTypeOpenWindowDetection = 1,
    CAPlanEventTypeAwayMode            = 2,
    CAPlanEventTypeVacationMode        = 3,
    CAPlanEventTypeWindNoAlarm         = 4,
    CAPlanEventTypeWindAlarm           = 5,
    CAPlanEventTypeDark                = 6,
    CAPlanEventTypeBright              = 7,
    CAPlanEventTypeReleaseTemperature  = 8,
    CAPlanEventTypeAtHomeMode          = 9,
    CAPlanEventTypeSleepingMode        = 10,
    CAPlanEventTypeMorning             = 11,
    CAPlanEventTypeEvening             = 12,
};

enum CAPlansTemplate : uint8_t
{
    CAPlansTemplateNone           = 0,
    CAPlansTemplateHeating        = 1,
    CAPlansTemplateHeatingEmpty   = 2,
    CAPlansTemplateWindMonitoring = 50,
};

enum CAPlanVariableType : uint16_t
{
    CAPlanVariableTypeNone                   = 0,
    CAPlanVariableTypeTemperatureComfortPlus = 1,
    CAPlanVariableTypeTemperatureComfort     = 2,
    CAPlanVariableTypeTemperatureEco         = 3,
    CAPlanVariableTypeTemperatureNight       = 4,
    CAPlanVariableTypeTemperatureAway        = 5,
    CAPlanVariableTypeTemperatureWindow      = 6,
    CAPlanVariableTypeTemperatureVacation    = 7,
    CAPlanVariableTypeWindNoAlarm            = 50,
    CAPlanVariableTypeWindAlarm              = 51,
    CAPlanVariableTypeGenericOnOffPlug       = 100,
    CAPlanVariableTypeTemperatureOnOffPlug   = 101,
    CAPlanVariableTypeCustom                 = 150,
    CAPlanVariableTypeStopped                = 151,
    CAPlanVariableTypeBlockingTime           = 200,
};
