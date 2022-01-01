#include<Servo.h>

struct NODE_JOINT{
  int node_id = -1;
  int base_angle = 90;
  int cur_angle;
  int unit;
  int delayValue;
  
  Servo actuator;

  void action(NODE_JOINT *my_node_ptr, int dst_angle){
      int degree = 0, total_cnt = 0;
      int cur_angle = my_node_ptr->cur_angle;
      
      if (cur_angle < dst_angle){
        while (cur_angle < dst_angle) 
        {
          cur_angle += my_node_ptr->unit;
          my_node_ptr->actuator.write(cur_angle);
          delay(my_node_ptr->delayValue);
        }
      } else if (cur_angle > dst_angle) {
        while (cur_angle > dst_angle) 
        {
          cur_angle -= my_node_ptr->unit;
          my_node_ptr->actuator.write(cur_angle);
          delay(my_node_ptr->delayValue);
        }
      } else {
        
          Serial.println("cur_angle & dst_angle are same");
      }
      my_node_ptr->actuator.write(dst_angle);
     }
  };

#define idle 0
#define busy 1
#define wait 2

#define command_pin_0  10
#define command_pin_1  9
#define command_pin_2  8
#define command_pin_val  11
                                // command_2 | command_1 | command_0
#define idle 0                  //    0      |    0      |    0     : IDLE
#define grabfrombelt 1          //    0      |    0      |    1     : grabFromBelt()
#define movetoweightsensor 2    //    0      |    1      |    0     : moveToWeightSensor()
#define grabfromweightsensor 3  //    0      |    1      |    1     : grabFromWeightSensor()
#define movetoA 4               //    1      |    0      |    0     : moveToA()
#define movetoB 5               //    1      |    0      |    1     : moveToB()
#define movetoC 6               //    1      |    1      |    0     : moveToC()
#define init 7                  //    1      |    1      |    1     : N/A
#define initReady 100

#define MCU_READ_VALID 7

struct FUNCTION{
  int function_id;
  int cur_status = idle;
  int dst_angle[4]= {90, 90, 90, 90};
  NODE_JOINT *node_1_ptr=NULL, *node_2_ptr=NULL, *node_3_ptr=NULL, *node_4_ptr=NULL;

