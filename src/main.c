/*
 * main.c
 *
 *  Created on: 2020. 2. 7.
 *      Author: baegdaeseong
 */


#define	SIZE_SENSOR_DATA		5

int sensorRawData[SIZE_SENSOR_DATA];


int sum(int *data, int size)
{

	int sum =0;
	for (int i=0; i<size; i++)
	{
		sum+=  *(data+i);
	}

	return sum;
}


void main()
{
	double sumOfSensor;

	sensorRawData[0] = 1;
	sensorRawData[1] = 1;
	sensorRawData[2] = 5;
	sensorRawData[3] = 1;
	sensorRawData[4] = 6;

	sumOfSensor = sum(&sensorRawData, SIZE_SENSOR_DATA);



}


