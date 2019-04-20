// Copyright (c) Microsoft. All rights reserved.
// Licensed under the MIT license.

#ifndef TELEMETRY_CONTROLLER
#define TELEMETRY_CONTROLLER

#include "globals.h"
#include "loop.h"
#include "AzureIOTClient.h"

class String;

class TelemetryController : public LoopController
{
    unsigned long lastTelemetrySend;
    unsigned long lastTimeSync;
    unsigned long lastShakeTime;
    unsigned long lastSwitchPress;
    int currentInfoPage;
    int lastInfoPage;
    int  desiredTemperatureSetting = 0;
    int  temperatureDifferenceSetting = 0;
    char refrigeratorBuff[STRING_BUFFER_16] = {0};
    uint8_t telemetryState;
    AzureIOTClient * iotClient;
    bool           can_send;

    void initializeTelemetryController(const char * iotCentralConfig);
    TelemetryController(const char * iotCentralConfig):
        lastTelemetrySend(0), lastTimeSync(0), lastShakeTime(0),
        lastSwitchPress(0), currentInfoPage(0), lastInfoPage(-1),
        telemetryState(0), iotClient(NULL), can_send(true)
    {
        initializeTelemetryController(iotCentralConfig);
    }

    void buildTelemetryPayload(String *payload);
    void sendTelemetryPayload(const char *payload);
public:
    static TelemetryController * New(const char * iotCentralConfig) {
        return new TelemetryController(iotCentralConfig);
    }

    bool withTelemetry() override { return true; }

    void loop() override;
    ~TelemetryController();

    AzureIOTClient * getClient() { return iotClient; }
    bool canSend() { return can_send; }
    void setCanSend(bool s) { can_send = s; }
    void sendStateChange();

    void changeSendInterval(int sendInterval);
    void changeDesiredTemperature(int desiredTemperature);
    void changeTemperatureDifference(int temperatureDifference);
};

#endif // TELEMETRY_CONTROLLER