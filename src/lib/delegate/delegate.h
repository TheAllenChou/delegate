/******************************************************************************/
/*
  Author  - Ming-Lun "Allen" Chou
  Web     - http://AllenChou.net
  Twitter - @TheAllenChou
*/
/******************************************************************************/

#ifndef DELEGATE_H
#define DELEGATE_H

#include <type_traits>

template <typename T> struct Delegate;

template <typename Ret, typename ...Arg>
struct Delegate<Ret(Arg...)>
{
  typedef Ret ReturnType;

  Delegate()
    : m_pObj(nullptr)
    , m_pInvoker(nullptr)
  { }

  Delegate(void *pObj, Ret (*pInvoker) (void *, Arg...))
    : m_pObj(pObj)
    , m_pInvoker(pInvoker)
  { }

  Delegate(const void *pObj, Ret (*pInvoker) (void *, Arg...))
    : m_pObj(const_cast<void *>(pObj))
    , m_pInvoker(pInvoker)
  { }
  
  template <Ret (*Func) (Arg...)>
  struct FuncInvoker
  {
    static Ret Invoke(void *pObj, Arg... arg)
    {
      return Func(arg...);
    }
  };

  template <typename T>
  struct MethodInvoker
  {
    typedef typename std::remove_const<T>::type BaseT;

    template <Ret (BaseT::*Method) (Arg...)>
    static Ret Invoke(void *pObj, Arg... arg)
    {
      return (static_cast<BaseT *>(pObj)->*Method)(arg...);
    }

    template <Ret (BaseT::*Method) (Arg...) const>
    static Ret Invoke(void *pObj, Arg... arg)
    {
      return (static_cast<BaseT *>(pObj)->*Method)(arg...);
    }

    template <Ret (BaseT::*Method) (Arg...) const>
    static Ret InvokeConst(void *pObj, Arg... arg)
    {
      return (static_cast<const BaseT *>(pObj)->*Method)(arg...);
    }
  };

  Ret operator() (Arg... arg)
  {
    return m_pInvoker(m_pObj, arg...);
  }

  void *m_pObj;
  Ret (*m_pInvoker) (void *, Arg...);
};

template <typename T, typename C>
struct GetDelegateType;

template <typename Ret, typename ...Arg>
struct GetDelegateType<void, Ret(Arg...)>
{
  typedef Delegate<Ret(Arg...)> Type;
};

template <typename T, typename Ret, typename ...Arg>
struct GetDelegateType<T, Ret (T::*) (Arg...)>
{
  typedef Delegate<Ret(Arg...)> Type;
};

template <typename T, typename Ret, typename ...Arg>
struct GetDelegateType<T, Ret (T::*) (Arg...) const>
{
  typedef Delegate<Ret(Arg...)> Type;
};

template <typename F, F Func>
auto DelegateFuncHelper() 
  -> typename GetDelegateType<void, F>::Type
{
  typedef GetDelegateType<void, F>::Type Delegate;
  return Delegate((void *) nullptr, Delegate::FuncInvoker<Func>::Invoke);
}

template <typename T, typename M, M Method>
auto DelegateMethodHelper(T *pObj) 
  -> typename GetDelegateType<T, M>::Type
{
  typedef GetDelegateType<T, M>::Type Delegate;
  return Delegate(pObj, Delegate::MethodInvoker<T>::Invoke<Method>);
}

template <typename T, typename M, M Method>
auto DelegateMethodHelper(const T *pObj) 
  -> typename GetDelegateType<T, M>::Type
{
  typedef GetDelegateType<T, M>::Type Delegate;
  return Delegate(pObj, Delegate::MethodInvoker<T>::InvokeConst<Method>);
}

#define DelegateFunc(Func) \
  (DelegateFuncHelper<decltype(Func), Func>())

#define REMOVE_POINTER_CONST(T) \
  std::remove_const<std::remove_pointer<T>::type>::type

#define DelegateMethod(pObj, Method)                 \
  (DelegateMethodHelper                              \
    <                                                \
      typename REMOVE_POINTER_CONST(decltype(pObj)), \
      decltype(Method),                              \
      Method                                         \
    >                                                \
    (pObj)                                           \
  )

#endif
