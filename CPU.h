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
	Stepper* stZ;					// ��� Z
	IPoint* points;					// ����� ������
	int numb;						// ���������� ��� ���������� �����
	int numbf;						// ���������� ��� ��������
	const char startSymbol = '#';	// ������ ������ ���������
	const char endSymbol = '$';		// ������ ����� ���������
	int i = 0;						// �������� ��� ������
	int readstate = 0;				// ��������� ������
										// 0 - ���� ���������
										// 1 - ������ ���������
										// 2 - �������� ���������
	char buff[25];					// ����� ��� ���������
	int xCounter = 0;					// ���������� ����� �� ��� X
	int yCounter = 0;					// ���������� ����� �� ��� Y
	float xSpeed = 0;				// �������� �� ��� X
	float ySpeed = 0;				// �������� �� ��� Y
	int res = 100;					// ���������� ����� ������
	bool execute = false;			// ��������� ���������� ������
	bool exX = false;				// ��������� ���������� ������ �� ��� X
	bool exY = false;				// ��������� ���������� ������ �� ��� Y
	bool moveSteppers = true;		// ������������ �������� �������

	void MoveCommand();				// ������� MOVE
	void StartPosCommand();			// ������� START POSITION
	void CalibrationYCommand();		// ������� ������������ ����� �� ��� Y
	void CalibrationXCommand();		// ������� ������������ ����� �� ��� X
	void StopCommand();				// ������� STOP
	void CircleCommand();			// ������� Circle
	void Circle2(int radX, int radY, float alpha);
	void SplineCommand(int x1, int y1, int x2, int y2, int x3, int y3); // ������� Spline 3
	void Test();
	void Test2();


	void CalculateSpeed(float maxSpeed,int xSteps, int ySteps);	// ��������� ��������
	void CaclulateSplinePoints(int x1, int y1, int x2, int y2, int x3, int y3);
	void CalculateCirclePoints(int cX, int cY, int radX, int radY);
	void CalculateCirclePointsA(int radX, int radY, float alpha);
	

	int Round(float number);

	void ExecuteCommand();			// ��������� �������
	void SendMessage(int state);	// ��������� ���������
public:
	CPU();
	void Run();						// ������� ������� ������� ���������
}; 


#endif