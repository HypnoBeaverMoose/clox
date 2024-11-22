#ifndef clox_object_h
#define clox_object_h

#include "common.h"
#include "value.h"

#define OBJ_TYPE(value) (AS_OBJ(value)->type)
#define IS_STRING(value) isObjType(value, OBJ_STRING)

#define AS_STRING(value) ((ObjString *)AS_OBJ(value))

#define IS_EXTERNAL(string) (string->externalChars != NULL)
#define EXTERNAL_STRING(string) (string->externalChars)
#define INTERNAL_STRING(string) ((char*)(string + offsetof(ObjString, internalChars)))

#define GET_CSTRING(string) (IS_EXTERNAL(string) ? EXTERNAL_STRING(string) : INTERNAL_STRING(string))

#define AS_CSTRING(value) (GET_CSTRING(AS_STRING(value)))

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

static inline bool isObjType(Value value, ObjType type)
{
    return IS_OBJ(value) && AS_OBJ(value)->type == type;
}

ObjString *copyString(const char *chars, int length);

ObjString *takeString(char *chars, int length);

void printObject(Value value);

#endif