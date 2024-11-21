#ifndef clox_object_h
#define clox_object_h

#include "common.h"
#include "value.h"

#define OBJ_TYPE(value) (AS_OBJ(value)->type)
#define IS_STRING(value) isObjType(value, OBJ_STRING)

#define GET_STRING_VALUE(string) (string->externalChars == NULL ? (char *)(string + offsetof(ObjString, internalChars)) : string->externalChars);
#define AS_STRING(value) ((ObjString *)AS_OBJ(value))
#define AS_CSTRING(value) (GetCString(AS_STRING(value)))

typedef enum
{
    OBJ_STRING
} ObjType;

struct Obj
{
    ObjType type;
    struct Obj *next;
};

struct ObjString
{
    Obj obj;
    int length;
    char *externalChars;
    char internalChars[];
};

static inline char *GetCString(ObjString* string)
{
    char *pointer = (char *)(string + offsetof(ObjString, internalChars));
    return string->externalChars == NULL ? pointer : string->externalChars;
}

static inline bool isObjType(Value value, ObjType type)
{
    return IS_OBJ(value) && AS_OBJ(value)->type == type;
}

ObjString *copyString(const char *chars, int length);

ObjString *takeString(char *chars, int length);

void printObject(Value value);

#endif