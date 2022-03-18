#ifndef __GPS_PARSER_H__
#define __GPS_PARSER_H__

#include "usart.h"
#include "stdlib.h"

extern uint8_t VbReadyNmeaMessage;

extern const char GPS_BAUD_115200;

extern const char ONLY_GPRMC;
extern const char GPRMC_GPGSA;

extern const char HOT_START;
extern const char WARM_START;
extern const char COLD_START;

extern const char GPS_2s;
extern const char GPS_1Hz;
extern const char GPS_2Hz;
extern const char GPS_5Hz;
extern const char GPS_10Hz;

float distance_between (float lat1, float long1, float lat2, float long2);
float radians(float val);
float GpsToDecimalDegrees(const char* nmeaPos, char quadrant);

uint16_t GetGpsSpeed();
float GetDistanceDelta();

#endif /* __GPS_PARSER_H__ */
