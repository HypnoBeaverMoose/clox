#include <stdio.h>
#include <string.h>

#include "memory.h"
#include "object.h"
#include "value.h"
#include "vm.h"

#define ALLOCATE_OBJ(type, objectType) \
    (type *)allocateObject(sizeof(type), objectType)

static Obj *allocateObject(int size, ObjType type)
{
    Obj *obj = (Obj *)reallocate(NULL, 0, size);
    obj->type = type;

    obj->next = vm.objects;
    vm.objects = obj;

    return obj;
}

ObjString *copyString(const char *chars, int length)
{
    ObjString *string = reallocate(NULL, 0, sizeof(ObjString) + length * sizeof(char));
    string->externalChars = NULL;
    string->length = length;
    size_t offset = offsetof(ObjString, internalChars);
    memcpy(string + offset, chars, length);
    string->internalChars[length + 1] = '\0';

    return string;
}

ObjString *takeString(char *chars, int length)
{
    ObjString *string = ALLOCATE_OBJ(ObjString, OBJ_STRING);
    string->length = length;
    string->externalChars = chars;
    return string;
}

void printObject(Value value)
{
    switch (OBJ_TYPE(value))
    {
    case OBJ_STRING:
        printf("%s", AS_CSTRING(value));
        break;
    }
}