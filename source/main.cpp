#include "mbed.h"
#include "BME280.h"
#include "ble/BLE.h"
#include "ble/services/EnvironmentalService.h"

#if defined(_DEBUG)
Serial pc(USBTX, USBRX);
#endif

#if defined(TARGET_TY51822R3)
BME280 sensor(I2C_SDA0, I2C_SCL0);
#else
BME280 sensor(I2C_SDA, I2C_SCL);
#endif

const static char DEVICE_NAME[] = "BME280";
const static uint16_t uuid16_list[] = {GattService::UUID_ENVIRONMENTAL_SERVICE};
static EnvironmentalService *service = NULL;
static EventQueue eventQueue(/* event count */ 16 * EVENTS_EVENT_SIZE);

void disconnectionCallback(const Gap::DisconnectionCallbackParams_t *params)
{
    BLE::Instance(BLE::DEFAULT_INSTANCE).gap().startAdvertising(); // restart advertising
}

void bleInitComplete(BLE::InitializationCompleteCallbackContext *params)
{
    BLE &ble = params->ble;
    ble_error_t error = params->error;
    service = new EnvironmentalService(ble);
#if defined(_DEBUG)
    pc.printf("Inside BLE..starting payload creation..\r\n");
#endif
    ble.gap().onDisconnection(disconnectionCallback);

    /* Setup advertising. */
    ble.gap().accumulateAdvertisingPayload(GapAdvertisingData::BREDR_NOT_SUPPORTED | GapAdvertisingData::LE_GENERAL_DISCOVERABLE);
    ble.gap().accumulateAdvertisingPayload(GapAdvertisingData::COMPLETE_LIST_16BIT_SERVICE_IDS, (uint8_t *)uuid16_list, sizeof(uuid16_list));
    ble.gap().accumulateAdvertisingPayload(GapAdvertisingData::COMPLETE_LOCAL_NAME, (uint8_t *)DEVICE_NAME, sizeof(DEVICE_NAME));
    ble.gap().setAdvertisingType(GapAdvertisingParams::ADV_CONNECTABLE_UNDIRECTED);
    ble.gap().setAdvertisingInterval(1000);
    error = ble.gap().startAdvertising();
#if defined(_DEBUG)
    pc.printf("ble.gap().startAdvertising() => %u\r\n", error);
#endif
}

void readSensorCallback(void)
{
    float tmp_t, tmp_p, tmp_h;
    
    if(service != NULL) {
        tmp_t = sensor.getTemperature();
        tmp_p = sensor.getPressure();
        tmp_h = sensor.getHumidity();
        service->updatePressure(tmp_p);
        service->updateTemperature(tmp_t);
        service->updateHumidity(tmp_h);
#if defined(_DEBUG)
        pc.printf("%04.2f hPa,  %2.2f degC,  %2.2f %%\r\n", tmp_p, tmp_t, tmp_h );
#endif
    }
}

void scheduleBleEventsProcessing(BLE::OnEventsToProcessCallbackContext* context)
{
    BLE &ble = BLE::Instance();
    eventQueue.call(Callback<void()>(&ble, &BLE::processEvents));
}

int main()
{
    eventQueue.call_every(1000, readSensorCallback);

    BLE &ble = BLE::Instance();
    ble.onEventsToProcess(scheduleBleEventsProcessing);
    ble.init(bleInitComplete);

    eventQueue.dispatch_forever();

    return 0;
}
