/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2023 David Williams (davidthings)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 */

#include "pwm.h"
#include "hardware/gpio.h"
#include "hardware/clocks.h"
#include "hardware/pwm.h"

void clk_pwm_init( int clk_pin, int freq_hz )
{
    // Set pin for PWM
    gpio_set_function(clk_pin, GPIO_FUNC_PWM);

    // PWM slice index
    uint slice_num = pwm_gpio_to_slice_num(clk_pin);
    uint chan  = pwm_gpio_to_channel(clk_pin);

    // Get PWM clock frequency (usually clk_sys = 125–133 MHz)
    uint32_t pwm_clk = clock_get_hz(clk_sys);
    float clock_freq = 125000000.0f;  // Default Pico clock frequency in Hz

    // Compute divider + wrap value
    // freq = pwm_clk / (divider * (wrap + 1))
    uint32_t divider = 1;
    uint32_t wrap = (clock_freq / freq_hz) - 1;
    if (wrap > 65535) wrap = 65535;  // safety
#if 0
    // Configure PWM
    pwm_config config = pwm_get_default_config();

    // Apply settings
    pwm_config_set_clkdiv(&config, (float)divider);
    pwm_config_set_wrap(&config, wrap);

    // 50% duty cycle
    pwm_set_chan_level(slice_num, chan, wrap / 2);

    // Start PWM
    pwm_init(slice_num, &config, true);
#else
    // Set divider
    pwm_set_clkdiv(slice_num, (float)divider);

    // Set the PWM counter wrap value
    pwm_set_wrap(slice_num, wrap);

    // 50% duty cycle
    // pwm_set_chan_level(slice_num, chan, wrap / 2);
    pwm_set_gpio_level(clk_pin, (uint16_t)(0.5 * 65536));

    // Start PWM
    pwm_set_enabled(slice_num, true);
#endif
}
