/**********************************************
*       missile.h                             *
*          by dovotos pcx->gba program        *
**********************************************/
#define  missile_WIDTH   16
#define  missile_HEIGHT  64


const u16 missileData[] = {
                    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
                    0x0000, 0x0000, 0x0000, 0x4D00, 0x4D4D, 0x0000, 0x0000, 0x0000, 0x4D4D, 0x4D4D,
                    0x0000, 0x2B2B, 0x4D2B, 0x4D4D, 0x2B00, 0x2727, 0x2B27, 0x4D4D, 0x272B, 0x0303,
                    0x2727, 0x4D4D, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
                    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
                    0x0000, 0x0000, 0x4D4D, 0x4D4D, 0x0606, 0x0000, 0x4D4D, 0x4D4D, 0x0606, 0x0006,
                    0x4D4D, 0x4D4D, 0x0606, 0x0606, 0x2B00, 0x2727, 0x2B27, 0x4D4D, 0x0000, 0x2B2B,
                    0x4D2B, 0x4D4D, 0x0000, 0x0000, 0x4D4D, 0x4D4D, 0x0000, 0x4D00, 0x4D4D, 0x0000,
                    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
                    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x4D4D, 0x4D4D, 0x0606, 0x0006,
                    0x4D4D, 0x4D4D, 0x0606, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
                    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
                    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
                    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
                    0x0000, 0x4D00, 0x4D4D, 0x0000, 0x0000, 0x0000, 0x4D4D, 0x4D4D, 0x0000, 0x2727,
                    0x4D27, 0x4D4D, 0x2700, 0x2B2B, 0x272B, 0x4D4D, 0x2B27, 0x0303, 0x2B2B, 0x4D4D,
                    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
                    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
                    0x4D4D, 0x4D4D, 0x0606, 0x0000, 0x4D4D, 0x4D4D, 0x0606, 0x0006, 0x4D4D, 0x4D4D,
                    0x0606, 0x0606, 0x2700, 0x2B2B, 0x272B, 0x4D4D, 0x0000, 0x2727, 0x4D27, 0x4D4D,
                    0x0000, 0x0000, 0x4D4D, 0x4D4D, 0x0000, 0x4D00, 0x4D4D, 0x0000, 0x0000, 0x0000,
                    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
                    0x0000, 0x0000, 0x0000, 0x0000, 0x4D4D, 0x4D4D, 0x0606, 0x0006, 0x4D4D, 0x4D4D,
                    0x0606, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
                    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
                    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
                    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
                    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x5300,
                    0x0000, 0x0000, 0x0000, 0x5353, 0x0000, 0x0000, 0x5300, 0x2053, 0x0000, 0x0000,
                    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
                    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
                    0x0000, 0x0000, 0x5353, 0x0000, 0x0000, 0x0000, 0x5320, 0x0053, 0x0000, 0x0000,
                    0x0000, 0x0000, 0x5300, 0x2053, 0x0000, 0x0000, 0x0000, 0x5353, 0x0000, 0x0000,
                    0x0000, 0x5300, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
                    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
                    0x0000, 0x0000, 0x5320, 0x0053, 0x0000, 0x0000, 0x5353, 0x0000, 0x0000, 0x0000,
                    0x0053, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
                    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
                    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
                    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
                    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
                    0x5300, 0x5300, 0x0000, 0x5300, 0x0000, 0x2020, 0x0000, 0x0000, 0x0000, 0x0000,
                    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
                    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x5300, 0x0000, 0x0000, 0x0000,
                    0x0053, 0x0000, 0x0000, 0x0000, 0x534F, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
                    0x2053, 0x4F4F, 0x0000, 0x0000, 0x0000, 0x4F20, 0x0000, 0x0000, 0x0053, 0x2053,
                    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x5300, 0x0000, 0x0000,
                    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
                    0x0020, 0x0000, 0x0000, 0x0000, 0x5320, 0x5300, 0x0000, 0x0000, 0x0000, 0x0000,
                    0x0000, 0x0000, 0x5300, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
                    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
                    0x0000, 0x0000,};

const u16 missilePalette[] = {
                    0x0000, 0x0002, 0x043E, 0x0C7A, 0x18D6, 0x1CEC, 0x0853, 0x042F, 0x042A, 0x211E,
                    0x2D7D, 0x463D, 0x5ADD, 0x7426, 0x5CE9, 0x3D09, 0x20A6, 0x5024, 0x2802, 0x750B,
                    0x75D0, 0x7654, 0x76F8, 0x0BA6, 0x16E8, 0x1DE8, 0x1926, 0x06A5, 0x0562, 0x1FAB,
                    0x3FB1, 0x57B6, 0x0BBD, 0x1339, 0x1E10, 0x194A, 0x06B5, 0x01EF, 0x0129, 0x1BBD,
                    0x2FBD, 0x47BD, 0x5BBD, 0x09FC, 0x15D6, 0x1D70, 0x190A, 0x0575, 0x0510, 0x00CB,
                    0x161C, 0x2A7C, 0x3ABD, 0x5B5E, 0x7458, 0x5CD4, 0x40EF, 0x28EA, 0x5432, 0x402D,
                    0x2808, 0x74F9, 0x755A, 0x763B, 0x76DC, 0x6B83, 0x52C6, 0x3E08, 0x2946, 0x4EA2,
                    0x3E01, 0x2D81, 0x6FA9, 0x6FAE, 0x73B3, 0x77B8, 0x294A, 0x4A52, 0x6B5A, 0x7FFF,
                    0x0C63, 0x0065, 0x17F6, 0x0A1D, 0x56B5, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
                    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
                    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
                    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
                    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
                    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
                    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
                    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
                    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
                    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
                    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
                    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
                    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
                    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
                    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
                    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
                    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
                    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,};
