/* Self-test for the DOSBox run pipeline.
 *
 * `check.py` only ever builds, so nothing in this repo had confirmed that a
 * linked .EXE from this toolchain can actually start and print.  When PETE.EXE
 * came back silent this is what separated "the harness cannot run anything"
 * from "PETE.EXE hangs before its first output".
 *
 * Prints one line per stage, then leaves with a distinctive exit code so a
 * caller can tell a clean return from a kill.
 */
#include <stdio.h>
#include <string.h>

void probe_far(void);

int main(int argc, char far *argv[], char far *envp[])
{
    setvbuf(stdout, NULL, _IONBF, 0);

    printf("probe: startup ok\n");
    printf("probe: argc=%d\n", argc);

    /* Replicates the first statement of main() in src/main.c.  With argc==1
       argv[1] is a null far pointer, so this reads the interrupt vector table
       at 0000:0000 -- the one thing PETE.EXE does before any of its own output
       that the rest of this probe does not. */
    printf("probe: before strcmp\n");
    if (strcmp(argv[1], "PETE") == 0)
        printf("probe: strcmp matched\n");
    printf("probe: after strcmp\n");

    /* Exercise a far call, which is the thing PETE.EXE depends on and which a
       small-model probe would not catch. */
    probe_far();

    printf("probe: done\n");
    return 42;
}