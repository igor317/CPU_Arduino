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
	Stepper* stY;					// ��� Y
	Stepper* stX;					// ��� X
	IPoint* points;
	int numb;						// ���������� ��� ���������� �����
	int numbf;						// ���������� ��� ��������
	const char startSymbol = '#';	// ������ ������ ���������
	const char endSymbol = '$';		// ������ ����� ���������
	int i = 0;						// �������� ��� ������
	int readstate = 0;				// ��������� ������
										// 0 - ���� ���������
										// 1 - ������ ���������
										// 2 - �������� ���������
	char buff[12];					// ����� ��� ���������
	int xcount = 0;					// ���������� ����� �� ��� X
	int ycount = 0;					// ���������� ����� �� ��� Y
	float xSpeed = 0;				// �������� �� ��� X
	float ySpeed = 0;				// �������� �� ��� Y
	bool execute = false;			// ��������� ���������� ������
	bool exX = false;				// ��������� ���������� ������ �� ��� X
	bool exY = false;				// ��������� ���������� ������ �� ��� Y
	bool moveSteppers = true;		// ������������ �������� �������

	int xCounter = 0;
	int yCounter = 0;

	int pX;
	int res;
	int splineCounter = 0;
	int cX, cY;
	bool resetC;
	bool resetD;

	void MoveCommand();				// ������� MOVE
	void StartPosCommand();			// ������� START POSITION
	void CalibrationYCommand();		// ������� ������������ ����� �� ��� Y
	void CalibrationXCommand();		// ������� ������������ ����� �� ��� X
	void StopCommand();				// ������� STOP
	void TestCommand(int x1, int y1, int x2, int y2, int res);				// ������� TEST
	void TestCircle(int rad, int res);

	void CalculateSpeed(float maxSpeed,int xSteps, int ySteps);	// ��������� ��������
	void CaclulateSplinePoints(int x1, int y1, int x2, int y2, int res);
	void CalculateCirclePoints(int rad, int res);

	int Round(float number);

	void ExecuteCommand();			// ��������� �������
	void SendMessage(int state);	// ��������� ���������
public:
	CPU();
	void Run();						// ������� ������� ������� ���������
}; 


#endif