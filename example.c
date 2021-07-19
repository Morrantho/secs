#include <stdio.h>
/******************************************************************************
 * 1. Tell the lib how many entities and systems we need.
 * 2. Create a blank X macro, followed by a COMPONENTS define. The first arg
 * to X is the enum name you want to give your component. The second arg is
 * the type name of a struct placed in the anonymous "ecs" struct. The third
 * arg is the member name to place in the anonymous "ecs" struct.
******************************************************************************/
#define ENT_MAX 4
#define SYS_MAX 4
#define X(A,B,C)
#define COMPONENTS\
	X(CMP_POINT,Point,points)\
	X(CMP_EDGE,Edge,edges)\
	X(CMP_POLY,Poly,polys)\
	X(CMP_PHYS,Phys,phys)\
	X(CMP_COLOR,Color,colors)

typedef unsigned long long Entity; /*Yes, that's really all it is.*/

typedef struct Point
{
	int x;
	int y;
	int ox;//old x
	int oy;//old y
}Point;

typedef struct Edge
{
	Entity a;
	Entity b;
}Edge;

#define CMP_POINT_MAX 1024
#define CMP_EDGE_MAX CMP_POINT_MAX

typedef struct Poly
{
	unsigned short nPoints;
	unsigned short nEdges;
	Entity points[CMP_POINT_MAX];
	Entity edges[CMP_EDGE_MAX];
}Poly;

typedef struct Phys
{
	int mass;
	int gravity;
	int friction;
}Phys;

typedef struct Color
{
	unsigned char r;
	unsigned char g;
	unsigned char b;
	unsigned char a;
}Color;

/* 
	The lib must be included after struct defs. You're probably defining struct
	layouts in headers, so shouldn't be an issue.
*/
#include "secs.h"

/*
	To create an entity, we call EntityNew(), which returns us an index. 
	We then add components to it using ToggleComponent().
	We then retrieve those components by indexing into the ecs struct
	using the member name you supplied as the third arg to the X macro,
	and the newly created entity id. From here, you can change anything
	you like about the entity.
*/

Entity PointNew(int x,int y)
{
	Entity E=EntityNew();
	ToggleComponent(E,CMP_POINT);
	ToggleComponent(E,CMP_PHYS);
	ToggleComponent(E,CMP_COLOR);

	Point* point=&ecs.points[E];
	point->x=x;
	point->y=y;
	point->ox=x;
	point->ox=y;

	Phys* phys=&ecs.phys[E];
	phys->mass=1;
	phys->gravity=1;
	phys->friction=1;

	Color* color=&ecs.colors[E];
	color->r=255;
	color->g=0;
	color->b=0;
	color->a=255;

	return E;
}

/*
	To create a system, you merely create a void function with an
	unsigned long long as its first parameter. This is the entity
	that the system is currently operating on.
*/

void PointTick(Entity E)
{
	Point* point=&ecs.points[E];
	Phys* phys=&ecs.phys[E];
	// do verlet integration or something.
	printf("PointTick: %d\n",E);
}

void PointRender(Entity E)
{
	Point* point=&ecs.points[E];
	Color* color=&ecs.colors[E];
	// rendering the point sounds like a good idea
	printf("PointRender: %d\n",E);
}

Entity EdgeNew(Entity A,Entity B)
{
	Entity E=EntityNew();
	ToggleComponent(E,CMP_EDGE);
	ToggleComponent(E,CMP_COLOR);

	Edge* edge=&ecs.edges[E];
	edge->a=A;
	edge->b=B;

	Color* color=&ecs.colors[E];
	color->r=255;
	color->g=255;
	color->b=255;
	color->a=255;

	return E;	
}

void EdgeTick(Entity E)
{
	Edge* edge=&ecs.edges[E];
	Point* A=&ecs.points[edge->a];//get the first point's point
	Point* B=&ecs.points[edge->b];//get the second point's point
	//constain them or something
	printf("EdgeTick: %d PointA:%d PointB:%d\n",E,A,B);
}

void EdgeRender(Entity E)
{
	Edge* edge=&ecs.edges[E];
	Color* color=&ecs.colors[E];
	//draw the edge
	printf("EdgeRender: %d\n",E);
}

/*
	To create our systems, we call SystemNew, supplying the
	components we wish to operate on and what an entity must consist of
	in order to execute. We then pass the callback we wish to execute
	when the time comes. The order in which you call SystemNew() is the
	order in which the systems will fire. Therefore, placing a render
	system before an input system is probably a bad idea.
*/

int main(int argc,char** argv)
{
	SystemNew((1<<CMP_POINT)|(1<<CMP_PHYS),PointTick);//9
	SystemNew((1<<CMP_EDGE),EdgeTick);//2
	SystemNew((1<<CMP_EDGE)|(1<<CMP_COLOR),EdgeRender);//18
	SystemNew((1<<CMP_POINT)|(1<<CMP_COLOR),PointRender);//17
	
	Entity P1=PointNew(200,200);//25
	Entity P2=PointNew(232,200);//25
	Entity E1=EdgeNew(P1,P2);//18

	/*
		Do your game loop or whatever. SystemTick will handle passing you each
		of its entities iteratively, so you operate on one at a time.
	*/
	int sims=10;
	for(;sims;sims--)
	{
		SystemTick();
	}
	return 0;
}