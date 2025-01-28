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
#define IS_NATIVE(value) isObjType(value, OBJ_NATIVE)
#define AS_NATIVE(value) (((ObjNative*)AS_OBJ(value))->function)

typedef enum {
	OBJ_FUNCTION,
	OBJ_NATIVE,
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

typedef Value(*NativeFn) (int arg_count, Value * args);

typedef struct {
	Obj obj;
	NativeFn function;
} ObjNative;

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
ObjNative* newNative(NativeFn function);

static inline bool isObjType(Value value, ObjType type) {
	return IS_OBJ(value) && OBJ_TYPE(value) == type;	//change after && to (AS_OBJ(value)->type) if it eats shit
}

#endif
