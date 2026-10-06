/* Far-call half of the run-pipeline self-test.  Separate module on purpose: in
   the large model a call to another module becomes `lcall`, and a probe that
   stayed in one file would never exercise that path. */
#include <stdio.h>

void probe_far(void)
{
    printf("probe: far call ok\r\n");
}