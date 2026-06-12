#ifndef C_TEMPLATE_CONTAINER_LIST_DOUBLY_H
#define C_TEMPLATE_CONTAINER_LIST_DOUBLY_H

#include "../allocator.h"
#include <stdbool.h>
#include <stddef.h>

#pragma region Dec
#define C_TEMPLATE_DECL_DLIST(ATTR, SPREFIX, LPREFIX, NAME, TYPE) \
	typedef struct LPREFIX##NAME##_T LPREFIX##NAME##_T; \
	typedef LPREFIX##NAME##_T* LPREFIX##NAME; \
    /* 内部类 */ \
	typedef struct LPREFIX##NAME##Node LPREFIX##NAME##Node; \
	typedef void(*LPREFIX##NAME##Signal)(const LPREFIX##NAME container, TYPE* element); \
	typedef bool(*LPREFIX##NAME##Equalor)(const TYPE* curr, const TYPE* goal); \
	typedef bool(*LPREFIX##NAME##Predicate)(const TYPE* curr); \
	typedef void(*LPREFIX##NAME##Visitor)(TYPE* curr); \
    /* 定义类 */ \
	typedef struct LPREFIX##NAME##Def LPREFIX##NAME##Def; \
	static inline LPREFIX##NAME##Def SPREFIX##Null##NAME##Def(void); \
	/* 创建销毁 */ \
	ATTR bool SPREFIX##NAME##Init(LPREFIX##NAME me, const LPREFIX##NAME##Def* def); \
	ATTR LPREFIX##NAME SPREFIX##NAME##Create(const LPREFIX##NAME##Def* def); \
	ATTR LPREFIX##NAME SPREFIX##NAME##Copy(const LPREFIX##NAME src); \
	ATTR void SPREFIX##NAME##CopyTo(LPREFIX##NAME me, const LPREFIX##NAME src); \
	ATTR LPREFIX##NAME SPREFIX##NAME##Move(const LPREFIX##NAME src); \
	ATTR void SPREFIX##NAME##MoveTo(LPREFIX##NAME me, const LPREFIX##NAME src); \
	ATTR void SPREFIX##NAME##Free(LPREFIX##NAME me); \
	ATTR void SPREFIX##NAME##Destroy(LPREFIX##NAME me); \
	/* 访问 */ \
	ATTR TYPE* SPREFIX##NAME##Front(LPREFIX##NAME me); \
	ATTR TYPE* SPREFIX##NAME##Back(LPREFIX##NAME me); \
	/* 属性 */ \
	ATTR size_t SPREFIX##NAME##Size(LPREFIX##NAME me); \
	ATTR bool SPREFIX##NAME##Empty(LPREFIX##NAME me); \
	/* 增删改 */ \
	ATTR size_t SPREFIX##NAME##PushFront(LPREFIX##NAME me, TYPE* element); \
	ATTR size_t SPREFIX##NAME##PushBack(LPREFIX##NAME me, TYPE* element); \
	ATTR void SPREFIX##NAME##PopFront(LPREFIX##NAME me); \
	ATTR void SPREFIX##NAME##PopBack(LPREFIX##NAME me); \
	ATTR void SPREFIX##NAME##InsertAfter(LPREFIX##NAME me, TYPE* index, TYPE* element); \
	ATTR void SPREFIX##NAME##InsertBefore(LPREFIX##NAME me, TYPE* index, TYPE* element); \
	ATTR void SPREFIX##NAME##Erase(LPREFIX##NAME me, TYPE* element); \
	ATTR TYPE* SPREFIX##NAME##Find(LPREFIX##NAME me, const TYPE* goal, LPREFIX##NAME##Equalor equalor); \
	ATTR bool SPREFIX##NAME##Contains(LPREFIX##NAME me, const TYPE* goal, LPREFIX##NAME##Equalor equalor); \
	/* 批处理 */ \
	ATTR void SPREFIX##NAME##Foreach(LPREFIX##NAME me, LPREFIX##NAME##Visitor visitor); \
	ATTR void SPREFIX##NAME##RForeach(LPREFIX##NAME me, LPREFIX##NAME##Visitor visitor); \
	ATTR void SPREFIX##NAME##RemoveIf(LPREFIX##NAME me, LPREFIX##NAME##Predicate predicate); \
	ATTR void SPREFIX##NAME##Clear(LPREFIX##NAME me); \
	/* 交换 */ \
	ATTR void SPREFIX##NAME##Swap(LPREFIX##NAME me, LPREFIX##NAME other);
#pragma endregion
#pragma region Def
#define C_TEMPLATE_DEFN_DLIST(ATTR, SPREFIX, LPREFIX, NAME, TYPE) \
	typedef struct LPREFIX##NAME##Node \
	{ \
		TYPE* next; \
		TYPE* prev; \
	} LPREFIX##NAME##Node; \
	typedef struct LPREFIX##NAME##_T \
	{ \
		TYPE* head; \
		TYPE* tail; \
		size_t size; \
	} LPREFIX##NAME##_T; \
    /* 定义类 */ \
	typedef struct LPREFIX##NAME##Def \
	{ \
	} LPREFIX##NAME##Def; \
	static inline LPREFIX##NAME##Def SPREFIX##Null##NAME##Def(void) \
	{ \
		LPREFIX##NAME##Def def; \
		return def; \
	};
#pragma endregion
#pragma region Impl
#define C_TEMPLATE_IMPL_DLIST(ATTR, SPREFIX, LPREFIX, NAME, TYPE, NODE_NAME, ALLOCATOR) \
    /* 私有 */ \
	ATTR LPREFIX##NAME##Node* SPREFIX##NAME##_GetNode(TYPE* e) { return &((e)->NODE_NAME); } \
	/* 创建销毁 */ \
	ATTR bool SPREFIX##NAME##Init(LPREFIX##NAME me, const LPREFIX##NAME##Def* def) \
	{ \
		if (me == NULL) return false; \
		me->head = NULL; \
		me->tail = NULL; \
		me->size = 0; \
		return true; \
	} \
	ATTR LPREFIX##NAME SPREFIX##NAME##Create(const LPREFIX##NAME##Def* def) \
	{ \
		LPREFIX##NAME me = (LPREFIX##NAME)(ALLOCATOR)->malloc((ALLOCATOR)->context, sizeof(LPREFIX##NAME##_T)); \
		if (!SPREFIX##NAME##Init(me, def)) return NULL; \
		return me; \
	} \
	ATTR LPREFIX##NAME SPREFIX##NAME##Copy(const LPREFIX##NAME src) \
	{ \
		if (src == NULL) return NULL; \
		LPREFIX##NAME me = (LPREFIX##NAME)(ALLOCATOR)->malloc((ALLOCATOR)->context, sizeof(LPREFIX##NAME##_T)); \
		if (me == NULL) return NULL; \
		SPREFIX##NAME##Init(me, NULL); \
		for (TYPE* it = src->head; it != NULL; it = SPREFIX##NAME##_GetNode(it)->next) \
			SPREFIX##NAME##PushBack(me, it); \
		return me; \
	} \
	ATTR void SPREFIX##NAME##CopyTo(LPREFIX##NAME me, const LPREFIX##NAME src) \
	{ \
		if (src == NULL || me == src) return; \
		SPREFIX##NAME##Clear(me); \
		for (TYPE* it = src->head; it != NULL; it = SPREFIX##NAME##_GetNode(it)->next) \
			SPREFIX##NAME##PushBack(me, it); \
	} \
	ATTR LPREFIX##NAME SPREFIX##NAME##Move(const LPREFIX##NAME src) \
	{ \
		if (src == NULL) return NULL; \
		LPREFIX##NAME me = (LPREFIX##NAME)(ALLOCATOR)->malloc((ALLOCATOR)->context, sizeof(LPREFIX##NAME##_T)); \
		if (me == NULL) return NULL; \
		me->head = src->head; \
		me->tail = src->tail; \
		me->size = src->size; \
		src->head = NULL; \
		src->tail = NULL; \
		src->size = 0; \
		return me; \
	} \
	ATTR void SPREFIX##NAME##MoveTo(LPREFIX##NAME me, const LPREFIX##NAME src) \
	{ \
		if (src == NULL || me == src) return; \
		SPREFIX##NAME##Free(me); \
		me->head = src->head; \
		me->tail = src->tail; \
		me->size = src->size; \
		src->head = NULL; \
		src->tail = NULL; \
		src->size = 0; \
	} \
	ATTR void SPREFIX##NAME##Free(LPREFIX##NAME me) \
	{ \
		if (me == NULL) return; \
		SPREFIX##NAME##Clear(me); \
	} \
	ATTR void SPREFIX##NAME##Destroy(LPREFIX##NAME me) \
	{ \
		if (me == NULL) return; \
		SPREFIX##NAME##Free(me); \
		(ALLOCATOR)->free((ALLOCATOR)->context, me); \
	} \
	/* 访问 */ \
	ATTR TYPE* SPREFIX##NAME##Front(LPREFIX##NAME me) { return me->head; } \
	ATTR TYPE* SPREFIX##NAME##Back(LPREFIX##NAME me) { return me->tail; } \
	/* 属性 */ \
	ATTR size_t SPREFIX##NAME##Size(LPREFIX##NAME me) { return me->size; } \
	ATTR bool SPREFIX##NAME##Empty(LPREFIX##NAME me) { return me->size == 0; } \
	/* 增删改 */ \
	ATTR size_t SPREFIX##NAME##PushFront(LPREFIX##NAME me, TYPE* element) \
	{ \
		LPREFIX##NAME##Node* n = SPREFIX##NAME##_GetNode(element); \
		n->prev = NULL; \
		n->next = me->head; \
		if (me->head) SPREFIX##NAME##_GetNode(me->head)->prev = element; \
		me->head = element; \
		if (me->tail == NULL) me->tail = element; \
		me->size += 1; \
		return me->size - 1; \
	} \
	ATTR size_t SPREFIX##NAME##PushBack(LPREFIX##NAME me, TYPE* element) \
	{ \
		LPREFIX##NAME##Node* n = SPREFIX##NAME##_GetNode(element); \
		n->next = NULL; \
		n->prev = me->tail; \
		if (me->tail) SPREFIX##NAME##_GetNode(me->tail)->next = element; \
		me->tail = element; \
		if (me->head == NULL) me->head = element; \
		me->size += 1; \
		return me->size - 1; \
	} \
	ATTR void SPREFIX##NAME##PopFront(LPREFIX##NAME me) \
	{ \
		if (me->head == NULL) return; \
		TYPE* old = me->head; \
		TYPE* next = SPREFIX##NAME##_GetNode(old)->next; \
		if (next) SPREFIX##NAME##_GetNode(next)->prev = NULL; \
		me->head = next; \
		if (me->head == NULL) me->tail = NULL; \
		me->size -= 1; \
	} \
	ATTR void SPREFIX##NAME##PopBack(LPREFIX##NAME me) \
	{ \
		if (me->tail == NULL) return; \
		TYPE* old = me->tail; \
		TYPE* prev = SPREFIX##NAME##_GetNode(old)->prev; \
		if (prev) SPREFIX##NAME##_GetNode(prev)->next = NULL; \
		me->tail = prev; \
		if (me->tail == NULL) me->head = NULL; \
		me->size -= 1; \
	} \
	ATTR void SPREFIX##NAME##InsertAfter(LPREFIX##NAME me, TYPE* index, TYPE* element) \
	{ \
		if (index == NULL) { SPREFIX##NAME##PushFront(me, element); return; } \
		if (index == me->tail) { SPREFIX##NAME##PushBack(me, element); return; } \
		TYPE* next = SPREFIX##NAME##_GetNode(index)->next; \
		SPREFIX##NAME##_GetNode(element)->prev = index; \
		SPREFIX##NAME##_GetNode(element)->next = next; \
		SPREFIX##NAME##_GetNode(index)->next = element; \
		SPREFIX##NAME##_GetNode(next)->prev = element; \
		me->size += 1; \
	} \
	ATTR void SPREFIX##NAME##InsertBefore(LPREFIX##NAME me, TYPE* index, TYPE* element) \
	{ \
		if (index == NULL) { SPREFIX##NAME##PushBack(me, element); return; } \
		if (index == me->head) { SPREFIX##NAME##PushFront(me, element); return; } \
		TYPE* prev = SPREFIX##NAME##_GetNode(index)->prev; \
		SPREFIX##NAME##_GetNode(element)->next = index; \
		SPREFIX##NAME##_GetNode(element)->prev = prev; \
		SPREFIX##NAME##_GetNode(prev)->next = element; \
		SPREFIX##NAME##_GetNode(index)->prev = element; \
		me->size += 1; \
	} \
	ATTR void SPREFIX##NAME##Erase(LPREFIX##NAME me, TYPE* element) \
	{ \
		if (element == NULL) return; \
		TYPE* prev = SPREFIX##NAME##_GetNode(element)->prev; \
		TYPE* next = SPREFIX##NAME##_GetNode(element)->next; \
		if (prev) SPREFIX##NAME##_GetNode(prev)->next = next; \
		if (next) SPREFIX##NAME##_GetNode(next)->prev = prev; \
		if (me->head == element) me->head = next; \
		if (me->tail == element) me->tail = prev; \
		SPREFIX##NAME##_GetNode(element)->next = NULL; \
		SPREFIX##NAME##_GetNode(element)->prev = NULL; \
		me->size -= 1; \
	} \
	ATTR TYPE* SPREFIX##NAME##Find(LPREFIX##NAME me, const TYPE* goal, LPREFIX##NAME##Equalor equalor) \
	{ \
		for (TYPE* it = me->head; it != NULL; it = SPREFIX##NAME##_GetNode(it)->next) \
			if (equalor(it, goal)) return it; \
		return NULL; \
	} \
	ATTR bool SPREFIX##NAME##Contains(LPREFIX##NAME me, const TYPE* goal, LPREFIX##NAME##Equalor equalor) \
	{ \
		return SPREFIX##NAME##Find(me, goal, equalor) != NULL; \
	} \
	/* 批处理 */ \
	ATTR void SPREFIX##NAME##Foreach(LPREFIX##NAME me, LPREFIX##NAME##Visitor visitor) \
	{ \
		for (TYPE* it = me->head; it != NULL; it = SPREFIX##NAME##_GetNode(it)->next) visitor(it); \
	} \
	ATTR void SPREFIX##NAME##RForeach(LPREFIX##NAME me, LPREFIX##NAME##Visitor visitor) \
	{ \
		for (TYPE* it = me->tail; it != NULL; it = SPREFIX##NAME##_GetNode(it)->prev) visitor(it); \
	} \
	ATTR void SPREFIX##NAME##RemoveIf(LPREFIX##NAME me, LPREFIX##NAME##Predicate predicate) \
	{ \
		TYPE* it = me->head; \
		while (it != NULL) \
		{ \
			TYPE* next = SPREFIX##NAME##_GetNode(it)->next; \
			if (predicate(it)) SPREFIX##NAME##Erase(me, it); \
			it = next; \
		} \
	} \
	ATTR void SPREFIX##NAME##Clear(LPREFIX##NAME me) \
	{ \
		TYPE* it = me->head; \
		while (it != NULL) \
		{ \
			TYPE* next = SPREFIX##NAME##_GetNode(it)->next; \
			SPREFIX##NAME##_GetNode(it)->next = NULL; \
			SPREFIX##NAME##_GetNode(it)->prev = NULL; \
			it = next; \
		} \
		me->head = NULL; \
		me->tail = NULL; \
		me->size = 0; \
	} \
	/* 交换 */ \
	ATTR void SPREFIX##NAME##Swap(LPREFIX##NAME me, LPREFIX##NAME other) \
	{ \
		TYPE* t_head = me->head; \
		TYPE* t_tail = me->tail; \
		size_t t_size = me->size; \
		me->head = other->head; me->tail = other->tail; me->size = other->size; \
		other->head = t_head; other->tail = t_tail; other->size = t_size; \
	}
#pragma endregion
#pragma region Sugar
#define C_TEMPLATE_DLIST_FOREACH(TYPE, ELEM_PTR_NAME, NODE_NAME, CONTAINER) \
	for (TYPE* ELEM_PTR_NAME = (CONTAINER)->head; ELEM_PTR_NAME != NULL; ELEM_PTR_NAME = (ELEM_PTR_NAME)->NODE_NAME.next)
#define C_TEMPLATE_DLIST_RFOREACH(TYPE, ELEM_PTR_NAME, NODE_NAME, CONTAINER) \
	for (TYPE* ELEM_PTR_NAME = (CONTAINER)->tail; ELEM_PTR_NAME != NULL; ELEM_PTR_NAME = (ELEM_PTR_NAME)->NODE_NAME.prev)
#pragma endregion

#endif
