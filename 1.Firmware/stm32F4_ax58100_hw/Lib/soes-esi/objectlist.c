#include "esc_coe.h"
#include "utypes.h"
#include <stddef.h>


static const char acName1000[] = "Device Type";
static const char acName1001[] = "Error register";
static const char acName1008[] = "Device Name";
static const char acName1009[] = "Hardware Version";
static const char acName100A[] = "Software Version";
static const char acName1018[] = "Identity Object";
static const char acName1018_00[] = "Max SubIndex";
static const char acName1018_01[] = "Vendor ID";
static const char acName1018_02[] = "Product Code";
static const char acName1018_03[] = "Revision Number";
static const char acName1018_04[] = "Serial Number";
static const char acName10F1[] = "Error Settings";
static const char acName10F1_00[] = "Max SubIndex";
static const char acName10F1_01[] = "Local Error Reaction";
static const char acName10F1_02[] = "SyncErrorCounterLimit";
static const char acName1600[] = "can1_tx";
static const char acName1600_00[] = "Max SubIndex";
static const char acName1600_01[] = "StdId";
static const char acName1600_02[] = "ExtId";
static const char acName1600_03[] = "IDE";
static const char acName1600_04[] = "RTR";
static const char acName1600_05[] = "DLC";
static const char acName1601[] = "can1_tx_data";
static const char acName1601_00[] = "Max SubIndex";
static const char acName1601_01[] = "d1";
static const char acName1601_02[] = "d2";
static const char acName1601_03[] = "d3";
static const char acName1601_04[] = "d4";
static const char acName1601_05[] = "d5";
static const char acName1601_06[] = "d6";
static const char acName1601_07[] = "d7";
static const char acName1601_08[] = "d8";
static const char acName1602[] = "beat_heart";
static const char acName1602_00[] = "Max SubIndex";
static const char acName1602_01[] = "beat_heart";
static const char acName1603[] = "led";
static const char acName1603_00[] = "Max SubIndex";
static const char acName1603_01[] = "L0";
static const char acName1603_02[] = "L1";
static const char acName1603_03[] = "L3";
static const char acName1A00[] = "can1_rx";
static const char acName1A00_00[] = "Max SubIndex";
static const char acName1A00_01[] = "StdId";
static const char acName1A00_02[] = "ExtId";
static const char acName1A00_03[] = "IDE";
static const char acName1A00_04[] = "RTR";
static const char acName1A00_05[] = "DLC";
static const char acName1A01[] = "can1_rx_data";
static const char acName1A01_00[] = "Max SubIndex";
static const char acName1A01_01[] = "d1";
static const char acName1A01_02[] = "d2";
static const char acName1A01_03[] = "d3";
static const char acName1A01_04[] = "d4";
static const char acName1A01_05[] = "d5";
static const char acName1A01_06[] = "d6";
static const char acName1A01_07[] = "d7";
static const char acName1A01_08[] = "d8";
static const char acName1C00[] = "Sync Manager Communication Type";
static const char acName1C00_00[] = "Max SubIndex";
static const char acName1C00_01[] = "Communications Type SM0";
static const char acName1C00_02[] = "Communications Type SM1";
static const char acName1C00_03[] = "Communications Type SM2";
static const char acName1C00_04[] = "Communications Type SM3";
static const char acName1C12[] = "Sync Manager 2 PDO Assignment";
static const char acName1C12_00[] = "Max SubIndex";
static const char acName1C12_01[] = "PDO Mapping";
static const char acName1C12_02[] = "PDO Mapping";
static const char acName1C12_03[] = "PDO Mapping";
static const char acName1C12_04[] = "PDO Mapping";
static const char acName1C13[] = "Sync Manager 3 PDO Assignment";
static const char acName1C13_00[] = "Max SubIndex";
static const char acName1C13_01[] = "PDO Mapping";
static const char acName1C13_02[] = "PDO Mapping";
static const char acName1C32[] = "Sync Manager 2 Parameters";
static const char acName1C32_00[] = "Max SubIndex";
static const char acName1C32_01[] = "Sync mode";
static const char acName1C32_02[] = "CycleTime";
static const char acName1C32_03[] = "ShiftTime";
static const char acName1C32_04[] = "Sync modes supported";
static const char acName1C32_05[] = "Minimum Cycle Time";
static const char acName1C33[] = "Sync Manager 3 Parameters";
static const char acName1C33_00[] = "Max SubIndex";
static const char acName1C33_01[] = "Sync mode";
static const char acName1C33_02[] = "CycleTime";
static const char acName1C33_03[] = "ShiftTime";
static const char acName1C33_04[] = "Sync modes supported";
static const char acName1C33_05[] = "Minimum Cycle Time";
static const char acName6000[] = "can1_rx";
static const char acName6000_00[] = "Max SubIndex";
static const char acName6000_01[] = "StdId";
static const char acName6000_02[] = "ExtId";
static const char acName6000_03[] = "IDE";
static const char acName6000_04[] = "RTR";
static const char acName6000_05[] = "DLC";
static const char acName6001[] = "can1_rx_data";
static const char acName6001_00[] = "Max SubIndex";
static const char acName6001_01[] = "d1";
static const char acName6001_02[] = "d2";
static const char acName6001_03[] = "d3";
static const char acName6001_04[] = "d4";
static const char acName6001_05[] = "d5";
static const char acName6001_06[] = "d6";
static const char acName6001_07[] = "d7";
static const char acName6001_08[] = "d8";
static const char acName6060[] = "Modes of operation";
static const char acName6061[] = "Mode of operation display";
static const char acName6502[] = "Supported drive modes";
static const char acName7000[] = "can1_tx";
static const char acName7000_00[] = "Max SubIndex";
static const char acName7000_01[] = "StdId";
static const char acName7000_02[] = "ExtId";
static const char acName7000_03[] = "IDE";
static const char acName7000_04[] = "RTR";
static const char acName7000_05[] = "DLC";
static const char acName7001[] = "can1_tx_data";
static const char acName7001_00[] = "Max SubIndex";
static const char acName7001_01[] = "d1";
static const char acName7001_02[] = "d2";
static const char acName7001_03[] = "d3";
static const char acName7001_04[] = "d4";
static const char acName7001_05[] = "d5";
static const char acName7001_06[] = "d6";
static const char acName7001_07[] = "d7";
static const char acName7001_08[] = "d8";
static const char acName7002[] = "beat_heart";
static const char acName7003[] = "led";
static const char acName7003_00[] = "Max SubIndex";
static const char acName7003_01[] = "L0";
static const char acName7003_02[] = "L1";
static const char acName7003_03[] = "L3";

