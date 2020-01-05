/******************************************************************************

 @file       hidkbdservice.c

 @brief This file contains the HID service for a keyboard.

 Group: CMCU, SCS
 Target Device: CC2640R2

 ******************************************************************************
 
 Copyright (c) 2011-2017, Texas Instruments Incorporated
 All rights reserved.

 Redistribution and use in source and binary forms, with or without
 modification, are permitted provided that the following conditions
 are met:

 *  Redistributions of source code must retain the above copyright
    notice, this list of conditions and the following disclaimer.

 *  Redistributions in binary form must reproduce the above copyright
    notice, this list of conditions and the following disclaimer in the
    documentation and/or other materials provided with the distribution.

 *  Neither the name of Texas Instruments Incorporated nor the names of
    its contributors may be used to endorse or promote products derived
    from this software without specific prior written permission.

 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

 ******************************************************************************
 Release Name: simplelink_cc2640r2_sdk_01_50_00_58
 Release Date: 2017-10-17 18:09:51
 *****************************************************************************/

/*********************************************************************
 * INCLUDES
 */
#include <icall.h>
#include "util.h"
/* This Header file contains all BLE API and icall structure definition */
#include "icall_ble_api.h"

#include "hidkbdservice.h"
#include "hiddev.h"
#include "battservice.h"
#include "USBHID_Types.h"
/*********************************************************************
 * MACROS
 */

/*********************************************************************
 * CONSTANTS
 */

/*********************************************************************
 * TYPEDEFS
 */

/*********************************************************************
 * GLOBAL VARIABLES
 */
// HID service
CONST uint8 hidServUUID[ATT_BT_UUID_SIZE] =
{
  LO_UINT16(HID_SERV_UUID), HI_UINT16(HID_SERV_UUID)
};

// HID Boot Keyboard Input Report characteristic
CONST uint8 hidBootKeyInputUUID[ATT_BT_UUID_SIZE] =
{
  LO_UINT16(BOOT_KEY_INPUT_UUID), HI_UINT16(BOOT_KEY_INPUT_UUID)
};

// HID Boot Mouse Input Report characteristic
CONST uint8 hidBootMouseInputUUID[ATT_BT_UUID_SIZE] =
{
  LO_UINT16(BOOT_MOUSE_INPUT_UUID), HI_UINT16(BOOT_MOUSE_INPUT_UUID)
};

// HID Boot Keyboard Output Report characteristic
CONST uint8 hidBootKeyOutputUUID[ATT_BT_UUID_SIZE] =
{
  LO_UINT16(BOOT_KEY_OUTPUT_UUID), HI_UINT16(BOOT_KEY_OUTPUT_UUID)
};

// HID Information characteristic
CONST uint8 hidInfoUUID[ATT_BT_UUID_SIZE] =
{
  LO_UINT16(HID_INFORMATION_UUID), HI_UINT16(HID_INFORMATION_UUID)
};

// HID Report Map characteristic
CONST uint8 hidReportMapUUID[ATT_BT_UUID_SIZE] =
{
  LO_UINT16(REPORT_MAP_UUID), HI_UINT16(REPORT_MAP_UUID)
};

// HID Control Point characteristic
CONST uint8 hidControlPointUUID[ATT_BT_UUID_SIZE] =
{
  LO_UINT16(HID_CTRL_PT_UUID), HI_UINT16(HID_CTRL_PT_UUID)
};

// HID Report characteristic
CONST uint8 hidReportUUID[ATT_BT_UUID_SIZE] =
{
  LO_UINT16(REPORT_UUID), HI_UINT16(REPORT_UUID)
};

// HID Protocol Mode characteristic
CONST uint8 hidProtocolModeUUID[ATT_BT_UUID_SIZE] =
{
  LO_UINT16(PROTOCOL_MODE_UUID), HI_UINT16(PROTOCOL_MODE_UUID)
};

/*********************************************************************
 * EXTERNAL VARIABLES
 */

/*********************************************************************
 * EXTERNAL FUNCTIONS
 */

/*********************************************************************
 * LOCAL VARIABLES
 */

// HID Information characteristic value
static CONST uint8 hidInfo[HID_INFORMATION_LEN] =
{
  LO_UINT16(0x0111), HI_UINT16(0x0111),           // bcdHID (USB HID version)
  0x00,                                           // bCountryCode
  HID_KBD_FLAGS                                   // Flags
};

