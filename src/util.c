#include "util.h"
#include <stdio.h>
#include <string.h>
#include <time.h>

#if defined(_WIN32)
  #include <windows.h>
  #include <winreg.h>
#else
  #include <unistd.h>
  #include <fcntl.h>
#endif

// --- CRC32 (навчальний) ---
uint32_t simple_crc32(const uint8_t* data, size_t n) {
    uint32_t crc = 0xFFFFFFFFu;
    for (size_t i = 0; i < n; i++) {
        crc ^= data[i];
        for (int j = 0; j < 8; j++) {
            uint32_t mask = (uint32_t)-(int)(crc & 1u);
            crc = (crc >> 1) ^ (0xEDB88320u & mask);
        }
    }
    return ~crc;
}

// --- Допоміжні функції для розбору параметрів / дизасемблювання ---
int parse_int(const char* s) {
    int sign = 1;
    int val = 0;
    if (!s) return 0;
    if (*s == '-') { sign = -1; s++; }
    while (*s) {
        if (*s < '0' || *s > '9') break;
        val = val * 10 + (*s - '0');
        s++;
    }
    return val * sign;
}

int compute_score(int a, int b, int c) {
    int score = a * 3 + b * 2 + c;
    if (score < 0) score = -score;
    if (score > 100) score = 100;
    return score;
}

// --- "Key check" для навчального патчингу (ПР24) ---
int check_key(const char* user_key) {
    // Це НЕ захист, лише навчальний приклад для демонстрації умов/гілок.
    const char* salt = "KSU-RE-2025";
    if (!user_key) return 0;

    uint32_t crc = simple_crc32((const uint8_t*)user_key, strlen(user_key));
    uint32_t crc2 = simple_crc32((const uint8_t*)salt, strlen(salt));
    uint32_t mixed = (crc ^ (crc2 << 1)) + 0x1234u;

    // Умова зроблена так, щоб її було легко знайти в дизасемблері як cmp/test -> jcc.
    if ((mixed & 0xFFu) == 0x5Au) {
        return 1; // allowed
    }
    return 0; // denied
}

// --- Мінімальні системні дії (ПР19/ПР26): файлова активність ---
void do_file_ops(const char* path) {
    const char* p = path ? path : "re_demo_output.txt";

    FILE* f = fopen(p, "a");
    if (f) {
        fprintf(f, "log: demo write\n");
        fclose(f);
    }

    f = fopen(p, "r");
    if (f) {
        char buf[64] = {0};
        (void)fgets(buf, (int)sizeof(buf), f);
        fclose(f);
    }
}

void busy_wait_ms(int ms) {
    clock_t start = clock();
    clock_t ticks = (clock_t)((double)ms * (double)CLOCKS_PER_SEC / 1000.0);
    while ((clock() - start) < ticks) { /* spin */ }
}

// --- "OS probes": лише як ознаки для спостереження (ПР21), без обходів ---
void do_os_probes(void) {
#if defined(_WIN32)
    // 1) Ознака наявності дебагера (факт для навчальної демонстрації)
    BOOL dbg = IsDebuggerPresent();

    // 2) Легальне читання "звичайного" ключа реєстру (Procmon артефакти)
    HKEY hKey = NULL;
    LONG rc = RegOpenKeyExA(
        HKEY_CURRENT_USER,
        "Software\\Microsoft\\Windows\\CurrentVersion\\Explorer",
        0,
        KEY_READ,
        &hKey
    );
    if (rc == ERROR_SUCCESS && hKey) RegCloseKey(hKey);

    // 3) Простий таймінг (як ознака)
    if (dbg) busy_wait_ms(30);
#else
    // Linux/macOS: легальні системні дії для strace
    (void)getpid();
    busy_wait_ms(10);
#endif
}