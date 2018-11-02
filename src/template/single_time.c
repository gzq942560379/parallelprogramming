#include <stdlib.h>
#include "time_utils.h"

int main(int argc, char **argv) {
    timer timer;
    start(&timer);

    end(&timer);
    print_milli_interval(&timer);
    exit(0);
}