// HID Report Map characteristic value
// Keyboard report descriptor (using format for Boot interface descriptor)
#if  defined(GENERNAL)
static CONST uint8 hidReportMap[] =
{
  0x05, 0x01,     // Usage Pg (Generic Desktop)
  0x09, 0x06,     // Usage (Keyboard)
  0xA1, 0x01,     // Collection: (Application)
                  //
  0x05, 0x07,     // Usage Pg (Key Codes)
  0x19, 0xE0,     // Usage Min (224)
  0x29, 0xE7,     // Usage Max (231)
  0x15, 0x00,     // Log Min (0)
  0x25, 0x01,     // Log Max (1)
                  //
                  // Modifier byte
  0x75, 0x01,     // Report Size (1)
  0x95, 0x08,     // Report Count (8)
  0x81, 0x02,     // Input: (Data, Variable, Absolute)
                  //
                  // Reserved byte
  0x95, 0x01,     // Report Count (1)
  0x75, 0x08,     // Report Size (8)
  0x81, 0x01,     // Input: (Constant)
                  //
                  // LED report
  0x95, 0x05,     // Report Count (5)
  0x75, 0x01,     // Report Size (1)
  0x05, 0x08,     // Usage Pg (LEDs)
  0x19, 0x01,     // Usage Min (1)
  0x29, 0x05,     // Usage Max (5)
  0x91, 0x02,     // Output: (Data, Variable, Absolute)
                  //
                  // LED report padding
  0x95, 0x01,     // Report Count (1)
  0x75, 0x03,     // Report Size (3)
  0x91, 0x01,     // Output: (Constant)
                  //
                  // Key arrays (6 bytes)
  0x95, 0x06,     // Report Count (6)
  0x75, 0x08,     // Report Size (8)
  0x15, 0x00,     // Log Min (0)
  0x25, 0x65,     // Log Max (101)
  0x05, 0x07,     // Usage Pg (Key Codes)
  0x19, 0x00,     // Usage Min (0)
  0x29, 0x65,     // Usage Max (101)
  0x81, 0x00,     // Input: (Data, Array)
                  //
  0xC0            // End Collection
};
#elif defined(CUSTOMER)
static CONST uint8 hidReportMap[] =
{
   0x05, 0x0c,                    // USAGE_PAGE (Consumer Devices)
   0x09, 0x01,                    // USAGE (Consumer Control)
   0xa1, 0x01,                    // COLLECTION (Application)
   0x85, 0x01,                    //   REPORT_ID (1)
   0x09, 0xCD,                    //   USAGE (Play/Pause)
   0x09, 0xB5,                    //   USAGE (Scan next track)
   0x09, 0xB6,                    //   USAGE (Scan previous track)
   0x09, 0xE9,                    //   USAGE (Volume up)
   0x09, 0xEA,                    //   USAGE (Volume down)
   0x09, 0xE2,                    //   USAGE (Volume mute)
   0x09, 0x40,                    //   USAGE (Menu)
   0x09, 0x41,                    //   USAGE (Menu Pick)
   0x09, 0x42,                    //   USAGE (Menu up)
   0x09, 0x43,                    //   USAGE (Menu down)
   0x09, 0x44,                    //   USAGE (Menu left)
   0x09, 0x45,                    //   USAGE (Menu right)
   0x09, 0x46,                    //   USAGE (Menu escape)
   0x09, 0x82,                    //   USAGE (Assign Selection)
   0x09, 0x83,                    //   USAGE (Mode Step)
   0x09, 0x94,                    //   USAGE (Quit)
   0x09, 0x9C,                    //   USAGE (Channel Increment)
   0x09, 0x9D,                    //   USAGE (Channel Decrement)
   0x0A, 0x25, 0x02,              //   USAGE (AL FORWARD)
   0x0A, 0x24, 0x02,              //   USAGE (AL BACK)
   0x15, 0x01,                    //   LOGICAL_MINIMUM (1)
   0x25, 0x14,                    //   LOGICAL_MAXIMUM (14)
   0x95, 0x01,                    //   REPORT_COUNT (1)
   0x75, 0x08,                    //   REPORT_SIZE (8)
   0x81, 0x00,                    //   INPUT (Data,Ary,Abs)
   0xc0                           // END_COLLECTION
};
#elif defined(JOYSTICK)
//  JOYSTICK report descriptor (using format for Boot interface descriptor)
static const uint8 hidReportMap = {
    USAGE_PAGE(1),      0x01,         // Generic Desktop
    USAGE(1),           0x04,         // Joystick
    COLLECTION(1),      0x01,         // Application
    COLLECTION(1),      0x00,         //  Physical
    USAGE_PAGE(1),      0x09,         //   Buttons
    USAGE_MINIMUM(1),   0x01,
    USAGE_MAXIMUM(1),   0x03,
    LOGICAL_MINIMUM(1), 0x00,
    LOGICAL_MAXIMUM(1), 0x01,
    REPORT_COUNT(1),    0x03,         //   2 bits (Buttons)
    REPORT_SIZE(1),     0x01,
    INPUT(1),           0x02,         //   Data, Variable, Absolute
    REPORT_COUNT(1),    0x01,         //   6 bits (Padding)
    REPORT_SIZE(1),     0x05,
    INPUT(1),           0x01,         //   Constant
    USAGE_PAGE(1),      0x01,         //   Generic Desktop
    USAGE(1),           0x30,         //   X
    USAGE(1),           0x31,         //   Y
    USAGE(1),           0x32,         //   Z
    USAGE(1),           0x33,         //   Rx
    LOGICAL_MINIMUM(1), 0x81,         //   -127
    LOGICAL_MAXIMUM(1), 0x7f,         //   127
    REPORT_SIZE(1),     0x08,         //   Three bytes
    REPORT_COUNT(1),    0x04,
    INPUT(1),           0x02,         //   Data, Variable, Absolute (unlike mouse)
    END_COLLECTION(0),
    END_COLLECTION(0),
};
#elif defined(KEYBOARD)
static const uint8 KEYBOARD_REPORT_MAP = {
    USAGE_PAGE(1),      0x01,       // Generic Desktop Ctrls
    USAGE(1),           0x06,       // Keyboard
    COLLECTION(1),      0x01,       // Application
    USAGE_PAGE(1),      0x07,       // Kbrd/Keypad
    USAGE_MINIMUM(1),   0xE0,
    USAGE_MAXIMUM(1),   0xE7,
    LOGICAL_MINIMUM(1), 0x00,
    LOGICAL_MAXIMUM(1), 0x01,
    REPORT_SIZE(1),     0x01,       //   1 byte (Modifier)
    REPORT_COUNT(1),    0x08,
    INPUT(1),           0x02,       //   Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position
    REPORT_COUNT(1),    0x01,       //   1 byte (Reserved)
    REPORT_SIZE(1),     0x08,
    INPUT(1),           0x01,       //   Const,Array,Abs,No Wrap,Linear,Preferred State,No Null Position
    REPORT_COUNT(1),    0x05,       //   5 bits (Num lock, Caps lock, Scroll lock, Compose, Kana)
    REPORT_SIZE(1),     0x01,
    USAGE_PAGE(1),      0x08,       //   LEDs
    USAGE_MINIMUM(1),   0x01,       //   Num Lock
    USAGE_MAXIMUM(1),   0x05,       //   Kana
    OUTPUT(1),          0x02,       //   Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile
    REPORT_COUNT(1),    0x01,       //   3 bits (Padding)
    REPORT_SIZE(1),     0x03,
    OUTPUT(1),          0x01,       //   Const,Array,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile
    REPORT_COUNT(1),    0x06,       //   6 bytes (Keys)
    REPORT_SIZE(1),     0x08,
    LOGICAL_MINIMUM(1), 0x00,
    LOGICAL_MAXIMUM(1), 0x65,       //   101 keys
    USAGE_PAGE(1),      0x07,       //   Kbrd/Keypad
    USAGE_MINIMUM(1),   0x00,
    USAGE_MAXIMUM(1),   0x65,
    INPUT(1),           0x00,       //   Data,Array,Abs,No Wrap,Linear,Preferred State,No Null Position
    END_COLLECTION(0),
};
#elif defined(MOUSE)
static const MOUSE_REPORT_MAP = {
    USAGE_PAGE(1),      0x01,         // Generic Desktop
    USAGE(1),           0x02,         // Mouse
    COLLECTION(1),      0x01,         // Application
    USAGE(1),           0x01,         // Pointer
    COLLECTION(1),      0x00,         // Physical
    USAGE_PAGE(1),      0x09,         // Buttons
    USAGE_MINIMUM(1),   0x01,
    USAGE_MAXIMUM(1),   0x03,
    LOGICAL_MINIMUM(1), 0x00,
    LOGICAL_MAXIMUM(1), 0x01,
    REPORT_COUNT(1),    0x03,         //   3 bits (Buttons)
    REPORT_SIZE(1),     0x01,
    INPUT(1),           0x02,         //   Data, Variable, Absolute
    REPORT_COUNT(1),    0x01,         //   5 bits (Padding)
    REPORT_SIZE(1),     0x05,
    INPUT(1),           0x01,         //   Constant
    USAGE_PAGE(1),      0x01,         //   Generic Desktop
    USAGE(1),           0x30,         //   X
    USAGE(1),           0x31,         //   Y
    USAGE(1),           0x38,         //   Wheel
    LOGICAL_MINIMUM(1), 0x81,         //   -127
    LOGICAL_MAXIMUM(1), 0x7f,         //   127
    REPORT_SIZE(1),     0x08,         //   Three bytes
    REPORT_COUNT(1),    0x03,
    INPUT(1),           0x06,         //   Data, Variable, Relative
    END_COLLECTION(0),
    END_COLLECTION(0),
};
#elif defined(GAME_PAD)
//  GAME_PAD report descriptor (using format for Boot interface descriptor)
//char hidReportMap[118] = {
//    0x05, 0x01,                    // USAGE_PAGE (Generic Desktop)
//    0x09, 0x05,                    // USAGE (Game Pad)
//    0xa1, 0x01,                    // COLLECTION (Application)
//    0x85, 0x03,                    //   REPORT_ID (3)
//    0x75, 0x01,                    //   REPORT_SIZE (1)
//    0x95, 0x08,                    //   REPORT_COUNT (8)
//    0x15, 0x01,                    //   LOGICAL_MINIMUM (1)
//    0x25, 0x08,                    //   LOGICAL_MAXIMUM (8)
//    0x81, 0x01,                    //   INPUT (Cnst,Ary,Abs)
//    0x05, 0x01,                    //   USAGE_PAGE (Generic Desktop)
//    0x75, 0x08,                    //   REPORT_SIZE (8)
//    0x95, 0x01,                    //   REPORT_COUNT (1)
//    0x15, 0x01,                    //   LOGICAL_MINIMUM (1)
//    0x25, 0x07,                    //   LOGICAL_MAXIMUM (7)
//    0x46, 0x3b, 0x01,              //   PHYSICAL_MAXIMUM (315)
//    0x65, 0x14,                    //   UNIT (Eng Rot:Angular Pos)
//    0x09, 0x39,                    //   USAGE (Hat switch)
//    0x81, 0x42,                    //   INPUT (Data,Var,Abs,Null)
//    0x65, 0x00,                    //   UNIT (None)
//    0x05, 0x09,                    //   USAGE_PAGE (Button)
//    0x15, 0x01,                    //   LOGICAL_MINIMUM (1)
//    0x25, 0x01,                    //   LOGICAL_MAXIMUM (1)
//    0x19, 0x01,                    //   USAGE_MINIMUM (Button 1)
//    0x29, 0x0f,                    //   USAGE_MAXIMUM (Button 15)
//    0x75, 0x01,                    //   REPORT_SIZE (1)
//    0x95, 0x0f,                    //   REPORT_COUNT (15)
//    0x81, 0x02,                    //   INPUT (Data,Var,Abs)
//    0x75, 0x01,                    //   REPORT_SIZE (1)
//    0x95, 0x01,                    //   REPORT_COUNT (1)
//    0x81, 0x01,                    //   INPUT (Cnst,Ary,Abs)
//    0x05, 0x01,                    //   USAGE_PAGE (Generic Desktop)
//    0x75, 0x08,                    //   REPORT_SIZE (8)
//    0x95, 0x02,                    //   REPORT_COUNT (2)
//    0x15, 0x00,                    //   LOGICAL_MINIMUM (0)
//    0x26, 0xff, 0x00,              //   LOGICAL_MAXIMUM (255)
//    0x35, 0x00,                    //   PHYSICAL_MINIMUM (0)
//    0x46, 0xff, 0x00,              //   PHYSICAL_MAXIMUM (255)
//    0xa1, 0x00,                    //   COLLECTION (Physical)
//    0x09, 0x30,                    //     USAGE (X)
//    0x09, 0x31,                    //     USAGE (Y)
//    0x81, 0x02,                    //     INPUT (Data,Var,Abs)
//    0xc0,                          //     END_COLLECTION
//    0xa1, 0x00,                    //   COLLECTION (Physical)
//    0x09, 0x32,                    //   USAGE (Z)
//    0x09, 0x35,                    //   USAGE (Rz)
//    0x81, 0x02,                    //   INPUT (Data,Var,Abs)
//    0xc0,                          // END_COLLECTION
//    0x05, 0x02,                    // USAGE_PAGE (Simulation Controls)
//    0x09, 0xc5,                    // USAGE (Brake)
//    0x09, 0xc4,                    // USAGE (Accelerator)
//    0x75, 0x08,                    // REPORT_SIZE (8)
//    0x95, 0x02,                    // REPORT_COUNT (2)
//    0x15, 0x00,                    // LOGICAL_MINIMUM (0)
//    0x26, 0xff, 0x00,              // LOGICAL_MAXIMUM (255)
//    0x35, 0x00,                    // PHYSICAL_MINIMUM (0)
//    0x46, 0xff, 0x00,              // PHYSICAL_MAXIMUM (255)
//    0x81, 0x02,                    // INPUT (Data,Var,Abs)
//    0xc0                           // END_COLLECTION
//};
static const uint8 hidReportMap[] = {
0x05,0x01,
0x09,0x05,
0xa1,0x01,
0x85,0x03,
0x75,0x01,
0x95,0x08,
0x81,0x01,
0x05,0x01,
0x75,0x08,
0x95,0x01,
0x25,0x07,
0x46,0x3b,0x01,
0x65,0x14,
0x09,0x39,
0x81,0x42,
0x65,0x00,
0x05,0x09,
0x25,0x01,
0x19,0x01,
0x29,0x0f,
0x75,0x01,
0x95,0x0f,
0x81,0x02,
0x75,0x01,
0x95,0x01,
0x81,0x01,
0x05,0x01,
0x75,0x08,
0x95,0x02,
0x15,0x00,
0x26,0xff,
0x00,0x35,
0x00,0x46,
0xff,0x00,
0xa1,0x00,
0x09,0x30,
0x09,0x31,
0x81,0x02,
0xc0,
0xa1,0x00,
0x09,0x32,
0x09,0x35,
0x81,0x02,
0xc0,
0x05,0x02,
0x09,0xc5,
0x09,0xc4,
0x75,0x08,
0x95,0x02,
0x15,0x00,
0x25,0xff,
0x35,0x00,
0x45,0xff,
0x81,0x02,
0xc0
};
#endif

