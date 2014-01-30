/*
  SoftSerialFirmata.h - Firmata library
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

#ifndef SoftSerialFirmata_h
#define SoftSerialFirmata_h

#include <Firmata.h>

#define SW_SERIAL_CONFIG   0x00
#define SW_SERIAL_TX       0x01
#define SW_SERIAL_RX       0x02
#define SW_SERIAL_FLUSH    0x03

class SoftSerialFirmata:public FirmataFeature
{
public:
  boolean handlePinMode(byte pin, int mode);
  void handleCapability(byte pin);
  boolean hnadleSysex(byte command, byte argc, byte *argv);
  void reset();
private:
  SoftwareSerial *swSerial;
};

#endif