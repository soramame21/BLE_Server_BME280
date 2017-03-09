#include "mbed.h"
#include "rtos.h"
#include "stdio.h"
#include "BME280.h"
#include "ble/BLE.h"
#include "ble/Gap.h"
#include "EnvironmentalService.h"

BLE &ble = BLE::Instance();
DigitalOut led1(LED1);
DigitalOut led2(LED2);

#if defined(TARGET_LPC1768)
BME280 sensor(p28, p27);
#elif defined(TARGET_TY51822R3)
BME280 sensor(I2C_SDA0, I2C_SCL0);
#else
BME280 sensor(I2C_SDA, I2C_SCL);
#endif

const static char DEVICE_NAME[] = "BME280";
const static uint16_t uuid16_list[] = {GattService::UUID_ENVIRONMENTAL_SERVICE};
static EnvironmentalService *EvnSer=NULL;

void disconnectionCallback(const Gap::DisconnectionCallbackParams_t *params)
{
    BLE::Instance(BLE::DEFAULT_INSTANCE).gap().startAdvertising(); // restart advertising
}

void bleInitComplete(BLE::InitializationCompleteCallbackContext *params)
{
    BLE &ble   = params->ble;
    ble_error_t error = params->error;
    EvnSer = new EnvironmentalService(ble);

	printf("Inside BLE..starting payload creation..\n");

    ble.gap().onDisconnection(disconnectionCallback);

    /* Setup advertising. */
    ble.gap().accumulateAdvertisingPayload(GapAdvertisingData::BREDR_NOT_SUPPORTED | GapAdvertisingData::LE_GENERAL_DISCOVERABLE);
    ble.gap().accumulateAdvertisingPayload(GapAdvertisingData::COMPLETE_LIST_16BIT_SERVICE_IDS, (uint8_t *)uuid16_list, sizeof(uuid16_list));
    ble.gap().accumulateAdvertisingPayload(GapAdvertisingData::COMPLETE_LOCAL_NAME, (uint8_t *)DEVICE_NAME, sizeof(DEVICE_NAME));
    ble.gap().setAdvertisingType(GapAdvertisingParams::ADV_CONNECTABLE_UNDIRECTED);

		if (error == BLE_ERROR_NONE) {
        led2 = 1;
    }

    ble.gap().setAdvertisingInterval(1000); /* 1000ms */
    error = ble.gap().startAdvertising();
	printf("ble.gap().startAdvertising() => %u\r\n", error);

	/****************************************

	TODO : Print the address of the server. The below is partially working.

	printf("Hello..My addr is: [%02x %02x %02x %02x %02x %02x] \r\n", params_advCallback->peerAddr[5], params_advCallback->peerAddr[4], params_advCallback->peerAddr[3], params_advCallback->peerAddr[2], params_advCallback->peerAddr[1], params_advCallback->peerAddr[0]);
	****************************************/

}
/*
void periodicCallback(void) {
    led1 = !led1;
}
*/


/************************ Thread #1 for light sensor ************************/

void read_sensor(void) {
    float tmp_t, tmp_p, tmp_h;
	while(true) {
		if(EvnSer!=NULL) {
        		tmp_t=sensor.getTemperature(); 
        		tmp_p=sensor.getPressure();    tmp_h=sensor.getHumidity();
               EvnSer->updatePressure(tmp_p);  EvnSer->updateTemperature(tmp_t);
               EvnSer->updateHumidity(tmp_h); 
               printf("%04.2f hPa,  %2.2f degC,  %2.2f %%\n", tmp_p, tmp_t, tmp_h );
        }
		Thread::wait(1000);
	}
}

/************************ Thread #2 for BLE activities ************************/

void Bluetooth_LE_server(void) {


		ble.init(bleInitComplete);
//    ble.onDisconnection(disconnectionCallback);
//
//		GattCharacteristic *charTable[] = {&SensorReading};
//    GattService sensorService(SERVICE_UUID, charTable, sizeof(charTable) / sizeof(GattCharacteristic *));
//    ble.addService(sensorService);
//
//    ble.gap().accumulateAdvertisingPayload(GapAdvertisingData::BREDR_NOT_SUPPORTED | GapAdvertisingData::LE_GENERAL_DISCOVERABLE);
//    ble.gap().accumulateAdvertisingPayload(GapAdvertisingData::COMPLETE_LIST_16BIT_SERVICE_IDS, (uint8_t *)uuid16_list, sizeof(uuid16_list));
//    ble.gap().accumulateAdvertisingPayload(GapAdvertisingData::COMPLETE_LOCAL_NAME, (uint8_t *) DEVICE_NAME, sizeof(DEVICE_NAME));
//		ble.gap().setAdvertisingType(GapAdvertisingParams::ADV_CONNECTABLE_UNDIRECTED);
//
//    ble.gap().setAdvertisingInterval(1000);
//    ble.gap().startAdvertising();
//
//	while(true) {
//		ble.waitForEvent();
//	}
	while(1) {
		ble.processEvents();
	}
	//This statement might be un-reachable...??
	Thread::wait(1000);
}

/************************ Thread #3 main() ************************/

int main() {
//	printf("Inside main\n");

		Thread thread1;
		Thread thread2;
		
		EvnSer=NULL;
		thread1.start(read_sensor);
		thread2.start(Bluetooth_LE_server);

/*
	while(true) {
		ble.waitForEvent();
	}
*/
	while(true) {
		led1 = !led1;
		Thread::wait(500);
	}
}
