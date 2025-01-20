#ifndef VOLTMETER_H_
#define VOLTMETER_H_

#include <stdint.h>
#include "Arduino.h"

/// Maximum battery voltage for 6S Li-ion pack (4.2V * 6 cells)
#define MAX_BAT_V 25.2
/// Minimum battery voltage for 6S Li-ion pack (3.0V * 6 cells)
#define MIN_BAT_V 18.0

/**
 * @struct vToSOC_t
 * @brief Structure to map battery voltage to state of charge percentage
 */
typedef struct vToSOC_t {
    float voltage;    ///< Battery voltage in volts
    uint8_t soc;     ///< State of charge percentage (0-100%)
} voltageToSOC;

/**
 * @brief Lookup table for 6S Li-ion battery voltage to state of charge mapping
 * Values are based on typical discharge curve for Li-ion cells
 */
const voltageToSOC lithium_ion_6s_SOC[] = {
    {18.0, 0},   ///< 0% SOC at 3.0V per cell
    {19.8, 10},  ///< 10% SOC at 3.3V per cell
    {21.6, 50},  ///< 50% SOC at 3.6V per cell
    {23.2, 90},  ///< 90% SOC at 3.87V per cell
    {25.2, 100}, ///< 100% SOC at 4.2V per cell
};

/**
 * @class VoltMeter
 * @brief Manages battery voltage measurement and state of charge calculation
 * 
 * Provides voltage measurement with moving average filtering and
 * converts voltage readings to battery state of charge percentage
 * using a lookup table for 6S Li-ion battery packs.
 */
class VoltMeter {
private:
    uint8_t _pin;              ///< Analog input pin for voltage measurement
    float   *v_avg_l;          ///< Circular buffer for voltage readings
    uint8_t _v_avg_l_size;     ///< Size of voltage readings buffer
    float   v_l_sum;           ///< Running sum of voltage readings
    uint8_t v_l_oldest_index;  ///< Index of oldest reading in circular buffer
    uint8_t _v_list_count;     ///< Number of readings currently in buffer
    float   voltage;           ///< Current raw voltage reading
    float   voltAvg;           ///< Current averaged voltage reading
    float   stateOfCharge;     ///< Calculated state of charge percentage
    
    /**
     * @brief Read raw voltage from ADC
     * @return Voltage in volts, scaled based on voltage divider
     */
    float   readVoltage();

    /**
     * @brief Calculate moving average of voltage readings
     * @return Averaged voltage in volts
     */
    float   averageVoltage();
    
public:
    /**
     * @brief Default constructor
     */
    VoltMeter() {}

    /**
     * @brief Initialize the voltage meter
     * @param pin Analog input pin number
     * @param num_v_readings_for_avg Number of readings to average (default: 1)
     */
    void      init(uint8_t pin, uint8_t num_v_readings_for_avg = 1);

    /**
     * @brief Get current battery state of charge
     * @return State of charge as percentage (0-100)
     */
    uint16_t  getSOC();
};

#endif // VOLTMETER_H_
