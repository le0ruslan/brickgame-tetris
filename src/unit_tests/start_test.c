#include <check.h>

#include "test.h"

int main(void) {
  int failed = 0;
  Suite *suite[] = {suite_calc_test(), NULL};

  for (int i = 0; suite[i] != NULL; i++) {  // (&& failed == 0)
    SRunner *sr = srunner_create(suite[i]);

    srunner_set_fork_status(sr, CK_NOFORK);
    srunner_run_all(sr, CK_NORMAL);

    failed += srunner_ntests_failed(sr);
    srunner_free(sr);
  }
  printf("\033[42m***** FAILED TESTS: %d *****\033[0m\n", failed);

  return failed == 0 ? 0 : 1;
}
