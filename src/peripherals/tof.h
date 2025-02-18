#ifndef TOF_H
#define TOF_H


#include <stdint.h> // for uint8_t
#include <Adafruit_VL53L0X.h>
#include <movingAvg.h>
#include "../../lib/Common/sensors_state.h"
#include "pindef.h"
#include "log.h"



class TOF {
  public:
    TOF();
    void init(SensorState& sensor,uint8_t i2c_addr);
    //uint16_t readLvl();
    bool readLvl(uint16_t *level);
    uint16_t readRangeToPct(uint16_t val);
    Adafruit_VL53L0X *tof_sensor;
    movingAvg *mvAvg;//
    uint8_t _i2c_addr;
  private:

    bool isInit;
    // HardwareTimer* hw_timer;
    // static void TimerHandler10(void);
    //uint32_t tofReading;
};

TOF::TOF() {

}

void TOF::init(SensorState& sensor,uint8_t i2c_addr) {
  tof_sensor=new Adafruit_VL53L0X;
  mvAvg=new movingAvg(4);
  _i2c_addr=i2c_addr;
  if(i2c_addr!=0x29){
    //first need to set the address (its non permanent)
    //shutting down other TOF_VL53L0X on the bus
    digitalWrite(tofShutdownPin, HIGH);
    delay(10);
    digitalWrite(tofShutdownPin, LOW);
  }

  for(int n = 1; n < 10; n++ ){
    isInit = tof_sensor->begin(i2c_addr, false, &Wire, Adafruit_VL53L0X::VL53L0X_SENSE_HIGH_ACCURACY);
    if(isInit){
      tof_sensor->startRangeContinuous();
      mvAvg->begin();
      if(i2c_addr!=0x29){
        digitalWrite(tofShutdownPin, HIGH);
      }
      return;
    }
    //delay(5);
  }
  // while(true){
  //   delay(1000);
  //   LOG_ERROR("TOF FAILED TO INIT: %d",i2c_addr);
  // }
}

bool TOF::readLvl(uint16_t *level) {
  
  if(!isInit){
    LOG_ERROR("TOF NOT INIT: %d",_i2c_addr);
    return false;
  }
  if(tof_sensor->isRangeComplete()) {
    uint32_t tofReading=0;
    tofReading=mvAvg->reading(tof_sensor->readRangeResult());
    //LOG_INFO("WL %d",tofReading);    
    *level=readRangeToPct(tofReading);
    return true;
  }
  return false;

  //return  tofReading != 0 ? readRangeToPct(tofReading) : 0;
}

//Gaggia:
//static const std::array<uint16_t, 9> ranges = { 15u, 30u, 45u, 60u, 75u, 90u, 105u, 115u, 125u };
//Rancilio Silvia Water tank
static const std::array<uint16_t, 10> wt_range = { 60u, 78u, 94u, 102u, 120u, 138u, 156u, 174u, 192u, 210u };

//Rancilio Silvia Drip tray (factory)
//static const std::array<uint16_t, 10> dt_range = { 95u, 98u, 101u, 104u, 107u, 110u, 113u, 116u, 119u, 122u };
static const std::array<uint16_t, 10> dt_range = { 100u, 102u, 104u, 106u, 108u, 110u, 112u, 114u, 116u, 118u };

static const std::array<uint16_t, 10> pct_lvl = { 100u, 90u, 80u, 70u, 60u, 50u, 40u, 30u, 20u, 10u };

uint16_t TOF::readRangeToPct(uint16_t val) {
  
  const uint16_t* range;
  if(_i2c_addr!=0x29){
    range =  wt_range.data();
  }else{
    range =  dt_range.data();
  }
  for (size_t i = 0; i < 10; i++) {
    if (val <= range[i]) {
      //LOG_INFO("PCT %d",pct_lvl[i]);
      return pct_lvl[i];
    }
  }

  return 9u;
}

#endif
