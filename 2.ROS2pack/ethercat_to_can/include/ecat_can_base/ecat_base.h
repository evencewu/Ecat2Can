#ifndef __ECAT_BASE__
#define __ECAT_BASE__

#include <stdio.h>
#include <string.h>
#include <inttypes.h>

#include "soem_ros2/soem.h"

#define PDO_OUTPUT_BYTE 33
#define PDO_INPUT_BYTE 32

void EcatStart(char *ifname);
void EcatSyncMsg(uint8_t *output_data, uint8_t *input_data);
void EcatStop();

#define EC_TIMEOUTMON 500

#include "ecat_can_base/ecat_typedef.h"

#endif