// HID report map length
uint8 hidReportMapLen = sizeof(hidReportMap);

// HID report mapping table
static hidRptMap_t  hidRptMap[HID_NUM_REPORTS];

/*********************************************************************
 * Profile Attributes - variables
 */

// HID Service attribute
static CONST gattAttrType_t hidService = { ATT_BT_UUID_SIZE, hidServUUID };

// Include attribute (Battery service)
static uint16 include = GATT_INVALID_HANDLE;

// HID Information characteristic
static uint8 hidInfoProps = GATT_PROP_READ;

// HID Report Map characteristic
static uint8 hidReportMapProps = GATT_PROP_READ;

// HID External Report Reference Descriptor
static uint8 hidExtReportRefDesc[ATT_BT_UUID_SIZE] =
             { LO_UINT16(BATT_LEVEL_UUID), HI_UINT16(BATT_LEVEL_UUID) };

// HID Control Point characteristic
static uint8 hidControlPointProps = GATT_PROP_WRITE_NO_RSP;
static uint8 hidControlPoint;

// HID Protocol Mode characteristic
static uint8 hidProtocolModeProps = GATT_PROP_READ | GATT_PROP_WRITE_NO_RSP;
uint8 hidProtocolMode = HID_PROTOCOL_MODE_REPORT;

