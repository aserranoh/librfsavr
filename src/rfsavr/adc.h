/*
adc.h - Use the ADC subsystem of the AVR microcontroller.

This file is part of RobotsFromScratch.

Copyright 2023 Antonio Serrano Hernandez

RobotsFromScratch is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

RobotsFromScratch is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with RobotsFromScratch; see the file COPYING.  If not, see
<http://www.gnu.org/licenses/>.
*/

#ifndef RFS_ADC_H
#define RFS_ADC_H

#include <avr/io.h>
#include <rfsavr/bits.h>

#define RFS_ADC_AUTO_TRIGGER_SOURCE_MASK    0b00000111
#define RFS_ADC_CHANNEL_MASK                0b00001111
#define RFS_ADC_PRESCALER_MASK              0b00000111
#define RFS_ADC_VOLTAGE_REF_MASK            0b11000000

/**
 * @brief Enumeration for the adjustment values
 */
enum rfs_adc_adjustment {
    RFS_ADC_RIGHT = 0,
    RFS_ADC_LEFT = _BV(ADLAR)
};

/**
 * @brief Enumeration for the auto-trigger sources
 */
enum rfs_adc_autotriggersource {
    RFS_ADC_FREERUNNING,
    RFS_ADC_ANALOGCOMPARATOR,
    RFS_ADC_EXTINT0,
    RFS_ADC_TIMER0_COMPAREMATCH,
    RFS_ADC_TIMER0_OVERFLOW,
    RFS_ADC_TIMER1_COMPAREMATCH,
    RFS_ADC_TIMER1_OVERFLOW,
    RFS_ADC_TIMER1_CAPTURE
};

/**
 * @brief Enumeration for the ADC channels that can be multiplexed
 */
enum rfs_adc_channel {
    RFS_ADC_CHANNEL_ADC0,
    RFS_ADC_CHANNEL_ADC1,
    RFS_ADC_CHANNEL_ADC2,
    RFS_ADC_CHANNEL_ADC3,
    RFS_ADC_CHANNEL_ADC4,
    RFS_ADC_CHANNEL_ADC5,
    RFS_ADC_CHANNEL_ADC6,
    RFS_ADC_CHANNEL_ADC7,
    RFS_ADC_CHANNEL_ADC8,
    RFS_ADC_CHANNEL_VBG,
    RFS_ADC_CHANNEL_GND
};

/**
 * @brief Enumeration for the digital inputs associated with the ADC channels
 */
enum rfs_adc_digitalinputs {
    RFS_ADC_ADC0 = 0b00000001,
    RFS_ADC_ADC1 = 0b00000010,
    RFS_ADC_ADC2 = 0b00000100,
    RFS_ADC_ADC3 = 0b00001000,
    RFS_ADC_ADC4 = 0b00010000,
    RFS_ADC_ADC5 = 0b00100000,
};

/**
 * @brief Enumeration for the ADC clock prescaler values
 */
enum rfs_adc_prescaler {
    RFS_ADC_2,
    RFS_ADC_4 = 2,
    RFS_ADC_8,
    RFS_ADC_16,
    RFS_ADC_32,
    RFS_ADC_64,
    RFS_ADC_128
};

/**
 * @brief Enumeration for the ADC voltage reference
 */
enum rfs_adc_reference {
    RFS_ADC_AREF = 0b00000000,
    RFS_ADC_AVCC = 0b01000000,
    RFS_ADC_1_1  = 0b11000000,
};

/**
 * @brief Enable some digital inputs
 * 
 * @param inputs The inputs to enable. This is an "or" of enum rfs_adc_digitalinputs.
 */
inline void rfs_adc_enabledigitalinputs(int8_t inputs)
{
    DIDR0 &= ~inputs;
}

/**
 * @brief Disable some digital inputs
 * 
 * @param inputs The inputs to disable. This is an "or" of enum rfs_adc_digitalinputs.
 */
