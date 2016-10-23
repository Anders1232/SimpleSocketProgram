#ifndef VECTOR_H
#define VECTOR_H

struct vector
{
	void *elements;
	int elementSize;
	int numberOfElements;
	int capacity;
};
typedef vector Vector;

Vector* NewVector(int elementSize);
int AppendCopy(Vector *vec, void* element);
void* GetElement(Vector *vec, int position);
#endif