// HID Report characteristic, key input
static uint8 hidReportKeyInProps = GATT_PROP_READ | GATT_PROP_NOTIFY;
static uint8 hidReportKeyIn;
static gattCharCfg_t *hidReportKeyInClientCharCfg;

// HID Report Reference characteristic descriptor, key input
static uint8 hidReportRefKeyIn[HID_REPORT_REF_LEN] =
             { HID_RPT_ID_KEY_IN, HID_REPORT_TYPE_INPUT };

// HID Report characteristic, LED output
static uint8 hidReportLedOutProps = GATT_PROP_READ  |
                                    GATT_PROP_WRITE |
                                    GATT_PROP_WRITE_NO_RSP;
static uint8 hidReportLedOut;

// HID Report Reference characteristic descriptor, LED output
static uint8 hidReportRefLedOut[HID_REPORT_REF_LEN] =
             { HID_RPT_ID_LED_OUT, HID_REPORT_TYPE_OUTPUT };

// HID Boot Keyboard Input Report
static uint8 hidReportBootKeyInProps = GATT_PROP_READ | GATT_PROP_NOTIFY;
static uint8 hidReportBootKeyIn;
static gattCharCfg_t *hidReportBootKeyInClientCharCfg;

// HID Boot Keyboard Output Report
static uint8 hidReportBootKeyOutProps = GATT_PROP_READ  |
                                        GATT_PROP_WRITE |
                                        GATT_PROP_WRITE_NO_RSP;
