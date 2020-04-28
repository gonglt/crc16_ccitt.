/*
 * @author Aleksandr Barunin barunin@arrival.com
 *
 * */
#include "crc16_ccitt.h"
#include <stdint.h>

static uint16_t calc_crc16_ccitt_false_fast (const uint8_t pDataf [], uint16_t const lengthf);
static uint16_t calc_crc16_ccitt_false_slow (const uint8_t pDatas [], uint16_t const lengths);

uint16_t calc_crc16_ccitt_false (const uint8_t pData [], uint16_t const length, const uint8_t algType) {
    uint16_t crc16 = 0u;
    if (algType < (uint8_t)AMOUNT_OF_ALG) {
        if ((uint8_t)ALG_FAST == algType) {
            crc16 = calc_crc16_ccitt_false_fast (pData, length);
        } else if ((uint8_t)ALG_SLOW == algType) {
            crc16 = calc_crc16_ccitt_false_slow (pData, length);
        } else {
            crc16 = 0u;
        }
    }

    return crc16;
}

/*
 * calc_crc16_ccitt_false is a (fast algorithm)
 */
static uint16_t calc_crc16_ccitt_false_fast (const uint8_t pDataf [], uint16_t const lengthf) {
    static const uint16_t crc16LookUpTable[LOOK_UP_TABLE_SIZE] = {
            0x0000u, 0x1021u, 0x2042u, 0x3063u, 0x4084u, 0x50a5u, 0x60c6u, 0x70e7u,
            0x8108u, 0x9129u, 0xa14au, 0xb16bu, 0xc18cu, 0xd1adu, 0xe1ceu, 0xf1efu,
            0x1231u, 0x0210u, 0x3273u, 0x2252u, 0x52b5u, 0x4294u, 0x72f7u, 0x62d6u,
            0x9339u, 0x8318u, 0xb37bu, 0xa35au, 0xd3bdu, 0xc39cu, 0xf3ffu, 0xe3deu,
            0x2462u, 0x3443u, 0x0420u, 0x1401u, 0x64e6u, 0x74c7u, 0x44a4u, 0x5485u,
            0xa56au, 0xb54bu, 0x8528u, 0x9509u, 0xe5eeu, 0xf5cfu, 0xc5acu, 0xd58du,
            0x3653u, 0x2672u, 0x1611u, 0x0630u, 0x76d7u, 0x66f6u, 0x5695u, 0x46b4u,
            0xb75bu, 0xa77au, 0x9719u, 0x8738u, 0xf7dfu, 0xe7feu, 0xd79du, 0xc7bcu,
            0x48c4u, 0x58e5u, 0x6886u, 0x78a7u, 0x0840u, 0x1861u, 0x2802u, 0x3823u,
            0xc9ccu, 0xd9edu, 0xe98eu, 0xf9afu, 0x8948u, 0x9969u, 0xa90au, 0xb92bu,
            0x5af5u, 0x4ad4u, 0x7ab7u, 0x6a96u, 0x1a71u, 0x0a50u, 0x3a33u, 0x2a12u,
            0xdbfdu, 0xcbdcu, 0xfbbfu, 0xeb9eu, 0x9b79u, 0x8b58u, 0xbb3bu, 0xab1au,
            0x6ca6u, 0x7c87u, 0x4ce4u, 0x5cc5u, 0x2c22u, 0x3c03u, 0x0c60u, 0x1c41u,
            0xedaeu, 0xfd8fu, 0xcdecu, 0xddcdu, 0xad2au, 0xbd0bu, 0x8d68u, 0x9d49u,
            0x7e97u, 0x6eb6u, 0x5ed5u, 0x4ef4u, 0x3e13u, 0x2e32u, 0x1e51u, 0x0e70u,
            0xff9fu, 0xefbeu, 0xdfddu, 0xcffcu, 0xbf1bu, 0xaf3au, 0x9f59u, 0x8f78u,
            0x9188u, 0x81a9u, 0xb1cau, 0xa1ebu, 0xd10cu, 0xc12du, 0xf14eu, 0xe16fu,
            0x1080u, 0x00a1u, 0x30c2u, 0x20e3u, 0x5004u, 0x4025u, 0x7046u, 0x6067u,
            0x83b9u, 0x9398u, 0xa3fbu, 0xb3dau, 0xc33du, 0xd31cu, 0xe37fu, 0xf35eu,
            0x02b1u, 0x1290u, 0x22f3u, 0x32d2u, 0x4235u, 0x5214u, 0x6277u, 0x7256u,
            0xb5eau, 0xa5cbu, 0x95a8u, 0x8589u, 0xf56eu, 0xe54fu, 0xd52cu, 0xc50du,
            0x34e2u, 0x24c3u, 0x14a0u, 0x0481u, 0x7466u, 0x6447u, 0x5424u, 0x4405u,
            0xa7dbu, 0xb7fau, 0x8799u, 0x97b8u, 0xe75fu, 0xf77eu, 0xc71du, 0xd73cu,
            0x26d3u, 0x36f2u, 0x0691u, 0x16b0u, 0x6657u, 0x7676u, 0x4615u, 0x5634u,
            0xd94cu, 0xc96du, 0xf90eu, 0xe92fu, 0x99c8u, 0x89e9u, 0xb98au, 0xa9abu,
            0x5844u, 0x4865u, 0x7806u, 0x6827u, 0x18c0u, 0x08e1u, 0x3882u, 0x28a3u,
            0xcb7du, 0xdb5cu, 0xeb3fu, 0xfb1eu, 0x8bf9u, 0x9bd8u, 0xabbbu, 0xbb9au,
            0x4a75u, 0x5a54u, 0x6a37u, 0x7a16u, 0x0af1u, 0x1ad0u, 0x2ab3u, 0x3a92u,
            0xfd2eu, 0xed0fu, 0xdd6cu, 0xcd4du, 0xbdaau, 0xad8bu, 0x9de8u, 0x8dc9u,
            0x7c26u, 0x6c07u, 0x5c64u, 0x4c45u, 0x3ca2u, 0x2c83u, 0x1ce0u, 0x0cc1u,
            0xef1fu, 0xff3eu, 0xcf5du, 0xdf7cu, 0xaf9bu, 0xbfbau, 0x8fd9u, 0x9ff8u,
            0x6e17u, 0x7e36u, 0x4e55u, 0x5e74u, 0x2e93u, 0x3eb2u, 0x0ed1u, 0x1ef0u
    };
    uint16_t crc16f = INIT_VALUE;
    uint16_t tableAmtElem = (sizeof(crc16LookUpTable)/sizeof(crc16LookUpTable[0]));
    uint16_t byteInd = 0u;
    uint16_t tabInd = 0u;
    for (byteInd = 0u; byteInd < lengthf; byteInd++) {
        tabInd = ((crc16f >> 8) ^ pDataf[byteInd]) & 0x00ffu;
        if (tabInd < tableAmtElem) {
            crc16f = ((uint16_t)(crc16f << 8)) ^ crc16LookUpTable[tabInd];
        }
    }
    return crc16f;
}

/*
 * calc_crc16_ccitt_false is a (slow algorithm)
 */
static uint16_t calc_crc16_ccitt_false_slow (const uint8_t pDatas [], uint16_t const lengths) {
    uint8_t i = 0u;
    uint16_t outCrc16 = 0u;
    uint16_t wCrc16 = INIT_VALUE;
    uint16_t curLength = lengths;
    uint16_t iter = 0u;
    uint16_t data16bit = 0u;
    uint16_t value;
    while (0u < curLength) {
        curLength--;
        data16bit = (uint16_t) pDatas [iter];
        wCrc16 ^= (uint16_t) (data16bit << 8);
        iter++;
        for (i = 0u; i < 8u; i++) {
            value = wCrc16 & 0x8000u;
            if (0u < value) {
                wCrc16 = ((uint16_t) (wCrc16 << 1)) ^ POLY_VALUE;
            } else {
                wCrc16 = (uint16_t) (wCrc16 << 1);
            }

        }
    }
    outCrc16 = wCrc16 & 0xffffu;
    return outCrc16;
}
