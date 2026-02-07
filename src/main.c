#include <stdio.h>
#include <string.h>
#include "util.h"

// Інформативні strings для практик зі strings/xrefs/логікою
static const char* BANNER =
    "Reverse Engineering Foundations Demo App v1.0\n"
    "mode=demo; config=default; path=./re_demo_output.txt; key=???\n";

static const char* MSG_OK   = "success: access granted";
static const char* MSG_FAIL = "error: access denied";
static const char* MSG_HELP =
    "usage:\n"
    "  re_demo --info\n"
    "  re_demo --run <key>\n"
    "  re_demo --calc <a> <b> <c>\n";

int main(int argc, char** argv) {
    printf("%s\n", BANNER);

    if (argc < 2) {
        printf("%s", MSG_HELP);
        return 0;
    }

    if (strcmp(argv[1], "--info") == 0) {
        printf("version=1.0; build=2025; platform=%s\n",
#if defined(_WIN32)
               "windows"
#elif defined(__APPLE__)
               "macos"
#else
               "linux"
#endif
        );
        do_os_probes();
        do_file_ops("./re_demo_output.txt");
        return 0;
    }

    if (strcmp(argv[1], "--calc") == 0 && argc >= 5) {
        int a = parse_int(argv[2]);
        int b = parse_int(argv[3]);
        int c = parse_int(argv[4]);
        int score = compute_score(a, b, c);
        printf("result=%d\n", score);
        return 0;
    }

    if (strcmp(argv[1], "--run") == 0 && argc >= 3) {
        const char* key = argv[2];

        // Ознаки для спостереження + мінімальні артефакти (без шкідливої поведінки)
        do_os_probes();
        do_file_ops("./re_demo_output.txt");

        // Контрольна точка для ПР24: if (...) -> умовний перехід
        if (check_key(key)) {
            printf("%s\n", MSG_OK);
            return 0;
        } else {
            printf("%s\n", MSG_FAIL);
            return 1;
        }
    }

    printf("error: invalid arguments\n");
    printf("%s", MSG_HELP);
    return 2;
}