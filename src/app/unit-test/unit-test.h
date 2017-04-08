/******************************************************************************/
/*
  Author  - Ming-Lun "Allen" Chou
  Web     - http://AllenChou.net
  Twitter - @TheAllenChou
 */
/******************************************************************************/

#ifndef UNIT_TEST_H
#define UNIT_TEST_H


class UnitTest : public CppUnit::TestFixture
{
  public:
    CPPUNIT_TEST_SUITE(UnitTest);
    CPPUNIT_TEST(TestFunctions);
    CPPUNIT_TEST(TestMethods);
    CPPUNIT_TEST_SUITE_END();

  public:
    void TestFunctions();
    void TestMethods();
};


#endif
