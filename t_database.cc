#include "gtest/gtest.h"

namespace {

/* Check container state following initialization using default constructor.
 */
TEST(TestMain, TestExecution) {
  ASSERT_EQ(true, false);
}

} // namespace

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
