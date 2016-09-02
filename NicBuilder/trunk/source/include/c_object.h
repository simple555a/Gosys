/*
	simulate c++ sematics thru c code
	Jackie, 2003/3/12

	general instructions
	to declare a class, you must
	1) in header file, use ``__class__ '' to introduce the definition.
		1.1) use '__inherit__' to define base classes, if any
		1.2) define any data members following __class__ directive

		N.B. __class__ will invoke `__declare_object__' to give prototypes 
		for constructors/destructors and global-constructors.
		
		N.B. the '__inherit__' directive must precede any data
		members.

	2) if necessary, use ``__vtable__'' to define the virtual function table.
		2.1) use __vmethod__ to add virtual functions to the class
		2.2) use __vtable_end__ to end the virtual function table
		
		N.B. if the classes has base classes, you must first of all
		define the base-classs' virtual functions table, using the
		__inherit__ directive, and the base classes must appear in
		the same order they are declared in the derived classes's declaration.

		2003/3/18, virtual function table is mandatory now, every
		class should at least have one virtual destructors: destroy.

	3) use __method__ to define non-virtual functions.
	
	4) in implementation file,
		3.1) use '__constructor__','__destructor','__global_constructor__'
		to implement constructor/destructor and global constructors. this is
		mandatory.

		constructors and destructors work on a per-object basis, while
		global-constructors work with no objects, it's the appropriate place
		to initialize the (unique) virtual function table instance.

		in constructors, you should set the per-object virtual function
		table pointer(the vtbl field). for derived classes, you should also call
		base classes' constructors and modify their virtual function
		table pointer to some proper member of the derived class's vtbl.

		WARNING : 
		!!! YOU MUST CALL `__create_base__' ON EACH DIRECTLY INHERITED CLASSES. 
		This will ensure correct virtual function table setup cascadedly.
		!!! NEVER CALL ZeroMemory on any object declared with __class__
		because that will flush the vtable pointer contained.

		It's a good practice to call __touch__() on entry of constructors.

		In global constructors, you need do nothing for base objects. while
		for derived classes, you must setup it's vtbl instance correctly,
		this is done through the `__v_copy__' and `__override__' macros.

		2003/6/26, for classes implemented in win32 DLL, you must export all
		related functions and symbols so that clients from outside the DLL 
		can use the objects, these include the global constructor
		(<typename>_global_create), the	object constructor(<typename>_create), 
		the object destructor (<typename>_destroy), and all non-virtual member 
		functions of the object, besides, a function named 
		get_<typename>_vtable which is implicitly defined by 
		__define_vtable__ macro must also be exported. Virtual 
		functions need not be exported as they are not standalone symbols
		by themselves but rather pointers stored in the virtual function table,
		so references to them are not resolved by name at link or load time
		but rather by pointer indirection at run time.

	5) client side
		Always use __create__() to initialize static and automatic objects,
		while use __new__() to allocate and initialize objects on heap. This 
		will ensure the vtable be correctly set.

	Always use the macros defined here if applicable, this will make your
	code sustain changes in the protocol, such as the function calling
	conventions.
*/
#ifndef __c_object_h__
#define __c_object_h__

/* include offsetof definition */
#include <stddef.h>
// #include <stdlib.h>
#include <rtk_compat.h>
#include <cpu_features.h>

#if defined(_WIN32) && !defined(FCC_INVOKED)
#ifdef LIBCOBJ_EXPORTS
#define LIBCOBJ_API __declspec(dllexport)
#else
#define LIBCOBJ_API __declspec(dllimport)
#endif
#else
#define LIBCOBJ_API
#endif


#ifndef EXTERN_C
#ifdef __cplusplus
#define EXTERN_C extern "C"
#else
#define EXTERN_C extern
#endif
#endif

 EXTERN_C void * __malloc__(__uint size);
 EXTERN_C void __free__(void * buf);

#define __public
#define __private

#define __vmethod__(__rettype, __name, __parm)\
	__rettype (*__name) __parm

#define __method__(__objtype, __rettype, __name, __parm)\
	/*LIBCOBJ_API*/ __rettype __objtype##_##__name __parm                            

#define __imethod__(__objtype, __rettype, __name, __parm)\
	__inline __rettype __objtype##_##__name __parm

/*  
   	__inherit__ is used in a derived class declaration to include another 
	class(base class)
*/
#define __inherit__(__basetype) __basetype __inherited_##__basetype;

/*
 use __declare_vtbl__ and __declare_vtbl_end__ to define an instance of 
 the virtual function table, that's enough since there should be only 
 once such instance. the instance is global so you can reference it by 
 __type##_vtbl_instance.
 !! all objects must have virtual function table, this is different from
 real c++ implementation, in which only classes with virtual functions have
 virtual tables.
*/

#define __vtable__(__type) typedef struct __tag_##__type##_vtbl {\
	__vmethod__(void *, destroy, (__type * _this));

#define __vtable_end__(__type) \
}PACKED __type##_vtbl;\
extern __type##_vtbl __type##_vtbl_instance;\
/*LIBCOBJ_API extern*/ __type##_vtbl * get_##__type##_vtable();

#define __define_vtbl__(__type)\
	struct __tag_##__type##_vtbl __type##_vtbl_instance = {\
		__type##_destroy,
#define __define_vtbl_end__(__type) \
};\
/*LIBCOBJ_API*/ __type##_vtbl * get_##__type##_vtable(){return &__type##_vtbl_instance;}

