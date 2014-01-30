/*
  SoftSerialFirmata.cpp - Firmata library
  Copyright (C) 2006-2008 Hans-Christoph Steiner.  All rights reserved.
  Copyright (C) 2010-2011 Paul Stoffregen.  All rights reserved.
  Copyright (C) 2009 Shigeru Kobayashi.  All rights reserved.
  Copyright (C) 2009-2014 Jeff Hoefs.  All rights reserved.
  Copyright (C) 2013 Norbert Truchsess. All rights reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  See file LICENSE.txt for further informations on licensing terms.
*/

#include <SoftSerialFirmata.h>
#include <Firmata.h>
#include <Encoder7Bit.h>
#include <SoftwareSerial.h>

boolean SoftSerialFirmata::handlePinMode(byte pin, int mode)
{
  if (mode == SW_SERIAL) {

  }
  return true;

}

void SoftSerialFirmata::hangleCapability(byte pin)
{
  if (IS_PIN_DIGITAL(pin)) {
    Firmata.write(SW_SERIAL);
    Firmata.write(8); // arbitrary
  }

}

boolean StepperFirmata::handleSysex(byte command, byte argc, byte *argv)
{
  if (command == SW_SERIAL_DATA) {

    subCommand = argv[0];

    if (subCommand == SW_SERIAL_CONFIG) {
      byte txPin = argv[1];
      byte rxPin = argv[2];
      long baud = (long)argv[3] | ((long)argv[4] << 7) | ((long)argv[5] << 14);

      if (!swSerial) {
        swSerial = new SoftwareSerial(txPin, rxPin);
        swSerial->begin(baud);
      }
    } else if (subCommand == SW_SERIAL_TX) {
      // read contents of sw serial buffer and send to Firmata clients
      if (swSerial->available() > 0) {
        Firmata.write(START_SYSEX);
        Firmata.write(SW_SERIAL_DATA);
        Firmata.write(SW_SERIAL_TX);
        Firmata.write(txPin);
        Encoder7Bit.startBinaryWrite();
        while (swSerial->available() > 0) {
          Encoder7Bit.writeBinary(swSerial->read());
        }
        Encoder7Bit.endBinaryWrite();
        Firmata.write(END_SYSEX);
      }
    } else if (subCommand == SW_SERIAL_RX) {
      // receive serial data from Firmata client, reassemble and
      // call swSerial->write(data) for each byte
      byte data;

      // reassemble data bytes and forward to sw serial write buffer
      for (byte i = 2; i < argc; i += 2) {
        data = argv[i] + (argv[i + 1] << 7);
        swSerial->write(data);
      }
    }

  }
}

void SoftSerialFirmata::reset()
{
  if (swSerial) {
    free(swSerial);
  }
}
