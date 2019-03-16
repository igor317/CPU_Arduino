#include "Stepper.h"

Stepper::Stepper(int ld1, int ld2, int ld3, int ld4,int btnZeroPos, int btnEndPos)
{
	countstep = 0;
	moveRun = false;
	inZero = false;
	step = 1;
	_ld1 = ld1;
	_ld2 = ld2;
	_ld3 = ld3;
	_ld4 = ld4;
	_btnZeroPos = btnZeroPos;
	_btnEndPos = btnEndPos;
	curtime = 0;
	_pmills = 0;
	left = false;
	pinMode(_ld1, OUTPUT);
	pinMode(_ld2, OUTPUT);
	pinMode(_ld3, OUTPUT);
	pinMode(_ld4, OUTPUT);
	SetSpeed(1);
	SetStepMode(true);
}

void Stepper::Move()
{
	if (_pmills > curtime)
		_pmills = 0;
	if (step > 8)
		step = 1;
	if (step < 1)
		step = 8;
	curtime = micros();
	if ((curtime - _pmills) >= interval*1000)
	{
		counter++;
		_pmills = curtime;
		countstep--;
		if (left)
		{
			step--;
			if (!halfstep)
			{
				step--;
				counter++;
				countstep--;
			}
		}
		else
		{
			step++;
			if (!halfstep)
			{
				step++;
				counter++;
				countstep--;
			}
		}
	}
	curstep(step);
}

void Stepper::Stop()
{
	digitalWrite(_ld1, LOW);
	digitalWrite(_ld2, LOW);
	digitalWrite(_ld3, LOW);
	digitalWrite(_ld4, LOW);
}

void Stepper::curstep(int step)
{
	switch (step)
	{
	case 1:
		digitalWrite(_ld1, LOW);
		digitalWrite(_ld2, LOW);
		digitalWrite(_ld3, HIGH);
		digitalWrite(_ld4, LOW);
		break;
	case 2:
		digitalWrite(_ld1, LOW);
		digitalWrite(_ld2, HIGH);
		digitalWrite(_ld3, HIGH);
		digitalWrite(_ld4, LOW);
		break;
	case 3:
		digitalWrite(_ld1, LOW);
		digitalWrite(_ld2, HIGH);
		digitalWrite(_ld3, LOW);
		digitalWrite(_ld4, LOW);
		break;
	case 4:
		digitalWrite(_ld1, LOW);
		digitalWrite(_ld2, HIGH);
		digitalWrite(_ld3, LOW);
		digitalWrite(_ld4, HIGH);
		break;
	case 5:
		digitalWrite(_ld1, LOW);
		digitalWrite(_ld2, LOW);
		digitalWrite(_ld3, LOW);
		digitalWrite(_ld4, HIGH);
	case 6:
		digitalWrite(_ld1, HIGH);
		digitalWrite(_ld2, LOW);
		digitalWrite(_ld3, LOW);
		digitalWrite(_ld4, HIGH);
		break;
	case 7:
		digitalWrite(_ld1, HIGH);
		digitalWrite(_ld2, LOW);
		digitalWrite(_ld3, LOW);
		digitalWrite(_ld4, LOW);
		break;
	case 8:
		digitalWrite(_ld1, HIGH);
		digitalWrite(_ld2, LOW);
		digitalWrite(_ld3, HIGH);
		digitalWrite(_ld4, LOW);
		break;
	}
}

void Stepper::Run()
{
	if (!digitalRead(_btnZeroPos))
	{
		inZero = true;
		if (clickbZpos && left)
		{
			countstep = 0;
			clickbZpos = false;
		}
	}
	else
	{
		inZero = false;
		clickbZpos = true;
	}
	if (!digitalRead(_btnEndPos))
	{
		inEnd = true;
		if (clickbEpos && !left)
		{
			countstep = 0;
			clickbEpos = false;
		}
	}
	else
	{
		inEnd = false;
		clickbEpos = true;
	}
	if (countstep > 0)
	{
		Move();
	}
	else
	{
		Stop();
	}
}

void Stepper::SetSteps(int countStepS, bool leftS)
{
	countstep = countStepS;
	left = leftS;
}

void Stepper::SetOrient(bool leftS)
{
	left = leftS;
}

bool Stepper::GetZeroPos()
{
	return inZero;
}

bool Stepper::GetEndPos()
{
	return inEnd;
}

int Stepper::GetSteps()
{
	return countstep;
}

void Stepper::SetSpeed(float invervalS)
{
	interval = invervalS;
}

void Stepper::SetStepMode(bool halfstepS)
{
	halfstep = halfstepS;
}
int Stepper::GetCounter()
{
	return counter;
}
void Stepper::SetZeroCounter()
{
	counter = 0;
}