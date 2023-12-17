#ifndef __EcatBase__
#define __EcatBase__

#include <stdio.h>
#include <string.h>
#include <inttypes.h>

#include "soem_ros2/soem.h"

#define EC_VER1

#define EC_TIMEOUTMON 500

namespace ecat
{
    class EcatBase
    {
    public:
        EcatBase();
        ~EcatBase();

        void EcatStart(char *ifname);
        void EcatSyncMsg(uint8_t *output_data, uint8_t *input_data);
        void EcatStop();

        void SetUserStop(void (*save_stop)());
    private:
        void (*UserStop)();

        int pdo_output_byte = 30;
        int pdo_input_byte = 34;

        char IOmap[4096];
        volatile int wkc;
        int expectedWKC;
    };
}

// #include "ecat_can_base/ecat_typedef.h"

#endif