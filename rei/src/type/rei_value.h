#ifndef REI_TYPE_VALUE_H
#define REI_TYPE_VALUE_H

#include "rei_internal.h"
#include <stdbool.h>
#include <stdint.h>
#include REI_C_TEMPLATE_LIB_CONTAINER_STR_H

typedef struct ReiObj           ReiObj;
typedef struct ReiObjString     ReiObjString;
typedef struct ReiObjList       ReiObjList;
typedef struct ReiObjMap        ReiObjMap;
typedef struct ReiObjRange      ReiObjRange;
typedef struct ReiObjModule     ReiObjModule;
typedef struct ReiObjFunction   ReiObjFunction;
typedef struct ReiObjClosure    ReiObjClosure;
typedef struct ReiObjMethodBind ReiObjMethodBind;
typedef struct ReiObjUpvalue    ReiObjUpvalue;
typedef struct ReiObjFiber      ReiObjFiber;
typedef struct ReiObjClass      ReiObjClass;
typedef struct ReiObjInstance   ReiObjInstance;

typedef enum ReiValueType
{
    REI_VALUE_TYPE_NIL,
    REI_VALUE_TYPE_BOOL,
    REI_VALUE_TYPE_INT,
    REI_VALUE_TYPE_FLOAT,
    REI_VALUE_TYPE_OBJ,
    REI_VALUE_TYPE_UNDEFINED,
    REI_VALUE_TYPE_MAX_COUNT
} ReiValueType;
typedef struct ReiValue
{
    ReiValueType type;
    union
    {
        bool vBool;
        int64_t vInt;
        double vFloat;
        ReiObj* pObj;
    } as;
} ReiValue;

#define REI_IS_NIL(value)       ((value).type == REI_VALUE_TYPE_NIL)
#define REI_IS_BOOL(value)      ((value).type == REI_VALUE_TYPE_BOOL)
#define REI_IS_INT(value)       ((value).type == REI_VALUE_TYPE_INT)
#define REI_IS_FLOAT(value)     ((value).type == REI_VALUE_TYPE_FLOAT)
#define REI_IS_NUMBER(value)    ((value).type == REI_VALUE_TYPE_INT || (value).type == REI_VALUE_TYPE_FLOAT)
#define REI_IS_OBJ(value)       ((value).type == REI_VALUE_TYPE_OBJ)
#define REI_IS_UNDEFINED(value) ((value).type == REI_VALUE_TYPE_UNDEFINED)

#define REI_AS_BOOL(value)      ((value).as.vBool)
#define REI_AS_INT(value)       ((value).as.vInt)
#define REI_AS_FLOAT(value)     ((value).as.vFloat)
#define REI_AS_NUMBER(value)    ((value).as.vFloat)
#define REI_AS_OBJ(value)       ((value).as.pObj)

#define REI_MK_NIL          ((ReiValue){REI_VALUE_TYPE_NIL,     {.vBool = false}})
#define REI_MK_BOOL(v)      ((ReiValue){REI_VALUE_TYPE_BOOL,    {.vBool = v}})
#define REI_MK_INT(v)       ((ReiValue){REI_VALUE_TYPE_INT,     {.vInt = v}})
#define REI_MK_FLOAT(v)     ((ReiValue){REI_VALUE_TYPE_FLOAT,   {.vFloat = v}})
#define REI_MK_NUMBER(v)    ((ReiValue){REI_VALUE_TYPE_FLOAT,   {.vFloat = v}})
#define REI_MK_OBJ(p)       ((ReiValue){REI_VALUE_TYPE_OBJ,     {.pObj = (ReiObj*)p}})

