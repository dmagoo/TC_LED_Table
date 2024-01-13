#  include <unity.h>
#include "basic_cluster_tests.h"
#include "basic_cluster_manager_tests.h"

int main(int argc, char **argv) {
    UNITY_BEGIN(); // IMPORTANT LINE!

    run_basic_cluster_tests(argc, argv);

    run_basic_cluster_manager_tests(argc, argv);

    UNITY_END();

    return 0;
}
