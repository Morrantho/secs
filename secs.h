#ifndef __SECS_H__
#define __SECS_H__
#define CMP_MAX 64
#ifdef  X
#undef  X
#define X(A,B,C) A,
#endif/*X*/
enum Component{COMPONENTS};

struct System
{
	unsigned char entities[ENT_MAX];/*Bool array*/
	unsigned long long mask;
	void (*run)(unsigned long long);
};

struct
{
	unsigned long long entities[ENT_MAX];
	#undef  X
	#define X(A,B,C) B C[ENT_MAX];
	COMPONENTS
	struct System systems[SYS_MAX];
}ecs=
{
	.entities={0},
	.systems={0},
	#undef  X
	#define X(A,B,C) .C={0},
	COMPONENTS
};

unsigned long long EntityNew()
{
	for(unsigned long long I=0;I<ENT_MAX;I++) if(!ecs.entities[I]) return I;
	return ENT_MAX+1;
}

void SystemNew(unsigned long long M,void (*F)(unsigned long long))
{
	static int sys_id=0;
	ecs.systems[sys_id].mask=M;
	ecs.systems[sys_id].run=F;
	sys_id++;
}

void ToggleComponent(unsigned long long E,enum Component C)
{
	ecs.entities[E]^=1<<C;
	unsigned long long EM=ecs.entities[E];
	for(unsigned long long S=0;S<SYS_MAX;S++)
	{
		unsigned long long SM=ecs.systems[S].mask;
		unsigned long long R=EM&SM;
		ecs.systems[S].entities[E]=(R==EM||R==SM)&&(R)!=1;
	}
}

void SystemTick()
{
	for(int S=0;S<SYS_MAX;S++)
	{
		if(!ecs.systems[S].run) continue;
		for(int E=0;E<ENT_MAX;E++)
			if(ecs.systems[S].entities[E])
				ecs.systems[S].run(E);
	}
}
#endif/*__SECS_H__*/