  int do_seq_action(FUNCTION *function_ptr, NODE_JOINT *node_1_ptr=NULL,NODE_JOINT *node_2_ptr=NULL,NODE_JOINT *node_3_ptr=NULL,NODE_JOINT *node_4_ptr=NULL){
    int node_1_dst_angle = function_ptr->dst_angle[0];
    int node_2_dst_angle = function_ptr->dst_angle[1];
    int node_3_dst_angle = function_ptr->dst_angle[2];
    int node_4_dst_angle = function_ptr->dst_angle[3];

    int function_id = function_ptr->function_id;
    int node_3_dst_temp_angle = 0, node_2_dst_temp_angle=0;
    
    switch(function_id){
        case grabfrombelt:
        node_1_ptr->action(node_1_ptr, node_1_dst_angle);
        node_1_ptr->cur_angle = node_1_dst_angle;
        
        node_3_ptr->action(node_3_ptr, node_3_dst_angle);
        node_3_ptr->cur_angle = node_3_dst_angle;
        
        node_2_ptr->action(node_2_ptr, node_2_dst_angle);
        node_2_ptr->cur_angle = node_2_dst_angle;
        
        node_4_ptr->action(node_4_ptr, node_4_dst_angle);
        node_4_ptr->cur_angle = node_4_dst_angle;
        
        break;

        case movetoweightsensor:
        node_3_dst_temp_angle = node_3_dst_angle + 30;
        node_3_ptr->action(node_3_ptr, node_3_dst_temp_angle);
        node_3_ptr->cur_angle = node_3_dst_temp_angle;
        
        node_1_ptr->action(node_1_ptr, node_1_dst_angle);
        node_1_ptr->cur_angle = node_1_dst_angle;
        
        
        
        node_2_ptr->action(node_2_ptr, node_2_dst_angle);
        node_2_ptr->cur_angle = node_2_dst_angle;

        node_3_ptr->action(node_3_ptr, node_3_dst_angle);
        node_3_ptr->cur_angle = node_3_dst_angle;
        
        node_4_ptr->action(node_4_ptr, node_4_dst_angle);
        node_4_ptr->cur_angle = node_4_dst_angle;
        break;

        case movetoA:
        node_4_ptr->action(node_4_ptr, 30);
        node_4_ptr->cur_angle = 30;

        node_2_dst_temp_angle = node_2_ptr->cur_angle - 18;
        node_2_ptr->action(node_2_ptr, node_2_dst_temp_angle);
        node_2_ptr->cur_angle = node_2_dst_temp_angle;
        
        node_1_ptr->action(node_1_ptr, node_1_dst_angle);
        node_1_ptr->cur_angle = node_1_dst_angle;
        
        node_2_ptr->action(node_2_ptr, node_2_dst_angle);
        node_2_ptr->cur_angle = node_2_dst_angle;

        node_3_ptr->action(node_3_ptr, node_3_dst_angle);
        node_3_ptr->cur_angle = node_3_dst_angle;
        
        node_4_ptr->action(node_4_ptr, node_4_dst_angle);
        node_4_ptr->cur_angle = node_4_dst_angle;

        delay(5000);
        node_4_ptr->action(node_4_ptr, 90);
        node_4_ptr->cur_angle = 90;
        node_3_ptr->action(node_3_ptr, 108);
        node_3_ptr->cur_angle = 108;
        node_2_ptr->action(node_2_ptr, 72);
        node_2_ptr->cur_angle = 72;
        node_1_ptr->action(node_1_ptr, 90);
        node_1_ptr->cur_angle = 90;
        
        break;

        case movetoB:
        node_4_ptr->action(node_4_ptr, 30);
        node_4_ptr->cur_angle = 30;

        node_2_dst_temp_angle = node_2_ptr->cur_angle - 18;
        node_2_ptr->action(node_2_ptr, node_2_dst_temp_angle);
        node_2_ptr->cur_angle = node_2_dst_temp_angle;
        
        node_1_ptr->action(node_1_ptr, node_1_dst_angle);
        node_1_ptr->cur_angle = node_1_dst_angle;
        
        node_2_ptr->action(node_2_ptr, node_2_dst_angle);
        node_2_ptr->cur_angle = node_2_dst_angle;

        node_3_ptr->action(node_3_ptr, node_3_dst_angle);
        node_3_ptr->cur_angle = node_3_dst_angle;
        
        node_4_ptr->action(node_4_ptr, node_4_dst_angle);
        node_4_ptr->cur_angle = node_4_dst_angle;

        delay(5000);
        node_4_ptr->action(node_4_ptr, 90);
        node_4_ptr->cur_angle = 90;
        node_3_ptr->action(node_3_ptr, 108);
        node_3_ptr->cur_angle = 108;
        node_2_ptr->action(node_2_ptr, 72);
        node_2_ptr->cur_angle = 72;
        node_1_ptr->action(node_1_ptr, 90);
        node_1_ptr->cur_angle = 90;
        break;
        
        default:
          node_1_ptr->action(node_1_ptr, node_1_dst_angle);
          node_1_ptr->cur_angle = node_1_dst_angle;
          
          node_2_ptr->action(node_2_ptr, node_2_dst_angle);
          node_2_ptr->cur_angle = node_2_dst_angle;
          
          node_3_ptr->action(node_3_ptr, node_3_dst_angle);
          node_3_ptr->cur_angle = node_3_dst_angle;
          
          node_4_ptr->action(node_4_ptr, node_4_dst_angle);
          node_4_ptr->cur_angle = node_4_dst_angle;
          break;
    }
    return 1;
  }
  int wait_mcu_check();
  
};




int command_bit_0 = -1;
int command_bit_1 = -1;
int command_bit_2 = -1;
int command_bit_3 = -1;


NODE_JOINT node_1, node_2, node_3, node_4;
NODE_JOINT *cur_node_ptr = NULL, *node_1_ptr, *node_2_ptr, *node_3_ptr, *node_4_ptr;
FUNCTION grabFromBelt, mov2Sensor, grabFromSensor, mov2A, mov2B, mov2C;
FUNCTION *grabFromBelt_ptr, *mov2Sensor_ptr, *grabFromSensor_ptr, *mov2A_ptr, *mov2B_ptr, *mov2C_ptr;

