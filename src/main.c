/*
 * main.c
 *
 *  Created on: 2020. 2. 7.
 *      Author: baegdaeseong
 *      자동차 전방 장애물 감지 및 차간격 유지 목적
 */

#include<stdio.h>
#include<stdlib.h>
#define	SIZE_SENSOR_DATA		5

int sensorRawData[SIZE_SENSOR_DATA];


typedef struct __car{

	//제어 입력
	int acc;					//가속페달값            [%]
	int brake;      			//브레이크 페달          [%]

	//제어 출력
	int velve;					//연료 및 공기 밸브 변위  [%]
	int brakeDisplacement;      //브레이크 패드간격     [cm]

	// 파라메터
	int weight;     			//무게                [kg]
	int length;     			//길이                [m]

	float Gain_AccToVelve; 		//가속페달과 연료밸브의 게인
	float Gain_BrakeToPad; 		//브레이크 페달과 브레이크 패드 게인
	int Max_brake_displacement; //최대 브레이크 패드 간격

	//함수 포인터
	void (*forward)();
	void (*reverse)();
	void (*acceleration)();
	void (*speedDown)();
	void (*parkingBrake)();
	void (*parkingBrakeOff)();

}car;


void go_forward(void){
	printf("앞으로 갑니다.");
}

void go_reverse(void){
	printf("뒤로 갑니다.");
}

void Acceleration(car *p){

	if (p->brake < 10){
		p->velve = 0;
		printf("가속도가 10 m/s이하라서  밸브를 0%로 설정합니다.");
	}
	else{
		p->velve = p->acc * p->Gain_AccToVelve;
		printf("가속하기 위해서 밸브를 컨트롤 합니다.");
	}

}

void BrakeControl(car *p){

	if (p->brake < 10){
		p->brakeDisplacement = p->Max_brake_displacement;
		printf("브레이크 패드 간격을  10cm로 설정합니다.");
	}
	else{
		p->brakeDisplacement = p->Max_brake_displacement - (p->brake * p->Gain_BrakeToPad);
		printf("브레이크 패드의 간격을 제어 합니다.");
	}

}

void Parking(void){
	printf("주차브레이크를 작동합니다. ");

}

void ParkingOff(void){
	printf("주차브레이크를 해제 합니다. ");

}


int sum(int *data, int size)
{

	int sum =0;
	for (int i=0; i<size; i++)
	{
		sum+=  *(data+i);
	}

	return sum;
}

car test_car;

void main()
{
	double sumOfSensor;

	//레이더 센서값
	sensorRawData[0] = 1;
	sensorRawData[1] = 1;
	sensorRawData[2] = 100;
	sensorRawData[3] = 1;
	sensorRawData[4] = 6;

	//전방 레이더 센서의 합산된 값
	sumOfSensor = sum(&sensorRawData, SIZE_SENSOR_DATA);


	//변수 초기화
	test_car.Gain_AccToVelve = 0.5;
	test_car.Gain_BrakeToPad = 1.6;
	test_car.Max_brake_displacement = 6; // 6cm


	//함수 등록
	test_car.forward 		= &go_forward;
	test_car.reverse 		= &go_reverse;
	test_car.acceleration 	= &Acceleration;
	test_car.speedDown		= &BrakeControl;
	test_car.parkingBrake 	= &Parking;
	test_car.parkingBrakeOff= &ParkingOff;


	test_car.parkingBrakeOff();	//주차 브레이크를 해제 한다.
	test_car.reverse();
	test_car.forward();

	while(1){

		if(sumOfSensor > 100) //전방 레이더 센서의 값이 100보다 크면 앞에 앞에 장애물이 없음 , <가속해도 됨 >
		{
			test_car.acc = 50;
			test_car.acceleration(&test_car);
			test_car.forward();
		}
		else if(sumOfSensor > 50){
			test_car.acc = 50;
			test_car.acceleration(&test_car);
			test_car.forward();

		}
		else if(sumOfSensor > 50){
			test_car.brake = 30;
			test_car.speedDown(&test_car);

			test_car.acc = 0;
			test_car.acceleration(&test_car);
		}
		else
		{
			test_car.parkingBrake();

			test_car.acc = 0;
			test_car.acceleration(&test_car);
		}

		sleep(1); //1초대기 컴퓨터 cpu 부하 감소 (없으면 이클립스 뻗음)
	}

}


