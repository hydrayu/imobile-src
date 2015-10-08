using System;
using System.Collections.Generic;
using System.Text;

namespace iMobileBarcode
{
    internal static class SSI
    {
        internal struct CodeTypeDB
        {
            private readonly byte codeType;
            private readonly string codeTypeStr;
            public CodeTypeDB (byte codeType, string codeTypeStr)
            {
                this.codeType = codeType;
                this.codeTypeStr = codeTypeStr;
            }
            public byte CodeType { get { return codeType; } }
            public string CodeTypeStr { get { return codeTypeStr; } }
        }
        internal const byte OP_FLUSH_MACRO_PDF = 0x10;
        internal const byte OP_ABORT_MACRO_PDF = 0x11;
        internal const byte OP_REQUEST_REVISION = 0xA3;
        internal const byte OP_REPLY_REVISION = 0xA4;
        internal const byte OP_IMAGE_DATA = 0xB1;
        internal const byte OP_VIDEO_DATA = 0xB4;
        internal const byte OP_AIM_OFF = 0xC4;
        internal const byte OP_AIM_ON = 0xC5;
        internal const byte OP_PARAM_SEND = 0xC6;
        internal const byte OP_PARAM_REQUEST = 0xC7;
        internal const byte OP_PARAM_DEFAULTS = 0xC8;
        internal const byte OP_CMD_ACK = 0xD0;
        internal const byte OP_CMD_NAK = 0xD1;
        internal const byte OP_FLUSH_QUEUE = 0xD2;
        internal const byte OP_CAPABILITIES_REQUEST = 0xD3;
        internal const byte OP_CAPABILITIES_REPLY = 0xD4;
        internal const byte OP_BATCH_REQUEST = 0xD5;
        internal const byte OP_BATCH_DATA = 0xD6;
        internal const byte OP_START_SESSION = 0xE4;
        internal const byte OP_STOP_SESSION = 0xE5;
        internal const byte OP_BEEP = 0xE6;
        internal const byte OP_LED_ON = 0xE7;
        internal const byte OP_LED_OFF = 0xE8;
        internal const byte OP_SCAN_ENABLE = 0xE9;
        internal const byte OP_SCAN_DISABLE = 0xEA;
        internal const byte OP_SLEEP = 0xEB;
        internal const byte OP_DECODE_DATA = 0xF3;
        internal const byte OP_EVENT = 0xF6;
        internal const byte OP_IMAGER_MODE = 0xF7;
        internal static readonly byte[] ACK = new byte[] { 0x04, 0xD0, 0x04, 0x00, 0xFF, 0x28 };
        internal static readonly CodeTypeDB[] codeTypeMaps = 
            new CodeTypeDB[]
            {
                new CodeTypeDB(0x01, "Code 39"),
                new CodeTypeDB(0x02, "Codebar"),
                new CodeTypeDB(0x03, "Code 128"),
                new CodeTypeDB(0x04, "D25"),
                new CodeTypeDB(0x05, "IATA"),
                new CodeTypeDB(0x06, "ITF"),
                new CodeTypeDB(0x07, "Code 93"),
                new CodeTypeDB(0x08, "UPCA"),
                new CodeTypeDB(0x09, "UPCE"),
                new CodeTypeDB(0x0A, "EAN-8"),
                new CodeTypeDB(0x0B, "EAN-13"),
                new CodeTypeDB(0x0C, "Code 11"),
                new CodeTypeDB(0x0E, "MSI"),
                new CodeTypeDB(0x0F, "EAN-128"),
                new CodeTypeDB(0x10, "UPCE1"),
                new CodeTypeDB(0x11, "PDF-417"),
                new CodeTypeDB(0x13, "Code 39 Full ASCII"),
                new CodeTypeDB(0x15, "Trioptic"),
                new CodeTypeDB(0x16, "Bookland"),
                new CodeTypeDB(0x17, "Coupon Code"),
                new CodeTypeDB(0x19, "ISBT-128"),
                new CodeTypeDB(0x1A, "Micro PDF"),
                new CodeTypeDB(0x1B, "Data Matrix"),
                new CodeTypeDB(0x1C, "QR Code"),
                new CodeTypeDB(0x1E, "Postnet (US)"),
                new CodeTypeDB(0x1F, "Planet (US)"),
                new CodeTypeDB(0x20, "Code 32"),
                new CodeTypeDB(0x21, "ISBT-128 Concat."),
                new CodeTypeDB(0x22, "Postal (Japan)"),
                new CodeTypeDB(0x23, "Postal (Australia)"),
                new CodeTypeDB(0x24, "Postal (Dutch)"),
                new CodeTypeDB(0x25, "Maxicode"),
                new CodeTypeDB(0x26, "Postbar (CA)"),
                new CodeTypeDB(0x27, "Postal (UK)"),
                new CodeTypeDB(0x28, "Macro PDF-417"),
                new CodeTypeDB(0x30, "RSS-14"),
                new CodeTypeDB(0x31, "RSS-Limited"),
                new CodeTypeDB(0x32, "RSS-Expanded"),
                new CodeTypeDB(0x37, "Scanlet Webcode"),
                new CodeTypeDB(0x38, "Cue CAT Code"),
                new CodeTypeDB(0x48, "UPCA + 2"),
                new CodeTypeDB(0x49, "UPCE + 2"),
                new CodeTypeDB(0x4A, "EAN-8 + 2"),
                new CodeTypeDB(0x4B, "EAN-13 + 2"),
                new CodeTypeDB(0x50, "UPCE1 + 2"),
                new CodeTypeDB(0x51, "CC-A + EAN-128"),
                new CodeTypeDB(0x52, "CC-A + EAN-13"),
                new CodeTypeDB(0x53, "CC-A + EAN-8"),
                new CodeTypeDB(0x54, "CC-A + RSS Expanded"),
                new CodeTypeDB(0x55, "CC-A + RSS Limited"),
                new CodeTypeDB(0x56, "CC-A + RSS-14"),
                new CodeTypeDB(0x57, "CC-A + UPC-A"),
                new CodeTypeDB(0x58, "CC-A + UPC-E"),
                new CodeTypeDB(0x59, "CC-C + EAN-128"),
                new CodeTypeDB(0x5A, "TLC-39"),
                new CodeTypeDB(0x61, "CC-B + EAN-128"),
                new CodeTypeDB(0x62, "CC-B + EAN-13"),
                new CodeTypeDB(0x63, "CC-B + EAN-8"),
                new CodeTypeDB(0x64, "CC-B + RSS Expanded"),
                new CodeTypeDB(0x65, "CC-B + RSS Limited"),
                new CodeTypeDB(0x66, "CC-B + RSS-14"),
                new CodeTypeDB(0x67, "CC-B + UPC-A"),
                new CodeTypeDB(0x68, "CC-B + UPC-E"),
                new CodeTypeDB(0x88, "UPCA + 5"),
                new CodeTypeDB(0x89, "UPCE + 5"),
                new CodeTypeDB(0x8A, "EAN-8 + 5"),
                new CodeTypeDB(0x8B, "EAN-13 + 5"),
                new CodeTypeDB(0x90, "UPCE1 + 5"),
                new CodeTypeDB(0x9A, "Macro Micro PDF"),
            };
        internal static string codeTypeToStr(byte t)
        {
            for (int i = 0; i < codeTypeMaps.Length; i++)
            {
                if (codeTypeMaps[i].CodeType == t) return codeTypeMaps[i].CodeTypeStr;
            }
            return "";
        }
    }
}
