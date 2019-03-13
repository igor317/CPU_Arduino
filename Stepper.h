#ifndef STEPPER_H
#include "Arduino.h"
#define STEPPER_H
class Stepper
{
private:
	float interval;					// Скорость двигателя (1 - макс)
	bool halfstep;					// Режим полушага
	int step;						// Шаг
	int _ld1, _ld2, _ld3, _ld4;		// Пины обмотки	
	int _btnZeroPos, _btnEndPos;	// Пины кнопок
	bool clickbZpos = false;				// Зажатие кнопки начальной позиции
	bool clickbEpos = false;				// Зажатие кнопки конечной позиции
	int counter = 0;

	bool inZero;					// Стоит ли каретка в начале
	bool inEnd;						// Стоит ли каретка в конце
	
	unsigned long curtime;			// Текущее время
	unsigned long _pmills;			// Предыдущее время

	bool left;						// Направление двигателя
	int countstep;					// Количество шагов
	bool moveRun;					// Состояние двигателя (работает\стоит)

	void curstep(int step);			// ТЕКУЩЕЕ ПОЛОЖЕНИЕ ДВИГАТЕЛЯ
	void Move();					// ДВИЖЕНИЕ ДВИГАТЕЛЯ
	void Stop();					// ОСТАНОВКА ДВИГАТЕЛЯ
public:
	Stepper(int ld1, int ld2, int ld3, int ld4, int btnZeroPos, int btnEndPos);

	void Run();									// ГЛАВНАЯ ФУНКЦИЯ ДВИГАТЕЛЯ	
	void SetSteps(int countStepS,bool leftS);	// ЗАДАТЬ КОЛИЧЕСТВО ШАГОВ И НАПРАВЛЕНИЕ
	void SetOrient(bool leftS);					// ЗАДАТЬ НАПРАВЛЕНИЕ
	int GetSteps();								// ПОЛУЧИТЬ ОСТАВШЕЕСЯ КОЛИЧЕСТВО ШАГОВ
	void SetSpeed(float intervalS);				// ЗАДАТЬ СКОРОСТЬ ДВИГАТЕЛЯ
	void SetStepMode(bool halfstepS);			// УСТАНОВИТЬ РЕЖИМ (true - полушаговый | false - полношаговый)
	bool GetZeroPos();							// ПОЛУЧИТЬ ЗНАЧЕНИЕ НУЛЕВОЙ ПОЗИЦИИ 
	bool GetEndPos();							// ПОЛУЧИТЬ ЗНАЧЕНИЕ КОНЕЧНОЙ ПОЗИЦИИ
	int GetCounter();							// ПОЛУЧИТЬ КОЛИЧЕСТВО ШАГОВ
	void SetZeroCounter();						// ОБНУЛИТЬ СЧЕТЧИК ШАГОВ

};
#endif
