// E:\BT\BLE\TI\TI_BLE_HID\game.h


char ReportDescriptor[34] = {
    0x05, 0x05,                    // USAGE_PAGE (Gaming Controls)
    0x0b, 0x01, 0x00, 0x01, 0x00,  // USAGE (Generic Desktop:Pointer)
    0xa1, 0x01,                    //   COLLECTION (Application)
    0x75, 0x01,                    //   REPORT_SIZE (1)
    0x09, 0x24,                    // USAGE (Move Right/Left)
    0x09, 0x26,                    // USAGE (Move Up/Down)
    0x09, 0x25,                    // USAGE (Move Forward/Backward)
    0x09, 0x27,                    // USAGE (Lean Right/Left)
    0x09, 0x28,                    // USAGE (Lean Forward/Backward)
    0x09, 0x32,                    // USAGE (Gun Selector)
    0x15, 0x01,                    // LOGICAL_MINIMUM (1)
    0x25, 0x06,                    // LOGICAL_MAXIMUM (6)
    0x95, 0x01,                    //   REPORT_COUNT (1)
    0x75, 0x06,                    //   REPORT_SIZE (6)
    0x81, 0x00,                    // INPUT (Data,Ary,Abs)
    0xc0                           // END_COLLECTION
};