inline void rfs_adc_disabledigitalinputs(int8_t inputs)
{
    DIDR0 |= inputs;
}

/**
 * @brief Check whether the ADC conversion has finished. If so, read the result.
 * 
 * This function reads only the 8 most significant bits of the conversion.
 * To use this function, the result of the conversion must be left aligned.
 * Return 0 if the ADC conversion is still in progress, a number different than 0 otherwise.
 * When the conversion is finished, the result is read and returned in the variable pointed
 * by result.
 * To see if the conversion has finished, the ADC Interrupt Flag is checked. If the conversion
 * has finished, this flag is cleared.
 * 
 * @returns Whether the ADC conversion in progress has finished
 */
int8_t rfs_adc_get8(uint8_t *result);

/**
 * @brief Check whether the ADC conversion has finished. If so, read the result.
 * 
 * This function reads the whole 10-bit result of the conversion.
 * Return 0 if the ADC conversion is still in progress, a number different than 0 otherwise.
 * When the conversion is finished, the result is read and returned in the variable pointed
 * by result.
 * To see if the conversion has finished, the ADC Interrupt Flag is checked. If the conversion
 * has finished, this flag is cleared.
 * 
 * @returns Whether the ADC conversion in progress has finished
 */
int8_t rfs_adc_get16(uint16_t *result);

/**
 * @brief Set the adjusment of the ADC result word
 * 
 * @param adjustment The adjustment of the ADC result word
 */
inline void rfs_adc_setadjustment(enum rfs_adc_adjustment adjustment)
{
    rfs_bits_setbit(ADMUX, ADLAR, adjustment);
}

/**
 * @brief Set the enable status of the auto-trigger function
 * 
 * @param enabled Whether the auto-trigger function is enabled
 */
inline void rfs_adc_setautotrigger(int8_t enabled)
{
    rfs_bits_setbit(ADCSRA, ADATE, enabled);
}

/**
 * @brief Set the auto-trigger source
 * 
 * @param source The auto-trigger source
 */
inline void rfs_adc_setautotriggersource(enum rfs_adc_autotriggersource source)
{
    rfs_bits_setmask(ADCSRB, RFS_ADC_AUTO_TRIGGER_SOURCE_MASK, source);
}

/**
 * @brief Set the channel to convert
 * 
 * @param channel The channel
 */
inline void rfs_adc_setchannel(enum rfs_adc_channel channel)
{
    rfs_bits_setmask(ADMUX, RFS_ADC_CHANNEL_MASK, channel);
}

/**
 * @brief Set the enable status of the ADC
 * 
 * @param enabled Whether the ADC is enabled
 */
inline void rfs_adc_setenabled(int8_t enabled)
{
    rfs_bits_setbit(ADCSRA, ADEN, enabled);
}

/**
 * @brief Set the enable status of the ADC interrupt
 * 
 * @param enabled Whether the ADC interrupt is enabled
 */
inline void rfs_adc_setinterruptenabled(int8_t enabled)
{
    rfs_bits_setbit(ADCSRA, ADIE, enabled);
}

/**
 * @brief Set the clock prescaler for the ADC clock
 * 
 * @param enabled Whether the ADC interrupt is enabled
 */
inline void rfs_adc_setprescaler(enum rfs_adc_prescaler prescaler)
{
    rfs_bits_setmask(ADCSRA, RFS_ADC_PRESCALER_MASK, prescaler);
}

/**
 * @brief Set the reference voltage for the ADC conversion
 * 
 * @param enabled Whether the ADC interrupt is enabled
 */
inline void rfs_adc_setreference(enum rfs_adc_reference reference)
{
    rfs_bits_setmask(ADMUX, RFS_ADC_VOLTAGE_REF_MASK, reference);
}

/**
 * @brief Start the ADC conversion
 */
inline void rfs_adc_start()
{
    ADCSRA |= _BV(ADSC);
}

#endif