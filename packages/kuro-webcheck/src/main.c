#include <kurogane/kurogane.h>
#include <stdio.h>
#include <string.h>
#include "kuro_util.h"

static unsigned char response[65536];

int main(void) {
    ku_http_request request;
    ku_status_t status;
    memset(&request, 0, sizeof(request));
    request.structure_size = sizeof(request);
    strlcpy(request.host, "example.com", sizeof(request.host));
    strlcpy(request.path, "/", sizeof(request.path));
    request.output = response;
    request.output_capacity = sizeof(response);

    puts("KuroganeOS // HTTPS check");
    status = ku_https_get(&request);
    if (status != KU_STATUS_OK) {
        printf("transport  FAIL status=%d\n", (int)status);
        return 1;
    }
    printf("transport  PASS\n");
    printf("http       %u\n", (unsigned)request.http_status);
    printf("received   "); ku_pkg_print_bytes(request.bytes_received); putchar('\n');
    return request.http_status >= 200U && request.http_status < 400U ? 0 : 2;
}
