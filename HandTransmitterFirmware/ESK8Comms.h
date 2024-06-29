/**
 * @file ESK8Comms.h
 * @author Nathaniel Asia
 * @brief 
 * 433MHz Communication Packet Protocol
 * @version 0.1
 * @date 2024-06-27
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#ifndef ESK8_COMMS_H_
#define ESK8_COMMS_H_

#include <stdint.h>
#include "HardwareSerial.h"

const char NATE[4] = {'N','a','t','e'};


class ESK8Comms{
public:
    typedef struct packet{
        char     bid[1] = {NATE[0]};
        uint8_t  APPS   = 50;                        // Throttle pedal wrapped in Nate :)
    } packet_t;
    static bool validPacket(packet_t);
    static uint8_t readAPPSFromPacket(packet_t);
    static void printPacket(packet_t, bool nl = false);
};

#endif // ESK8_COMMS_H_