static uint8 hidReportBootKeyOut;

// HID Boot Mouse Input Report
static uint8 hidReportBootMouseInProps = GATT_PROP_READ | GATT_PROP_NOTIFY;
static uint8 hidReportBootMouseIn;
static gattCharCfg_t *hidReportBootMouseInClientCharCfg;

// Feature Report
static uint8 hidReportFeatureProps = GATT_PROP_READ | GATT_PROP_WRITE;
static uint8 hidReportFeature;

// HID Report Reference characteristic descriptor, Feature
static uint8 hidReportRefFeature[HID_REPORT_REF_LEN] =
             { HID_RPT_ID_FEATURE, HID_REPORT_TYPE_FEATURE };

/*********************************************************************
 * Profile Attributes - Table
 */

static gattAttribute_t hidAttrTbl[] =
{
  // HID Service
  {
    { ATT_BT_UUID_SIZE, primaryServiceUUID }, /* type */
    GATT_PERMIT_READ,                         /* permissions */
    0,                                        /* handle */
    (uint8 *) &hidService                     /* pValue */
  },

    // Included service (battery)
    {
      { ATT_BT_UUID_SIZE, includeUUID },
      GATT_PERMIT_READ,
      0,
      (uint8 *)&include
    },

    // HID Information characteristic declaration
    {
      { ATT_BT_UUID_SIZE, characterUUID },
      GATT_PERMIT_READ,
      0,
      &hidInfoProps
    },

      // HID Information characteristic
      {
        { ATT_BT_UUID_SIZE, hidInfoUUID },
        GATT_PERMIT_ENCRYPT_READ,
        0,
        (uint8 *) hidInfo
      },

    // HID Control Point characteristic declaration
    {
      { ATT_BT_UUID_SIZE, characterUUID },
      GATT_PERMIT_READ,
      0,
      &hidControlPointProps
    },

      // HID Control Point characteristic
      {
        { ATT_BT_UUID_SIZE, hidControlPointUUID },
        GATT_PERMIT_ENCRYPT_WRITE,
        0,
        &hidControlPoint
      },

    // HID Protocol Mode characteristic declaration
    {
      { ATT_BT_UUID_SIZE, characterUUID },
      GATT_PERMIT_READ,
      0,
      &hidProtocolModeProps
    },

      // HID Protocol Mode characteristic
      {
        { ATT_BT_UUID_SIZE, hidProtocolModeUUID },
        GATT_PERMIT_ENCRYPT_READ | GATT_PERMIT_ENCRYPT_WRITE,
        0,
        &hidProtocolMode
      },


    // HID Report Map characteristic declaration
    {
      { ATT_BT_UUID_SIZE, characterUUID },
      GATT_PERMIT_READ,
      0,
      &hidReportMapProps
    },

      // HID Report Map characteristic
      {
        { ATT_BT_UUID_SIZE, hidReportMapUUID },
        GATT_PERMIT_ENCRYPT_READ,
        0,
        (uint8 *) hidReportMap
      },

      // HID External Report Reference Descriptor
      {
        { ATT_BT_UUID_SIZE, extReportRefUUID },
        GATT_PERMIT_READ,
        0,
        hidExtReportRefDesc
      },

    // HID Report characteristic, key input declaration
    {
      { ATT_BT_UUID_SIZE, characterUUID },
      GATT_PERMIT_READ,
      0,
      &hidReportKeyInProps
    },

      // HID Report characteristic, key input
      {
        { ATT_BT_UUID_SIZE, hidReportUUID },
        GATT_PERMIT_ENCRYPT_READ,
        0,
        &hidReportKeyIn
      },

      // HID Report characteristic client characteristic configuration
      {
        { ATT_BT_UUID_SIZE, clientCharCfgUUID },
        GATT_PERMIT_READ | GATT_PERMIT_ENCRYPT_WRITE,
        0,
        (uint8 *) &hidReportKeyInClientCharCfg
      },

      // HID Report Reference characteristic descriptor, key input
      {
        { ATT_BT_UUID_SIZE, reportRefUUID },
        GATT_PERMIT_READ,
        0,
        hidReportRefKeyIn
      },

    // HID Report characteristic, LED output declaration
    {
      { ATT_BT_UUID_SIZE, characterUUID },
      GATT_PERMIT_READ,
      0,
      &hidReportLedOutProps
    },

      // HID Report characteristic, LED output
      {
        { ATT_BT_UUID_SIZE, hidReportUUID },
        GATT_PERMIT_ENCRYPT_READ | GATT_PERMIT_ENCRYPT_WRITE,
        0,
        &hidReportLedOut
      },

      // HID Report Reference characteristic descriptor, LED output
      {
        { ATT_BT_UUID_SIZE, reportRefUUID },
        GATT_PERMIT_READ,
        0,
        hidReportRefLedOut
      },

    // HID Boot Keyboard Input Report declaration
    {
      { ATT_BT_UUID_SIZE, characterUUID },
      GATT_PERMIT_READ,
      0,
      &hidReportBootKeyInProps
    },

      // HID Boot Keyboard Input Report
      {
        { ATT_BT_UUID_SIZE, hidBootKeyInputUUID },
        GATT_PERMIT_ENCRYPT_READ,
        0,
        &hidReportBootKeyIn
      },

      // HID Boot Keyboard Input Report characteristic client characteristic configuration
      {
        { ATT_BT_UUID_SIZE, clientCharCfgUUID },
        GATT_PERMIT_READ | GATT_PERMIT_ENCRYPT_WRITE,
        0,
        (uint8 *) &hidReportBootKeyInClientCharCfg
      },

    // HID Boot Keyboard Output Report declaration
    {
      { ATT_BT_UUID_SIZE, characterUUID },
      GATT_PERMIT_READ,
      0,
      &hidReportBootKeyOutProps
    },

      // HID Boot Keyboard Output Report
      {
        { ATT_BT_UUID_SIZE, hidBootKeyOutputUUID },
        GATT_PERMIT_ENCRYPT_READ | GATT_PERMIT_ENCRYPT_WRITE,
        0,
        &hidReportBootKeyOut
      },

    // HID Boot Mouse Input Report declaration
    {
      { ATT_BT_UUID_SIZE, characterUUID },
      GATT_PERMIT_READ,
      0,
      &hidReportBootMouseInProps
    },

      // HID Boot Mouse Input Report
      {
        { ATT_BT_UUID_SIZE, hidBootMouseInputUUID },
        GATT_PERMIT_ENCRYPT_READ,
        0,
        &hidReportBootMouseIn
      },

      // HID Boot Mouse Input Report characteristic client characteristic configuration
      {
        { ATT_BT_UUID_SIZE, clientCharCfgUUID },
        GATT_PERMIT_READ | GATT_PERMIT_ENCRYPT_WRITE,
        0,
        (uint8 *) &hidReportBootMouseInClientCharCfg
      },

    // Feature Report declaration
    {
      { ATT_BT_UUID_SIZE, characterUUID },
      GATT_PERMIT_READ,
      0,
      &hidReportFeatureProps
    },

      // Feature Report
      {
        { ATT_BT_UUID_SIZE,  hidReportUUID},
        GATT_PERMIT_ENCRYPT_READ | GATT_PERMIT_ENCRYPT_WRITE,
        0,
        &hidReportFeature
      },

      // HID Report Reference characteristic descriptor, feature
      {
        { ATT_BT_UUID_SIZE, reportRefUUID },
        GATT_PERMIT_READ,
        0,
        hidReportRefFeature
      },
};

