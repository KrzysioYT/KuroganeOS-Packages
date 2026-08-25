#include <kurogane/kurogane.h>
#include <stdio.h>
#include <string.h>
#include "kuro_util.h"

int main(void) {
    const char path[] = "/apps";
    ku_result_t opened = ku_file_open_ex(path, sizeof(path) - 1U, KU_FILE_OPEN_DIRECTORY);
    unsigned count = 0U;
    if (opened < 0) {
        puts("kuro-apps: cannot open /apps");
        return 1;
    }
    puts("Installed /apps entries:");
    for (;;) {
        ku_directory_entry entry;
        ku_status_t status;
        memset(&entry, 0, sizeof(entry));
        status = ku_file_readdir((ku_file_t)opened, &entry);
        if (status != KU_STATUS_OK) break;
        printf("  %s", entry.name);
        if (entry.type == KU_FILE_TYPE_DIRECTORY) printf("/");
        if (entry.size != 0U) { printf("  "); ku_pkg_print_bytes(entry.size); }
        putchar('\n');
        ++count;
    }
    (void)ku_file_close((ku_file_t)opened);
    printf("entries: %u\n", count);
    return 0;
}
