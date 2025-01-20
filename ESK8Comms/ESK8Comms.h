/**
 * @file ESK8Comms.h
 * @author Nathaniel Asia
 * @brief 433MHz Communication Protocol for Electric Skateboard
 * @version 0.1
 * @date 2024-06-27
 * 
 * @copyright Copyright (c) 2024
 */

#ifndef ESK8_COMMS_H_
#define ESK8_COMMS_H_

#include <stdint.h>
#include "HardwareSerial.h"

/// Signature bytes for packet validation
const char NATE[4] = {'N','a','t','e'};

/**
 * @class ESK8Comms
 * @brief Handles communication protocol between skateboard and remote
 * 
 * Implements a simple packet-based protocol for transmitting throttle
 * control data over 433MHz radio link. Includes packet validation
 * and data extraction functionality.
 */
class ESK8Comms {
public:
    /**
     * @struct packet
     * @brief Communication packet structure
     * 
     * Contains throttle position data with a signature byte
     * for packet validation. Total size: 2 bytes
     */
    typedef struct packet {
        char     bid[1] = {NATE[0]}; ///< Signature byte for validation
        uint8_t  APPS   = 50;        ///< Throttle position (0-100%)
    } packet_t;

    /**
     * @brief Validate received packet signature
     * @param packet Packet to validate
     * @return true if packet signature is valid
     */
    static bool validPacket(packet_t packet);

    /**
     * @brief Extract throttle position from packet
     * @param packet Packet containing throttle data
     * @return Throttle position as percentage (0-100)
     */
    static uint8_t readAPPSFromPacket(packet_t packet);

    /**
     * @brief Print packet contents for debugging
     * @param packet Packet to display
     * @param nl Optional newline flag (default: false)
     */
    static void printPacket(packet_t packet, bool nl = false);
};

#endif // ESK8_COMMS_H_
