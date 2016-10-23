#include"vector.h"
#include<stdio.h>

Vector* NewVector(int elementSize)
{
	Vector *ret= malloc(sizeof(Vector));
	if(NULL == ret)
	{
		fprintf(stderr, "[ERROR] Allocation Error\n");
		return NULL;
	}
	ret->elementSize= elementSize;
	ret->numberOfElements=0;
	ret->capacity= 0
	ret->elements= NULL;
	return ret;
}

static void Resize(Vector * vec)
{
	vec->capacity= 2 * (vec->capacity) + 1;
	vec->elements= realloc(vec->elements, 2 * vec->elementSize + vec->elementSize);
	if(NULL == vec-> elements)
	{
		fprintf("[ERROR] Memory reallocation error\n");
		exit(1);
	}
}

int AppendCopy(Vector *vec, void* element)
{
	if(vec->numberOfElements == vec->capacity)
	{
		Resize(vec);
	}
	char* newElementArea= ( ( char*) vec->elements) + (vec->elementSize)*(vec->numberOfElements);
	memcpy(newElementArea, element, vec->elementSize);
	(vec->numberOfElements)++;
	return (vec->numberOfElements)-1;
}

void* GetElement(Vector *vec, int position)
{
/*	while(position > capacity)
	{
		Resize(vec);
	}
*/
	if( (vec->numberOfElements) <= position)
	{
		fprintf(stderr, "[ERROR] Bad element access\n");
		return NULL;
	}
	return ((char*)elements)+ ( position*(vec->elementSize) );
}

