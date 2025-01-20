#include "voltmeter.h"

void VoltMeter::init(uint8_t pin, uint8_t num_v_readings_for_avg) {
    this->_pin = pin;
    
    this->v_avg_l = (float*)malloc(num_v_readings_for_avg * sizeof(float));
    this->_v_avg_l_size = num_v_readings_for_avg;
    this->v_l_oldest_index = 0;
    this->v_l_sum = 0;
    this->_v_list_count = 0;

    for(int i = 0; i < this->_v_avg_l_size; i++) { 
        this->v_avg_l[i] = 0; 
    }
}

float VoltMeter::readVoltage() {
    this->voltage = analogRead(this->_pin);
    this->voltage *= 5.0;  // ADC Voltage Max
    this->voltage *= 9.0;  // Voltage Divider Multiplier Inverse 
    this->voltage /= 1023.0;
    return this->voltage;
}

float VoltMeter::averageVoltage() {
    this->v_l_sum -= this->v_avg_l[this->v_l_oldest_index];
    this->v_avg_l[this->v_l_oldest_index] = readVoltage();
    this->v_l_sum += this->v_avg_l[this->v_l_oldest_index];
    this->v_l_oldest_index = (this->v_l_oldest_index + 1) % this->_v_avg_l_size;

    if (this->_v_list_count < this->_v_avg_l_size) {
        this->_v_list_count++;
    }

    return this->voltAvg = this->v_l_sum / (float)this->_v_list_count;
}

uint16_t VoltMeter::getSOC() {
    this->averageVoltage();

    const int numEntries = sizeof(lithium_ion_6s_SOC) / sizeof(lithium_ion_6s_SOC[0]);

    if (this->voltAvg <= lithium_ion_6s_SOC[0].voltage) {
        return lithium_ion_6s_SOC[0].soc;
    }

    if (this->voltAvg >= lithium_ion_6s_SOC[numEntries-1].voltage) {
        return lithium_ion_6s_SOC[numEntries-1].soc;
    }

    for (int i = 1; i < numEntries; i++) {
        if (this->voltAvg < lithium_ion_6s_SOC[i].voltage) {
            float slope = (float)(lithium_ion_6s_SOC[i].soc - lithium_ion_6s_SOC[i-1].soc) /
                         (lithium_ion_6s_SOC[i].voltage - lithium_ion_6s_SOC[i-1].voltage);
            return lithium_ion_6s_SOC[i-1].soc + 
                   slope * (this->voltAvg - lithium_ion_6s_SOC[i-1].voltage);
        }
    }
    return 0;
}