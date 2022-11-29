#include <nahs-Bricks-Feature-Sleep.h>
#include <nahs-Bricks-Lib-SerHelp.h>
#include <nahs-Bricks-Feature-All.h>
#include <ESP8266WiFi.h>

NahsBricksFeatureSleep::NahsBricksFeatureSleep() {
}

/*
Returns name of feature
*/
String NahsBricksFeatureSleep::getName() {
    return "sleep";
}

/*
Returns version of feature
*/
uint16_t NahsBricksFeatureSleep::getVersion() {
    return version;
}

/*
Configures FSmem und RTCmem variables (prepares feature to be fully operational)
*/
void NahsBricksFeatureSleep::begin() {
    if (!FSdata.containsKey("disabled")) FSdata["disabled"] = false;
    if (!RTCmem.isValid()) {
        RTCdata->disabled = FSdata["disabled"].as<bool>();
    }
}

/*
Starts background processes like fetching data from other components
*/
void NahsBricksFeatureSleep::start() {
}

/*
Adds data to outgoing json, that is send to BrickServer
*/
void NahsBricksFeatureSleep::deliver(JsonDocument* out_json) {
    // deliver disabled value if is active
    if (RTCdata->disabled) {
        if (!out_json->containsKey("y")) out_json->createNestedArray("y");
        JsonArray y_array = out_json->getMember("y").as<JsonArray>();
        y_array.add("q");
    }
}

/*
Processes feedback coming from BrickServer
*/
void NahsBricksFeatureSleep::feedback(JsonDocument* in_json) {
    // check if new disabled value is delivered
    if (in_json->containsKey("q")) {
        RTCdata->disabled = in_json->getMember("q").as<bool>();
    }
}

/*
Finalizes feature (closes stuff)
*/
void NahsBricksFeatureSleep::end() {
    // Following causes issues during wifi connection (rising connect time von 1s to around 6s)
    //WiFi.disconnect(true);
    //delay(1);
    //ESP.deepSleep(RTCdata->deepSleepDelay * 1e6, WAKE_RF_DISABLED);

    // go to deepSleep (if enabled)
    if (!RTCdata->disabled) ESP.deepSleep(FeatureAll.getDelay() * 1e6);
}

/*
Prints the features RTCdata in a formatted way to Serial (used for brickSetup)
*/
void NahsBricksFeatureSleep::printRTCdata() {
    Serial.print("  disabled: ");
    SerHelp.printlnBool(RTCdata->disabled);
}

/*
Prints the features FSdata in a formatted way to Serial (used for brickSetup)
*/
void NahsBricksFeatureSleep::printFSdata() {
    Serial.print("  disabled: ");
    SerHelp.printlnBool(FSdata["disabled"].as<bool>());
}

/*
BrickSetup hands over to this function, when features-submenu is selected
*/
void NahsBricksFeatureSleep::brickSetupHandover() {
    _printMenu();
    while (true) {
        Serial.println();
        Serial.print("Select: ");
        uint8_t input = SerHelp.readLine().toInt();
        switch(input) {
            case 1:
                _toggleDisabled();
                break;
            case 9:
                Serial.println("Returning to MainMenu!");
                return;
                break;
            default:
                Serial.println("Invalid input!");
                _printMenu();
                break;
        }
    }
}

/*
Helper to print Feature submenu during BrickSetup
*/
void NahsBricksFeatureSleep::_printMenu() {
    Serial.println("1) Toggle disabled");
    Serial.println("9) Return to MainMenu");
}

/*
BrickSetup function to set defaultSleepDelay
*/
void NahsBricksFeatureSleep::_toggleDisabled() {
    FSdata["disabled"] = !FSdata["disabled"].as<bool>();
    Serial.print("disabled set to: ");
    SerHelp.printlnBool(FSdata["disabled"].as<bool>());
}

//------------------------------------------
// globally predefined variable
#if !defined(NO_GLOBAL_INSTANCES)
NahsBricksFeatureSleep FeatureSleep;
#endif
