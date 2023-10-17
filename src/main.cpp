//main.cpp - 

#include <cstdio>
#include <cstdlib>
#include <cmath>



char chs[] = { ' ', '.', ':', '+', '=', '%', '#', '@' };
char chFromFloat(double tone)
{
	tone = tone > 1.0f ? 1.0f : (tone < 0.0f ? 0.0f : tone);
	int index = (int)round(tone * 7.0f);
	return chs[index];
}

double viewTan = 1.0; //90deg FOV

struct Point
{
	double x, y;
};
struct Line
{
	Point start, end;
};

char renderPoint(Point p);



int main()
{
	system("clear");
	for (int y = 0; y < 40; y++)
	{
		//int y = 0;
		for (int x = 0; x < 40; x++)
		{
			char ch;
			Point p = { .x = (x-20) / 20.0, .y= (y-20) / 20.0 };
			ch = renderPoint(p);
			printf("%c%c", ch, ch);
		}
	}
}


Line walls[] = { 
	{ { 1.2, -1.1 }, { 2.0, 0.1 } },
	{ { 1.9, 0.3 }, { 0.9, 0.8 } },	
};

char renderPoint(Point p)
{
	double lowestD = 10000.0;

	for (int i = 0; i < 2; i++)
	{
		Line wall = walls[i];
		//ray:
		//incline = p.x * viewTan (forwards is 0)
		//y_org = 0 (centered cam)
		//y = incline*x + y_org
		double rIncline = p.x * viewTan;
		double rOrg = 0;

		//wall:
		//incline = (e.y - s.y)/(e.x - s.x)
		//y_org = s.y - (s.x)*incline
		//wall => y = incline*x + y_org
		double wIncline = (wall.end.y - wall.start.y) / (wall.end.x - wall.start.x);
		double wOrg = wall.start.y - wall.start.x * wIncline;

		//Interception:
		//    wIncline*x + wOrg = rIncline*x + rOrg
		//<=> (wIncline - rIncline)*x = rOrg - wOrg
		//<=> x = (rOrg - wOrg)/(wIncline - rIncline)
		// => y = x*rIncline + rOrg
		double x = (rOrg - wOrg)/(wIncline - rIncline);
		double y = x*rIncline + rOrg;

		//printf("rInc: %.2f; rOrg: %.2f; wInc: %.2f; wOrg: %.2f\n", rIncline, rOrg, wIncline, wOrg);

		//Verify is within wall bounds
		if ((wall.start.y > y || y > wall.end.y) && (wall.start.y < y || y < wall.end.y))
			continue;

		if (x < 0) //behind camera
		{
			//printf("Clipped\n");
			continue;
		}

		//Distance
		double d = sqrt(x*x + y*y);

		//Check is within wall vertically (all walls +1,-1 tall)
		if (fabs(p.y * viewTan * d) > 1)
			continue;

		//scale distance for color adjustment
		double scale = 50.0;
		d *= scale;

		//Set if closer
		if (d < lowestD)
			lowestD = d;
	}

	return chFromFloat(7.0 - log2(lowestD));
}