const _objd SDO1000[] =
{
  {0x0, DTYPE_UNSIGNED32, 32, ATYPE_RO, acName1000, 5001, NULL},
};
const _objd SDO1001[] =
{
  {0x0, DTYPE_UNSIGNED8, 8, ATYPE_RO, acName1001, 0, &Obj.Error_register},
};
const _objd SDO1008[] =
{
  {0x0, DTYPE_VISIBLE_STRING, 64, ATYPE_RO, acName1008, 0, "Ecat2can"},
};
const _objd SDO1009[] =
{
  {0x0, DTYPE_VISIBLE_STRING, 40, ATYPE_RO, acName1009, 0, "0.0.1"},
};
const _objd SDO100A[] =
{
  {0x0, DTYPE_VISIBLE_STRING, 40, ATYPE_RO, acName100A, 0, "0.0.1"},
};
const _objd SDO1018[] =
{
  {0x00, DTYPE_UNSIGNED8, 8, ATYPE_RO, acName1018_00, 4, NULL},
  {0x01, DTYPE_UNSIGNED32, 32, ATYPE_RO, acName1018_01, 0, NULL},
  {0x02, DTYPE_UNSIGNED32, 32, ATYPE_RO, acName1018_02, 700707, NULL},
  {0x03, DTYPE_UNSIGNED32, 32, ATYPE_RO, acName1018_03, 2, NULL},
  {0x04, DTYPE_UNSIGNED32, 32, ATYPE_RO, acName1018_04, 1, &Obj.serial},
};
const _objd SDO10F1[] =
{
  {0x00, DTYPE_UNSIGNED8, 8, ATYPE_RO, acName10F1_00, 2, NULL},
  {0x01, DTYPE_UNSIGNED32, 32, ATYPE_RO, acName10F1_01, 0, &Obj.Error_Settings.Local_Error_Reaction},
  {0x02, DTYPE_UNSIGNED32, 32, ATYPE_RO, acName10F1_02, 200, &Obj.Error_Settings.SyncErrorCounterLimit},
};
const _objd SDO1600[] =
{
  {0x00, DTYPE_UNSIGNED8, 8, ATYPE_RO, acName1600_00, 5, NULL},
  {0x01, DTYPE_UNSIGNED32, 32, ATYPE_RO, acName1600_01, 0x70000120, NULL},
  {0x02, DTYPE_UNSIGNED32, 32, ATYPE_RO, acName1600_02, 0x70000208, NULL},
  {0x03, DTYPE_UNSIGNED32, 32, ATYPE_RO, acName1600_03, 0x70000308, NULL},
  {0x04, DTYPE_UNSIGNED32, 32, ATYPE_RO, acName1600_04, 0x70000408, NULL},
  {0x05, DTYPE_UNSIGNED32, 32, ATYPE_RO, acName1600_05, 0x70000508, NULL},
};
const _objd SDO1601[] =
{
  {0x00, DTYPE_UNSIGNED8, 8, ATYPE_RO, acName1601_00, 8, NULL},
  {0x01, DTYPE_UNSIGNED32, 32, ATYPE_RO, acName1601_01, 0x70010108, NULL},
  {0x02, DTYPE_UNSIGNED32, 32, ATYPE_RO, acName1601_02, 0x70010208, NULL},
  {0x03, DTYPE_UNSIGNED32, 32, ATYPE_RO, acName1601_03, 0x70010308, NULL},
  {0x04, DTYPE_UNSIGNED32, 32, ATYPE_RO, acName1601_04, 0x70010408, NULL},
  {0x05, DTYPE_UNSIGNED32, 32, ATYPE_RO, acName1601_05, 0x70010508, NULL},
  {0x06, DTYPE_UNSIGNED32, 32, ATYPE_RO, acName1601_06, 0x70010608, NULL},
  {0x07, DTYPE_UNSIGNED32, 32, ATYPE_RO, acName1601_07, 0x70010708, NULL},
  {0x08, DTYPE_UNSIGNED32, 32, ATYPE_RO, acName1601_08, 0x70010808, NULL},
};
const _objd SDO1602[] =
{
  {0x00, DTYPE_UNSIGNED8, 8, ATYPE_RO, acName1602_00, 1, NULL},
  {0x01, DTYPE_UNSIGNED32, 32, ATYPE_RO, acName1602_01, 0x70020008, NULL},
};
const _objd SDO1603[] =
{
  {0x00, DTYPE_UNSIGNED8, 8, ATYPE_RO, acName1603_00, 3, NULL},
  {0x01, DTYPE_UNSIGNED32, 32, ATYPE_RO, acName1603_01, 0x70030101, NULL},
  {0x02, DTYPE_UNSIGNED32, 32, ATYPE_RO, acName1603_02, 0x70030201, NULL},
  {0x03, DTYPE_UNSIGNED32, 32, ATYPE_RO, acName1603_03, 0x70030301, NULL},
};
const _objd SDO1A00[] =
{
  {0x00, DTYPE_UNSIGNED8, 8, ATYPE_RO, acName1A00_00, 5, NULL},
  {0x01, DTYPE_UNSIGNED32, 32, ATYPE_RO, acName1A00_01, 0x60000120, NULL},
  {0x02, DTYPE_UNSIGNED32, 32, ATYPE_RO, acName1A00_02, 0x60000208, NULL},
  {0x03, DTYPE_UNSIGNED32, 32, ATYPE_RO, acName1A00_03, 0x60000308, NULL},
  {0x04, DTYPE_UNSIGNED32, 32, ATYPE_RO, acName1A00_04, 0x60000408, NULL},
  {0x05, DTYPE_UNSIGNED32, 32, ATYPE_RO, acName1A00_05, 0x60000508, NULL},
};
const _objd SDO1A01[] =
{
  {0x00, DTYPE_UNSIGNED8, 8, ATYPE_RO, acName1A01_00, 8, NULL},
  {0x01, DTYPE_UNSIGNED32, 32, ATYPE_RO, acName1A01_01, 0x60010108, NULL},
  {0x02, DTYPE_UNSIGNED32, 32, ATYPE_RO, acName1A01_02, 0x60010208, NULL},
  {0x03, DTYPE_UNSIGNED32, 32, ATYPE_RO, acName1A01_03, 0x60010308, NULL},
  {0x04, DTYPE_UNSIGNED32, 32, ATYPE_RO, acName1A01_04, 0x60010408, NULL},
  {0x05, DTYPE_UNSIGNED32, 32, ATYPE_RO, acName1A01_05, 0x60010508, NULL},
  {0x06, DTYPE_UNSIGNED32, 32, ATYPE_RO, acName1A01_06, 0x60010608, NULL},
  {0x07, DTYPE_UNSIGNED32, 32, ATYPE_RO, acName1A01_07, 0x60010708, NULL},
  {0x08, DTYPE_UNSIGNED32, 32, ATYPE_RO, acName1A01_08, 0x60010808, NULL},
};
const _objd SDO1C00[] =
{
  {0x00, DTYPE_UNSIGNED8, 8, ATYPE_RO, acName1C00_00, 4, NULL},
  {0x01, DTYPE_UNSIGNED8, 8, ATYPE_RO, acName1C00_01, 1, NULL},
  {0x02, DTYPE_UNSIGNED8, 8, ATYPE_RO, acName1C00_02, 2, NULL},
  {0x03, DTYPE_UNSIGNED8, 8, ATYPE_RO, acName1C00_03, 3, NULL},
  {0x04, DTYPE_UNSIGNED8, 8, ATYPE_RO, acName1C00_04, 4, NULL},
};
const _objd SDO1C12[] =
{
  {0x00, DTYPE_UNSIGNED8, 8, ATYPE_RO, acName1C12_00, 4, NULL},
  {0x01, DTYPE_UNSIGNED16, 16, ATYPE_RO, acName1C12_01, 0x1600, NULL},
  {0x02, DTYPE_UNSIGNED16, 16, ATYPE_RO, acName1C12_02, 0x1601, NULL},
  {0x03, DTYPE_UNSIGNED16, 16, ATYPE_RO, acName1C12_03, 0x1602, NULL},
  {0x04, DTYPE_UNSIGNED16, 16, ATYPE_RO, acName1C12_04, 0x1603, NULL},
};
const _objd SDO1C13[] =
{
  {0x00, DTYPE_UNSIGNED8, 8, ATYPE_RO, acName1C13_00, 2, NULL},
  {0x01, DTYPE_UNSIGNED16, 16, ATYPE_RO, acName1C13_01, 0x1A00, NULL},
  {0x02, DTYPE_UNSIGNED16, 16, ATYPE_RO, acName1C13_02, 0x1A01, NULL},
};
const _objd SDO1C32[] =
{
  {0x00, DTYPE_UNSIGNED8, 8, ATYPE_RO, acName1C32_00, 5, NULL},
  {0x01, DTYPE_UNSIGNED16, 16, ATYPE_RWpre, acName1C32_01, 1, &Obj.Sync_Manager_2_Parameters.Sync_mode},
  {0x02, DTYPE_UNSIGNED32, 32, ATYPE_RO, acName1C32_02, 0, &Obj.Sync_Manager_2_Parameters.CycleTime},
  {0x03, DTYPE_UNSIGNED32, 32, ATYPE_RO, acName1C32_03, 0, &Obj.Sync_Manager_2_Parameters.ShiftTime},
  {0x04, DTYPE_UNSIGNED16, 16, ATYPE_RO, acName1C32_04, 6, &Obj.Sync_Manager_2_Parameters.Sync_modes_supported},
  {0x05, DTYPE_UNSIGNED32, 32, ATYPE_RO, acName1C32_05, 125000, &Obj.Sync_Manager_2_Parameters.Minimum_Cycle_Time},
};
const _objd SDO1C33[] =
{
  {0x00, DTYPE_UNSIGNED8, 8, ATYPE_RO, acName1C33_00, 5, NULL},
  {0x01, DTYPE_UNSIGNED16, 16, ATYPE_RWpre, acName1C33_01, 1, &Obj.Sync_Manager_3_Parameters.Sync_mode},
  {0x02, DTYPE_UNSIGNED32, 32, ATYPE_RO, acName1C33_02, 0, &Obj.Sync_Manager_3_Parameters.CycleTime},
  {0x03, DTYPE_UNSIGNED32, 32, ATYPE_RO, acName1C33_03, 0, &Obj.Sync_Manager_3_Parameters.ShiftTime},
  {0x04, DTYPE_UNSIGNED16, 16, ATYPE_RO, acName1C33_04, 6, &Obj.Sync_Manager_3_Parameters.Sync_modes_supported},
  {0x05, DTYPE_UNSIGNED32, 32, ATYPE_RO, acName1C33_05, 125000, &Obj.Sync_Manager_3_Parameters.Minimum_Cycle_Time},
};
const _objd SDO6000[] =
{
  {0x00, DTYPE_UNSIGNED8, 8, ATYPE_RO, acName6000_00, 5, NULL},
  {0x01, DTYPE_UNSIGNED32, 32, ATYPE_RW, acName6000_01, 0, &Obj.can1_rx.StdId},
  {0x02, DTYPE_UNSIGNED8, 8, ATYPE_RW, acName6000_02, 0, &Obj.can1_rx.ExtId},
  {0x03, DTYPE_UNSIGNED8, 8, ATYPE_RO, acName6000_03, 0, &Obj.can1_rx.IDE},
  {0x04, DTYPE_UNSIGNED8, 8, ATYPE_RO, acName6000_04, 0, &Obj.can1_rx.RTR},
  {0x05, DTYPE_UNSIGNED8, 8, ATYPE_RO, acName6000_05, 0, &Obj.can1_rx.DLC},
};
const _objd SDO6001[] =
{
  {0x00, DTYPE_UNSIGNED8, 8, ATYPE_RO, acName6001_00, 8, NULL},
  {0x01, DTYPE_UNSIGNED8, 8, ATYPE_RO | ATYPE_TXPDO, acName6001_01, 0, &Obj.can1_rx_data[0]},
  {0x02, DTYPE_UNSIGNED8, 8, ATYPE_RO | ATYPE_TXPDO, acName6001_02, 0, &Obj.can1_rx_data[1]},
  {0x03, DTYPE_UNSIGNED8, 8, ATYPE_RO | ATYPE_TXPDO, acName6001_03, 0, &Obj.can1_rx_data[2]},
  {0x04, DTYPE_UNSIGNED8, 8, ATYPE_RO | ATYPE_TXPDO, acName6001_04, 0, &Obj.can1_rx_data[3]},
  {0x05, DTYPE_UNSIGNED8, 8, ATYPE_RO | ATYPE_TXPDO, acName6001_05, 0, &Obj.can1_rx_data[4]},
  {0x06, DTYPE_UNSIGNED8, 8, ATYPE_RO | ATYPE_TXPDO, acName6001_06, 0, &Obj.can1_rx_data[5]},
  {0x07, DTYPE_UNSIGNED8, 8, ATYPE_RO | ATYPE_TXPDO, acName6001_07, 0, &Obj.can1_rx_data[6]},
  {0x08, DTYPE_UNSIGNED8, 8, ATYPE_RO | ATYPE_TXPDO, acName6001_08, 0, &Obj.can1_rx_data[7]},
};
const _objd SDO6060[] =
{
  {0x0, DTYPE_INTEGER8, 8, ATYPE_RO, acName6060, 0, &Obj.Modes_of_operation},
};
const _objd SDO6061[] =
{
  {0x0, DTYPE_INTEGER8, 8, ATYPE_RO, acName6061, 0, &Obj.Mode_of_operation_display},
};
const _objd SDO6502[] =
{
  {0x0, DTYPE_UNSIGNED32, 32, ATYPE_RO, acName6502, 0, &Obj.Supported_drive_modes},
};
const _objd SDO7000[] =
{
  {0x00, DTYPE_UNSIGNED8, 8, ATYPE_RO, acName7000_00, 5, NULL},
  {0x01, DTYPE_UNSIGNED32, 32, ATYPE_RO, acName7000_01, 0, &Obj.can1_tx.StdId},
  {0x02, DTYPE_UNSIGNED8, 8, ATYPE_RO, acName7000_02, 0, &Obj.can1_tx.ExtId},
  {0x03, DTYPE_UNSIGNED8, 8, ATYPE_RO, acName7000_03, 0, &Obj.can1_tx.IDE},
  {0x04, DTYPE_UNSIGNED8, 8, ATYPE_RO, acName7000_04, 0, &Obj.can1_tx.RTR},
  {0x05, DTYPE_UNSIGNED8, 8, ATYPE_RO, acName7000_05, 0, &Obj.can1_tx.DLC},
};
const _objd SDO7001[] =
{
  {0x00, DTYPE_UNSIGNED8, 8, ATYPE_RO, acName7001_00, 8, NULL},
  {0x01, DTYPE_UNSIGNED8, 8, ATYPE_RO | ATYPE_RXPDO, acName7001_01, 0, &Obj.can1_tx_data[0]},
  {0x02, DTYPE_UNSIGNED8, 8, ATYPE_RO | ATYPE_RXPDO, acName7001_02, 0, &Obj.can1_tx_data[1]},
  {0x03, DTYPE_UNSIGNED8, 8, ATYPE_RO | ATYPE_RXPDO, acName7001_03, 0, &Obj.can1_tx_data[2]},
  {0x04, DTYPE_UNSIGNED8, 8, ATYPE_RO | ATYPE_RXPDO, acName7001_04, 0, &Obj.can1_tx_data[3]},
  {0x05, DTYPE_UNSIGNED8, 8, ATYPE_RO | ATYPE_RXPDO, acName7001_05, 0, &Obj.can1_tx_data[4]},
  {0x06, DTYPE_UNSIGNED8, 8, ATYPE_RO | ATYPE_RXPDO, acName7001_06, 0, &Obj.can1_tx_data[5]},
  {0x07, DTYPE_UNSIGNED8, 8, ATYPE_RO | ATYPE_RXPDO, acName7001_07, 0, &Obj.can1_tx_data[6]},
  {0x08, DTYPE_UNSIGNED8, 8, ATYPE_RO | ATYPE_RXPDO, acName7001_08, 0, &Obj.can1_tx_data[7]},
};
const _objd SDO7002[] =
{
  {0x0, DTYPE_UNSIGNED8, 8, ATYPE_RO | ATYPE_RXPDO, acName7002, 0, &Obj.beat_heart},
};
const _objd SDO7003[] =
{
  {0x00, DTYPE_UNSIGNED8, 8, ATYPE_RO, acName7003_00, 3, NULL},
  {0x01, DTYPE_BOOLEAN, 1, ATYPE_RO | ATYPE_RXPDO, acName7003_01, 0, &Obj.led[0]},
  {0x02, DTYPE_BOOLEAN, 1, ATYPE_RO | ATYPE_RXPDO, acName7003_02, 0, &Obj.led[1]},
  {0x03, DTYPE_BOOLEAN, 1, ATYPE_RO | ATYPE_RXPDO, acName7003_03, 0, &Obj.led[2]},
};

