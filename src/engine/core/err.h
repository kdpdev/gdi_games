#pragma once;

#include <exception>
#include <string>

namespace Engine
{
  namespace Core
  {
    std::string FormatException(const std::exception& e);
    std::string FormatException(std::exception_ptr e);

    std::string FormatErrorDescription(const std::string& descr, std::exception_ptr e);
    std::string FormatErrorDescription(const std::string& descr, const char* file, const char* function, int line);

    void ThrowRuntimeException(const std::string& descr);

    template <typename ExceptionType>
    void ThrowTypedException(const std::string& descr)
    {
      throw ExceptionType(descr);
    }
  }
}


#define MAKE_THROW_CONTEXT(descr) ::Engine::Core::FormatErrorDescription(descr, __FILE__, __FUNCTION__, __LINE__)


#define ERR_THROW(descr)                                     ::Engine::Core::ThrowRuntimeException(::Engine::Core::FormatErrorDescription(descr, __FILE__, __FUNCTION__, __LINE__))
#define ERR_THROW_IF(condition, descr)     if (condition)    ::Engine::Core::ThrowRuntimeException(::Engine::Core::FormatErrorDescription(descr, __FILE__, __FUNCTION__, __LINE__))
#define ERR_THROW_IF_NOT(condition, descr) if (!(condition)) ::Engine::Core::ThrowRuntimeException(::Engine::Core::FormatErrorDescription(descr, __FILE__, __FUNCTION__, __LINE__))


#define ERR_THROW_TYPED(type, descr)                                     ::Engine::Core::ThrowTypedException<type>(::Engine::Core::FormatErrorDescription(descr, __FILE__, __FUNCTION__, __LINE__))
#define ERR_THROW_TYPED_IF(condition, type, descr)     if (condition)    ::Engine::Core::ThrowTypedException<type>(::Engine::Core::FormatErrorDescription(descr, __FILE__, __FUNCTION__, __LINE__))
#define ERR_THROW_TYPED_IF_NOT(condition, type, descr) if (!(condition)) ::Engine::Core::ThrowTypedException<type>(::Engine::Core::FormatErrorDescription(descr, __FILE__, __FUNCTION__, __LINE__))


#define ERR_THROW_CONSTRUCTED(constructor)                                     throw (constructor(__FILE__, __FUNCTION__, __LINE__))
#define ERR_THROW_CONSTRUCTED_IF(condition, constructor)     if (condition)    throw (constructor(__FILE__, __FUNCTION__, __LINE__))
#define ERR_THROW_CONSTRUCTED_IF_NOT(condition, constructor) if (!(condition)) throw (constructor(__FILE__, __FUNCTION__, __LINE__))


#define ERR_THROW_OBJECT(type, obj)                               throw obj
#define ERR_THROW_OBJECT_IF(condition, obj)     if (condition)    throw obj
#define ERR_THROW_OBJECT_IF_NOT(condition, obj) if (!(condition)) throw obj
