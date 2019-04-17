#include"CPU.h"

CPU::CPU()
{
	stY = new Stepper(2, 3, 4, 5, A0, A2);
	stX = new Stepper(6, 7, 8, 9, A1, A3);
	stZ = new Stepper(10, 11, 12, 13, A4, A5);
	i = 0;
	readstate = 0;
}

void CPU::Run()
{
	stY->Run();
	stX->Run();
	stZ->Run();
	ExecuteCommand();
	
	if (Serial.available() > 0)
	{
		char s = Serial.read();
		if (readstate == 1)
		{
			buff[i] = s;
			i++;
		}
		if (s == startSymbol && readstate == 0)
		{
			i = 0;
			readstate = 1;
		}
		if (s == endSymbol && readstate == 1)
		{
			readstate = 2;
		}
		if (readstate == 2)
		{
			moveSteppers = true;
			execute = true;
			exX = false;
			exY = false;
			if (points != NULL)
				delete points;
			readstate = 0;
			Serial.flush();
		}
			
	}
}

void CPU::SendMessage(int state)
{
	execute = false;
	if (points != NULL)
		delete points;
	switch (state)
	{
		case 0: // MOVE
			Serial.print(startSymbol);
			Serial.print("X moved on ");
			Serial.print(xCounter);
			Serial.print(" ");
			Serial.print("Y moved on ");
			Serial.print(yCounter);
			Serial.print(" ");
			Serial.print(endSymbol);
			break;
		case 1: // START POSITION
			Serial.print(startSymbol);
			Serial.print("In Start Position");
			Serial.print(endSymbol);
			break;
		case 2: // STOP
			Serial.print(startSymbol);
			Serial.print("Stopped");
			Serial.print(endSymbol);
			break;
		case 3: // CALIBRATION Y
			Serial.print(startSymbol);
			Serial.print("Max Y lenght is ");
			Serial.print(yCounter);
			Serial.print(endSymbol);
			break;
		case 4: // CALIBRATION X
			Serial.print(startSymbol);
			Serial.print("Max X lenght is ");
			Serial.print(xCounter);
			Serial.print(endSymbol);
			break;
		case 6: // Circle
			Serial.print(startSymbol);
			Serial.print("Circle");
			Serial.print(endSymbol);
			break;
		case 7: // Spline 3
			Serial.print(startSymbol);
			Serial.print("Spline 3");
			Serial.print(endSymbol);
			break;
		case 10:
			Serial.print(startSymbol);
			Serial.print("test");
			Serial.print(endSymbol);
			break;
	}
}

void CPU::ExecuteCommand()
{
	if (execute)
	{
		if (buff[0] == 'm' && buff[1] == 'v')
			MoveCommand();

		if (buff[0] == 's' && buff[1] == 't' && buff[2] == 'p' && buff[3] == 'o' && buff[4] == 's')
			StartPosCommand();

		if (buff[0] == 's' && buff[1] == 't' && buff[2] == 'o' && buff[3] == 'p')
			StopCommand();

		if (buff[0] == 'c' && buff[1] == 'a' && buff[2] == 'l' && buff[3] == 'i' && buff[4] == 'b' && buff[5] == 'y')
			CalibrationYCommand();

		if (buff[0] == 'c' && buff[1] == 'a' && buff[2] == 'l' && buff[3] == 'i' && buff[4] == 'b' && buff[5] == 'x')
			CalibrationXCommand();

		if (buff[0] == 'e' && buff[1] == 'l' && buff[2] == 'p')
			CircleCommand();

		if (buff[0] == 't' && buff[1] == 'e' && buff[2] == 's' && buff[3] == 't' && buff[4] == '1')
			//Circle2(250, 250, 0);
			Test();
			//SplineCommand(600, 1500, 1200, 0);

		if (buff[0] == 't' && buff[1] == 'e' && buff[2] == 's' && buff[3] == 't' && buff[4] == '3')
			//Circle2(250, 250, PI / 4);
			Test2();
			//Spline3Command(2000, 1000, -1000, 1000, 1000, 0);
	}
}

