/*
 * BSD 3-Clause License
 *
 * Copyright (c) 2023, Northern Mechatronics, Inc.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 *    list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its
 *    contributors may be used to endorse or promote products derived from
 *    this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <Arduino.h>
#include <lorawan.h>

using namespace arduino;

static void lorawan_on_receive(lorawan_rx_packet_t *packet, lorawan_rx_params_t *params)
{
}

static void lorawan_on_join_request(lorawan_join_params_t *params)
{
    if (params->Status == LORAMAC_HANDLER_ERROR)
    {
        lorawan_join();
    }
    else
    {
        lorawan_class_set(LORAWAN_CLASS_A);
    }
}

static void lorawan_on_wake(void)
{
    digitalWrite(18, LOW);
}

static void lorawan_on_sleep(void)
{
    digitalWrite(18, HIGH);
}

static void lorawan_setup()
{
    pinMode(18, OUTPUT);
    digitalWrite(18, LOW);

    lorawan_enable();

    lorawan_tracing_set(1);

    lorawan_network_config(LORAWAN_REGION_US915, LORAWAN_DATARATE_0, true, true);

    lorawan_activation_config(LORAWAN_ACTIVATION_OTAA, NULL);
    lorawan_key_set_by_str(LORAWAN_KEY_JOIN_EUI, "b4c231a359bc2e3d");
    lorawan_key_set_by_str(LORAWAN_KEY_APP, "01c3f004a2d6efffe32c4eda14bcd2b4");
    lorawan_key_set_by_str(LORAWAN_KEY_NWK, "3f4ca100e2fc675ea123f4eb12c4a012");

    lorawan_event_callback_register(LORAWAN_EVENT_RX_DATA,
                                    (lorawan_event_callback_t)lorawan_on_receive);
    lorawan_event_callback_register(LORAWAN_EVENT_JOIN_REQUEST,
                                    (lorawan_event_callback_t)lorawan_on_join_request);

    // lorawan_event_callback_register(LORAWAN_EVENT_SLEEP,
    //                                 (lorawan_event_callback_t)lorawan_on_sleep);
    // lorawan_event_callback_register(LORAWAN_EVENT_WAKE, (lorawan_event_callback_t)lorawan_on_wake);

    lorawan_stack_state_set(LORAWAN_STACK_WAKE);

    if (lorawan_get_join_state())
    {
        lorawan_class_set(LORAWAN_CLASS_A);
    }
}

static void button0_handler()
{
    lorawan_join();
}

void setup(void)
{
    Serial.begin(115200);

    pinMode(17, OUTPUT);
    pinMode(14, OUTPUT);
    pinMode(15, OUTPUT);
    pinMode(30, OUTPUT);
    pinMode(10, OUTPUT);

    digitalWrite(17, LOW);
    digitalWrite(14, LOW);
    digitalWrite(15, LOW);
    digitalWrite(30, LOW);
    digitalWrite(10, LOW);

    pinMode(16, INPUT);
    attachInterrupt(16, button0_handler, FALLING);

    lorawan_setup();
}

void loop(void)
{
    delay(500);
    digitalToggle(17);
}
