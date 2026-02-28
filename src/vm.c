#include "vm.h"

luna_VM luna_CreateVM(luna_State state)
{
    luna_VM vm = {.state = state};
    LUNA_POOL_INIT(vm.strings, luna_String, LUNA_MAX_OBJ_POOL_SIZE);
    LUNA_POOL_INIT(vm.tables, luna_Table, LUNA_MAX_OBJ_POOL_SIZE);
    LUNA_POOL_INIT(vm.funcs, luna_Func, LUNA_MAX_OBJ_POOL_SIZE);
    return vm;
}
void luna_DestroyVM(luna_VM *vm)
{
    lunaVM_Clear(vm);
    free(vm->strings.data);
    free(vm->strings.header.next_free);
    free(vm->tables.data);
    free(vm->tables.header.next_free);
    free(vm->funcs.data);
    free(vm->funcs.header.next_free);
    memset(vm, 0, sizeof(luna_VM));
}
void lunaVM_Clear(luna_VM *vm)
{
    for (luna_UInt i = 0; i < vm->tables.header.capacity; i++) {
        luna_Table *t = &vm->tables.data[i];
        if (t->data != NULL)
            lunaTable_Free(t);
    }
    for (luna_UInt i = 0; i < vm->strings.header.capacity; i++) {
        luna_String *s = &vm->strings.data[i];
        if (s->data != NULL)
            lunaString_Free(s);
    }
    for (luna_UInt i = 0; i < vm->funcs.header.capacity; i++) {
        luna_Func *f = &vm->funcs.data[i];
        if (f->bytecode != NULL)
            lunaFunc_Free(f);
    }
    LUNA_POOL_RESET(vm->strings, vm->state.max_ObjPoolSize);
    LUNA_POOL_RESET(vm->tables, vm->state.max_ObjPoolSize);
    LUNA_POOL_RESET(vm->funcs, vm->state.max_ObjPoolSize);
}
// void lunaVM_GC(luna_VM *vm)
// {
//     LUNA_POOL_GC(vm->tables, luna_Table, lunaTable_Free);
//     LUNA_POOL_GC(vm->strings, luna_String, lunaString_Free);
//     LUNA_POOL_GC(vm->funcs, luna_Func, lunaFunc_Free);
// }
