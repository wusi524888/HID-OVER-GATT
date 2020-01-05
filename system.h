// E:\BT\BLE\TI\TI_BLE_HID\system.h


char ReportDescriptor[33] = {
    0x05, 0x01,                    // USAGE_PAGE (Generic Desktop)
    0xa1, 0x01,                    // COLLECTION (Application)
    0x85, 0x01,                    //   REPORT_ID (1)
    0x09, 0x80,                    // USAGE (System Control)
    0x09, 0x85,                    // USAGE (System Main Menu)
    0x09, 0x86,                    // USAGE (System App Menu)
    0x09, 0x89,                    // USAGE (System Menu Select)
    0x09, 0x8a,                    // USAGE (System Menu Right)
    0x09, 0x8b,                    // USAGE (System Menu Left)
    0x09, 0x8c,                    // USAGE (System Menu Up)
    0x09, 0x8d,                    // USAGE (System Menu Down)
    0x15, 0x01,                    //   LOGICAL_MINIMUM (1)
    0x25, 0x08,                    //   LOGICAL_MAXIMUM (8)
    0x95, 0x01,                    //   REPORT_COUNT (1)
    0x75, 0x08,                    //   REPORT_SIZE (8)
    0x81, 0x00,                    // INPUT (Data,Ary,Abs)
    0xc0                           // END_COLLECTION
};

