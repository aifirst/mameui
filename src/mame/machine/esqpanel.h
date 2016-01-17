// license:BSD-3-Clause
// copyright-holders:R. Belmont
#pragma once

#ifndef __ESQPANEL_H__
#define __ESQPANEL_H__

#include "emu.h"
#include "machine/esqvfd.h"

//**************************************************************************
//  INTERFACE CONFIGURATION MACROS
//**************************************************************************

#define MCFG_ESQPANEL1x22_ADD(_tag) \
	MCFG_DEVICE_ADD(_tag, ESQPANEL1x22, 0)

#define MCFG_ESQPANEL1x22_REPLACE(_tag) \
	MCFG_DEVICE_REPLACE(_tag, ESQPANEL1x22, 0)

#define MCFG_ESQPANEL1x22_REMOVE(_tag) \
	MCFG_DEVICE_REMOVE(_tag)

#define MCFG_ESQPANEL2x40_ADD(_tag) \
	MCFG_DEVICE_ADD(_tag, ESQPANEL2x40, 0)

#define MCFG_ESQPANEL2x40_REPLACE(_tag) \
	MCFG_DEVICE_REPLACE(_tag, ESQPANEL2x40, 0)

#define MCFG_ESQPANEL_2x40_REMOVE(_tag) \
	MCFG_DEVICE_REMOVE(_tag)

#define MCFG_ESQPANEL2x40_SQ1_ADD(_tag) \
	MCFG_DEVICE_ADD(_tag, ESQPANEL2x40_SQ1, 0)

#define MCFG_ESQPANEL2x40_SQ1_REPLACE(_tag) \
	MCFG_DEVICE_REPLACE(_tag, ESQPANEL2x40_SQ1, 0)

#define MCFG_ESQPANEL2x40_SQ1_REMOVE(_tag) \
	MCFG_DEVICE_REMOVE(_tag)

#define MCFG_ESQPANEL_TX_CALLBACK(_write) \
	devcb = &esqpanel_device::set_tx_wr_callback(*device, DEVCB_##_write);

#define MCFG_ESQPANEL_ANALOG_CALLBACK(_write) \
	devcb = &esqpanel_device::set_analog_wr_callback(*device, DEVCB_##_write);

//**************************************************************************
//  TYPE DEFINITIONS
//**************************************************************************

// ======================> esqpanel_device

class esqpanel_device :  public device_t, public device_serial_interface
{
public:
	// construction/destruction
	esqpanel_device(const machine_config &mconfig, device_type type, std::string name, std::string tag, device_t *owner, UINT32 clock, std::string shortname, std::string source);

	template<class _Object> static devcb_base &set_tx_wr_callback(device_t &device, _Object object) { return downcast<esqpanel_device &>(device).m_write_tx.set_callback(object); }
	template<class _Object> static devcb_base &set_analog_wr_callback(device_t &device, _Object object) { return downcast<esqpanel_device &>(device).m_write_analog.set_callback(object); }

	virtual void send_to_display(UINT8 data) = 0;

	void xmit_char(UINT8 data);
	void set_analog_value(offs_t offset, UINT16 value);

protected:
	// device-level overrides
	virtual void device_start() override;
	virtual void device_reset() override;
	virtual void device_timer(emu_timer &timer, device_timer_id id, int param, void *ptr) override;

	// serial overrides
	virtual void rcv_complete() override;    // Rx completed receiving byte
	virtual void tra_complete() override;    // Tx completed sending byte
	virtual void tra_callback() override;    // Tx send bit

	bool m_eps_mode;

private:
	static const int XMIT_RING_SIZE = 16;

	bool  m_bCalibSecondByte;
	bool  m_bButtonLightSecondByte;

	devcb_write_line m_write_tx;
	devcb_write16 m_write_analog;
	UINT8 m_xmitring[XMIT_RING_SIZE];
	int m_xmit_read, m_xmit_write;
	bool m_tx_busy;
};

class esqpanel1x22_device : public esqpanel_device {
public:
	esqpanel1x22_device(const machine_config &mconfig, std::string tag, device_t *owner, UINT32 clock);

	required_device<esq1x22_t> m_vfd;

	virtual void send_to_display(UINT8 data) override { m_vfd->write_char(data); }

protected:
	virtual machine_config_constructor device_mconfig_additions() const override;

private:
};

class esqpanel2x40_device : public esqpanel_device {
public:
	esqpanel2x40_device(const machine_config &mconfig, std::string tag, device_t *owner, UINT32 clock);

	required_device<esq2x40_t> m_vfd;

	virtual void send_to_display(UINT8 data) override { m_vfd->write_char(data); }

protected:
	virtual machine_config_constructor device_mconfig_additions() const override;

private:
};

class esqpanel2x40_sq1_device : public esqpanel_device {
public:
	esqpanel2x40_sq1_device(const machine_config &mconfig, std::string tag, device_t *owner, UINT32 clock);

	required_device<esq2x40_sq1_t> m_vfd;

	virtual void send_to_display(UINT8 data) override { m_vfd->write_char(data); }

protected:
	virtual machine_config_constructor device_mconfig_additions() const override;

private:
};

extern const device_type ESQPANEL1x22;
extern const device_type ESQPANEL2x40;
extern const device_type ESQPANEL2x40_SQ1;

#endif
