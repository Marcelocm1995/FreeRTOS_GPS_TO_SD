		if(READY_MESSAGE == 1)
		{
			//HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_SET);
			
			memcpy(myNMEA, Rx_data, 80);
			memset(Rx_data,0x00,80);
			
			if((myNMEA[1]=='G') && (myNMEA[2]=='P') && (myNMEA[3]=='R') && (myNMEA[4]=='M') && (myNMEA[5]=='C'))
			{
				HAL_TIM_Base_Stop(&htim1);
				TIMER_MS =__HAL_TIM_GetCounter(&htim1);
				__HAL_TIM_SetCounter(&htim1, 0);
				HAL_TIM_Base_Start(&htim1);
				result = 1;
				HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_SET);
			}
			
			else
			{
				result = 0;
				memset(myNMEA,0x00,80);
			}
			
			//Example $GPRMC,151802.962,A,2338.944,S,04639.370,W,208.0,219.1,240320,000.0,W*7E
			//vector  0123456789
			if(result)
			{	
				memcpy(OLD_NMEA_latitude, NMEA_latitude, sizeof(NMEA_latitude));
				memcpy(OLD_NMEA_longitude, NMEA_longitude, sizeof(NMEA_longitude));				
				memcpy(OLD_NMEA_latitude_pole, NMEA_latitude_pole, sizeof(NMEA_latitude_pole));
				memcpy(OLD_NMEA_longitude_side, NMEA_longitude_side, sizeof(NMEA_longitude_side));
				
				//HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_SET);
				NMEA_length_pause = 7;
				j = 7;
				while(myNMEA[j] != COMMA)
				{
					NMEA_time[j-NMEA_length_pause] = myNMEA[j];
					j++;
				}
				NMEA_time_length = j - NMEA_length_pause  - 1;
				NMEA_length_pause = j;
				
				j++;
			
				while(myNMEA[j] != COMMA)
				{
					NMEA_fix[j-NMEA_length_pause-1] = myNMEA[j];
					j++;
				}
				NMEA_fix_length = j - NMEA_length_pause - 1;
				NMEA_length_pause = j;
				
				j++;
			
				while(myNMEA[j] != COMMA)
				{
					NMEA_latitude[j-NMEA_length_pause-1] = myNMEA[j];
					j++;
				}
				NMEA_latitude_length = j - NMEA_length_pause - 1;
				NMEA_length_pause = j;
				
				j++;
			
				while(myNMEA[j] != COMMA)
				{
					NMEA_latitude_pole[j-NMEA_length_pause-1] = myNMEA[j];
					j++;
				}
				NMEA_latitude_pole_length = j - NMEA_length_pause - 1;
				NMEA_length_pause = j;
				
				j++;
			
				while(myNMEA[j] != COMMA)
				{
					NMEA_longitude[j-NMEA_length_pause-1] = myNMEA[j];
					j++;
				}
				NMEA_longitude_length = j - NMEA_length_pause - 1;
				NMEA_length_pause = j;
				
				j++;
			
				while(myNMEA[j] != COMMA)
				{
					NMEA_longitude_side[j-NMEA_length_pause-1] = myNMEA[j];
					j++;
				}
				NMEA_longitude_side_length = j - NMEA_length_pause - 1;
				NMEA_length_pause = j;
				
				j++;
			
				while(myNMEA[j] != COMMA)
				{
					NMEA_speed[j-NMEA_length_pause-1] = myNMEA[j];
					j++;
				}
				NMEA_speed_length = j - NMEA_length_pause - 1;
				NMEA_length_pause = j;
				
				j++;
			
				while(myNMEA[j] != COMMA)
				{
					NMEA_course[j-NMEA_length_pause-1] = myNMEA[j];
					j++;
				}
				NMEA_course_length = j - NMEA_length_pause - 1;
				NMEA_length_pause = j;
				
				j++;
			
				while(myNMEA[j] != COMMA)
				{
					NMEA_date[j-NMEA_length_pause-1] = myNMEA[j];
					j++;
				}
				NMEA_date_length = j - NMEA_length_pause - 1;
				NMEA_length_pause = j;
				
				j++;
			
				while(myNMEA[j] != COMMA)
				{
					NMEA_variation[j-NMEA_length_pause-1] = myNMEA[j];
					j++;
				}
				NMEA_variation_length = j - NMEA_length_pause - 1;
				NMEA_length_pause = j;
				
				j++;
			
				while(myNMEA[j] != '*')
				{
					NMEA_variation_side[j-NMEA_length_pause-1] = myNMEA[j];
					j++;
				}
				NMEA_variation_side_length = j - NMEA_length_pause - 1;
			}
			
			j=0;
			k=0;		
			result = 0;	
			NMEA_length_pause = 0;
			//memset(myNMEA,0x00,80);
			
						
			int SPEED = (atoi(NMEA_speed)) * 1.852;
			sprintf(strbff, "%03iKm/h ", SPEED);
			ssd1306_SetCursor (2,0);
			ssd1306_WriteString (strbff, Font_16x26);
			
			rawHOUR[0] = NMEA_time[0];
			rawHOUR[1] = NMEA_time[1];
			HOUR = atoi(rawHOUR);
			HOUR = HOUR - 3;
			if(HOUR < 0)
				HOUR += 24;
			sprintf(strbff, "%02i", HOUR);
			
			ssd1306_SetCursor (2,25);
			ssd1306_WriteString (strbff, Font_7x10);
			
			//ssd1306_SetCursor (2,30);		
			//ssd1306_WriteChar (NMEA_time[0], Font_7x10);ssd1306_WriteChar (NMEA_time[1], Font_7x10); 
			ssd1306_WriteChar (0x3A, Font_7x10);
			ssd1306_WriteChar (NMEA_time[2], Font_7x10);ssd1306_WriteChar (NMEA_time[3], Font_7x10); //MINUTES
			ssd1306_WriteChar (0x3A, Font_7x10);
			ssd1306_WriteChar (NMEA_time[4], Font_7x10);ssd1306_WriteChar (NMEA_time[5], Font_7x10); //SECONDS
			ssd1306_WriteChar (NMEA_time[6], Font_7x10);ssd1306_WriteChar (NMEA_time[7], Font_7x10); //MILISSECOND
			
			if((NMEA_fix[0] == 'A') && (SPEED > 0))
			{
				float lat1  = GpsToDecimalDegrees(OLD_NMEA_latitude,  OLD_NMEA_latitude_pole[0]);
				float long1 = GpsToDecimalDegrees(OLD_NMEA_longitude, OLD_NMEA_longitude_side[0]);
				float lat2  = GpsToDecimalDegrees(NMEA_latitude,  NMEA_latitude_pole[0]);
				float long2 = GpsToDecimalDegrees(NMEA_longitude, NMEA_longitude_side[0]);
    
				float delta = distance_between(lat1, long1, lat2, long2);
				DISTANCE_METERS = DISTANCE_METERS + delta;
				DISTANCE_KM = DISTANCE_METERS / 1000.0;
			}
			if(HAL_GPIO_ReadPin(B1_GPIO_Port, B1_Pin) == 0)
			{
				DISTANCE_METERS = 0;
				DISTANCE_KM = 0;
			}
			
			if((SPEED == 0) && (DRAG == 0))
			{
				rawSECOND[0] = NMEA_time[4]; //second
				rawSECOND[1] = NMEA_time[5]; //second
				rawSECOND[2] = NMEA_time[6]; //dot
				rawSECOND[3] = NMEA_time[7]; //milissecond
				START_TIME_0_100 = atof(rawSECOND);
				DRAG = 1;
			}
			
			if((SPEED >= 100) && (DRAG == 1))
			{
				rawSECOND[0] = NMEA_time[4]; //second
				rawSECOND[1] = NMEA_time[5]; //second
				rawSECOND[2] = NMEA_time[6]; //dot
				rawSECOND[3] = NMEA_time[7]; //milissecond
				END_TIME_0_100 = atof(rawSECOND);
				DRAG = 0;
				TOTAL_TIME_0_100 = END_TIME_0_100 - START_TIME_0_100;
				
			}
			sprintf(strbff, "0-100: %04.1f", TOTAL_TIME_0_100);
			ssd1306_SetCursor (2,38);
			ssd1306_WriteString(strbff, Font_7x10);
			
			memset(strbff,0x00,30);
			sprintf(strbff, "D %07.2fKm   ", DISTANCE_KM);
			ssd1306_SetCursor (30,53);
			ssd1306_WriteString(strbff, Font_7x10);
			
			ssd1306_SetCursor (2,53);
			ssd1306_WriteChar(NMEA_fix[0], Font_7x10);
			ssd1306_UpdateScreen ();
			
			READY_MESSAGE = 0;
			HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_RESET);
		}
