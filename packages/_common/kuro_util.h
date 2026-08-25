#ifndef KUROGANE_PACKAGE_UTIL_H
#define KUROGANE_PACKAGE_UTIL_H

#include <stdint.h>
#include <stdio.h>

static inline void ku_pkg_print_u64(uint64_t value) {
    char buffer[32];
    size_t cursor = sizeof(buffer);
    do {
        buffer[--cursor] = (char)('0' + (value % UINT64_C(10)));
        value /= UINT64_C(10);
    } while (value != 0U && cursor != 0U);
    while (cursor < sizeof(buffer)) putchar(buffer[cursor++]);
}

static inline void ku_pkg_print_bytes(uint64_t bytes) {
    const uint64_t mib = bytes / UINT64_C(1048576);
    const uint64_t kib = bytes / UINT64_C(1024);
    if (mib != 0U) {
        ku_pkg_print_u64(mib);
        printf(" MiB");
    } else {
        ku_pkg_print_u64(kib);
        printf(" KiB");
    }
}

static inline void ku_pkg_print_ipv4(const uint8_t address[4]) {
    printf("%u.%u.%u.%u",
           (unsigned)address[0], (unsigned)address[1],
           (unsigned)address[2], (unsigned)address[3]);
}

static inline void ku_pkg_print_duration(uint64_t seconds) {
    const uint64_t days = seconds / UINT64_C(86400);
    const uint64_t hours = (seconds / UINT64_C(3600)) % UINT64_C(24);
    const uint64_t minutes = (seconds / UINT64_C(60)) % UINT64_C(60);
    const uint64_t secs = seconds % UINT64_C(60);
    if (days != 0U) { ku_pkg_print_u64(days); printf("d "); }
    ku_pkg_print_u64(hours); printf("h ");
    ku_pkg_print_u64(minutes); printf("m ");
    ku_pkg_print_u64(secs); printf("s");
}

#endif
