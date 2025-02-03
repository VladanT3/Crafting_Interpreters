#ifndef clox_object_h
#define clox_object_h

#include "common.h"
#include "value.h"
#include "chunk.h"
#include "table.h"

#define OBJ_TYPE(value) (AS_OBJ(value)->type)
#define IS_STRING(value) isObjType(value, OBJ_STRING)
#define AS_STRING(value) ((ObjString*)AS_OBJ(value))
#define AS_CSTRING(value) (((ObjString*)AS_OBJ(value))->chars)
#define IS_FUNCTION(value) isObjType(value, OBJ_FUNCTION)
#define AS_FUNCTION(value) ((ObjFunction*)AS_OBJ(value))
#define IS_NATIVE(value) isObjType(value, OBJ_NATIVE)
#define AS_NATIVE(value) (((ObjNative*)AS_OBJ(value))->function)
#define IS_CLOSURE(value) isObjType(value, OBJ_CLOSURE)
#define AS_CLOSURE(value) ((ObjClosure*)AS_OBJ(value))
#define IS_CLASS(value) isObjType(value, OBJ_CLASS)
#define AS_CLASS(value) ((ObjClass*)AS_OBJ(value))
#define IS_INSTANCE(value) isObjType(value, OBJ_INSTANCE)
#define AS_INSTANCE(value) ((ObjInstance*)AS_OBJ(value))

typedef enum {
	OBJ_CLASS,
	OBJ_CLOSURE,
	OBJ_FUNCTION,
	OBJ_INSTANCE,
	OBJ_NATIVE,
	OBJ_STRING,
	OBJ_UPVALUE
} ObjType;

struct Obj {
	ObjType type;
	struct Obj* next;
	bool is_marked;
};

typedef struct {
	Obj obj;
	int arity;
	Chunk chunk;
	ObjString* name;
	int upvalue_count;
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

typedef struct ObjUpvalue {
	Obj obj;
	Value* location;
	struct ObjUpvalue* next;
	Value closed;
} ObjUpvalue;

typedef struct {
	Obj obj;
	ObjFunction* function;
	ObjUpvalue** upvalues;
	int upvalue_count;
} ObjClosure;

typedef struct {
	Obj obj;
	ObjString* name;
} ObjClass;

typedef struct {
	Obj obj;
	ObjClass* klass;
	Table fields;
} ObjInstance;

ObjString* copyString(const char* chars, int length);
void printObject(Value value);
ObjString* takeString(char* chars, int length);
ObjFunction* newFunction();
ObjNative* newNative(NativeFn function);
ObjClosure* newClosure(ObjFunction * function);
ObjUpvalue* newUpvalue(Value * slot);
ObjClass* newClass(ObjString * name);
ObjInstance* newInstance(ObjClass * klass);

static inline bool isObjType(Value value, ObjType type) {
	return IS_OBJ(value) && OBJ_TYPE(value) == type;	//change after && to (AS_OBJ(value)->type) if it eats shit
}

#endif