// Attribute index enumeration-- these indexes match array elements above
enum
{
  HID_SERVICE_IDX,                // HID Service
  HID_INCLUDED_SERVICE_IDX,       // Included Service
  HID_INFO_DECL_IDX,              // HID Information characteristic declaration
  HID_INFO_IDX,                   // HID Information characteristic
  HID_CONTROL_POINT_DECL_IDX,     // HID Control Point characteristic declaration
  HID_CONTROL_POINT_IDX,          // HID Control Point characteristic
  HID_PROTOCOL_MODE_DECL_IDX,     // HID Protocol Mode characteristic declaration
  HID_PROTOCOL_MODE_IDX,          // HID Protocol Mode characteristic
  HID_REPORT_MAP_DECL_IDX,        // HID Report Map characteristic declaration
  HID_REPORT_MAP_IDX,             // HID Report Map characteristic
  HID_EXT_REPORT_REF_DESC_IDX,    // HID External Report Reference Descriptor
  HID_REPORT_KEY_IN_DECL_IDX,     // HID Report characteristic, key input declaration
  HID_REPORT_KEY_IN_IDX,          // HID Report characteristic, key input
  HID_REPORT_KEY_IN_CCCD_IDX,     // HID Report characteristic client characteristic configuration
  HID_REPORT_REF_KEY_IN_IDX,      // HID Report Reference characteristic descriptor, key input
  HID_REPORT_LED_OUT_DECL_IDX,    // HID Report characteristic, LED output declaration
  HID_REPORT_LED_OUT_IDX,         // HID Report characteristic, LED output
  HID_REPORT_REF_LED_OUT_IDX,     // HID Report Reference characteristic descriptor, LED output
  HID_BOOT_KEY_IN_DECL_IDX,       // HID Boot Keyboard Input Report declaration
  HID_BOOT_KEY_IN_IDX,            // HID Boot Keyboard Input Report
  HID_BOOT_KEY_IN_CCCD_IDX,       // HID Boot Keyboard Input Report characteristic client characteristic configuration
  HID_BOOT_KEY_OUT_DECL_IDX,      // HID Boot Keyboard Output Report declaration
  HID_BOOT_KEY_OUT_IDX,           // HID Boot Keyboard Output Report
  HID_BOOT_MOUSE_IN_DECL_IDX,     // HID Boot Mouse Input Report declaration
  HID_BOOT_MOUSE_IN_IDX,          // HID Boot Mouse Input Report
  HID_BOOT_MOUSE_IN_CCCD_IDX,     // HID Boot Mouse Input Report characteristic client characteristic configuration
  HID_FEATURE_DECL_IDX,           // Feature Report declaration
  HID_FEATURE_IDX,                // Feature Report
  HID_REPORT_REF_FEATURE_IDX      // HID Report Reference characteristic descriptor, feature
};

