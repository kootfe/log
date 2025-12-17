#include "log.h"

int main()
{
    kl_logger_t log = kl_setup_logger(stderr, OFF);
    kl_set_def_logger(log);
    warnf("test\n");
    return 0;
}
