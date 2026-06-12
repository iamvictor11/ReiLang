#ifndef C_TEMPLATE_CONTAINER_DICT_H
#define C_TEMPLATE_CONTAINER_DICT_H

#include "../allocator.h"
#include <stdbool.h>
#include <stddef.h>

#pragma region Dec
#define C_TEMPLATE_DECL_DICT(ATTR, SPREFIX, LPREFIX, NAME, ELEM, NODE_NAME, KEY, VALUE) \
	typedef struct LPREFIX##NAME##_T LPREFIX##NAME##_T; \
	typedef LPREFIX##NAME##_T* LPREFIX##NAME; \
	typedef ELEM LPREFIX##NAME##Elem; \
	typedef size_t(*LPREFIX##NAME##Hash)(const KEY* key); \
	typedef bool(*LPREFIX##NAME##Equalor)(const KEY* a, const KEY* b); \
	typedef void(*LPREFIX##NAME##Signal)(const LPREFIX##NAME container, LPREFIX##NAME##Elem* element); \
	typedef void(*LPREFIX##NAME##Visitor)(LPREFIX##NAME##Elem* element); \
	typedef struct LPREFIX##NAME##Def LPREFIX##NAME##Def; \
	static inline LPREFIX##NAME##Def SPREFIX##Null##NAME##Def(void); \
	/* 创建/销毁 */ \
	ATTR LPREFIX##NAME SPREFIX##NAME##Create(const LPREFIX##NAME##Def* def, size_t bucket_count); \
	ATTR void SPREFIX##NAME##Destroy(LPREFIX##NAME me); \
	/* 属性 */ \
	ATTR size_t SPREFIX##NAME##Size(LPREFIX##NAME me); \
	ATTR bool SPREFIX##NAME##Empty(LPREFIX##NAME me); \
	/* 操作 (侵入式): 插入/查找/删除使用元素指针或 key */ \
	ATTR void SPREFIX##NAME##PutElem(LPREFIX##NAME me, LPREFIX##NAME##Elem* element); \
	ATTR LPREFIX##NAME##Elem* SPREFIX##NAME##Get(LPREFIX##NAME me, const KEY* key); \
	ATTR bool SPREFIX##NAME##RemoveElem(LPREFIX##NAME me, LPREFIX##NAME##Elem* element); \
	ATTR bool SPREFIX##NAME##RemoveKey(LPREFIX##NAME me, const KEY* key); \
	ATTR bool SPREFIX##NAME##Contains(LPREFIX##NAME me, const KEY* key); \
	/* 遍历/清空 */ \
	ATTR void SPREFIX##NAME##Foreach(LPREFIX##NAME me, LPREFIX##NAME##Visitor visitor); \
	ATTR void SPREFIX##NAME##Clear(LPREFIX##NAME me);
#pragma endregion
#pragma region Def
#define C_TEMPLATE_DEFN_DICT(ATTR, SPREFIX, LPREFIX, NAME, ELEM, NODE_NAME, KEY, VALUE) \
	typedef struct LPREFIX##NAME##Node \
	{ \
		ELEM* next; \
	} LPREFIX##NAME##Node; \
	typedef struct LPREFIX##NAME##_T \
	{ \
		ELEM** buckets; \
		size_t bucket_count; \
		size_t size; \
		LPREFIX##NAME##Hash hash; \
		LPREFIX##NAME##Equalor equalor; \
		LPREFIX##NAME##Signal insertSignal; \
		LPREFIX##NAME##Signal removeSignal; \
	} LPREFIX##NAME##_T; \
	typedef struct LPREFIX##NAME##Def \
	{ \
		LPREFIX##NAME##Hash hash; \
		LPREFIX##NAME##Equalor equalor; \
		LPREFIX##NAME##Signal insertSignal; \
		LPREFIX##NAME##Signal removeSignal; \
	} LPREFIX##NAME##Def; \
	static inline LPREFIX##NAME##Def SPREFIX##Null##NAME##Def(void) \
	{ \
		LPREFIX##NAME##Def def; \
		def.hash = NULL; def.equalor = NULL; def.insertSignal = NULL; def.removeSignal = NULL; \
		return def; \
	}
#pragma endregion
#pragma region Impl
#define C_TEMPLATE_IMPL_DICT(ATTR, SPREFIX, LPREFIX, NAME, ELEM, NODE_NAME, KEY, VALUE, ALLOCATOR) \
	/* 私有：获取元素内的节点 */ \
	ATTR LPREFIX##NAME##Node* SPREFIX##NAME##_GetNode(ELEM* e) { return &((e)->NODE_NAME); } \
	ATTR LPREFIX##NAME SPREFIX##NAME##Create(const LPREFIX##NAME##Def* def, size_t bucket_count) \
	{ \
		if (def == NULL || def->hash == NULL || def->equalor == NULL) return NULL; \
		if (bucket_count == 0) bucket_count = 16; \
		LPREFIX##NAME me = (LPREFIX##NAME)(ALLOCATOR)->malloc((ALLOCATOR)->context, sizeof(LPREFIX##NAME##_T)); \
		if (!me) return NULL; \
		me->bucket_count = bucket_count; \
		me->size = 0; \
		me->hash = def->hash; \
		me->equalor = def->equalor; \
		me->insertSignal = def->insertSignal; \
		me->removeSignal = def->removeSignal; \
		me->buckets = (ELEM**)(ALLOCATOR)->malloc((ALLOCATOR)->context, sizeof(ELEM*) * bucket_count); \
		if (!me->buckets) { (ALLOCATOR)->free((ALLOCATOR)->context, me); return NULL; } \
		for (size_t i = 0; i < bucket_count; ++i) me->buckets[i] = NULL; \
		return me; \
	} \
	ATTR void SPREFIX##NAME##Destroy(LPREFIX##NAME me) \
	{ \
		if (!me) return; \
		SPREFIX##NAME##Clear(me); \
		(ALLOCATOR)->free((ALLOCATOR)->context, me->buckets); \
		(ALLOCATOR)->free((ALLOCATOR)->context, me); \
	} \
	ATTR size_t SPREFIX##NAME##Size(LPREFIX##NAME me) { return me ? me->size : 0; } \
	ATTR bool SPREFIX##NAME##Empty(LPREFIX##NAME me) { return me ? me->size == 0 : true; } \
	ATTR void SPREFIX##NAME##PutElem(LPREFIX##NAME me, ELEM* element) \
	{ \
		if (!me || !element) return; \
		size_t h = me->hash(&element->key) % me->bucket_count; \
		ELEM* head = me->buckets[h]; \
		SPREFIX##NAME##_GetNode(element)->next = head; \
		me->buckets[h] = element; \
		me->size += 1; \
		if (me->insertSignal) me->insertSignal(me, element); \
	} \
	ATTR ELEM* SPREFIX##NAME##Get(LPREFIX##NAME me, const KEY* key) \
	{ \
		if (!me || !key) return NULL; \
		size_t h = me->hash(key) % me->bucket_count; \
		for (ELEM* it = me->buckets[h]; it != NULL; it = SPREFIX##NAME##_GetNode(it)->next) \
			if (me->equalor(&it->key, key)) return it; \
		return NULL; \
	} \
	ATTR bool SPREFIX##NAME##RemoveElem(LPREFIX##NAME me, ELEM* element) \
	{ \
		if (!me || !element) return false; \
		size_t h = me->hash(&element->key) % me->bucket_count; \
		ELEM* prev = NULL; \
		for (ELEM* it = me->buckets[h]; it != NULL; prev = it, it = SPREFIX##NAME##_GetNode(it)->next) \
		{ \
			if (it == element) \
			{ \
				if (prev) SPREFIX##NAME##_GetNode(prev)->next = SPREFIX##NAME##_GetNode(it)->next; else me->buckets[h] = SPREFIX##NAME##_GetNode(it)->next; \
				if (me->removeSignal) me->removeSignal(me, it); \
				SPREFIX##NAME##_GetNode(it)->next = NULL; \
				me->size -= 1; \
				return true; \
			} \
		} \
		return false; \
	} \
	ATTR bool SPREFIX##NAME##RemoveKey(LPREFIX##NAME me, const KEY* key) \
	{ \
		if (!me || !key) return false; \
		size_t h = me->hash(key) % me->bucket_count; \
		ELEM* prev = NULL; \
		for (ELEM* it = me->buckets[h]; it != NULL; prev = it, it = SPREFIX##NAME##_GetNode(it)->next) \
		{ \
			if (me->equalor(&it->key, key)) \
			{ \
				if (prev) SPREFIX##NAME##_GetNode(prev)->next = SPREFIX##NAME##_GetNode(it)->next; else me->buckets[h] = SPREFIX##NAME##_GetNode(it)->next; \
				if (me->removeSignal) me->removeSignal(me, it); \
				SPREFIX##NAME##_GetNode(it)->next = NULL; \
				me->size -= 1; \
				return true; \
			} \
		} \
		return false; \
	} \
	ATTR bool SPREFIX##NAME##Contains(LPREFIX##NAME me, const KEY* key) \
	{ \
		return SPREFIX##NAME##Get(me, key) != NULL; \
	} \
	ATTR void SPREFIX##NAME##Foreach(LPREFIX##NAME me, LPREFIX##NAME##Visitor visitor) \
	{ \
		if (!me || !visitor) return; \
		for (size_t i = 0; i < me->bucket_count; ++i) \
		{ \
			for (ELEM* it = me->buckets[i]; it != NULL; it = SPREFIX##NAME##_GetNode(it)->next) visitor(it); \
		} \
	} \
	ATTR void SPREFIX##NAME##Clear(LPREFIX##NAME me) \
	{ \
		if (!me) return; \
		for (size_t i = 0; i < me->bucket_count; ++i) \
		{ \
			ELEM* it = me->buckets[i]; \
			while (it) { ELEM* next = SPREFIX##NAME##_GetNode(it)->next; if (me->removeSignal) me->removeSignal(me, it); SPREFIX##NAME##_GetNode(it)->next = NULL; it = next; } \
			me->buckets[i] = NULL; \
		} \
		me->size = 0; \
	}
#pragma endregion

#endif
