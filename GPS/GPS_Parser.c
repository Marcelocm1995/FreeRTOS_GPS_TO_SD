#include "GPS_Parser.h"

#define COMMA ','

const char GPS_BAUD_115200[] = "$PMTK251,115200*1F";

const char ONLY_GPRMC[] = "$PMTK314,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0*29";
const char GPRMC_GPGSA[] = "$PMTK314,0,1,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0*28";

const char HOT_START[] = "$PMTK101*32";
const char WARM_START[] = "$PMTK102*31";
const char COLD_START[] = "$PMTK103*30";

const char GPS_2s[] = "$PMTK300,2000,0,0,0,0*1F";
const char GPS_1Hz[] = "$PMTK300,1000,0,0,0,0*1C";
const char GPS_2Hz[] = "$PMTK300,500,0,0,0,0*28";
const char GPS_5Hz[] = "$PMTK300,200,0,0,0,0*2F";
const char GPS_10Hz[] = "$PMTK300,100,0,0,0,0*2C";

const char CRLF[2] = {0x0D, 0x0A};

char SaGPS_NmeaType[6];
char SaGPS_NmeaTime[12];
char SaGPS_NmeaFix[2];
char SaGPS_NmeaLatitude[10];
char SaGPS_NmeaLatitudePole[2];
char SaGPS_NmeaLongitude[10];
char SaGPS_NmeaLongitudeSide[2];
char SaGPS_NmeaSpeed[8];
char SaGPS_NmeaCourse[8];
char SaGPS_NmeaDate[8];
char SaGPS_NmeaVariation[8];
char SaGPS_NmeaVariationSide[2];

char SaGPS_RawHour[2];
char SaGPS_RawMinute[2];
char SaGPS_RawSecond[6];

uint8_t SyGPS_Hour;
uint8_t SyGPS_Minute;
float SfGPS_Second;

char SaGPS_OldNmeaLatitude[10];
char SaGPS_OldNmeaLongitude[10];
char SaGPS_OldNmeaLatitudePole[2];
char SaGPS_OldNmeaLongitudeSide[2];

uint8_t SyGPS_NmeaTypeLength;
uint8_t SyGPS_NmeaTimeLength;
uint8_t SyGPS_NmeaFixLength;
uint8_t SyGPS_NmeaLatitudeLength;
uint8_t SyGPS_NmeaLatitudePoleLength;
uint8_t SyGPS_NmeaLongitudeLength;
uint8_t SyGPS_NmeaLongitudeSideLength;
uint8_t SyGPS_NmeaSpeedLength;
uint8_t SyGPS_NmeaCourseLength;
uint8_t SyGPS_NmeaDateLength;
uint8_t SyGPS_NmeaVariationLength;
uint8_t SyGPS_NmeaVariationSideLength;

char SaGPS_NmeaData[80];

uint8_t SiGPS_NmeaCharIndex = 0;
uint8_t SiGPS_OldNmeaCharIndex = 0;

bool VbGPS_ReadyNmeaMsg = 0;

float SfGPS_DistanceMeters = 0.0f;
float SfGPS_DistanceKm = 0.0f;

uint16_t SwGPS_Speed;
float SfGPS_DeltaMeters;

