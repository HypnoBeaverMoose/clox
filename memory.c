#include <stdlib.h>

#include "memory.h"
#include "vm.h"

static void freeObject(Obj *object)
{
    switch (object->type)
    {
    case OBJ_STRING:
    {
        ObjString *string = (ObjString *)object;
        if (string->externalChars != NULL)
        {
            FREE_ARRAY(char, string->externalChars, string->length + 1);
            FREE(ObjString, object);
        }
        else
        {
            reallocate(object, sizeof(ObjString) + string->length * sizeof(char), 0);
        }
        break;
    }
    }
}

void *reallocate(void *pointer, size_t oldSize, size_t newSize)
{
    if (newSize == 0)
    {
        free(pointer);
        return NULL;
    }

    void *result = realloc(pointer, newSize);

    if (result == NULL)
    {
        exit(1);
    }

    return result;
}

void freeObjects()
{
    Obj *object = vm.objects;
    while (object != NULL)
    {
        Obj *next = object->next;
        freeObject(object);
        object = next;
    }
}