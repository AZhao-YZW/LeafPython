#include <stdio.h>
#include "leafpy_api.h"

int main()
{
    int ret;

    printf("===== Running repl_mode_demo =====\n");

    ret = leafpy_init();
    if (ret != 0) {
        printf("leafpy_init failed, ret[%d]\n", ret);
        return -1;
    }

    ret = LEAFPY_REGISTER_CFUNC("test",
                                "test_func",
                                CFUNC_TYPE(void, 0),
                                3,
                                CFUNC_TYPE(u16, 0),
                                CFUNC_TYPE(float, 1),
                                CFUNC_TYPE(u8, 2));
    if (ret != 0) {
        printf("LEAFPY_REGISTER_CFUNC failed, ret[%d]\n", ret);
        return -1;
    }
    return 0;
}