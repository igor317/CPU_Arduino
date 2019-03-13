#ifndef STEPPER_H
#include "Arduino.h"
#define STEPPER_H
class Stepper
{
private:
	float interval;					// �������� ��������� (1 - ����)
	bool halfstep;					// ����� ��������
	int step;						// ���
	int _ld1, _ld2, _ld3, _ld4;		// ���� �������	
	int _btnZeroPos, _btnEndPos;	// ���� ������
	bool clickbZpos = false;				// ������� ������ ��������� �������
	bool clickbEpos = false;				// ������� ������ �������� �������
	int counter = 0;

	bool inZero;					// ����� �� ������� � ������
	bool inEnd;						// ����� �� ������� � �����
	
	unsigned long curtime;			// ������� �����
	unsigned long _pmills;			// ���������� �����

	bool left;						// ����������� ���������
	int countstep;					// ���������� �����
	bool moveRun;					// ��������� ��������� (��������\�����)

	void curstep(int step);			// ������� ��������� ���������
	void Move();					// �������� ���������
	void Stop();					// ��������� ���������
public:
	Stepper(int ld1, int ld2, int ld3, int ld4, int btnZeroPos, int btnEndPos);

	void Run();									// ������� ������� ���������	
	void SetSteps(int countStepS,bool leftS);	// ������ ���������� ����� � �����������
	void SetOrient(bool leftS);					// ������ �����������
	int GetSteps();								// �������� ���������� ���������� �����
	void SetSpeed(float intervalS);				// ������ �������� ���������
	void SetStepMode(bool halfstepS);			// ���������� ����� (true - ����������� | false - ������������)
	bool GetZeroPos();							// �������� �������� ������� ������� 
	bool GetEndPos();							// �������� �������� �������� �������
	int GetCounter();							// �������� ���������� �����
	void SetZeroCounter();						// �������� ������� �����

};
#endif
