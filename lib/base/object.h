#ifndef __base_object_h
#define __base_object_h

#include <assert.h>
#include <lib/base/smartptr.h>
#include <lib/base/elock.h>

#include <lib/base/eerror.h>

typedef int RESULT;

class iObject
{
private:
		/* we don't allow the default operator here, as it would break the refcount. */
	void operator=(const iObject &);
protected:
	void operator delete(void *p) { ::operator delete(p); }
	virtual ~iObject() { }
#ifdef SWIG
	virtual void AddRef()=0;
	virtual void Release()=0;
#endif
public:
#ifndef SWIG
	virtual void AddRef()=0;
	virtual void Release()=0;
#endif
};

#ifndef SWIG

class oRefCount
{
	int ref;
public:
	oRefCount(): ref(0) {}

	int operator++()
	{          
                return ++ref;
	}

	int operator--()
	{           
                return --ref;
	}

	operator int() const
	{
                return ref;
	}
};

		#define DECLARE_REF(x) 			\
			public: \
					void AddRef(); 		\
					void Release();		\
			private:\
					oRefCount ref;

		#define DEFINE_REF(c) \
			void c::AddRef() \
			{ \
				++ref; \
			} \
			void c::Release() \
	 		{ \
				if (!(--ref)) \
					delete this; \
			}

#else  // SWIG
	#define DECLARE_REF(x) \
		private: \
			void AddRef(); \
			void Release();
#endif  // SWIG

#endif  // __base_object_h
