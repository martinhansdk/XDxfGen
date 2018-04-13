#include "stdafx.h"

#include <cstdlib>
#include <cstdio>

#include <iostream>
#include <ios>

#include <vector>

#include "xdxfgen.h"


int main(int argc, char* argv[])
{
	std::printf("Testing XDxfGen library\n");
	std::printf("Usage: dxfTest [<filename>]\n");

	std::string filename;
	if (argc <= 1)
	{
		// default filename
		filename = std::string("test.dxf");
	}
	else
	{
		filename = std::string(argv[1]);
	}

	// Write some awesome DXF figures :D
	XDxfGen<float> dxf;
	dxf.begin(filename);
	
	// Put some cicles
	dxf.circle(100,100,100, 0, 0, 1);
	dxf.circle(20,200-64,145, 3, 0, 2);
	dxf.circle(20,64,145);

	dxf.line(35,75,200-35,75);

	double arc_r = (200-35-35)/2.0;
	dxf.arc(100,75,arc_r,180,360);

	// Form some trapezoid
	std::vector<float> vx;
	std::vector<float> vy;

	vx.push_back(0);
	vy.push_back(0);

	vx.push_back(0);
	vy.push_back(200);

	vx.push_back(300);
	vy.push_back(200);

	vx.push_back(300);
	vy.push_back(0);

	// And draw it onto a layer 2 and width = 10 units
	dxf.polyline(vx,vy,10,2);

	// Test some unfinished functions
	//dxf.ellipse(0,0,10,10,1);

	bool result = dxf.end();

	if (!result)
		std::printf("Output finished. There were errors.\n");
	else
		std::printf("Output finished. Successfully.\n");


	std::system("pause");
	return result;
}