const _objectlist SDOobjects[] =
{
  {0x1000, OTYPE_VAR, 0, 0, acName1000, SDO1000},
  {0x1001, OTYPE_VAR, 0, 0, acName1001, SDO1001},
  {0x1008, OTYPE_VAR, 0, 0, acName1008, SDO1008},
  {0x1009, OTYPE_VAR, 0, 0, acName1009, SDO1009},
  {0x100A, OTYPE_VAR, 0, 0, acName100A, SDO100A},
  {0x1018, OTYPE_RECORD, 4, 0, acName1018, SDO1018},
  {0x10F1, OTYPE_RECORD, 2, 0, acName10F1, SDO10F1},
  {0x1600, OTYPE_RECORD, 5, 0, acName1600, SDO1600},
  {0x1601, OTYPE_RECORD, 8, 0, acName1601, SDO1601},
  {0x1602, OTYPE_RECORD, 1, 0, acName1602, SDO1602},
  {0x1603, OTYPE_RECORD, 3, 0, acName1603, SDO1603},
  {0x1A00, OTYPE_RECORD, 5, 0, acName1A00, SDO1A00},
  {0x1A01, OTYPE_RECORD, 8, 0, acName1A01, SDO1A01},
  {0x1C00, OTYPE_ARRAY, 4, 0, acName1C00, SDO1C00},
  {0x1C12, OTYPE_ARRAY, 4, 0, acName1C12, SDO1C12},
  {0x1C13, OTYPE_ARRAY, 2, 0, acName1C13, SDO1C13},
  {0x1C32, OTYPE_RECORD, 5, 0, acName1C32, SDO1C32},
  {0x1C33, OTYPE_RECORD, 5, 0, acName1C33, SDO1C33},
  {0x6000, OTYPE_RECORD, 5, 0, acName6000, SDO6000},
  {0x6001, OTYPE_ARRAY, 8, 0, acName6001, SDO6001},
  {0x6060, OTYPE_VAR, 0, 0, acName6060, SDO6060},
  {0x6061, OTYPE_VAR, 0, 0, acName6061, SDO6061},
  {0x6502, OTYPE_VAR, 0, 0, acName6502, SDO6502},
  {0x7000, OTYPE_RECORD, 5, 0, acName7000, SDO7000},
  {0x7001, OTYPE_ARRAY, 8, 0, acName7001, SDO7001},
  {0x7002, OTYPE_VAR, 0, 0, acName7002, SDO7002},
  {0x7003, OTYPE_ARRAY, 3, 0, acName7003, SDO7003},
  {0xffff, 0xff, 0xff, 0xff, NULL, NULL}
};
