#include "ecat_can_base/ecat_base.h"

#include <unistd.h>

namespace ecat
{
    EcatBase::EcatBase()
    {
        memset(&packet_tx, 0, sizeof(Ecat_Outputs_Pack));
        memset(&packet_rx, 0, sizeof(Ecat_Inputs_Pack));
    }

    EcatBase::~EcatBase()
    {
        // EcatStop();
    }

    /// @brief Set up your safety stop function
    /// @param save_stop Handle to a user-defined safe stop function
    void EcatBase::SetUserStop(void (*save_stop)())
    {
        UserStop = save_stop;
    }

    /// @brief Start your ecat port
    /// @param ifname Network port name
    void EcatBase::EcatStart(char *ifname)
    {
        int chk;
        if (ec_init(ifname))
        {
            // ec_DCtime = 1000;
            printf("ec_init on %s succeeded.\n", ifname);
            if (ec_config_init(FALSE) > 0)
            {
                printf("%d slaves found and configured.\n", ec_slavecount);

                ec_config_map(&IOmap);

                printf("Slaves mapped, state to SAFE_OP.\n");
                /* wait for all slaves to reach SAFE_OP state */
                ec_statecheck(0, EC_STATE_SAFE_OP, EC_TIMEOUTSTATE * 4);

                ec_configdc();

                printf("Request operational state for all slaves\n");

                expectedWKC = (ec_group[0].outputsWKC * 2) + ec_group[0].inputsWKC;
                ec_slave[0].state = EC_STATE_OPERATIONAL;
                /* send one valid process data to make outputs in slaves happy*/

                ec_send_processdata();
                ec_receive_processdata(EC_TIMEOUTRET);
                /* request OP state for all slaves */
                ec_writestate(0);

                chk = 200;
                /* wait for all slaves to reach OP state */
                do
                {
                    ec_send_processdata();
                    ec_receive_processdata(EC_TIMEOUTRET);
                    ec_statecheck(0, EC_STATE_OPERATIONAL, EC_TIMEOUTRET);
                } while (chk-- && (ec_slave[0].state != EC_STATE_OPERATIONAL));
            }
            else
            {
                printf("No slaves found!\n");
            }
        }
        else
        {
            printf("No socket connection on %s\nExcecute as root\n", ifname);
        }
    }

    /// @brief Data sending and receiving, calling in a loop
    /// @param output_data Pointer to the output data
    /// @param input_data  Pointer to the input data
    void EcatBase::EcatSyncMsg()
    {
        if (ec_slave[0].state == EC_STATE_OPERATIONAL)
        {
            memcpy(ec_slave[0].outputs, &packet_tx, pdo_output_byte);

            ec_send_processdata();

            wkc = ec_receive_processdata(EC_TIMEOUTRET);

            if (wkc >= expectedWKC)
            {
                memcpy(&packet_rx, ec_slave[0].inputs, pdo_input_byte);
            }

            osal_usleep(500);
        }
        else
        {
            int i = 0;
            printf("Not all slaves reached operational state.\n");
            ec_readstate();
            for (i = 1; i <= ec_slavecount; i++)
            {
                if (ec_slave[i].state != EC_STATE_OPERATIONAL)
                {
                    printf("Slave %d State=0x%2.2x StatusCode=0x%4.4x : %s\n",
                           i, ec_slave[i].state, ec_slave[i].ALstatuscode, ec_ALstatuscode2string(ec_slave[i].ALstatuscode));
                }
            }
        }
    }

    /// @brief Stop the ecat connection and perform a secure stop before that
    void EcatBase::EcatStop()
    {
        UserStop();
        printf("\nRequest init state for all slaves\n");
        ec_slave[0].state = EC_STATE_INIT;
        /* request INIT state for all slaves */
        ec_writestate(0);
    }
}
