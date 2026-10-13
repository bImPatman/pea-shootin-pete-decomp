/* Run-mode stand-ins for the callees of main() that are not reconstructed yet.
 *
 * `main.c` is byte-exact, so nothing in it may change.  Two consequences shape
 * this file:
 *
 *  - The nine *same-module* callees (m_1df6, m_38a2, m_2415, m_24e7, m_0dd6,
 *    m_154f, m_013b, m_34e5, m_35dd) are declared as empty bodies inside
 *    main.c and have to stay that way.  main() reaches them with `call rel32`,
 *    so growing any one of them shifts the targets of the calls before it and
 *    breaks main's own bytes.  They run as genuine no-ops and cannot log; the
 *    banner below names them instead.
 *
 *  - The cross-module callees live in their own files so each can be replaced
 *    wholesale.  All of main()'s far callees are now linked from their own
 *    reconstructions except m_f44b, whose stand-in is in f44bstub.c next door.
 *    This file is kept for the shared reporting helpers (stub_boot / emit /
 *    note) that f44bstub.c calls, plus the run-time storage below.
 *
 *  - m_f44b's stand-in is *not* here, but in f44bstub.c next door.  That one is
 *    a genuine reconstruction (src/f44b.c) held back only because it writes VGA
 *    CRTC registers, and --with-partial swaps one for the other.  A public symbol
 *    cannot be defined in both a file both builds link and a file only one links,
 *    so the one that moves between builds has to live in the file that moves.
 *
 * Why every message goes to two places
 * ------------------------------------
 * main() is an unconditional `for(;;)` that nothing reconstructed yet can break
 * out of, so the program is always killed by the runner's timeout.  Under
 * DOSBox X that matters: console output, the shell's `>>` redirect and even a
 * guest fflush all sit in a buffer that is only written out when the process
 * exits, so a killed run leaves an empty log however much it printed.  Opening
 * the log with "a" and closing it per line is the one thing that forces DOSBox
 * to finalise the file, so the text survives the kill.  The printf still goes
 * to the console for anyone watching the DOSBox window.
 */
#include <stdio.h>

/* Read by tools/py/run_game.py after the run. */
#define GUEST_LOG "C:\\OUT\\GUEST.LOG"

/* Declared in main.c and null in a freshly linked image. */
extern char far *g_3858;
extern char far *g_27aa;      /* m_e256 and buf_to_vga */
extern char far *g_34a9;      /* node_free */

/* Real storage for the reconstructed callees that dereference their argument.
   statebak() reads state at +2 and copies 0x300 bytes when it is 1;
   clrbit3() writes +3; node_free() walks the count at +0 and clears the entries
   behind it; m_e256() and buf_to_vga() both read the entry count at +0x109.
   Left pointing at segment 0 they would scribble on the interrupt vector
   table, so point them at zeroed memory instead: statebak then declines to
   copy, clrbit3 writes to our own buffer, and every loop bound above reads 0
   and so runs zero times.
   rec[] is big enough for every one of them: 0x109 + 1 for the count byte and
   0x50 + 0xB9 for buf_to_vga's two parallel arrays. */
static unsigned char rec[0x308];

static int booted = 0;

void emit(char *line)
{
    FILE *f;

    printf("%s\n", line);

    f = fopen(GUEST_LOG, "a");
    if (!f)
        return;               /* logging must never take the game down */
    fprintf(f, "%s\n", line);
    fclose(f);
}

void stub_boot(void)
{
    int i;

    if (booted)
        return;
    booted = 1;

    /* main() never returns, so a buffered stdout is never flushed and the
       console stays blank however much gets printed. */
    setvbuf(stdout, NULL, _IONBF, 0);

    for (i = 0; i < 0x308; i++)
        rec[i] = 0;
    g_3858 = (char far *)rec;
    g_27aa = (char far *)rec;
    g_34a9 = (char far *)rec;

    /* The counts below describe the default build.  This file is linked by
       --with-partial too, and that build swaps in the exact m_f44b, so its real
       list is one longer and it prints no m_f44b gap line -- that difference is
       the whole way to tell the two builds apart from the log alone. */
    emit("[stub] main() has 18 direct callees; 17 are reconstructed and linked");
    emit("[stub]   real: key_poll key_read statebak clrbit3 m_1a40");
    emit("[stub]         m_b159 m_f44c m_e256 node_free buf_to_vga");
    emit("[stub]   real in src/mcallees.c:");
    emit("[stub]     m_1df6 m_24e7 m_013b m_2c80 m_0dd6 m_b11a m_154f m_2415 m_34e5 m_38a2");
    emit("[stub]     m_35dd m_f44d");
    emit("[stub]   not reconstructed: m_f44b (stand-in)");
    emit("[stub] next gap reached will be reported below");
}

void note(char *name, char *target)
{
    char line[160];

    sprintf(line, "[stub] %-11s -> %-16s not reconstructed, ignored",
            name, target);
    emit(line);
}

/* m_f44d is now the exact src/f44d.c, so no stand-in is needed for it. */

/* m_f44b's stand-in lives in f44bstub.c, because --with-partial replaces it with
   the exact src/f44b.c and two definitions of _m_f44b make Turbo Link run away. */

/* m_e256 (src/e256.c), m_f44c (src/dacupd.c) and m_b159 (src/mb159.c) are all
   byte-exact and linked from there, so no stand-in is needed for any of them. */

/* node_free (src/nodefree.c, exact) and buf_to_vga (src/buftovg.c, exact) are
   linked from those files, so no stand-in is needed for either. */