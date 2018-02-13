#include <HBridge.h>

HBridge::HBridge(uint8_t forward_pin, uint8_t backward_pin,bridge_type_def bridge_type = hbTwoWay,bool break_enabled = true)
{
	_motor_last_pwm = 0;
	_motor_status = msStop;
	_pwm_signal_from = 80;
	_forward_pin = forward_pin;
	_backward_pin = backward_pin;
	_bridge_type = bridge_type;
	_break_enabled = break_enabled;  
	if (_bridge_type == hbOneWay) 
	{
	  break_enabled = false;	
	  pinMode(_forward_pin, OUTPUT);
	  analogWrite(_forward_pin, 0);
	}
	
	if (_bridge_type == hbTwoWay) 
	{
	  pinMode(_backward_pin, OUTPUT);
	  analogWrite(_backward_pin, 0);
	}  
}


void HBridge::Initialize(uint8_t PWMsignalFrom = 80)
{	
	_pwm_signal_from = PWMsignalFrom;
	if (_pwm_signal_from > 250) {_pwm_signal_from = 250;}
}


void HBridge::Forward(uint8_t speedpercent = 100)
{
	if (speedpercent > 100) {speedpercent = 100;}
	float pwmsignal = _pwm_signal_from + float((float(255 - _pwm_signal_from) / 100) * speedpercent);
	_motor_last_pwm = (uint8_t) pwmsignal;
	analogWrite(_backward_pin, 0);
	analogWrite(_forward_pin,(uint8_t) pwmsignal);
	_motor_status = msForward;
}

void HBridge::Backward(uint8_t speedpercent = 100)
{
	if (_bridge_type == hbOneWay)  return;
	if (speedpercent > 100) {speedpercent = 100;}
	float pwmsignal = _pwm_signal_from + float((float(255 - _pwm_signal_from) / 100) * speedpercent);
	_motor_last_pwm = (uint8_t) pwmsignal;
	analogWrite(_forward_pin, 0);
	analogWrite(_backward_pin,(uint8_t) pwmsignal);
	_motor_status = msBackward;
}

void HBridge::Stop(uint8_t stopsteppercent = 10)
{
	if (_motor_status == msStop) return;
	if (_motor_last_pwm <= _pwm_signal_from) 
	{
		_motor_last_pwm = 0;
		_motor_status == msStop;
		analogWrite(_forward_pin, 0);
		if (_bridge_type == hbTwoWay) {analogWrite(_backward_pin, 0);}
		return;
	}
	float pwmsignalstep = float((float(_motor_last_pwm - _pwm_signal_from) / 100) * stopsteppercent);
	for (uint8_t pwmsignal=_motor_last_pwm; pwmsignal > _pwm_signal_from; pwmsignal = pwmsignal - (uint8_t) pwmsignalstep) 
	{
		switch (_motor_status) {
		   case msForward: analogWrite(_forward_pin,pwmsignal); break;
		   case msBackward: analogWrite(_backward_pin,pwmsignal); break;
		}	
		delay(150);		
	}
	delay(100);		
	_motor_last_pwm = 0;
	_motor_status == msStop;
	analogWrite(_forward_pin, 0);
	if (_bridge_type == hbTwoWay) { analogWrite(_backward_pin, 0); }
}

void HBridge::Break(uint8_t breakpercent = 100)
{
	if (_bridge_type == hbOneWay) 
	{
		_motor_last_pwm = 0;
		_motor_status = msStop;
		analogWrite(_forward_pin, 0);
		delay(100);	
		return;
	}
	analogWrite(_backward_pin, 0);
	delay(100);	
	if (!_break_enabled) return;	
	
	float pwmsignal = _pwm_signal_from + float((float(255 - _pwm_signal_from) / 100) * breakpercent);
	switch (_motor_status) {
	   case msForward: analogWrite(_backward_pin,(uint8_t) pwmsignal); break;
	   case msBackward: analogWrite(_forward_pin,(uint8_t) pwmsignal); break;
	}	

	delay(100);		
	_motor_last_pwm = 0;
	_motor_status = msStop;	
	analogWrite(_forward_pin, 0);
	analogWrite(_backward_pin, 0);
}