uint8_t ParseGpsData()
{
	if (VbGPS_ReadyNmeaMsg == 1)
	{
		memcpy(SaGPS_NmeaData, Rx_data, 80);
		memset(Rx_data, 0x00, 80);

		if (!((SaGPS_NmeaData[1] == 'G') && (SaGPS_NmeaData[2] == 'P') && (SaGPS_NmeaData[3] == 'R') && (SaGPS_NmeaData[4] == 'M') && (SaGPS_NmeaData[5] == 'C')))
		{
			memset(SaGPS_NmeaData, 0x00, 80);
			return 0;
		}

		// Example $GPRMC,151802.962,A,2338.944,S,04639.370,W,208.0,219.1,240320,000.0,W*7E
		// vector  0123456789
		memcpy(SaGPS_OldNmeaLatitude, SaGPS_NmeaLatitude, sizeof(SaGPS_NmeaLatitude));
		memcpy(SaGPS_OldNmeaLongitude, SaGPS_NmeaLongitude, sizeof(SaGPS_NmeaLongitude));
		memcpy(SaGPS_OldNmeaLatitudePole, SaGPS_NmeaLatitudePole, sizeof(SaGPS_NmeaLatitudePole));
		memcpy(SaGPS_OldNmeaLongitudeSide, SaGPS_NmeaLongitudeSide, sizeof(SaGPS_NmeaLongitudeSide));

		SiGPS_OldNmeaCharIndex = 7;
		SiGPS_NmeaCharIndex = 7;
		while (SaGPS_NmeaData[SiGPS_NmeaCharIndex] != COMMA)
		{
			SaGPS_NmeaTime[SiGPS_NmeaCharIndex - SiGPS_OldNmeaCharIndex] = SaGPS_NmeaData[SiGPS_NmeaCharIndex];
			SiGPS_NmeaCharIndex++;
		}
		SyGPS_NmeaTimeLength = SiGPS_NmeaCharIndex - SiGPS_OldNmeaCharIndex - 1;
		SiGPS_OldNmeaCharIndex = SiGPS_NmeaCharIndex;

		SiGPS_NmeaCharIndex++;

		while (SaGPS_NmeaData[SiGPS_NmeaCharIndex] != COMMA)
		{
			SaGPS_NmeaFix[SiGPS_NmeaCharIndex - SiGPS_OldNmeaCharIndex - 1] = SaGPS_NmeaData[SiGPS_NmeaCharIndex];
			SiGPS_NmeaCharIndex++;
		}
		SyGPS_NmeaFixLength = SiGPS_NmeaCharIndex - SiGPS_OldNmeaCharIndex - 1;
		SiGPS_OldNmeaCharIndex = SiGPS_NmeaCharIndex;

		SiGPS_NmeaCharIndex++;

		while (SaGPS_NmeaData[SiGPS_NmeaCharIndex] != COMMA)
		{
			SaGPS_NmeaLatitude[SiGPS_NmeaCharIndex - SiGPS_OldNmeaCharIndex - 1] = SaGPS_NmeaData[SiGPS_NmeaCharIndex];
			SiGPS_NmeaCharIndex++;
		}
		SyGPS_NmeaLatitudeLength = SiGPS_NmeaCharIndex - SiGPS_OldNmeaCharIndex - 1;
		SiGPS_OldNmeaCharIndex = SiGPS_NmeaCharIndex;

		SiGPS_NmeaCharIndex++;

		while (SaGPS_NmeaData[SiGPS_NmeaCharIndex] != COMMA)
		{
			SaGPS_NmeaLatitudePole[SiGPS_NmeaCharIndex - SiGPS_OldNmeaCharIndex - 1] = SaGPS_NmeaData[SiGPS_NmeaCharIndex];
			SiGPS_NmeaCharIndex++;
		}
		SyGPS_NmeaLatitudePoleLength = SiGPS_NmeaCharIndex - SiGPS_OldNmeaCharIndex - 1;
		SiGPS_OldNmeaCharIndex = SiGPS_NmeaCharIndex;

		SiGPS_NmeaCharIndex++;

		while (SaGPS_NmeaData[SiGPS_NmeaCharIndex] != COMMA)
		{
			SaGPS_NmeaLongitude[SiGPS_NmeaCharIndex - SiGPS_OldNmeaCharIndex - 1] = SaGPS_NmeaData[SiGPS_NmeaCharIndex];
			SiGPS_NmeaCharIndex++;
		}
		SyGPS_NmeaLongitudeLength = SiGPS_NmeaCharIndex - SiGPS_OldNmeaCharIndex - 1;
		SiGPS_OldNmeaCharIndex = SiGPS_NmeaCharIndex;

		SiGPS_NmeaCharIndex++;

		while (SaGPS_NmeaData[SiGPS_NmeaCharIndex] != COMMA)
		{
			SaGPS_NmeaLongitudeSide[SiGPS_NmeaCharIndex - SiGPS_OldNmeaCharIndex - 1] = SaGPS_NmeaData[SiGPS_NmeaCharIndex];
			SiGPS_NmeaCharIndex++;
		}
		SyGPS_NmeaLongitudeSideLength = SiGPS_NmeaCharIndex - SiGPS_OldNmeaCharIndex - 1;
		SiGPS_OldNmeaCharIndex = SiGPS_NmeaCharIndex;

		SiGPS_NmeaCharIndex++;

		while (SaGPS_NmeaData[SiGPS_NmeaCharIndex] != COMMA)
		{
			SaGPS_NmeaSpeed[SiGPS_NmeaCharIndex - SiGPS_OldNmeaCharIndex - 1] = SaGPS_NmeaData[SiGPS_NmeaCharIndex];
			SiGPS_NmeaCharIndex++;
		}
		SyGPS_NmeaSpeedLength = SiGPS_NmeaCharIndex - SiGPS_OldNmeaCharIndex - 1;
		SiGPS_OldNmeaCharIndex = SiGPS_NmeaCharIndex;

		SiGPS_NmeaCharIndex++;

		while (SaGPS_NmeaData[SiGPS_NmeaCharIndex] != COMMA)
		{
			SaGPS_NmeaCourse[SiGPS_NmeaCharIndex - SiGPS_OldNmeaCharIndex - 1] = SaGPS_NmeaData[SiGPS_NmeaCharIndex];
			SiGPS_NmeaCharIndex++;
		}
		SyGPS_NmeaCourseLength = SiGPS_NmeaCharIndex - SiGPS_OldNmeaCharIndex - 1;
		SiGPS_OldNmeaCharIndex = SiGPS_NmeaCharIndex;

		SiGPS_NmeaCharIndex++;

		while (SaGPS_NmeaData[SiGPS_NmeaCharIndex] != COMMA)
		{
			SaGPS_NmeaDate[SiGPS_NmeaCharIndex - SiGPS_OldNmeaCharIndex - 1] = SaGPS_NmeaData[SiGPS_NmeaCharIndex];
			SiGPS_NmeaCharIndex++;
		}
		SyGPS_NmeaDateLength = SiGPS_NmeaCharIndex - SiGPS_OldNmeaCharIndex - 1;
		SiGPS_OldNmeaCharIndex = SiGPS_NmeaCharIndex;

		SiGPS_NmeaCharIndex++;

		while (SaGPS_NmeaData[SiGPS_NmeaCharIndex] != COMMA)
		{
			SaGPS_NmeaVariation[SiGPS_NmeaCharIndex - SiGPS_OldNmeaCharIndex - 1] = SaGPS_NmeaData[SiGPS_NmeaCharIndex];
			SiGPS_NmeaCharIndex++;
		}
		SyGPS_NmeaVariationLength = SiGPS_NmeaCharIndex - SiGPS_OldNmeaCharIndex - 1;
		SiGPS_OldNmeaCharIndex = SiGPS_NmeaCharIndex;

		SiGPS_NmeaCharIndex++;

		while (SaGPS_NmeaData[SiGPS_NmeaCharIndex] != '*')
		{
			SaGPS_NmeaVariationSide[SiGPS_NmeaCharIndex - SiGPS_OldNmeaCharIndex - 1] = SaGPS_NmeaData[SiGPS_NmeaCharIndex];
			SiGPS_NmeaCharIndex++;
		}
		SyGPS_NmeaVariationSideLength = SiGPS_NmeaCharIndex - SiGPS_OldNmeaCharIndex - 1;
	}
}

