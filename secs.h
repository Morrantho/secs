#ifndef __SECS_H__
#define __SECS_H__
#define CMP_MAX 64
#include <stdarg.h>
typedef unsigned long long entity,mask;
typedef void (*fn)(entity);
#ifdef  X
#undef  X
#define X(A,B,C,D) A,
#endif/*X*/
typedef enum component{COMPONENTS}component;
typedef struct system
{
	entity entities[ENT_MAX];
	mask mask;
	fn run;
}system;
struct
{
	entity entities[ENT_MAX];
	unsigned short structs[NCOMPONENTS]; /* Struct sizes. */
	#undef  X
	#define X(A,B,C,D) B C[ENT_MAX];
	COMPONENTS /* Component data. */
	system systems[SYS_MAX];
}ecs= /* Anon handle */
{
	.entities={0},
	#undef  X
	#define X(A,B,C,D) D,
	.structs={COMPONENTS}, /* Expand struct sizes. */
	.systems={0}
};
unsigned long long entity_new()
{
	for(unsigned long long I=0;I<ENT_MAX;I++) if(!ecs.entities[I]) return I;
	return ENT_MAX+1;/* force them to overflow if they use it. */
}
unsigned char system_new(unsigned long long M,fn F)
{
	for(unsigned long long I=0;I<SYS_MAX;I++)
	{
		if(!ecs.systems[I].mask)
		{
			ecs.systems[I].mask=M;
			ecs.systems[I].run=F;
			return 1;
		}
	}
	return 0;
}
void match_systems(entity E)
{
	unsigned long long EM=ecs.entities[E];
	for(unsigned long long I=0;I<SYS_MAX;I++)
	{
		unsigned long long SM=ecs.systems[I].mask;		
		ecs.systems[I].entities[E]=SM&EM==SM;
	}
}
unsigned char has_component(entity E,component C)
{
	return ecs.entities[E]&C==ecs.entities[E];
}
void toggle_component(entity E,component C)
{
	ecs.entities[E]&=~(1<<C);
	match_systems(E);
}
void* get_component(entity E,component C)
{
	unsigned short S=ecs.structs[C];//Struct size.
	unsigned long long A=(unsigned long long)(ecs.structs+NCOMPONENTS);//start of components / end of sizes address.
	for(unsigned char I=0;I<C;I++) A+=ecs.structs[I]*ENT_MAX;//skip by struct arrays
	return (void*)A+S*E;//index into the right component
}
#endif/*__SECS_H__*/