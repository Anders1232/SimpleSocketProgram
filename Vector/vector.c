struct vector
{
	void *elements;
	int elementSize;
	int numberOfElements;
	int capacity;
};
typedef vector Vector;

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

void Resize(Vector * vec)
{
	vec->capacity= 2*vec->capacity +1;
	vec->elements= realloc(2 * vec->elementSize + vec->elementSize);
	if(NULL == vec-> elements)
	{
		fprintf("[ERROR] Memory reallocation error\n");
		exit(1);
	}
}

int AppendCopy(Vector *vec, void* element)
{
	if(vec->numberOfElements+1 == vec->capacity)
	{
		Resize(vec);
	}
	
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

