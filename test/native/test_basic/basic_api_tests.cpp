#include <unity.h>

#include "api/LedTableApi.h"
#include "native/ClusterManager.h"

#include "config/make_cluster_config.h"

void test_basic_api_creation() {
    ClusterManager clusterManager(makeClusterConfigs());
    LedTableApi api(clusterManager);
    api.setSuppressMessages(true);
    api.setSuppressRefresh(true);

    // TEST_ASSERT_EQUAL_BOOL(api.suppressMessages, true);
    // TEST_ASSERT_EQUAL_BOOL(api.suppressRefresh, true);

}

int run_basic_api_tests(int argc, char **argv) {
    RUN_TEST(test_basic_api_creation);
    return 0;
}