#include "GPS_Parser.h"

const char GPS_BAUD_115200[] = "$PMTK251,115200*1F";

const char ONLY_GPRMC[] = "$PMTK314,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0*29";
const char GPRMC_GPGSA[] = "$PMTK314,0,1,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0*28";

const char HOT_START[] = "$PMTK101*32";
const char WARM_START[] = "$PMTK102*31";
const char COLD_START[] = "$PMTK103*30";

const char GPS_2s[]   = "$PMTK300,2000,0,0,0,0*1F";
const char GPS_1Hz[]  = "$PMTK300,1000,0,0,0,0*1C";
const char GPS_2Hz[]  = "$PMTK300,500,0,0,0,0*28";
const char GPS_5Hz[]  = "$PMTK300,200,0,0,0,0*2F";
const char GPS_10Hz[] = "$PMTK300,100,0,0,0,0*2C";

const char CRLF[2] = {0x0D,0x0A};

char NMEA_type[6];
char NMEA_time[12];
char NMEA_fix[2];
char NMEA_latitude[10];
char NMEA_latitude_pole[2];
char NMEA_longitude[10];
char NMEA_longitude_side[2];
char NMEA_speed[8];
char NMEA_course[8];
char NMEA_date[8];
char NMEA_variation[8];
char NMEA_variation_side[2];

char rawHOUR[2];
char rawMINUTE[2];
char rawSECOND[6];

uint8_t HOUR;
uint8_t MINUTE;
float SECOND;

char OLD_NMEA_latitude[10];
char OLD_NMEA_longitude[10];
char OLD_NMEA_latitude_pole[2];
char OLD_NMEA_longitude_side[2];

uint8_t NMEA_type_length;
uint8_t NMEA_time_length;
uint8_t NMEA_fix_length;
uint8_t NMEA_latitude_length;
uint8_t NMEA_latitude_pole_length;
uint8_t NMEA_longitude_length;
uint8_t NMEA_longitude_side_length;
uint8_t NMEA_speed_length;
uint8_t NMEA_course_length;
uint8_t NMEA_date_length;
uint8_t NMEA_variation_length;
uint8_t NMEA_variation_side_length;

char myNMEA[80];

unsigned int i = 0;

uint16_t TIMER_MS;
uint8_t k = 0;
uint8_t j = 0;
uint8_t result;
uint8_t NMEA_length = 0;
uint8_t COMMA_COUNT = 0;
uint8_t NMEA_length_pause = 0;

uint8_t READY_MESSAGE = 0;

float DISTANCE_METERS = 0.0f;
float DISTANCE_KM = 0.0f;

uint16_t SPEED;
float deltakm;