void CPU::CalculateSpeed(float maxSpeed,int xSteps, int ySteps)
{
	xSpeed = (ySteps > xSteps) ? maxSpeed *(float)ySteps / (float)xSteps : maxSpeed;
	ySpeed = (xSteps > ySteps) ? maxSpeed *(float)xSteps / (float)ySteps : maxSpeed;
	if (xSpeed >= 100)
		xSpeed = 99.99f;
	if (ySpeed >= 100)
		ySpeed = 99.99f;
}

void CPU::CaclulateSplinePoints(int x1, int y1, int x2, int y2, int x3, int y3)
{
	points = new IPoint[res];
	float t = 0;
	float g = 0;
	float delta = 1 / (float)res;
	int x = 0;
	int y = 0;
	int xt = 0;
	int yt = 0;
	for (int i = 0; i < res; ++i)
	{
		t += delta;
		g = t - delta;
		x = (3 * t - 6 * t*t + 3 * t*t*t)*x1 + (3 * t*t - 3 * t*t*t)*x2 + t * t*t*x3;
		y = (3 * t - 6 * t*t + 3 * t*t*t)*y1 + (3 * t*t - 3 * t*t*t)*y2 + t * t*t*y3;
		xt = (3 * g - 6 * g*g + 3 * g*g*g)*x1 + (3 * g*g - 3 * g*g*g)*x2 + g * g*g*x3;
		yt = (3 * g - 6 * g*g + 3 * g*g*g)*y1 + (3 * g*g - 3 * g*g*g)*y2 + g * g*g*y3;

		if (x >= xt)
		{
			points[i].X = Round(x - xt);
			points[i].leftX = false;
		}
		else
		{
			points[i].X = Round(xt - x);
			points[i].leftX = true;
		}
		if (y >= yt)
		{
			points[i].Y = Round(y - yt);
			points[i].leftY = false;
		}
		else
		{
			points[i].Y = Round(yt - y);
			points[i].leftY = true;
		}
		CalculateSpeed(1, points[i].X, points[i].Y);
		points[i].xSpeed = xSpeed;
		points[i].ySpeed = ySpeed;
	}
}

void CPU::CalculateCirclePoints(int cX, int cY, int radX, int radY)
{
	if (points != NULL)
		delete points;
	points = new IPoint[res];
	float t = 0;
	float g = 0;
	float delta = PI*2 / (float)res;
	float x = 0;
	float y = 0;
	float xt = 0;
	float yt = 0;
	x = cX + radX;
	y = cY;
	CalculateSpeed(1, x, y);
	points[0].X = x;
	points[0].Y = y;
	points[0].xSpeed = xSpeed;
	points[0].ySpeed = ySpeed;
	points[0].leftX = (x >= 0) ? false : true;
	points[0].leftY = (y >= 0) ? false : true;
	for (int i = 1; i < res; ++i)
	{
		t += delta;
		g = t - delta;
		x = radX * cos(t);
		y = radY * sin(t);
		xt = radX * cos(g);
		yt = radY * sin(g);
		if (x >= xt)
		{
			points[i].X = Round(x - xt);
			points[i].leftX = false;
		}
		else
		{
			points[i].X = Round(xt - x);
			points[i].leftX = true;
		}
		if (y >= yt)
		{
			points[i].Y = Round(y - yt);
			points[i].leftY = false;
		}
		else
		{
			points[i].Y = Round(yt - y);
			points[i].leftY = true;
		}
		CalculateSpeed(1, points[i].X, points[i].Y);
		points[i].xSpeed = xSpeed;
		points[i].ySpeed = ySpeed;

	}
}

