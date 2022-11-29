#ifndef NAHS_BRICKS_FEATURE_SLEEP_H
#define NAHS_BRICKS_FEATURE_SLEEP_H

#include <Arduino.h>
#include <ArduinoJson.h>
#include <nahs-Bricks-Feature-BaseClass.h>
#include <nahs-Bricks-Lib-RTCmem.h>
#include <nahs-Bricks-Lib-FSmem.h>

class NahsBricksFeatureSleep : public NahsBricksFeatureBaseClass {
    private:  // Variables
        static const uint16_t version = 2;
        typedef struct {
            bool disabled;
        } _RTCdata;
        _RTCdata* RTCdata = RTCmem.registerData<_RTCdata>();
        JsonObject FSdata = FSmem.registerData("s");

    public: // BaseClass implementations
        NahsBricksFeatureSleep();
        String getName();
        uint16_t getVersion();
        void begin();
        void start();
        void deliver(JsonDocument* out_json);
        void feedback(JsonDocument* in_json);
        void end();
        void printRTCdata();
        void printFSdata();
        void brickSetupHandover();

    public:  // Brick-Specific setter

    private:  // internal Helpers

    private:  // BrickSetup Helpers
        void _printMenu();
        void _toggleDisabled();
};

#if !defined(NO_GLOBAL_INSTANCES)
extern NahsBricksFeatureSleep FeatureSleep;
#endif

#endif // NAHS_BRICKS_FEATURE_SLEEP_H