if (READY_MESSAGE == 1)
{
	memcpy(myNMEA, Rx_data, 80);
	memset(Rx_data, 0x00, 80);

	if ((myNMEA[1] == 'G') && (myNMEA[2] == 'P') && (myNMEA[3] == 'R') && (myNMEA[4] == 'M') && (myNMEA[5] == 'C'))
	{
		result = 1;
	}

	else
	{
		result = 0;
		memset(myNMEA, 0x00, 80);
	}

	// Example $GPRMC,151802.962,A,2338.944,S,04639.370,W,208.0,219.1,240320,000.0,W*7E
	// vector  0123456789
	if (result)
	{
		memcpy(OLD_NMEA_latitude, NMEA_latitude, sizeof(NMEA_latitude));
		memcpy(OLD_NMEA_longitude, NMEA_longitude, sizeof(NMEA_longitude));
		memcpy(OLD_NMEA_latitude_pole, NMEA_latitude_pole, sizeof(NMEA_latitude_pole));
		memcpy(OLD_NMEA_longitude_side, NMEA_longitude_side, sizeof(NMEA_longitude_side));

		NMEA_length_pause = 7;
		j = 7;
		while (myNMEA[j] != COMMA)
		{
			NMEA_time[j - NMEA_length_pause] = myNMEA[j];
			j++;
		}
		NMEA_time_length = j - NMEA_length_pause - 1;
		NMEA_length_pause = j;

		j++;

		while (myNMEA[j] != COMMA)
		{
			NMEA_fix[j - NMEA_length_pause - 1] = myNMEA[j];
			j++;
		}
		NMEA_fix_length = j - NMEA_length_pause - 1;
		NMEA_length_pause = j;

		j++;

		while (myNMEA[j] != COMMA)
		{
			NMEA_latitude[j - NMEA_length_pause - 1] = myNMEA[j];
			j++;
		}
		NMEA_latitude_length = j - NMEA_length_pause - 1;
		NMEA_length_pause = j;

		j++;

		while (myNMEA[j] != COMMA)
		{
			NMEA_latitude_pole[j - NMEA_length_pause - 1] = myNMEA[j];
			j++;
		}
		NMEA_latitude_pole_length = j - NMEA_length_pause - 1;
		NMEA_length_pause = j;

		j++;

		while (myNMEA[j] != COMMA)
		{
			NMEA_longitude[j - NMEA_length_pause - 1] = myNMEA[j];
			j++;
		}
		NMEA_longitude_length = j - NMEA_length_pause - 1;
		NMEA_length_pause = j;

		j++;

		while (myNMEA[j] != COMMA)
		{
			NMEA_longitude_side[j - NMEA_length_pause - 1] = myNMEA[j];
			j++;
		}
		NMEA_longitude_side_length = j - NMEA_length_pause - 1;
		NMEA_length_pause = j;

		j++;

		while (myNMEA[j] != COMMA)
		{
			NMEA_speed[j - NMEA_length_pause - 1] = myNMEA[j];
			j++;
		}
		NMEA_speed_length = j - NMEA_length_pause - 1;
		NMEA_length_pause = j;

		j++;

		while (myNMEA[j] != COMMA)
		{
			NMEA_course[j - NMEA_length_pause - 1] = myNMEA[j];
			j++;
		}
		NMEA_course_length = j - NMEA_length_pause - 1;
		NMEA_length_pause = j;

		j++;

		while (myNMEA[j] != COMMA)
		{
			NMEA_date[j - NMEA_length_pause - 1] = myNMEA[j];
			j++;
		}
		NMEA_date_length = j - NMEA_length_pause - 1;
		NMEA_length_pause = j;

		j++;

		while (myNMEA[j] != COMMA)
		{
			NMEA_variation[j - NMEA_length_pause - 1] = myNMEA[j];
			j++;
		}
		NMEA_variation_length = j - NMEA_length_pause - 1;
		NMEA_length_pause = j;

		j++;

		while (myNMEA[j] != '*')
		{
			NMEA_variation_side[j - NMEA_length_pause - 1] = myNMEA[j];
			j++;
		}
		NMEA_variation_side_length = j - NMEA_length_pause - 1;
	}

	j = 0;
	k = 0;
	result = 0;
	NMEA_length_pause = 0;
	// memset(myNMEA,0x00,80);

	SPEED = (atoi(NMEA_speed)) * 1.852f;

	rawHOUR[0] = NMEA_time[0];
	rawHOUR[1] = NMEA_time[1];
	HOUR = atoi(rawHOUR);
	HOUR = HOUR - 3;
	if (HOUR < 0)
		HOUR += 24;
	
	if ((NMEA_fix[0] == 'A') && (SPEED > 0))
	{
		float lat1 = GpsToDecimalDegrees(OLD_NMEA_latitude, OLD_NMEA_latitude_pole[0]);
		float long1 = GpsToDecimalDegrees(OLD_NMEA_longitude, OLD_NMEA_longitude_side[0]);
		float lat2 = GpsToDecimalDegrees(NMEA_latitude, NMEA_latitude_pole[0]);
		float long2 = GpsToDecimalDegrees(NMEA_longitude, NMEA_longitude_side[0]);

		deltakm = distance_between(lat1, long1, lat2, long2);
		DISTANCE_METERS = DISTANCE_METERS + deltakm;
		DISTANCE_KM = DISTANCE_METERS / 1000.0;
	}
	if (HAL_GPIO_ReadPin(B1_GPIO_Port, B1_Pin) == 0)
	{
		DISTANCE_METERS = 0;
		DISTANCE_KM = 0;
	}

	READY_MESSAGE = 0;
}


float GpsToDecimalDegrees(const char* nmeaPos, char quadrant)
{
  float v= 0;
  if(strlen(nmeaPos)>5)
  {
    char integerPart[3+1];
    int digitCount= (nmeaPos[4]=='.' ? 2 : 3);
    memcpy(integerPart, nmeaPos, digitCount);
    integerPart[digitCount]= 0;
    nmeaPos+= digitCount;
    v= atoi(integerPart) + atof(nmeaPos)/60.;
    if(quadrant=='W' || quadrant=='S')
      v= -v;
  }
  return v;
}

float radians(float val)
{
    float radians_val = (val * 71) / 4068;
    return radians_val;
    //float rad = val * 1000.0 / 57296.0;
    //return rad;
}

float distance_between (float lat1, float long1, float lat2, float long2)
{	
  // returns distance in meters between two positions, both specified 
  // as signed decimal-degrees latitude and longitude. Uses great-circle 
  // distance computation for hypothetical sphere of radius 6372795 meters.
  // Because Earth is no exact sphere, rounding errors may be up to 0.5%.
  // Courtesy of Maarten Lamers
  deltakm = radians(long1-long2);
  float sdlong = sin(deltakm);
  float cdlong = cos(deltakm);
  lat1 = radians(lat1);
  lat2 = radians(lat2);
  float slat1 = sin(lat1);
  float clat1 = cos(lat1);
  float slat2 = sin(lat2);
  float clat2 = cos(lat2);
  deltakm = (clat1 * slat2) - (slat1 * clat2 * cdlong); 
  deltakm = deltakm * deltakm; 
  deltakm += ((clat2 * sdlong)*(clat2 * sdlong)); 
  deltakm = sqrt(deltakm); 
  float denom = ((slat1 * slat2) + (clat1 * clat2 * cdlong)); 
  deltakm = atan2(deltakm, denom); 
  deltakm =  deltakm * 6372795;
    
  return deltakm;
}

uint16_t GetGpsSpeed()
{
	return SPEED;
}

float GetDistanceDelta()
{
	return deltakm;
}