/*********************************************************************
 * LOCAL FUNCTIONS
 */

/*********************************************************************
 * PROFILE CALLBACKS
 */

// Service Callbacks
// Note: When an operation on a characteristic requires authorization and
// pfnAuthorizeAttrCB is not defined for that characteristic's service, the
// Stack will report a status of ATT_ERR_UNLIKELY to the client.  When an
// operation on a characteristic requires authorization the Stack will call
// pfnAuthorizeAttrCB to check a client's authorization prior to calling
// pfnReadAttrCB or pfnWriteAttrCB, so no checks for authorization need to be
// made within these functions.
CONST gattServiceCBs_t hidKbdCBs =
{
  HidDev_ReadAttrCB,  // Read callback function pointer
  HidDev_WriteAttrCB, // Write callback function pointer
  NULL                // Authorization callback function pointer
};

/*********************************************************************
 * PUBLIC FUNCTIONS
 */

/*********************************************************************
 * @fn      HidKbd_AddService
 *
 * @brief   Initializes the HID Service by registering
 *          GATT attributes with the GATT server.
 *
 * @param   none
 *
 * @return  Success or Failure
 */
bStatus_t HidKbd_AddService(void)
{
  uint8 status = SUCCESS;

  // Allocate Client Charateristic Configuration tables.
  hidReportKeyInClientCharCfg = (gattCharCfg_t *)ICall_malloc(sizeof(gattCharCfg_t) *
                                                              linkDBNumConns);

  if (hidReportKeyInClientCharCfg == NULL)
  {
    return ( bleMemAllocError );
  }

  hidReportBootKeyInClientCharCfg = (gattCharCfg_t *)ICall_malloc(sizeof(gattCharCfg_t) *
                                                                  linkDBNumConns);

  if (hidReportBootKeyInClientCharCfg == NULL)
  {
    ICall_free(hidReportKeyInClientCharCfg);

    return ( bleMemAllocError );
  }

  hidReportBootMouseInClientCharCfg = (gattCharCfg_t *)ICall_malloc(sizeof(gattCharCfg_t) *
                                                                    linkDBNumConns);
  if (hidReportBootMouseInClientCharCfg == NULL)
  {
    ICall_free(hidReportKeyInClientCharCfg);

    ICall_free(hidReportBootKeyInClientCharCfg);

    return ( bleMemAllocError );
  }

  // Initialize Client Characteristic Configuration attributes
  GATTServApp_InitCharCfg(INVALID_CONNHANDLE, hidReportKeyInClientCharCfg);
  GATTServApp_InitCharCfg(INVALID_CONNHANDLE, hidReportBootKeyInClientCharCfg);
  GATTServApp_InitCharCfg(INVALID_CONNHANDLE,
                          hidReportBootMouseInClientCharCfg);

  // Register GATT attribute list and CBs with GATT Server App
  status = GATTServApp_RegisterService(hidAttrTbl, GATT_NUM_ATTRS(hidAttrTbl),
                                       GATT_MAX_ENCRYPT_KEY_SIZE, &hidKbdCBs);

  // Set up included service
  Batt_GetParameter(BATT_PARAM_SERVICE_HANDLE,
                    &GATT_INCLUDED_HANDLE(hidAttrTbl, HID_INCLUDED_SERVICE_IDX));

  // Construct map of reports to characteristic handles
  // Each report is uniquely identified via its ID and type

  // Key input report
  hidRptMap[0].id = hidReportRefKeyIn[0];
  hidRptMap[0].type = hidReportRefKeyIn[1];
  hidRptMap[0].handle = hidAttrTbl[HID_REPORT_KEY_IN_IDX].handle;
  hidRptMap[0].pCccdAttr = &hidAttrTbl[HID_REPORT_KEY_IN_CCCD_IDX];
  hidRptMap[0].mode = HID_PROTOCOL_MODE_REPORT;

  // LED output report
  hidRptMap[1].id = hidReportRefLedOut[0];
  hidRptMap[1].type = hidReportRefLedOut[1];
  hidRptMap[1].handle = hidAttrTbl[HID_REPORT_LED_OUT_IDX].handle;
  hidRptMap[1].pCccdAttr = NULL;
  hidRptMap[1].mode = HID_PROTOCOL_MODE_REPORT;

  // Boot keyboard input report
  // Use same ID and type as key input report
  hidRptMap[2].id = hidReportRefKeyIn[0];
  hidRptMap[2].type = hidReportRefKeyIn[1];
  hidRptMap[2].handle = hidAttrTbl[HID_BOOT_KEY_IN_IDX].handle;
  hidRptMap[2].pCccdAttr = &hidAttrTbl[HID_BOOT_KEY_IN_CCCD_IDX];
  hidRptMap[2].mode = HID_PROTOCOL_MODE_BOOT;

  // Boot keyboard output report
  // Use same ID and type as LED output report
  hidRptMap[3].id = hidReportRefLedOut[0];
  hidRptMap[3].type = hidReportRefLedOut[1];
  hidRptMap[3].handle = hidAttrTbl[HID_BOOT_KEY_OUT_IDX].handle;
  hidRptMap[3].pCccdAttr = NULL;
  hidRptMap[3].mode = HID_PROTOCOL_MODE_BOOT;

  // Boot mouse input report
  hidRptMap[4].id = HID_RPT_ID_MOUSE_IN;
  hidRptMap[4].type = HID_REPORT_TYPE_INPUT;
  hidRptMap[4].handle = hidAttrTbl[HID_BOOT_MOUSE_IN_IDX].handle;
  hidRptMap[4].pCccdAttr = &hidAttrTbl[HID_BOOT_MOUSE_IN_CCCD_IDX];
  hidRptMap[4].mode = HID_PROTOCOL_MODE_BOOT;

  // Feature report
  hidRptMap[5].id = hidReportRefFeature[0];
  hidRptMap[5].type = hidReportRefFeature[1];
  hidRptMap[5].handle = hidAttrTbl[HID_FEATURE_IDX].handle;
  hidRptMap[5].pCccdAttr = NULL;
  hidRptMap[5].mode = HID_PROTOCOL_MODE_REPORT;

  // Battery level input report
  VOID Batt_GetParameter(BATT_PARAM_BATT_LEVEL_IN_REPORT, &(hidRptMap[6]));

  // Setup report ID map
  HidDev_RegisterReports(HID_NUM_REPORTS, hidRptMap);

  return (status);
}

