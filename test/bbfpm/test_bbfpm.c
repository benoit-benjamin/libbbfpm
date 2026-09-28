#include "bbfpm/bbfpm.h"
#include "unity.h"
#include "unity_internals.h"

static const char*                         metadata__1__file_path     = "data/metadata.bbfpm.1";
static const char*                         metadata__2__file_path     = "data/metadata.bbfpm.2";
static BBFPM__Metadata*                    metadata__current          = NULL;
static BBFPM__CorrelatedArbitraryValueList metadata__current__authors = { 0 };
static BBFPM__CorrelatedArbitraryValueList metadata__current__dependencies = { 0 };

void setUp( void )
{
    metadata__current               = NULL;
    metadata__current__authors      = (BBFPM__CorrelatedArbitraryValueList) { 0 };
    metadata__current__dependencies = (BBFPM__CorrelatedArbitraryValueList) { 0 };
};
void tearDown( void )
{
    if ( metadata__current != NULL )
    {
        BBFPM__DestroyMetadata( metadata__current );
        metadata__current = NULL;
    }
    if ( metadata__current__authors.correlations != NULL )
    {
        BBFPM__DestroyCorrelatedArbitraryValueList( &metadata__current__authors );
        metadata__current__authors = (BBFPM__CorrelatedArbitraryValueList) { 0 };
    }
    if ( metadata__current__dependencies.correlations != NULL )
    {
        BBFPM__DestroyCorrelatedArbitraryValueList( &metadata__current__dependencies );
        metadata__current__dependencies = (BBFPM__CorrelatedArbitraryValueList) { 0 };
    }
};

void test__metadata__1__lifecycle( void )
{
    TEST_ASSERT_NOT_NULL( ( metadata__current = BBFPM__LoadMetadata( metadata__1__file_path ) ) );

    TEST_ASSERT_NOT_NULL( BBFPM__GetMetadataName( metadata__current ) );
    TEST_ASSERT_NOT_NULL( BBFPM__GetMetadataRepository( metadata__current ) );
    TEST_ASSERT_NOT_NULL( BBFPM__GetMetadataVersion( metadata__current ) );
    TEST_ASSERT_NOT_NULL( BBFPM__GetMetadataReleaseDate( metadata__current ) );
    TEST_ASSERT_NOT_NULL( BBFPM__GetMetadataLicense( metadata__current ) );
    TEST_ASSERT_NOT_NULL(
        ( metadata__current__authors = BBFPM__GetMetadataAuthors( metadata__current ) )
            .correlations );
    TEST_ASSERT_NOT_NULL(
        ( metadata__current__dependencies = BBFPM__GetMetadataDependencies( metadata__current ) )
            .correlations );
}

void test__metadata__2__lifecycle( void )
{
    TEST_ASSERT_NOT_NULL( ( metadata__current = BBFPM__LoadMetadata( metadata__2__file_path ) ) );
    TEST_ASSERT_NULL( BBFPM__GetMetadataRepository( metadata__current ) );
    TEST_ASSERT_NULL( BBFPM__GetMetadataVersion( metadata__current ) );
    TEST_ASSERT_NULL( BBFPM__GetMetadataReleaseDate( metadata__current ) );
    TEST_ASSERT_NULL( BBFPM__GetMetadataLicense( metadata__current ) );
    TEST_ASSERT_NULL( BBFPM__GetMetadataDependencies( metadata__current ).correlations );
}

int main( void )
{
    UNITY_BEGIN();
    RUN_TEST( test__metadata__1__lifecycle );
    RUN_TEST( test__metadata__2__lifecycle );
    return UNITY_END();
}
