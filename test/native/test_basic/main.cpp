#include <unity.h>
#include "basic_cluster_tests.h"
#include "basic_cluster_manager_tests.h"
#include "basic_message_tests.h"
#include "basic_api_tests.h"

void setUp(void) {
    // This will run before each test
}

void tearDown(void) {
    // This will run after each test
}

int main(int argc, char **argv) {
    UNITY_BEGIN(); // IMPORTANT LINE!

    run_basic_cluster_tests(argc, argv);
    run_basic_cluster_manager_tests(argc, argv);
    run_basic_api_tests(argc, argv);
    run_basic_message_tests(argc, argv);
    UNITY_END();

    return 0;
}
