#ifndef REI_TYPE_TYPE_H
#define REI_TYPE_TYPE_H

#include "str.h"

typedef struct ReiTypeInfo ReiTypeInfo;
C_TEMPLATE_DECL_VECTOR(, rei, Rei, TypeInfoPtrVector, ReiTypeInfo*)
C_TEMPLATE_DEFN_VECTOR(, rei, Rei, TypeInfoPtrVector, ReiTypeInfo*)

typedef struct ReiTypeTable ReiTypeTable;

typedef enum ReiTypeCategory
{
    REI_TYPE_CATEGORY_NONE,
    REI_TYPE_CATEGORY_CLASS,
    REI_TYPE_CATEGORY_META,
    REI_TYPE_CATEGORY_TRAIT,
    REI_TYPE_CATEGORY_FUNCTION,
    REI_TYPE_CATEGORY_FIELD,
    REI_TYPE_CATEGORY_METHOD,
    REI_TYPE_CATEGORY_FORMAL,
    REI_TYPE_CATEGORY_GENERIC,
    REI_TYPE_CATEGORY_ALIAS,
    REI_TYPE_CATEGORY_VOID
} ReiTypeCategory;

typedef struct ReiTypeInfo
{
    int id;
    ReiTypeCategory category;
    ReiString name;
} ReiTypeInfo;
typedef struct ReiBehaviorTypeInfo
{
    ReiTypeInfo baseType;
    ReiTypeInfo* superType;
    ReiTypeInfoPtrVector* traitTypes;
    ReiTypeInfoPtrVector* formalTypeParams;
    ReiTypeTable* fields;
    ReiTypeTable* methods;
} ReiBehaviorTypeInfo;

#endif