void setup() {
  // put your setup code here, to run once:
  
  node_1_ptr = &node_1;
  node_1.node_id = 1;
  node_1.cur_angle = node_1.base_angle;
  node_1.unit = 6;
  node_1.delayValue = 500;
  
  node_2_ptr = &node_2;
  node_2.node_id = 2;
  node_2.cur_angle = node_2.base_angle;
  node_2.unit = 3;
  node_2.delayValue = 700;
  
  node_3_ptr = &node_3;
  node_3.node_id = 3;
  node_3.cur_angle = node_3.base_angle;
  node_3.unit = 6;
  node_3.delayValue = 500;
  
  node_4_ptr = &node_4;
  node_4.node_id = 4;
  node_4.cur_angle = node_4.base_angle;
  node_4.unit = 6;
  node_4.delayValue = 500;


  grabFromBelt_ptr      = &grabFromBelt;
  grabFromBelt_ptr->function_id = grabfrombelt;
  grabFromBelt_ptr->dst_angle[0] = 95;
  grabFromBelt_ptr->dst_angle[1] = 114;
  grabFromBelt_ptr->dst_angle[2] = 126;
  grabFromBelt_ptr->dst_angle[3] = 25;
   
  mov2Sensor_ptr        = &mov2Sensor;
  mov2Sensor_ptr->function_id = movetoweightsensor;
  mov2Sensor_ptr->dst_angle[0] = 70;
  mov2Sensor_ptr->dst_angle[1] = 102;
  mov2Sensor_ptr->dst_angle[2] = 102;
  mov2Sensor_ptr->dst_angle[3] = 48;
  
  grabFromSensor_ptr    = &grabFromSensor;
  grabFromSensor_ptr->function_id = grabfromweightsensor;
  grabFromSensor_ptr->dst_angle[0] = 80;
  grabFromSensor_ptr->dst_angle[1] = 120;
  grabFromSensor_ptr->dst_angle[2] = 130;
  grabFromSensor_ptr->dst_angle[3] = 18;
  
  mov2A_ptr             = &mov2A;
  mov2A_ptr->function_id = movetoA;
  mov2A_ptr->dst_angle[0] = 48;
  mov2A_ptr->dst_angle[1] = 108;
  mov2A_ptr->dst_angle[2] = 96;
  mov2A_ptr->dst_angle[3] = 60;
  
  mov2B_ptr             = &mov2B;
  mov2B_ptr->function_id = movetoB;
  mov2B_ptr->dst_angle[0] = 30;
  mov2B_ptr->dst_angle[1] = 108;
  mov2B_ptr->dst_angle[2] = 96;
  mov2B_ptr->dst_angle[3] = 60;
  
  mov2C_ptr             = &mov2C; 
  
  // Arduino ->> Servo motor
  node_1.actuator.attach(2);
  node_2.actuator.attach(3);
  node_3.actuator.attach(4);
  node_4.actuator.attach(5);

  // Arduino <-> ADC <-> MCU
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);

  // MCU <-> Arduino
  pinMode(command_pin_0, INPUT);
  pinMode(command_pin_1, INPUT);
  pinMode(command_pin_2, INPUT);
  pinMode(command_pin_val, INPUT);

  Serial.begin(9600);
}

int cur_state = idle;
int command_line = initReady;
int finish_flag_seq_action = 0;
int idle_print_flag = 0;
int command_line_real = 0;
int init_done_flag = 0;

void sendSignal()
{
  digitalWrite(6, HIGH);
  delay(4000);
  digitalWrite(6, LOW);
}

void sendFinishSignal(){
  digitalWrite(6, HIGH);
  while(1){
    while(1){
      if(digitalRead(command_pin_val))
          break;
    }
    command_bit_0 = digitalRead(command_pin_0); //1_28
    command_bit_1 = digitalRead(command_pin_1); //1_29
    command_bit_2 = digitalRead(command_pin_2); //1_30
    
    command_line = (4*command_bit_0) + (2*command_bit_1) + (1*command_bit_2);
    
    if(command_line == MCU_READ_VALID){
      digitalWrite(6, LOW);
      break;
    }
  }
}

void check_val_signal(){
  if (digitalRead(command_pin_val)){
      while(1)
        Serial.println("Check valid signal");
  }  
}

int do_init_pose(){
    int node_2_dst_angle = 74;
    int node_3_dst_angle = 108;
    
    node_2_ptr->action(node_2_ptr, node_2_dst_angle);
    node_2_ptr->cur_angle = node_2_dst_angle;
    
    node_3_ptr->action(node_3_ptr, node_3_dst_angle);
    node_3_ptr->cur_angle = node_3_dst_angle;
    
    return 1;
    
}

