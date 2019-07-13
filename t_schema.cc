#include "gtest/gtest.h"

#include "schema.h"

namespace {

using Database::Schema;

/*! @brief Wrapper for Schema class to enable the Schema constructor to be called.
 */
class EmptySchema
    : public Schema 
{
public: 
    /*! @brief Construct an empty schema object.
     */
    EmptySchema(): Schema() {
    }

    /*! @brief Perform a migration on the empty schema.
     */
    const bool migrate() {
        return true;
    }
};

TEST(TestSchema, SchemaConstructor) {
    EXPECT_NO_THROW(EmptySchema());
}

/*! @brief Schema test fixture.
 */
class SchemaTestFixture
    : public ::testing::Test
{
protected:
    /*! @brief Set up the schema object.
     */
    void SetUp() override {
        s = new EmptySchema;
    }

    /*! @brief Tear down the schema object.
     */
    void TearDown() override {
        delete s;
    }

    EmptySchema *s; //! The empty schema object.
};

TEST_F(SchemaTestFixture, SchemaMigrate) {
    ASSERT_EQ(true, s->migrate());
}

} // namespace

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
