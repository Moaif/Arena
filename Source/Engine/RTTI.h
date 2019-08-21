#pragma once
#include <vector>
#include <map>
#include <algorithm>
#include <assert.h>

template <class A, class B>
struct SameType { static const bool value = false; };

template <class A>
struct SameType<A,A> { static const bool value = true; };

class RTTIInfo
{
public:
    typedef unsigned TypeId;
    typedef void *(*CreateFn)();
    
    static RTTIInfo invalid() { return RTTIInfo((TypeId) -1); }

    template <class T>
    static RTTIInfo get()
    { 
        static const RTTIInfo val = RTTIRepo::instance()->getOrRegister(T::getClassName(), SameType<T, T::Base>::value ? invalid() : get<T::Base>(), (CreateFn) T::createInstance);
        return val;
    }

    template <class T>
    static RTTIInfo get(T*) { return get<T>(); }

    inline static RTTIInfo get(std::string className);

    bool isValid() { return m_typeId != invalid().m_typeId; }

    bool operator ==(RTTIInfo other) { return m_typeId == other.m_typeId && isValid(); }

    inline RTTIInfo getBase();

    inline const char *getName();

    template <class T>
    T *createInstance()
    {
        assert(derivedFrom<T>());
        return (T*) (RTTIRepo::instance()->getCreateFn(*this))();
    }

    bool derivedFrom(RTTIInfo target)
    {
        RTTIInfo type = *this;
        while (type.isValid())
        {
            if (type == target)
                return true;
            type = type.getBase();
        }
        return false;
    }

    template <class T>
    bool derivedFrom()
    {
        return derivedFrom(get<T>());
    }

    RTTIInfo() : m_typeId(invalid().m_typeId) {}
protected:
    RTTIInfo(TypeId typeId): m_typeId(typeId) {}
    friend class RTTIRepo;

    TypeId m_typeId;
};

template <class T, class S>
T* rtti_cast(S *obj)
{
    if (obj->getRTTI().derivedFrom<T>())
        return (T*)obj;
    return nullptr;
}

class RTTIRepo
{
public:
    struct TypeInfo 
    {
        const char* m_className;
        RTTIInfo::TypeId m_typeId;
        RTTIInfo m_base;
        RTTIInfo::CreateFn m_createFn;
    };

    RTTIInfo getOrRegister(const char* className, RTTIInfo baseClass, RTTIInfo::CreateFn createFn)
    {
        std::vector<TypeInfo>::iterator it =  std::find_if(m_types.begin(), m_types.end(), [=](const TypeInfo& t){ return !strcmp(className, t.m_className); });
        if (it != m_types.end())
            return RTTIInfo((*it).m_typeId);
        m_types.resize(m_types.size() + 1);
        m_types.back().m_className = className;
        m_types.back().m_typeId = m_types.size() - 1;
        m_types.back().m_base = baseClass;
        m_types.back().m_createFn = createFn;
        RTTIInfo type(m_types.back().m_typeId);
        m_names[std::string(className)] = type;
        return type;
    }

    RTTIInfo getByName(std::string className) const
    {
        auto it = m_names.find(className);
        if (it == m_names.end())
            return RTTIInfo::invalid();
        return (*it).second;
    }

    RTTIInfo getBase(RTTIInfo type) const
    {
        return m_types[type.m_typeId].m_base;
    }

    const char *getName(RTTIInfo type) const
    {
        return m_types[type.m_typeId].m_className;
    }

    RTTIInfo::CreateFn getCreateFn(RTTIInfo type) const
    {
        return m_types[type.m_typeId].m_createFn;
    }

    static RTTIRepo *instance();

protected:
    static RTTIRepo *s_instance;
    std::vector<TypeInfo> m_types;
    std::map<std::string, RTTIInfo> m_names;
};

RTTIInfo RTTIInfo::getBase()
{
    if (!isValid())
        return invalid();
    return RTTIRepo::instance()->getBase(*this);
}

const char *RTTIInfo::getName()
{
    if (!isValid())
        return nullptr;
    return RTTIRepo::instance()->getName(*this);
}

RTTIInfo RTTIInfo::get(std::string className)
{
    return RTTIRepo::instance()->getByName(className);
}

template <class T>
struct RTTIRegister
{
    RTTIRegister() { RTTIInfo::get<T>(); }
};

#define CAT1(A, B) A##B
#define CAT(A, B) CAT1(A, B)

#define RTTI_REGISTER(T) RTTIRegister<T> CAT(autoReg, __LINE__);

#define RTTI_COMMON(CLASS, BASE) \
    typedef BASE Base; \
    virtual RTTIInfo getRTTI() const { return RTTIInfo::get(this); } \
    static const char *getClassName() { return #CLASS; } 

#define RTTI_CREATE(CLASS) \
    static CLASS *createInstance() { return new CLASS(); }

#define RTTI_NOCREATE(CLASS) \
    static CLASS *createInstance() { return nullptr; } \

#define RTTI_ENABLE(CLASS, BASE) \
public: \
    RTTI_COMMON(CLASS, BASE) \
    RTTI_CREATE(CLASS) \
private:

#define RTTI_ABSTRACT(CLASS, BASE) \
public: \
    RTTI_COMMON(CLASS, BASE) \
    RTTI_NOCREATE(CLASS) \
private:

#define RTTI_ENABLE_BASE(CLASS) RTTI_ENABLE(CLASS, CLASS)
#define RTTI_ABSTRACT_BASE(CLASS) RTTI_ABSTRACT(CLASS, CLASS)