void CPU::CalculateCirclePointsA(int radX, int radY,float alpha)
{
	if (points != NULL)
		delete points;
	points = new IPoint[res];
	float t = 0;
	float g = 0;
	float delta = PI * 2 / (float)res;
	float x = 0;
	float y = 0;
	float xt = 0;
	float yt = 0;
	for (int i = 0; i < res; ++i)
	{
		t += delta;
		g = t - delta;
		x = radX * cos(t) * cos(alpha) + radY * sin(t) * sin(alpha);
		y = -radX * cos(t) * sin(alpha) + radY * sin(t) * cos(alpha);
		xt = radX * cos(g) * cos(alpha) + radY * sin(g) * sin(alpha);
		yt = -radX * cos(g) * sin(alpha) + radY * sin(g) * cos(alpha);
		if (x >= xt)
		{
			points[i].X = Round(x - xt);
			points[i].leftX = false;
		}
		else
		{
			points[i].X = Round(xt - x);
			points[i].leftX = true;
		}
		if (y >= yt)
		{
			points[i].Y = Round(y - yt);
			points[i].leftY = false;
		}
		else
		{
			points[i].Y = Round(yt - y);
			points[i].leftY = true;
		}
		CalculateSpeed(1, points[i].X, points[i].Y);
		points[i].xSpeed = xSpeed;
		points[i].ySpeed = ySpeed;

	}
}

void CPU::MoveCommand()
{
	if (moveSteppers)
	{
		moveSteppers = false;
		int numbX = 0;
		int numbY = 0;
		bool lX = false;
		bool lY = false;

		for (int i = 3; i < 7; ++i) // ируш X
		{
			numbX *= 10;
			numbX += buff[i] - 48;
		}
		for (int i = 8; i < 12; ++i) // ируш Y
		{
			numbY *= 10;
			numbY += buff[i] - 48;
		}

		if (buff[2] == 'l')
			lX = true;
		if (buff[2] == 'r')
			lX = false;
		if (buff[7] == 'l')
			lY = true;
		if (buff[7] == 'r')
			lY = false;

		xCounter = numbX;
		yCounter = numbY;
		CalculateSpeed(1,numbX, numbY);
		stX->SetSpeed(xSpeed);
		stY->SetSpeed(ySpeed);
		stX->SetSteps(xCounter, lX);
		stY->SetSteps(yCounter, lY);
	}
	if (stX->GetSteps() == 0 && stY->GetSteps() == 0)
		SendMessage(0);
}

void CPU::StartPosCommand()
{
	unsigned long curtime = millis();
	if (!stX->GetZeroPos() && !exX)
	{
		stX->SetSpeed(1);
		stX->SetSteps(5000, true);
	}
	else
		exX = true;

	if (!stY->GetZeroPos() && !exY)
	{
		stY->SetSpeed(1);
		stY->SetSteps(5000, true);
	}
	else
		exY = true;

	if (exX && exY)
		SendMessage(1);
}

void CPU::CalibrationYCommand()
{
	unsigned long curtime = millis();
	if (moveSteppers)
	{
		if (!stY->GetZeroPos())
		{
			stY->SetSpeed(1);
			stY->SetSteps(5000, true);
		}
		else
		{
			stY->SetZeroCounter();
			moveSteppers = false;
		}
	}
	else
	{
		if (!stY->GetEndPos())
		{
			stY->SetSpeed(1);
			stY->SetSteps(5000, false);
		}
		else
		{
			yCounter = stY->GetCounter();
			SendMessage(3);
		}
	}
}

void CPU::CalibrationXCommand()
{
	unsigned long curtime = millis();
	if (moveSteppers)
	{
		if (!stX->GetZeroPos())
		{
			stX->SetSpeed(1);
			stX->SetSteps(5000, true);
		}
		else
		{
			stX->SetZeroCounter();
			moveSteppers = false;
		}
	}
	else
	{
		if (!stX->GetEndPos())
		{
			stX->SetSpeed(1);
			stX->SetSteps(5000, false);
		}
		else
		{
			xCounter = stX->GetCounter();
			SendMessage(4);
		}
	}
}

void CPU::StopCommand()
{
	stX->SetSteps(0, true);
	stY->SetSteps(0, true);
	stZ->SetSteps(0, true);
	SendMessage(2);
}

