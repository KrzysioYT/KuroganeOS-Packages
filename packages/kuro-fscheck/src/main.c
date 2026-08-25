#include <kurogane/kurogane.h>
#include <stdio.h>
#include <string.h>
#include "kuro_util.h"

static int check_path(const char* path, uint32_t expected_type) {
    ku_file_stat info;
    memset(&info, 0, sizeof(info));
    if (ku_file_stat_path(path, strlen(path), &info) != KU_STATUS_OK) {
        printf("%-10s FAIL\n", path);
        return 0;
    }
    printf("%-10s PASS  type=%u  size=", path, (unsigned)info.type);
    ku_pkg_print_bytes(info.size);
    putchar('\n');
    return expected_type == KU_FILE_TYPE_UNKNOWN || info.type == expected_type;
}

static unsigned count_apps(void) {
    const char path[] = "/apps";
    ku_result_t opened = ku_file_open_ex(path, sizeof(path) - 1U, KU_FILE_OPEN_DIRECTORY);
    unsigned count = 0U;
    if (opened < 0) return 0U;
    for (;;) {
        ku_directory_entry entry;
        memset(&entry, 0, sizeof(entry));
        if (ku_file_readdir((ku_file_t)opened, &entry) != KU_STATUS_OK) break;
        ++count;
    }
    (void)ku_file_close((ku_file_t)opened);
    return count;
}

int main(void) {
    int ok = 1;
    unsigned apps;
    puts("KuroganeOS // filesystem check");
    ok &= check_path("/", KU_FILE_TYPE_DIRECTORY);
    ok &= check_path("/apps", KU_FILE_TYPE_DIRECTORY);
    ok &= check_path("/home", KU_FILE_TYPE_DIRECTORY);
    ok &= check_path("/etc", KU_FILE_TYPE_DIRECTORY);
    apps = count_apps();
    printf("/apps entries  %u\n", apps);
    printf("result         %s\n", ok ? "HEALTHY" : "DEGRADED");
    return ok ? 0 : 2;
}
