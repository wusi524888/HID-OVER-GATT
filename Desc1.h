// E:\BT\BLE\TI\TI_BLE_HID\Desc1.h


char ReportDescriptor[118] = {
    0x05, 0x01,                    // USAGE_PAGE (Generic Desktop)
    0x09, 0x05,                    // USAGE (Game Pad)
    0xa1, 0x01,                    // COLLECTION (Application)
    0x85, 0x03,                    //   REPORT_ID (3)
    0x75, 0x01,                    //   REPORT_SIZE (1)
    0x95, 0x08,                    //   REPORT_COUNT (8)
    0x15, 0x01,                    //   LOGICAL_MINIMUM (1)
    0x25, 0x08,                    //   LOGICAL_MAXIMUM (8)
    0x81, 0x01,                    //   INPUT (Cnst,Ary,Abs)
    0x05, 0x01,                    //   USAGE_PAGE (Generic Desktop)
    0x75, 0x08,                    //   REPORT_SIZE (8)
    0x95, 0x01,                    //   REPORT_COUNT (1)
    0x15, 0x01,                    //   LOGICAL_MINIMUM (1)
    0x25, 0x07,                    //   LOGICAL_MAXIMUM (7)
    0x46, 0x3b, 0x01,              //   PHYSICAL_MAXIMUM (315)
    0x65, 0x14,                    //   UNIT (Eng Rot:Angular Pos)
    0x09, 0x39,                    //   USAGE (Hat switch)
    0x81, 0x42,                    //   INPUT (Data,Var,Abs,Null)
    0x65, 0x00,                    //   UNIT (None)
    0x05, 0x09,                    //   USAGE_PAGE (Button)
    0x15, 0x01,                    //   LOGICAL_MINIMUM (1)
    0x25, 0x01,                    //   LOGICAL_MAXIMUM (1)
    0x19, 0x01,                    //   USAGE_MINIMUM (Button 1)
    0x29, 0x0f,                    //   USAGE_MAXIMUM (Button 15)
    0x75, 0x01,                    //   REPORT_SIZE (1)
    0x95, 0x0f,                    //   REPORT_COUNT (15)
    0x81, 0x02,                    //   INPUT (Data,Var,Abs)
    0x75, 0x01,                    //   REPORT_SIZE (1)
    0x95, 0x01,                    //   REPORT_COUNT (1)
    0x81, 0x01,                    //   INPUT (Cnst,Ary,Abs)
    0x05, 0x01,                    //   USAGE_PAGE (Generic Desktop)
    0x75, 0x08,                    //   REPORT_SIZE (8)
    0x95, 0x02,                    //   REPORT_COUNT (2)
    0x15, 0x00,                    //   LOGICAL_MINIMUM (0)
    0x26, 0xff, 0x00,              //   LOGICAL_MAXIMUM (255)
    0x35, 0x00,                    //   PHYSICAL_MINIMUM (0)
    0x46, 0xff, 0x00,              //   PHYSICAL_MAXIMUM (255)
    0xa1, 0x00,                    //   COLLECTION (Physical)
    0x09, 0x30,                    //     USAGE (X)
    0x09, 0x31,                    //     USAGE (Y)
    0x81, 0x02,                    //     INPUT (Data,Var,Abs)
    0xc0,                          //     END_COLLECTION
    0xa1, 0x00,                    //   COLLECTION (Physical)
    0x09, 0x32,                    //   USAGE (Z)
    0x09, 0x35,                    //   USAGE (Rz)
    0x81, 0x02,                    //   INPUT (Data,Var,Abs)
    0xc0,                          // END_COLLECTION
    0x05, 0x02,                    // USAGE_PAGE (Simulation Controls)
    0x09, 0xc5,                    // USAGE (Brake)
    0x09, 0xc4,                    // USAGE (Accelerator)
    0x75, 0x08,                    // REPORT_SIZE (8)
    0x95, 0x02,                    // REPORT_COUNT (2)
    0x15, 0x00,                    // LOGICAL_MINIMUM (0)
    0x26, 0xff, 0x00,              // LOGICAL_MAXIMUM (255)
    0x35, 0x00,                    // PHYSICAL_MINIMUM (0)
    0x46, 0xff, 0x00,              // PHYSICAL_MAXIMUM (255)
    0x81, 0x02,                    // INPUT (Data,Var,Abs)
    0xc0                           // END_COLLECTION
};