void CPU::SplineCommand(int x1, int y1, int x2, int y2, int x3, int y3)
{
	if (moveSteppers)
	{
		exX = false;
		exY = false;
		xCounter = 0;
		yCounter = 0;
		CaclulateSpline3Points(x1, y1, x2, y2, x3, y3);
		moveSteppers = false;
	}
	if (stX->GetSteps() <= 0 && !exX)
	{
		if (xCounter >= res - 1)
			exX = true;
		stX->SetSteps(points[xCounter].X, points[xCounter].leftX);
		stX->SetSpeed(points[xCounter].xSpeed);
		xCounter++;
	}
	if (stY->GetSteps() <= 0 && !exY)
	{
		if (yCounter >= res - 1)
			exY = true;
		stY->SetSteps(points[yCounter].Y, points[yCounter].leftY);
		stY->SetSpeed(points[yCounter].ySpeed);
		yCounter++;
	}
	if (exX && exY)
		SendMessage(7);
}

void CPU::CircleCommand()
{
	if (moveSteppers)
	{
		exX = false;
		exY = false;
		xCounter = 0;
		yCounter = 0;
		int numbCx = 0;
		int numbCy = 0;
		int radX = 0;
		int radY = 0;
		for (int i = 4; i < 8; ++i)
		{
			numbCx *= 10;
			numbCx += buff[i] - 48;
		}
		if (buff[3] == '-')
			numbCx = -numbCx;
		for (int i = 9; i < 13; ++i)
		{
			numbCy *= 10;
			numbCy += buff[i] - 48;
		}
		if (buff[8] == '-')
			numbCy = -numbCy;
		for (int i = 13; i < 17; ++i)
		{
			radX *= 10;
			radX += buff[i] - 48;
		}
		for (int i = 17; i < 21; ++i)
		{
			radY *= 10;
			radY += buff[i] - 48;
		}
		CalculateCirclePoints(numbCx, numbCy, radX, radY);
		moveSteppers = false;
	}
	if (stX->GetSteps() <= 0 && !exX)
	{
		if (xCounter >= res - 1)
			exX = true;
		stX->SetSteps(points[xCounter].X, points[xCounter].leftX);
		stX->SetSpeed(points[xCounter].xSpeed);
		xCounter++;
	}
	if (stY->GetSteps() <= 0 && !exY)
	{
		if (yCounter >= res - 1)
			exY = true;
		stY->SetSteps(points[yCounter].Y, points[yCounter].leftY);
		stY->SetSpeed(points[yCounter].ySpeed);
		yCounter++;
	}
	if (exX && exY)
		SendMessage(6);
}

void CPU::Circle2(int radX, int radY, float alpha)
{
	if (moveSteppers)
	{
		moveSteppers = false;
		exX = false;
		exY = false;
		xCounter = 0;
		yCounter = 0;
		CalculateCirclePointsA(radX, radY,alpha);
	}
	if (stX->GetSteps() <= 0 && !exX)
	{
		if (xCounter >= res - 1)
			exX = true;
		stX->SetSteps(points[xCounter].X, points[xCounter].leftX);
		stX->SetSpeed(points[xCounter].xSpeed);
		xCounter++;
	}
	if (stY->GetSteps() <= 0 && !exY)
	{
		if (yCounter >= res - 1)
			exY = true;
		stY->SetSteps(points[yCounter].Y, points[yCounter].leftY);
		stY->SetSpeed(points[yCounter].ySpeed);
		yCounter++;
	}
	if (exX && exY && stY->GetSteps() == 0 && stX->GetSteps() == 0)
		SendMessage(6);
}

int CPU::Round(float number)
{
	return ((number - (int)number) >= 0.5f) ? (int)number + 1 : (int)number;
}

void CPU::Test()
{
	if (moveSteppers)
	{
		moveSteppers = false;
		stZ->SetSpeed(1.5f);
		stZ->SetSteps(500, false);
	}
	if (stZ->GetSteps() == 0)
		SendMessage(10);
}
void CPU::Test2()
{
	if (moveSteppers)
	{
		moveSteppers = false;
		stZ->SetSpeed(1.5f);
		stZ->SetSteps(500, true);
	}
	if (stZ->GetSteps() == 0)
		SendMessage(10);
}