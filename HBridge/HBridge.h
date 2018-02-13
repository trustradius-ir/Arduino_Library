/*
Write by Alireza Noubrakhsh
*/
#include "Arduino.h"

typedef enum
{
  hbOneWay        = (1),   
  hbTwoWay        = (2)
} bridge_type_def;

typedef enum
{
  msStop        = (0),   
  msForward     = (1),   
  msBackward    = (2)
} motor_status_def;

class HBridge {
 public:
  HBridge(uint8_t forward_pin, uint8_t backward_pin,bridge_type_def bridge_type = hbTwoWay,bool break_enabled = true);
  void Initialize(uint8_t PWMsignalFrom  = 80);
  void Forward(uint8_t speedpercent = 100);
  void Backward(uint8_t speedpercent = 100);
  void Stop(uint8_t stopsteppercent = 10);
  void Break(uint8_t breakpercent = 100);
 private:
  uint8_t _forward_pin,_backward_pin;
  bool _break_enabled;
  uint8_t _pwm_signal_from;
  motor_status_def _motor_status;
  uint8_t _motor_last_pwm;
  bridge_type_def _bridge_type;   
};

