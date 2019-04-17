#ifndef CPU_H
#define CPU_H
#include "Stepper.h"
struct IPoint
{
	int X;
	int Y;
	float xSpeed;
	float ySpeed;
	bool leftX;
	bool leftY;
};

class CPU
{
private:
	Stepper* stY;					// Ось Y
	Stepper* stX;					// Ось X
	Stepper* stZ;					// Ось Z
	IPoint* points;					// Точки кривых
	int numb;						// Переменная для количества шагов
	int numbf;						// Переменная для скорости
	const char startSymbol = '#';	// Символ начала сообщения
	const char endSymbol = '$';		// Символ конца сообщения
	int i = 0;						// итератор для буфера
	int readstate = 0;				// Состояние чтения
										// 0 - ждет сообщения
										// 1 - читает сообщение
										// 2 - прочитал сообщение
	char buff[25];					// буфер для сообщения
	int xCounter = 0;					// Количество шагов по оси X
	int yCounter = 0;					// Количество шагов по оси Y
	float xSpeed = 0;				// Скорость по оси X
	float ySpeed = 0;				// Скорость по оси Y
	int res = 100;					// Количество точек кривых
	bool execute = false;			// Состояние выполнения задачи
	bool exX = false;				// Состояние выполнения задачи по оси X
	bool exY = false;				// Состояние выполнения задачи по оси Y
	bool moveSteppers = true;		// Единоразовая перечада команды

	void MoveCommand();				// Команда MOVE
	void StartPosCommand();			// Команда START POSITION
	void CalibrationYCommand();		// Команда МАКСИМАЛЬНАЯ ДЛИНА ПО ОСИ Y
	void CalibrationXCommand();		// Команда МАКСИМАЛЬНАЯ ДЛИНА ПО ОСИ X
	void StopCommand();				// Команда STOP
	void CircleCommand();			// Команда Circle
	void Circle2(int radX, int radY, float alpha);
	void SplineCommand(int x1, int y1, int x2, int y2, int x3, int y3); // Команда Spline 3
	void Test();
	void Test2();


	void CalculateSpeed(float maxSpeed,int xSteps, int ySteps);	// Расчитать скорость
	void CaclulateSplinePoints(int x1, int y1, int x2, int y2, int x3, int y3);
	void CalculateCirclePoints(int cX, int cY, int radX, int radY);
	void CalculateCirclePointsA(int radX, int radY, float alpha);
	

	int Round(float number);

	void ExecuteCommand();			// ВЫПОЛНИТЬ КОМАНДУ
	void SendMessage(int state);	// ОТПРАВИТЬ СООБЩЕНИЕ
public:
	CPU();
	void Run();						// ГЛАВНАЯ ФУНКЦИЯ ЗАПУСКА ПРОГРАММЫ
}; 


#endif