C_TEMPLATE_DECL_VECTOR(, rei, Rei, ByteBuffer, ReiBytecode)
C_TEMPLATE_DEFN_VECTOR(, rei, Rei, ByteBuffer, ReiBytecode)
C_TEMPLATE_DECL_VECTOR(, rei, Rei, UIntBuffer, uint32_t)
C_TEMPLATE_DEFN_VECTOR(, rei, Rei, UIntBuffer, uint32_t)
C_TEMPLATE_DECL_VECTOR(, rei, Rei, ValueBuffer, ReiValue)
C_TEMPLATE_DEFN_VECTOR(, rei, Rei, ValueBuffer, ReiValue)
C_TEMPLATE_DECL_STRING(, rei, Rei, String)
C_TEMPLATE_DEFN_STRING(, rei, Rei, String)
typedef ReiObjClosure* ReiObjClosurePtr;
C_TEMPLATE_DECL_VECTOR(, rei, Rei, ObjClosurePtrBuffer, ReiObjClosurePtr)
C_TEMPLATE_DEFN_VECTOR(, rei, Rei, ObjClosurePtrBuffer, ReiObjClosurePtr)

typedef struct ReiMapEntry
{
  ReiValue key;
  ReiValue value;
} ReiMapEntry;
typedef struct ReiMap
{
    ReiMapEntry* entrys;
    size_t size;
    size_t capacity;
} ReiMap;
typedef struct ReiFunction
{
    uint32_t stackSize;
    ReiByteBuffer_T opcodes;
    ReiUIntBuffer_T oplines;
} ReiFunction;
typedef struct ReiCallFrame
{
  const uint8_t* ip;
  const ReiObjClosure* closure;
  ReiValue* rbp;
  ReiValue self;
} ReiCallFrame;
typedef enum ReiFiberState
{
    REI_FIBER_STATE_YIELDED,
    REI_FIBER_STATE_RUNING,
    REI_FIBER_STATE_DEAD,
    REI_FIBER_STATE_MAX_COUNT
} ReiFiberState;

typedef enum ReiObjType
{
    REI_OBJ_TYPE_STRING,
    REI_OBJ_TYPE_LIST,
    REI_OBJ_TYPE_MAP,
    REI_OBJ_TYPE_RANGE,
    REI_OBJ_TYPE_MODULE,
    REI_OBJ_TYPE_FUNCTION,
    REI_OBJ_TYPE_CLOSURE,
    REI_OBJ_TYPE_METHOD_BIND,
    REI_OBJ_TYPE_UPVALUE,
    REI_OBJ_TYPE_FIBER,
    REI_OBJ_TYPE_CLASS,
    REI_OBJ_TYPE_INSTANCE,
    REI_OBJ_TYPE_MAX_COUNT
} ReiObjType;
typedef struct ReiObj
{
    ReiObjType type;
    bool isMarked;
    ReiObj* next;
} ReiObj;
typedef struct ReiObjString
{
    ReiObj obj;
    ReiString_T vector;
    uint32_t hash;
} ReiObjString;
typedef struct ReiObjList
{
    ReiObj obj;
    ReiValueBuffer_T vector;
} ReiObjList;
typedef struct ReiObjMap
{
    ReiObj obj;
    ReiMap map;
} ReiObjMap;
typedef struct ReiObjRange
{
    ReiObj obj;
    double from;
    double to;
} ReiObjRange;
typedef struct ReiObjModule
{
    ReiObj obj;
    ReiObjString* name;
    ReiObjString* path;
    ReiValueBuffer_T constants;
    ReiValueBuffer_T globals;
    ReiUIntBuffer_T globalNameIndexs;
    ReiObjClosure* body;
    bool initialized;
} ReiObjModule;
typedef struct ReiObjFunction
{
    ReiObj obj;
    ReiObjModule* owner;
    const char* name;
    bool isMethod;
    bool isNative;
    int argc;
    int upvaluec;
    union
    {
    ReiNativeFn native;
    ReiFunction func;
    };
} ReiObjFunction;
typedef struct ReiObjClosure
{
    ReiObj obj;
    ReiObjFunction* func;
    ReiObjUpvalue* upvalues;
} ReiObjClosure;
typedef struct ReiObjMethodBind
{
    ReiObj obj;
    ReiObjClosure* method;
    ReiValue instance;
} ReiObjMethodBind;
typedef struct ReiObjUpvalue
{
    ReiObj obj;
    ReiValue* ptr;
    ReiValue closed;
    ReiObjUpvalue* next;
} ReiObjUpvalue;
typedef struct ReiObjFiber
{
    ReiObj obj;
    ReiFiberState state;
    ReiObjClosure* closure;
    ReiValue* stack;
    uint32_t stackSize;
    ReiValue* rsp;
    ReiCallFrame callFrames;
    uint32_t callFramesSize;
    uint32_t callFramesCapacity;
    ReiObjUpvalue* openUpvalues;
    ReiValue* currCallFrameRbp;
    ReiValue self;
    ReiObjFiber* caller;
    ReiObjFiber* native;
    ReiString errorMsg;
} ReiObjFiber;
typedef struct ReiObjClass
{
    ReiObj obj;
    ReiObjClass* super;
    ReiObjModule* owner;
    ReiObjString* name;
    ReiValueType classOf;
    ReiObjClosure* constructor;
    ReiObjClosure* destructor;
    ReiObjClosurePtrBuffer_T methods;
    ReiObjMap* staticAttribs;
    ReiNewInstanceFn userNewFn;
    ReiDelInstanceFn userDelFn;
} ReiObjClass;
typedef struct ReiObjInstance
{
    ReiObj obj;
    ReiObjClass* clazz;
    ReiObjMap* attribs;
    void* user;
} ReiObjInstance;