#define __global_constructor__(__type)\
	__method__(__type, void, global_create, (void))

#define __constructor__(__type)\
	__method__(__type, __type *, create, (__type * __this))

#define __destructor__(__type)\
	__method__(__type, void *, destroy, (__type * __this))

#define __declare_object__(__type) \
__global_constructor__(__type);\
__constructor__(__type);\
__destructor__(__type);

#define __class__(__type) \
	typedef struct __tag_##__type{\
	struct __tag_##__type##_vtbl * vtbl;
#define __class_end__(__type)	}PACKED __type;\
__declare_object__(__type)

/*
 setup virtual function table for a base class
 this macro function is expected to be called !!BEFORE!! constructors
 an extra function must be introduced because exported data in Win32
 DLL normally cannot be directly accessed(the __declspec(im/export)
 syntax is hard to simulate with macros.)
 2003/3/17, Jackie
*/
#ifdef _WIN32
#define __setup_vtbl__(__type, __obj) \
do{\
	__obj->vtbl = get_##__type##_vtable();\
}while(0)
#else
#define __setup_vtbl__(__type, __obj) \
do{\
	__obj->vtbl = &__type##_vtbl_instance;\
}while(0)
#endif

#ifdef NDEBUG
#define __touch__()
#else
#define __touch__()\
do{\
	void * saved_vtbl = (void *)__this->vtbl;\
	memset(__this, 0xf8, sizeof(*__this));\
	*((void**)&__this->vtbl) = saved_vtbl;\
}while(0)
#endif

/*
	override the virtual function table of the base class's contructor
	then call the base class's constructor. 
*/
#define __create_base__(__type, __basetype, __obj) \
do{\
	__basetype * b = __ucast__(__type, __basetype, __obj);\
	b->vtbl \
	= \
	&(__obj)->vtbl->__inherited_##__basetype##_vtbl;\
	b->vtbl->destroy = __basetype##_destroy;\
	__mcall__(__basetype, create, (b));\
}while(0)

#define __destroy_base__(__type, __basetype, __obj)\
do{\
	__basetype * b = __ucast__(__type, __basetype, __obj);\
	__destroy__(b);\
}while(0)

#define __set_vfunc__(__type, __func)\
do{__type##_vtbl_instance.__func = &__type##_##__func;}while(0)

#define __v_copy__(__type, __basetype)\
do{\
	memcpy(&__type##_vtbl_instance.__inherited_##__basetype##_vtbl,\
		 get_##__basetype##_vtable(),\
		   sizeof(__basetype##_vtbl_instance)\
		);\
}while(0)

#define __override__(__type, __basetype, __func)\
do{\
	__type##_vtbl_instance.__inherited_##__basetype##_vtbl.__func =\
		&__type##_##__func;\
}while(0)
	
#define __override2__(__type, __base1, __base2, __func)\
do{\
	__type##_vtbl_instance.__inherited_##__base1##_vtbl.__inherited_##__base2##_vtbl.__func =\
		&__type##_##__func;\
}while(0)

/* typecast from derived class to a base class */
#define __ucast__(_type, _basetype, _pointer) \
	(_pointer? (& (((_type*)(_pointer)) -> __inherited_##_basetype)) : 0)
#define __ucast2__(_type, _base2, _base1, _pointer) \
	__ucast__(_base2, _base1, __ucast__(_type, _base2, _pointer))

/*
 typecast from base class to a derived class
 !!! no run-time type checking is done, so use at your own risk
*/
#define __dcast__(_basetype, _type, _pointer) \
	(_pointer? ((_type *)((char *)(_pointer) - offsetof(_type, __inherited_##_basetype))) : 0)
#define __dcast2__(_base1, _base2, _type, _pointer) \
	__dcast__(_base2, _type, __dcast__(_base1, _base2, _pointer))

#if defined(NDEBUG) && !defined(LIBCOBJ_EXPORTS)
#define _CObj_CheckPureVCall(f,fn,file,line) 0
#else
EXTERN_C int LIBCOBJ_API _CObj_CheckPureVCall(void * f, char * func, char *file, int line);
#endif

/* call through virtual function table */
#define __vcall__(__obj, __func, __parm)\
	(	_CObj_CheckPureVCall( \
			(void*)(((__obj)->vtbl)->__func), \
			#__func, \
			__FILE__, __LINE__\
		), \
		((__obj)->vtbl)->__func\
	) \
	__parm

/* call non-virtual member functions */
#define __mcall__(__type, __func, __parm)\
	__type##_##__func __parm

#define __new__(__type, obj)\
do{\
	obj = (__type*)__malloc__(sizeof(__type));\
	if(obj){\
		__setup_vtbl__(__type, obj);\
		__mcall__(__type, create, (obj));\
	}\
}while(0)

#define __create__(__type, obj)\
do{\
	__setup_vtbl__(__type, (obj));\
	__mcall__(__type, create, (obj));\
}while(0)

/*
 bug bug bug bug
 2003/3/18, __delete__ doesn't support call of virtual destructors
 although it seems to support
 2003/3/21, this means that an object can only be deleted from specified
 point in the class hierarchy, normally, from the outmost layer
*/
#define __delete__(__this)\
do{\
	__free__((void*)__destroy__(__this));\
}while(0)

#define __destroy__(obj)\
	__vcall__(obj, destroy, (obj))

#endif	/* __c_object_h__ */

