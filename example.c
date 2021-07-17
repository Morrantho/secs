#define ENT_MAX 4096
#define SYS_MAX 128
#define NCOMPONENTS 5
#define X(A,B,C,D)
#define COMPONENTS\
	X(POINT,point,points,sizeof(point))\
	X(EDGE,edge,edges,sizeof(edge))\
	X(POLY,poly,polys,sizeof(poly))\
	X(PHYS,phys,physs,sizeof(phys))\
	X(COLOR,color,colors,sizeof(color))
/* The lib defines an entity the same way, but lowercase. */	
typedef unsigned long long ENTITY;

typedef struct point
{
	int x;
	int y;
	int ox;//old x
	int oy;//old y
}point;

typedef struct edge
{
	ENTITY a;
	ENTITY b;
}edge;

#define POINT_MAX 1024
#define EDGE_MAX POINT_MAX
typedef struct poly
{
	unsigned short n_points;
	unsigned short n_edges;
	ENTITY points[POINT_MAX];
	ENTITY edges[EDGE_MAX];
}poly;

typedef struct phys
{
	int mass;
	int gravity;
	int friction;
}phys;

typedef struct color
{
	unsigned char r;
	unsigned char g;
	unsigned char b;
	unsigned char a;
}color;

/* 
	lib must be include after struct defs. your probably defining struct
	layouts in headers, so shouldn't be an issue.
*/
#include "secs.h"
#include <stdio.h>

ENTITY point_new(int x,int y)
{
	ENTITY E=entity_new();
	toggle_component(E,1<<POINT);
	toggle_component(E,1<<COLOR);
	toggle_component(E,1<<PHYS);
	
	point* P=get_component(E,POINT);
	P->x=x;
	P->y=y;
	P->ox=x;
	P->ox=y;

	color* C=get_component(E,COLOR);
	C->r=255;
	C->g=0;
	C->b=0;
	C->a=255;

	return E;
}

void point_tick(ENTITY E)
{
	point* P=get_component(E,POINT);
	// do verlet integration or something.
}

void point_render(ENTITY E)
{
	// rendering the point sounds like a good idea
}

ENTITY edge_new(ENTITY A,ENTITY B)
{
	ENTITY E=entity_new();
	toggle_component(E,1<<EDGE);
	toggle_component(E,1<<COLOR);

	edge* Edge=get_component(E,EDGE);
	Edge->a=A;
	Edge->b=B;

	color* C=get_component(E,COLOR);
	C->r=255;
	C->g=255;
	C->b=255;
	C->a=255;

	return E;	
}

void edge_tick(ENTITY E)
{
	edge* Edge=get_component(E,EDGE);
	point* P1=get_component(Edge->a,POINT);//get the first point's point
	point* P2=get_component(Edge->b,POINT);//get the second point's point
	//constain them or something
}

void edge_render(ENTITY E)
{
	//draw the edge
}

/* I hope you get the idea by now, i'm not writing a whole physics engine for you. */

void tick(int sims)
{
	for(;sims>0;sims--)
		for(int I=0;I<SYS_MAX;I++)
			for(int J=0;J<ENT_MAX;J++)
				ecs.systems[I].run(ecs.systems[I].entities[J]);
}

int main(int argc,char** argv)
{
	/* Make 2 points and attach an edge between them. */
	/* You can expand on this concept with polygons. */
	ENTITY P1=point_new(100,100);
	ENTITY P2=point_new(100,150);
	ENTITY E1=edge_new(P1,P2);

	tick(10);
	return 0;
}