#define IS_STRING(value)        (REI_IS_OBJ(value) && REI_AS_OBJ(value)->type == REI_OBJ_TYPE_STRING)
#define IS_LIST(value)          (REI_IS_OBJ(value) && REI_AS_OBJ(value)->type == REI_OBJ_TYPE_LIST)
#define IS_MAP(value)           (REI_IS_OBJ(value) && REI_AS_OBJ(value)->type == REI_OBJ_TYPE_MAP)
#define IS_RANGE(value)         (REI_IS_OBJ(value) && REI_AS_OBJ(value)->type == REI_OBJ_TYPE_RANGE)
#define IS_MODULE(value)        (REI_IS_OBJ(value) && REI_AS_OBJ(value)->type == REI_OBJ_TYPE_MODULE)
#define IS_FUNCTION(value)      (REI_IS_OBJ(value) && REI_AS_OBJ(value)->type == REI_OBJ_TYPE_FUNCTION)
#define IS_CLOSURE(value)       (REI_IS_OBJ(value) && REI_AS_OBJ(value)->type == REI_OBJ_TYPE_CLOSURE)
#define IS_METHOD_BIND(value)   (REI_IS_OBJ(value) && REI_AS_OBJ(value)->type == REI_OBJ_TYPE_METHOD_BIND)
#define IS_UPVALUE(value)       (REI_IS_OBJ(value) && REI_AS_OBJ(value)->type == REI_OBJ_TYPE_UPVALUE)
#define IS_FIBER(value)         (REI_IS_OBJ(value) && REI_AS_OBJ(value)->type == REI_OBJ_TYPE_FIBER)
#define IS_CLASS(value)         (REI_IS_OBJ(value) && REI_AS_OBJ(value)->type == REI_OBJ_TYPE_CLASS)
#define IS_INSTANCE(value)      (REI_IS_OBJ(value) && REI_AS_OBJ(value)->type == REI_OBJ_TYPE_INSTANCE)

#define AS_STRING(value)        ((ReiObjString*)REI_AS_OBJ(value))
#define AS_LIST(value)          ((ReiObjList*)REI_AS_OBJ(value))
#define AS_MAP(value)           ((ReiObjMap*)REI_AS_OBJ(value))
#define AS_RANGE(value)         ((ReiObjRange*)REI_AS_OBJ(value))
#define AS_MODULE(value)        ((ReiObjModule*)REI_AS_OBJ(value))
#define AS_FUNCTION(value)      ((ReiObjFunction*)REI_AS_OBJ(value))
#define AS_CLOSURE(value)       ((ReiObjClosure*)REI_AS_OBJ(value))
#define AS_METHOD_BIND(value)   ((ReiObjMethodBind*)REI_AS_OBJ(value))
#define AS_UPVALUE(value)       ((ReiObjUpvalue*)REI_AS_OBJ(value))
#define AS_FIBER(value)         ((ReiObjFiber*)REI_AS_OBJ(value))
#define AS_CLASS(value)         ((ReiObjClass*)REI_AS_OBJ(value))
#define AS_INSTANCE(value)      ((ReiObjInstance*)REI_AS_OBJ(value))

#endif