void qlqrcoisa()
{
	SiGPS_NmeaCharIndex = 0;
	SiGPS_OldNmeaCharIndex = 0;
	// memset(SaGPS_NmeaData,0x00,80);

	SwGPS_Speed = (atoi(SaGPS_NmeaSpeed)) * 1.852f;

	SaGPS_RawHour[0] = SaGPS_NmeaTime[0];
	SaGPS_RawHour[1] = SaGPS_NmeaTime[1];
	SyGPS_Hour = atoi(SaGPS_RawHour);
	SyGPS_Hour = SyGPS_Hour - 3;
	if (SyGPS_Hour < 0)
		SyGPS_Hour += 24;

	if ((SaGPS_NmeaFix[0] == 'A') && (SwGPS_Speed > 0))
	{
		float LfGPS_Latitude1 = GpsToDecimalDegrees(SaGPS_OldNmeaLatitude, SaGPS_OldNmeaLatitudePole[0]);
		float LfGPS_Longitude1 = GpsToDecimalDegrees(SaGPS_OldNmeaLongitude, SaGPS_OldNmeaLongitudeSide[0]);
		float LfGPS_Latitude2 = GpsToDecimalDegrees(SaGPS_NmeaLatitude, SaGPS_NmeaLatitudePole[0]);
		float LfGPS_Longitude2 = GpsToDecimalDegrees(SaGPS_NmeaLongitude, SaGPS_NmeaLongitudeSide[0]);

		SfGPS_DeltaMeters = distance_between(LfGPS_Latitude1, LfGPS_Longitude1, LfGPS_Latitude2, LfGPS_Longitude2);
		SfGPS_DistanceMeters = SfGPS_DistanceMeters + SfGPS_DeltaMeters;
		SfGPS_DistanceKm = SfGPS_DistanceMeters / 1000.0;
	}
	VbGPS_ReadyNmeaMsg = 0;
}

