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
	IPoint* points;
	int numb;						// Переменная для количества шагов
	int numbf;						// Переменная для скорости
	const char startSymbol = '#';	// Символ начала сообщения
	const char endSymbol = '$';		// Символ конца сообщения
	int i = 0;						// итератор для буфера
	int readstate = 0;				// Состояние чтения
										// 0 - ждет сообщения
										// 1 - читает сообщение
										// 2 - прочитал сообщение
	char buff[12];					// буфер для сообщения
	int xcount = 0;					// Количество шагов по оси X
	int ycount = 0;					// Количество шагов по оси Y
	float xSpeed = 0;				// Скорость по оси X
	float ySpeed = 0;				// Скорость по оси Y
	bool execute = false;			// Состояние выполнения задачи
	bool exX = false;				// Состояние выполнения задачи по оси X
	bool exY = false;				// Состояние выполнения задачи по оси Y
	bool moveSteppers = true;		// Единоразовая перечада команды

	int xCounter = 0;
	int yCounter = 0;

	int pX;
	int res;
	int splineCounter = 0;
	int cX, cY;
	bool resetC;
	bool resetD;

	void MoveCommand();				// Команда MOVE
	void StartPosCommand();			// Команда START POSITION
	void CalibrationYCommand();		// Команда МАКСИМАЛЬНАЯ ДЛИНА ПО ОСИ Y
	void CalibrationXCommand();		// Команда МАКСИМАЛЬНАЯ ДЛИНА ПО ОСИ X
	void StopCommand();				// Команда STOP
	void TestCommand(int x1, int y1, int x2, int y2, int res);				// Команда TEST
	void TestCircle(int rad, int res);

	void CalculateSpeed(float maxSpeed,int xSteps, int ySteps);	// Расчитать скорость
	void CaclulateSplinePoints(int x1, int y1, int x2, int y2, int res);
	void CalculateCirclePoints(int rad, int res);

	int Round(float number);

	void ExecuteCommand();			// ВЫПОЛНИТЬ КОМАНДУ
	void SendMessage(int state);	// ОТПРАВИТЬ СООБЩЕНИЕ
public:
	CPU();
	void Run();						// ГЛАВНАЯ ФУНКЦИЯ ЗАПУСКА ПРОГРАММЫ
}; 


#endif