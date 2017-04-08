/******************************************************************************/
/*
  Author  - Ming-Lun "Allen" Chou
  Web     - http://AllenChou.net
  Twitter - @TheAllenChou
 */
/******************************************************************************/

#include "unit-test.h"

#include "delegate.h"


// Registers the fixture into the 'registry'
CPPUNIT_TEST_SUITE_REGISTRATION(UnitTest);

void VoidFunc() { std::cout << "voidFunc" << std::endl; }
int IntFunc() { return -1; }
float AddFunc(float a, float b) { return a + b; }

class A
{
public:
  void VoidMethod() const { std::cout << "A::void Func const" << std::endl; }
  float MultMethod(float a, float b) { return a * b; }
  virtual int VirtualIncMethod(int a) { return a + 1; }

  int ConstTestMethod(int x) { return x; }
  int ConstTestMethod(int x) const { return x + 10; }
};

class B : public A
{
public:
  virtual int VirtualIncMethod(int a) override { return a + 2; }
};

void UnitTest::TestFunctions()
{
  //DelegateFuncHelper<decltype(VoidFunc), VoidFunc>()();

  //DelegateFunc(VoidFunc)();
}

void UnitTest::TestMethods()
{
  const A a;
  //DelegateMethodHelper<A, decltype(&A::VoidMethod), &A::VoidMethod>(&a)();

  Delegate<void()> del;
  del = DelegateFunc(VoidFunc);
  del();

  DelegateMethod(&a, &A::VoidMethod)();
}