void loop() {
  // put your main code here, to run repeatedly:
  if(!init_done_flag){
    init_done_flag = do_init_pose();
    Serial.println("Success init");
  }

  
  digitalWrite(6, LOW);
  digitalWrite(7, LOW);
  
  while(1){
    if(digitalRead(command_pin_val))
        break;
  }
  command_bit_0 = digitalRead(command_pin_0); //1_28
  command_bit_1 = digitalRead(command_pin_1); //1_29
  command_bit_2 = digitalRead(command_pin_2); //1_30
  
  command_line = (4*command_bit_0) + (2*command_bit_1) + (1*command_bit_2);
  
  if (command_line != 0) {
    Serial.println(command_line);
    Serial.println(command_bit_0);
    Serial.println(command_bit_1);
    Serial.println(command_bit_2);
    Serial.println(digitalRead(command_pin_val));
  }
    switch(command_line){
                 
    case idle:
        break;
        
    case grabfrombelt:
        if(grabFromBelt_ptr->cur_status == idle){
          grabFromBelt_ptr->cur_status = busy;
          sendSignal();
          check_val_signal();
          Serial.println("Start sequence action grabFromBelt");
          digitalWrite(7, HIGH);
          finish_flag_seq_action = grabFromBelt_ptr->do_seq_action(grabFromBelt_ptr, node_1_ptr, node_2_ptr, node_3_ptr, node_4_ptr);
          
          if(!finish_flag_seq_action){
              while(1)
                Serial.println("Error in seq_action");
          }
          digitalWrite(7, LOW);
          sendSignal();
          grabFromBelt_ptr->cur_status = idle;
          Serial.println("Finish the seq_action");
          break;
        }

        else{
          while(1)
            Serial.println("grabFromBelt is not idle");
          break;
        }
    case movetoweightsensor:
        if(mov2Sensor_ptr->cur_status == idle){
          mov2Sensor_ptr->cur_status = busy;
          sendSignal();
          check_val_signal();
          Serial.println("Start sequence action movetoweightsensor");
          digitalWrite(7, HIGH);
          finish_flag_seq_action = mov2Sensor_ptr->do_seq_action(mov2Sensor_ptr, node_1_ptr, node_2_ptr, node_3_ptr, node_4_ptr);
          if(!finish_flag_seq_action){
              while(1)
                Serial.println("Error in seq_action");
          }
          digitalWrite(7, LOW);
          sendSignal();
          mov2Sensor_ptr->cur_status = idle;
          Serial.println("Finish the seq_action");
          break;
        }
        
        else{
          while(1)
            Serial.println("movetoweightsensor is not idle");
          break;
        }
        
    case grabfromweightsensor:
        if(grabFromSensor_ptr->cur_status == idle){
          grabFromSensor_ptr->cur_status = busy;
          sendSignal();
          check_val_signal();
          Serial.println("Start sequence action grabfromweightsensor");
          digitalWrite(7, HIGH);
          finish_flag_seq_action = grabFromSensor_ptr->do_seq_action(grabFromSensor_ptr, node_1_ptr, node_2_ptr, node_3_ptr, node_4_ptr);
          if(!finish_flag_seq_action){
              while(1)
                Serial.println("Error in seq_action");
          }
          digitalWrite(7, LOW);
          sendSignal();
          grabFromSensor_ptr->cur_status = idle;
          Serial.println("Finish the seq_action");
          break;
        }
        
        else{
          while(1)
            Serial.println("grabfromweightsensor is not idle");
          break;
        }
             
    case movetoA:
        if(mov2A_ptr->cur_status == idle){
          mov2A_ptr->cur_status = busy;
          sendSignal();
          check_val_signal();
          Serial.println("Start sequence action movetoA");
          digitalWrite(7, HIGH);
          finish_flag_seq_action = mov2A_ptr->do_seq_action(mov2A_ptr, node_1_ptr, node_2_ptr, node_3_ptr, node_4_ptr);
          if(!finish_flag_seq_action){
              while(1)
                Serial.println("Error in seq_action");
          }
          digitalWrite(7, LOW);
          sendSignal();
          mov2A_ptr->cur_status = idle;
          Serial.println("Finish the seq_action");
          break;
        }

        else{
          while(1)
            Serial.println("movetoA is not idle");
          break;
        }
          
    case movetoB:
        if(mov2B_ptr->cur_status == idle){
          mov2B_ptr->cur_status = busy;
          sendSignal();
          check_val_signal();
          Serial.println("Start sequence action movetoB");
          digitalWrite(7, HIGH);
          finish_flag_seq_action = mov2B_ptr->do_seq_action(mov2B_ptr, node_1_ptr, node_2_ptr, node_3_ptr, node_4_ptr);
          if(!finish_flag_seq_action){
              while(1)
                Serial.println("Error in seq_action");
          }
          digitalWrite(7, LOW);
          sendSignal();
          mov2B_ptr->cur_status = idle;
          Serial.println("Finish the seq_action");
          break;
        }
        
        else{
          while(1)
            Serial.println("movetoB is not idle");
          break;
        }
          
    case movetoC:
        if(mov2C_ptr->cur_status == idle){
          mov2C_ptr->cur_status = busy;
          sendSignal();
          check_val_signal();
          Serial.println("Start sequence action movetoC");
          digitalWrite(7, HIGH);
          finish_flag_seq_action = mov2C_ptr->do_seq_action(mov2C_ptr, node_1_ptr, node_2_ptr, node_3_ptr, node_4_ptr);
          if(!finish_flag_seq_action){
              while(1)
                Serial.println("Error in seq_action");
          }
          digitalWrite(7, LOW);
          sendSignal();
          mov2C_ptr->cur_status = idle;
          Serial.println("Finish the seq_action");
          break;
        }
        
       else{
          while(1)
            Serial.println("movetoC is not idle");
          break;
       }
          
    default:
        break;
    }
}
