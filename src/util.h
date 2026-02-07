#pragma once
#include <stddef.h>
#include <stdint.h>

uint32_t simple_crc32(const uint8_t* data, size_t n);

int check_key(const char* user_key);
int parse_int(const char* s);
int compute_score(int a, int b, int c);

void do_file_ops(const char* path);
void do_os_probes(void);
void busy_wait_ms(int ms);