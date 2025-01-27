#ifndef clox_object_h
#define clox_object_h

#include "common.h"
#include "value.h"
#include "chunk.h"

#define OBJ_TYPE(value) (AS_OBJ(value)->type)
#define IS_STRING(value) isObjType(value, OBJ_STRING)
#define AS_STRING(value) ((ObjString*)AS_OBJ(value))
#define AS_CSTRING(value) (((ObjString*)AS_OBJ(value))->chars)
#define IS_FUNCTION(value) isObjType(value, OBJ_FUNCTION)
#define AS_FUNCTION(value) ((ObjFunction*)AS_OBJ(value))

typedef enum {
	OBJ_FUNCTION,
	OBJ_STRING
} ObjType;

struct Obj {
	ObjType type;
	struct Obj* next;
};

typedef struct {
	Obj obj;
	int arity;
	Chunk chunk;
	ObjString* name;
} ObjFunction;

struct ObjString {
	Obj obj;
	int length;
	char* chars;
	uint32_t hash;
};

ObjString* copyString(const char* chars, int length);
void printObject(Value value);
ObjString* takeString(char* chars, int length);
ObjFunction* newFunction();

static inline bool isObjType(Value value, ObjType type) {
	return IS_OBJ(value) && OBJ_TYPE(value) == type;	//change after && to (AS_OBJ(value)->type) if it eats shit
}

#endif
