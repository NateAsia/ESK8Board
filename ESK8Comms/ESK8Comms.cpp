/**
 * @file ESK8Comms.cpp
 * @author Nathaniel Asia
 * @brief 
 * 433MHz Communication Packet Protocol Implementations 
 * @version 0.1
 * @date 2024-06-27
 * 
 * 
 */


#include "ESK8Comms.h"
#include "HardwareSerial.h"

bool ESK8Comms::validPacket(packet_t packet){
    return  (packet.bid[0] == NATE[0]);
}

uint8_t ESK8Comms::readAPPSFromPacket(packet_t packet){
    return packet.APPS;
}

void ESK8Comms::printPacket(packet_t packet, bool nl = false){
    Serial.print((*(uint16_t*) &packet), HEX);
    Serial.print("\tRADIO PACKET: ");
    for(int i = sizeof(ESK8Comms::packet)-1; i>=0; i--){
        Serial.print(*((uint8_t*) &packet+i), HEX);
        if(i-1!=-1) Serial.print("-");
    }
    if (nl) Serial.println();
}