//main.cpp - 

#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <unistd.h>



char chs[] = { ' ', '.', ':', '+', '=', '%', '#', '@' };
char chFromFloat(double tone)
{
	tone = tone > 1.0f ? 1.0f : (tone < 0.0f ? 0.0f : tone);
	int index = (int)round(tone * 7.0f);
	return chs[index];
}
char chFromInt(int i)
{
	return chs[i%7];
}

const double viewTan = 1.0; //90deg FOV
const double colorPreScale = 25.0; //higher=dimmer
const double width = 60;
const double hWidth = width / 2;

struct Point
{
	double x, y;
};
struct Line
{
	Point start, end;
};
struct Camera
{
	Point position;
	double rotation;
};

void render(Camera c);
char renderPoint(Point p, Camera c);
Point transformPoint(Point p, Camera c);
Line transformLine(Line l, Camera c);
//TODO: Point getForwards(Camera c);

// -> Y
//+----+
//|    |
//|    +--+
//|  X     \.
//+---------+
Line walls[] = { 
	{ { +3.0, -3.0 }, { +3.0, +1.0 } }, //Top wall
	{ { +3.0, -3.0 }, { -1.0, -3.0 } }, //Left wall
	{ { -1.0, -3.0 }, { -1.0, +7.0 } }, //Bottom wall
	{ { +1.0, +5.0 }, { -1.0, +7.0 } }, //Diagonal wall
	{ { +1.0, +2.0 }, { +1.0, +5.0 } }, //Right top wall
	{ { +3.0, +2.0 }, { +1.0, +2.0 } }, //Top right wall
};


int main()
{
	Camera cam = { {0, 0}, 0 };

	while (1)
	{
		render(cam);
		cam.rotation += 0.025;
		printf("Rotation: %f\n", cam.rotation);

		usleep(50 * 1000);
	}
}


void render(Camera c)
{
	system("clear");
	for (int y = 0; y < width; y++)
	{
		for (int x = 0; x < width; x++)
		{
			char ch;
			Point p = { .x = (x-hWidth) / hWidth, .y= (hWidth-y) / hWidth };
			ch = renderPoint(p, c);
			printf("%c%c", ch, ch);
		}
	}
}

char renderPoint(Point p, Camera c)
{
	//coordinates:
	//x = screen horizontal [-1,1]
	//y = screen vertical [-1, 1]
	//z = world depth (position (dot) forwards)
	//w = world offset (position (dot) right)
	//v = point height (normally z)

	double lowestD = 10000.0;
	int lowestD_i = -1;
	size_t wallCount = sizeof(walls) / sizeof(Line);

	for (size_t i = 0; i < wallCount; i++)
	{
		Line wall = transformLine(walls[i], c);

		//ray:
		//rIncline = p.x * viewTan (forwards is 0)
		//rOrg = 0 (centered cam)
		//ray => w = rIncline*z + rOrg
		double rIncline = p.x * viewTan;
		double rOrg = 0;

		//wall:
		//wIncline = (e.y - s.y)/(e.x - s.x)
		//wOrg = s.y - (s.x)*wIncline
		//wall => w = wIncline*z + wOrg
		double wIncline = (wall.end.y - wall.start.y) / (wall.end.x - wall.start.x);
		double wOrg = wall.start.y - wall.start.x * wIncline;

		//Interception:
		//    wIncline*z + wOrg = rIncline*z + rOrg
		//<=> (wIncline - rIncline)*z = rOrg - wOrg
		//<=> z = (rOrg - wOrg)/(wIncline - rIncline)
		// => w = z*rIncline + rOrg
		double z = (rOrg - wOrg)/(wIncline - rIncline);
		double w = z*rIncline + rOrg;

		//printf("rInc: %.2f; rOrg: %.2f; wInc: %.2f; wOrg: %.2f\n", rIncline, rOrg, wIncline, wOrg);

		//Verify is within wall bounds
		if ((wall.start.y > w || w > wall.end.y) && (wall.start.y < w || w < wall.end.y))
			continue;

		if (z < 0) //behind camera
			continue;

		//Check is within wall vertically (all walls +1,-1 tall)
		if (fabs(p.y * viewTan * z) > 1)
			continue;

		//Scaled distance for color adjustment
		double d = sqrt(z*z + w*w);
		d *= colorPreScale;

		//Set if closer
		if (d < lowestD)
		{
			lowestD = d;
			lowestD_i = i;
		}
	}

	//return chFromInt(lowestD_i + 1);
	return chFromFloat(7.0 - log2(lowestD));
}

Point transformPoint(Point p, Camera c)
{
	//cos(rot), -sin(rot), 	offX
	//sin(rot), cos(rot), 	offY
	//0,		0,			1
	double rCos = cos(c.rotation), rSin = sin(c.rotation);

	double x = p.x - c.position.x;
	double y = p.y - c.position.y;

	double fx = rCos * x - rSin * y;
	double fy = rSin * x + rCos * y;

	return { fx, fy };
}

Line transformLine(Line l, Camera c)
{
	return { transformPoint(l.start, c), transformPoint(l.end, c) };
}
