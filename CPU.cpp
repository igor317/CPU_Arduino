#include"CPU.h"

CPU::CPU()
{
	stY = new Stepper(2, 3, 4, 5, A0, A2);
	stX = new Stepper(6, 7, 8, 9, A1, A3);
	i = 0;
	readstate = 0;
}

void CPU::Run()
{
	stY->Run();
	stX->Run();
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
			Serial.print(xcount);
			Serial.print(" ");
			Serial.print("Y moved on ");
			Serial.print(ycount);
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
		case 5: // TEST
			Serial.print(startSymbol);
			Serial.print("test");
			Serial.print(endSymbol);
			break;
		case 6: // TEST
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

		if (buff[0] == 't' && buff[1] == 'e' && buff[2] == 's' && buff[3] == 't' && buff[4] == '1')
			TestCommand(-1000, -1000, -500, 500,100);

		if (buff[0] == 't' && buff[1] == 'e' && buff[2] == 's' && buff[3] == 't' && buff[4] == '2')
			TestCircle(50, 100);
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

void CPU::CaclulateSplinePoints(int x1, int y1, int x2, int y2, int res)
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
		x = (2 * t - 2 * t * t) * x1 + t * t * x2;
		y = (2 * t - 2 * t * t) * y1 + t * t * y2;
		xt = (2 * g - 2 * g * g) * x1 + g * g * x2;
		yt = (2 * g - 2 * g * g) * y1 + g * g * y2;

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

void CPU::CalculateCirclePoints(int rad, int res)
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
	for (int i = 0; i < res; ++i)
	{
		t += delta;
		g = t - delta;
		x = rad * cos(t);
		y = rad * sin(t);
		xt = rad * cos(g);
		yt = rad * sin(g);
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

		xcount = numbX;
		ycount = numbY;
		CalculateSpeed(1,numbX, numbY);
		stX->SetSpeed(xSpeed);
		stY->SetSpeed(ySpeed);
		stX->SetSteps(xcount, lX);
		stY->SetSteps(ycount, lY);
	}
	else
	{
		if (stX->GetSteps() == 0 && stY->GetSteps() == 0)
			SendMessage(0);
	}
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
	{
		exX = true;
	}

	if (!stY->GetZeroPos() && !exY)
	{
		stY->SetSpeed(1);
		stY->SetSteps(5000, true);
	}
	else
	{
		exY = true;
	}
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
	SendMessage(2);
}

void CPU::TestCommand(int x1, int y1, int x2, int y2, int res)
{
	if (moveSteppers)
	{
		resetC = false;
		resetD = false;
		cX = 0;
		cY = 0;
		CaclulateSplinePoints(x1, y1, x2, y2, res);
		moveSteppers = false;
	}
	if (stX->GetSteps() <= 0 && !resetC)
	{
		if (cX >= res - 1)
			resetC = true;
		stX->SetSteps(points[cX].X, points[cX].leftX);
		stX->SetSpeed(points[cX].xSpeed);
		cX++;
	}
	if (stY->GetSteps() <= 0 && !resetD)
	{
		if (cY >= res - 1)
			resetD = true;
		stY->SetSteps(points[cY].Y, points[cY].leftY);
		stY->SetSpeed(points[cY].ySpeed);
		cY++;
	}
	if (resetC && resetD)
	{
		SendMessage(6);
	}
}

void CPU::TestCircle(int rad, int res)
{
	if (moveSteppers)
	{
		resetC = false;
		resetD = false;
		cX = 0;
		cY = 0;
		CalculateCirclePoints(rad, res);
		moveSteppers = false;
	}
	if (stX->GetSteps() <= 0 && !resetC)
	{
		if (cX >= res - 1)
			resetC = true;
		stX->SetSteps(points[cX].X, points[cX].leftX);
		stX->SetSpeed(points[cX].xSpeed);
		cX++;
	}
	if (stY->GetSteps() <= 0 && !resetD)
	{
		if (cY >= res - 1)
			resetD = true;
		stY->SetSteps(points[cY].Y, points[cY].leftY);
		stY->SetSpeed(points[cY].ySpeed);
		cY++;
	}
	if (resetC && resetD)
	{
		SendMessage(6);

	}
}

int CPU::Round(float number)
{
	return ((number - (int)number) >= 0.5f) ? (int)number + 1 : (int)number;
}