/*********************************************************************
 * @fn      HidKbd_SetParameter
 *
 * @brief   Set a HID Kbd parameter.
 *
 * @param   id     - HID report ID.
 * @param   type   - HID report type.
 * @param   uuid   - attribute uuid.
 * @param   len    - length of data to right.
 * @param   pValue - pointer to data to write.  This is dependent on
 *          the input parameters and WILL be cast to the appropriate
 *          data type (example: data type of uint16 will be cast to
 *          uint16 pointer).
 *
 * @return  GATT status code.
 */
uint8 HidKbd_SetParameter(uint8 id, uint8 type, uint16 uuid, uint8 len,
                          void *pValue)
{
  bStatus_t ret = SUCCESS;

  switch (uuid)
  {
    case REPORT_UUID:
      if (type ==  HID_REPORT_TYPE_OUTPUT)
      {
        if (len == 1)
        {
          hidReportLedOut = *((uint8 *)pValue);
        }
        else
        {
          ret = ATT_ERR_INVALID_VALUE_SIZE;
        }
      }
      else if (type == HID_REPORT_TYPE_FEATURE)
      {
        if (len == 1)
        {
          hidReportFeature = *((uint8 *)pValue);
        }
        else
        {
          ret = ATT_ERR_INVALID_VALUE_SIZE;
        }
      }
      else
      {
        ret = ATT_ERR_ATTR_NOT_FOUND;
      }
      break;

    case BOOT_KEY_OUTPUT_UUID:
      if (len == 1)
      {
        hidReportBootKeyOut = *((uint8 *)pValue);
      }
      else
      {
        ret = ATT_ERR_INVALID_VALUE_SIZE;
      }
      break;

    default:
      // Ignore the request
      break;
  }

  return (ret);
}

/*********************************************************************
 * @fn      HidKbd_GetParameter
 *
 * @brief   Get a HID Kbd parameter.
 *
 * @param   id     - HID report ID.
 * @param   type   - HID report type.
 * @param   uuid   - attribute uuid.
 * @param   pLen   - length of data to be read
 * @param   pValue - pointer to data to get.  This is dependent on
 *          the input parameters and WILL be cast to the appropriate
 *          data type (example: data type of uint16 will be cast to
 *          uint16 pointer).
 *
 * @return  GATT status code.
 */
uint8 HidKbd_GetParameter(uint8 id, uint8 type, uint16 uuid, uint8 *pLen,
                          void *pValue)
{
  switch (uuid)
  {
    case REPORT_UUID:
      if (type ==  HID_REPORT_TYPE_OUTPUT)
      {
        *((uint8 *)pValue) = hidReportLedOut;
        *pLen = 1;
      }
      else if (type == HID_REPORT_TYPE_FEATURE)
      {
        *((uint8 *)pValue) = hidReportFeature;
        *pLen = 1;
      }
      else
      {
        *pLen = 0;
      }
      break;

    case BOOT_KEY_OUTPUT_UUID:
      *((uint8 *)pValue) = hidReportBootKeyOut;
      *pLen = 1;
      break;

    default:
      *pLen = 0;
      break;
  }

  return (SUCCESS);
}


/*********************************************************************
*********************************************************************/