float GpsToDecimalDegrees(const char *LyGPS_NmeaPos, char LyGPS_NmeaQuadrant)
{
	float LfGPS_DecimalDegrees = 0;
	if (strlen(LyGPS_NmeaPos) > 5)
	{
		char LyGPS_IntegerPart[3 + 1];
		int LwGPS_DigitCount = (LyGPS_NmeaPos[4] == '.' ? 2 : 3);
		memcpy(LyGPS_IntegerPart, LyGPS_NmeaPos, LwGPS_DigitCount);
		LyGPS_IntegerPart[LwGPS_DigitCount] = 0;
		LyGPS_NmeaPos += LwGPS_DigitCount;
		LfGPS_DecimalDegrees = atoi(LyGPS_IntegerPart) + atof(LyGPS_NmeaPos) / 60.;
		if (LyGPS_NmeaQuadrant == 'W' || LyGPS_NmeaQuadrant == 'S')
			LfGPS_DecimalDegrees = -LfGPS_DecimalDegrees;
	}
	return LfGPS_DecimalDegrees;
}

float radians(float LfGPS_Val)
{
	float LfGPS_RadVal = (LfGPS_Val * 71) / 4068;
	return LfGPS_RadVal;
	// float rad = LfGPS_Val * 1000.0 / 57296.0;
	// return rad;
}

float distance_between(float LfGPS_Latitude1, float LfGPS_Longitude1, float LfGPS_Latitude2, float LfGPS_Longitude2)
{
	// returns distance in meters between two positions, both specified
	// as signed decimal-degrees latitude and longitude. Uses great-circle
	// distance computation for hypothetical sphere of radius 6372795 meters.
	// Because Earth is no exact sphere, rounding errors may be up to 0.5%.
	// Courtesy of Maarten Lamers
	SfGPS_DeltaMeters = radians(LfGPS_Longitude1 - LfGPS_Longitude2);
	float LfGPS_LongitudeDeltaSin = sin(SfGPS_DeltaMeters);
	float LfGPS_LongitudeDeltaCos = cos(SfGPS_DeltaMeters);
	LfGPS_Latitude1 = radians(LfGPS_Latitude1);
	LfGPS_Latitude2 = radians(LfGPS_Latitude2);
	float LfGPS_LatitudeSin1 = sin(LfGPS_Latitude1);
	float LfGPS_LatitudeCos1 = cos(LfGPS_Latitude1);
	float LfGPS_LatitudeSin2 = sin(LfGPS_Latitude2);
	float LfGPS_LatitudeCos2 = cos(LfGPS_Latitude2);
	SfGPS_DeltaMeters = (LfGPS_LatitudeCos1 * LfGPS_LatitudeSin2) - (LfGPS_LatitudeSin1 * LfGPS_LatitudeCos2 * LfGPS_LongitudeDeltaCos);
	SfGPS_DeltaMeters = SfGPS_DeltaMeters * SfGPS_DeltaMeters;
	SfGPS_DeltaMeters += ((LfGPS_LatitudeCos2 * LfGPS_LongitudeDeltaSin) * (LfGPS_LatitudeCos2 * LfGPS_LongitudeDeltaSin));
	SfGPS_DeltaMeters = sqrt(SfGPS_DeltaMeters);
	float LfGPS_Denom = ((LfGPS_LatitudeSin1 * LfGPS_LatitudeSin2) + (LfGPS_LatitudeCos1 * LfGPS_LatitudeCos2 * LfGPS_LongitudeDeltaCos));
	SfGPS_DeltaMeters = atan2(SfGPS_DeltaMeters, LfGPS_Denom);
	SfGPS_DeltaMeters = SfGPS_DeltaMeters * 6372795;

	return SfGPS_DeltaMeters;
}

uint16_t GetGpsSpeed()
{
	return SwGPS_Speed;
}

float GetDistanceDelta()
{
	return SfGPS_DeltaMeters;
}
