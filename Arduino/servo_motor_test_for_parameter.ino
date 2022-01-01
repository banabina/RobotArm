#include<Servo.h>

struct NODE_JOINT{
  int node_id;
  int base_angle = 90;
  int cur_angle;
  
  Servo actuator;
  
  //void angleSteering(int cur_angle, int target_angle, boolean isIncrease);
  //int clearAll();
  };
  
#define UNIT_LARGE 36
#define UNIT_MEDIUM 18
#define UNIT_SMALL 6

// <Target Node> | L+  | M+  | S+  | L-  | M-  | S-  |
//       1          a     b     c     d     e     f
//       2          g     h     i     j     k     l
//       3          A     B     C     D     E     F
//       4          G     H     I     J     K     L


#define command_pin_0  10
#define command_pin_1  9
#define command_pin_2  8
                                // command_2 | command_1 | command_0
#define idle 0                  //    0      |    0      |    0     : IDLE
#define grabfrombelt 1          //    0      |    0      |    1     : grabFromBelt()
#define movetoweightsensor 2    //    0      |    1      |    0     : moveToWeightSensor()
#define grabfromweightsensor 3  //    0      |    1      |    1     : grabFromWeightSensor()
#define movetoA 4               //    1      |    0      |    0     : moveToA()
#define movetoB 5               //    1      |    0      |    1     : moveToB()
#define movetoC 6               //    1      |    1      |    0     : moveToC()
                                //    1      |    1      |    1     : N/A
#define MCU_READ_VALID 7

int command_bit_0 = 0;
int command_bit_1 = 0;
int command_bit_2 = 0;
int cur_state = -1;
int k = 3;

NODE_JOINT node_1, node_2, node_3, node_4;
NODE_JOINT *cur_node_ptr = NULL, *node_1_ptr, *node_2_ptr, *node_3_ptr, *node_4_ptr;

void setup() {
  // put your setup code here, to run once:

  node_1_ptr = &node_1;
  node_1.node_id = 1;
  node_1.cur_angle = node_1.base_angle;
  
  node_2_ptr = &node_2;
  node_2.node_id = 2;
  node_2.cur_angle = node_2.base_angle;
  
  node_3_ptr = &node_3;
  node_3.node_id = 3;
  node_3.cur_angle = node_3.base_angle;
  
  node_4_ptr = &node_4;
  node_4.node_id = 4;
  node_4.cur_angle = node_4.base_angle;
  
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

  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:

if(Serial.available()>0)      
  {
    int in_data;             
    in_data = int(Serial.read()); 
    //Serial.println(in_data);
    if (in_data != 10)
    {
       switch(in_data){
          case '1':
              cur_node_ptr = node_1_ptr;
              Serial.println("Current target control node's ID");
              Serial.println(cur_node_ptr->node_id);
              Serial.println("-----------------\n");
              break;
              
          case 'a':
              if (cur_node_ptr != node_1_ptr){
                Serial.println("Please check current node ID and control input");
                break;
              }
              for (int i =0; i<UNIT_LARGE; i+=k){
                cur_node_ptr->actuator.write(cur_node_ptr->cur_angle + i);
                delay(500);
                }
              cur_node_ptr->cur_angle += UNIT_LARGE;
              Serial.println("Current node's angle");
              Serial.println(cur_node_ptr->cur_angle);
              Serial.println("-----------------\n");      
              break;
                         
          case 'b':
              if (cur_node_ptr != node_1_ptr){
                Serial.println("Please check current node ID and control input");
                break;
              }        
              for (int i =0; i<UNIT_MEDIUM; i+=k){
                cur_node_ptr->actuator.write(cur_node_ptr->cur_angle + i);
                delay(500);
                }
              cur_node_ptr->cur_angle += UNIT_MEDIUM;
              Serial.println("Current node's angle");
              Serial.println(cur_node_ptr->cur_angle);
              Serial.println("-----------------\n");
              break;
                    
          case 'c':
              if (cur_node_ptr != node_1_ptr){
                Serial.println("Please check current node ID and control input");
                break;
              }        
              for (int i =0; i<UNIT_SMALL; i+=k){
                cur_node_ptr->actuator.write(cur_node_ptr->cur_angle + i);
                delay(500);
                }
              cur_node_ptr->cur_angle += UNIT_SMALL;
              Serial.println("Current node's angle");
              Serial.println(cur_node_ptr->cur_angle);
              Serial.println("-----------------\n");
              break;
                    
          case 'd':
              if (cur_node_ptr != node_1_ptr){
                Serial.println("Please check current node ID and control input");
                break;
              }        
              for (int i =0; i<UNIT_LARGE; i+=k){
                cur_node_ptr->actuator.write(cur_node_ptr->cur_angle - i);
                delay(500);
                }
              cur_node_ptr->cur_angle -= UNIT_LARGE;
              Serial.println("Current node's angle");
              Serial.println(cur_node_ptr->cur_angle);
              Serial.println("-----------------\n");
              break;
              
          case 'e':
              if (cur_node_ptr != node_1_ptr){
                Serial.println("Please check current node ID and control input");
                break;
              }        
              for (int i =0; i<UNIT_MEDIUM; i+=k){
                cur_node_ptr->actuator.write(cur_node_ptr->cur_angle - i);
                delay(500);
                }
              cur_node_ptr->cur_angle -= UNIT_MEDIUM;
              Serial.println("Current node's angle");
              Serial.println(cur_node_ptr->cur_angle);
              Serial.println("-----------------\n");
              break;
              
          case 'f':
              if (cur_node_ptr != node_1_ptr){
                Serial.println("Please check current node ID and control input");
                break;
              }        
              for (int i =0; i<UNIT_SMALL; i+=k){
                cur_node_ptr->actuator.write(cur_node_ptr->cur_angle - i);
                delay(500);
                }
              cur_node_ptr->cur_angle -= UNIT_SMALL;
              Serial.println("Current node's angle");
              Serial.println(cur_node_ptr->cur_angle);
              Serial.println("-----------------\n");
              break;
  
  
          case '2':
              cur_node_ptr = node_2_ptr;
              Serial.println("Current target control node's ID");
              Serial.println(cur_node_ptr->node_id);
              Serial.println("-----------------\n");
              break;
              
          case 'g':
              if (cur_node_ptr != node_2_ptr){
                Serial.println("Please check current node ID and control input");
                break;
              }        
              for (int i =0; i<UNIT_LARGE; i+=k){
                cur_node_ptr->actuator.write(cur_node_ptr->cur_angle + i);
                delay(500);
                }
              cur_node_ptr->cur_angle += UNIT_LARGE;
              Serial.println("Current node's angle");
              Serial.println(cur_node_ptr->cur_angle);
              Serial.println("-----------------\n");      
              break;
                         
          case 'h':
              if (cur_node_ptr != node_2_ptr){
                Serial.println("Please check current node ID and control input");
                break;
              }        
              for (int i =0; i<UNIT_MEDIUM; i+=k){
                cur_node_ptr->actuator.write(cur_node_ptr->cur_angle + i);
                delay(500);
                }
              cur_node_ptr->cur_angle += UNIT_MEDIUM;
              Serial.println("Current node's angle");
              Serial.println(cur_node_ptr->cur_angle);
              Serial.println("-----------------\n");
              break;
                    
          case 'i':
              if (cur_node_ptr != node_2_ptr){
                Serial.println("Please check current node ID and control input");
                break;
              }        
              for (int i =0; i<UNIT_SMALL; i+=k){
                cur_node_ptr->actuator.write(cur_node_ptr->cur_angle + i);
                delay(500);
                }
              cur_node_ptr->cur_angle += UNIT_SMALL;
              Serial.println("Current node's angle");
              Serial.println(cur_node_ptr->cur_angle);
              Serial.println("-----------------\n");
              break;
                    
          case 'j':
              if (cur_node_ptr != node_2_ptr){
                Serial.println("Please check current node ID and control input");
                break;
              }        
              for (int i =0; i<UNIT_LARGE; i+=k){
                cur_node_ptr->actuator.write(cur_node_ptr->cur_angle - i);
                delay(500);
                }
              cur_node_ptr->cur_angle -= UNIT_LARGE;
              Serial.println("Current node's angle");
              Serial.println(cur_node_ptr->cur_angle);
              Serial.println("-----------------\n");
              break;
              
          case 'k':
              if (cur_node_ptr != node_2_ptr){
                Serial.println("Please check current node ID and control input");
                break;
              }        
              for (int i =0; i<UNIT_MEDIUM; i+=k){
                cur_node_ptr->actuator.write(cur_node_ptr->cur_angle - i);
                delay(500);
                }
              cur_node_ptr->cur_angle -= UNIT_MEDIUM;
              Serial.println("Current node's angle");
              Serial.println(cur_node_ptr->cur_angle);
              Serial.println("-----------------\n");
              break;
              
          case 'l':
              if (cur_node_ptr != node_2_ptr){
                Serial.println("Please check current node ID and control input");
                break;
              }        
              for (int i =0; i<UNIT_SMALL; i+=k){
                cur_node_ptr->actuator.write(cur_node_ptr->cur_angle - i);
                delay(500);
                }
              cur_node_ptr->cur_angle -= UNIT_SMALL;
              Serial.println("Current node's angle");
              Serial.println(cur_node_ptr->cur_angle);
              Serial.println("-----------------\n");
              break;
  
          case '3':
              cur_node_ptr = node_3_ptr;
              Serial.println("Current target control node's ID");
              Serial.println(cur_node_ptr->node_id);
              Serial.println("-----------------\n");
              break;
              
          case 'A':
              if (cur_node_ptr != node_3_ptr){
                Serial.println("Please check current node ID and control input");
                break;
              }        
              for (int i =0; i<UNIT_LARGE; i+=k){
                cur_node_ptr->actuator.write(cur_node_ptr->cur_angle + i);
                delay(500);
                }
              cur_node_ptr->cur_angle += UNIT_LARGE;
              Serial.println("Current node's angle");
              Serial.println(cur_node_ptr->cur_angle);
              Serial.println("-----------------\n");      
              break;
                         
          case 'B':
              if (cur_node_ptr != node_3_ptr){
                Serial.println("Please check current node ID and control input");
                break;
              }        
              for (int i =0; i<UNIT_MEDIUM; i+=k){
                cur_node_ptr->actuator.write(cur_node_ptr->cur_angle + i);
                delay(500);
                }
              cur_node_ptr->cur_angle += UNIT_MEDIUM;
              Serial.println("Current node's angle");
              Serial.println(cur_node_ptr->cur_angle);
              Serial.println("-----------------\n");
              break;
                    
          case 'C':
              if (cur_node_ptr != node_3_ptr){
                Serial.println("Please check current node ID and control input");
                break;
              }        
              for (int i =0; i<UNIT_SMALL; i+=k){
                cur_node_ptr->actuator.write(cur_node_ptr->cur_angle + i);
                delay(500);
                }
              cur_node_ptr->cur_angle += UNIT_SMALL;
              Serial.println("Current node's angle");
              Serial.println(cur_node_ptr->cur_angle);
              Serial.println("-----------------\n");
              break;
                    
          case 'D':
              if (cur_node_ptr != node_3_ptr){
                Serial.println("Please check current node ID and control input");
                break;
              }        
              for (int i =0; i<UNIT_LARGE; i+=k){
                cur_node_ptr->actuator.write(cur_node_ptr->cur_angle - i);
                delay(500);
                }
              cur_node_ptr->cur_angle -= UNIT_LARGE;
              Serial.println("Current node's angle");
              Serial.println(cur_node_ptr->cur_angle);
              Serial.println("-----------------\n");
              break;
              
          case 'E':
              if (cur_node_ptr != node_3_ptr){
                Serial.println("Please check current node ID and control input");
                break;
              }        
              for (int i =0; i<UNIT_MEDIUM; i+=k){
                cur_node_ptr->actuator.write(cur_node_ptr->cur_angle - i);
                delay(500);
                }
              cur_node_ptr->cur_angle -= UNIT_MEDIUM;
              Serial.println("Current node's angle");
              Serial.println(cur_node_ptr->cur_angle);
              Serial.println("-----------------\n");
              break;
              
          case 'F':
              if (cur_node_ptr != node_3_ptr){
                Serial.println("Please check current node ID and control input");
                break;
              }        
              for (int i =0; i<UNIT_SMALL; i+=k){
                cur_node_ptr->actuator.write(cur_node_ptr->cur_angle - i);
                delay(500);
                }
              cur_node_ptr->cur_angle -= UNIT_SMALL;
              Serial.println("Current node's angle");
              Serial.println(cur_node_ptr->cur_angle);
              Serial.println("-----------------\n");
              break;
  
  
          case '4':
              cur_node_ptr = node_4_ptr;
              Serial.println("Current target control node's ID");
              Serial.println(cur_node_ptr->node_id);
              Serial.println("-----------------\n");
              break;
              
          case 'G':
              if (cur_node_ptr != node_4_ptr){
                Serial.println("Please check current node ID and control input");
                break;
              }        
              for (int i =0; i<UNIT_LARGE; i+=k){
                cur_node_ptr->actuator.write(cur_node_ptr->cur_angle + i);
                delay(500);
                }
              cur_node_ptr->cur_angle += UNIT_LARGE;
              Serial.println("Current node's angle");
              Serial.println(cur_node_ptr->cur_angle);
              Serial.println("-----------------\n");      
              break;
                         
          case 'H':
              if (cur_node_ptr != node_4_ptr){
                Serial.println("Please check current node ID and control input");
                break;
              }        
              for (int i =0; i<UNIT_MEDIUM; i+=k){
                cur_node_ptr->actuator.write(cur_node_ptr->cur_angle + i);
                delay(500);
                }
              cur_node_ptr->cur_angle += UNIT_MEDIUM;
              Serial.println("Current node's angle");
              Serial.println(cur_node_ptr->cur_angle);
              Serial.println("-----------------\n");
              break;
                    
          case 'I':
              if (cur_node_ptr != node_4_ptr){
                Serial.println("Please check current node ID and control input");
                break;
              }        
              for (int i =0; i<UNIT_SMALL; i+=k){
                cur_node_ptr->actuator.write(cur_node_ptr->cur_angle + i);
                delay(500);
                }
              cur_node_ptr->cur_angle += UNIT_SMALL;
              Serial.println("Current node's angle");
              Serial.println(cur_node_ptr->cur_angle);
              Serial.println("-----------------\n");
              break;
                    
          case 'J':
              if (cur_node_ptr != node_4_ptr){
                Serial.println("Please check current node ID and control input");
                break;
              }        
              for (int i =0; i<UNIT_LARGE; i+=k){
                cur_node_ptr->actuator.write(cur_node_ptr->cur_angle - i);
                delay(500);
                }
              cur_node_ptr->cur_angle -= UNIT_LARGE;
              Serial.println("Current node's angle");
              Serial.println(cur_node_ptr->cur_angle);
              Serial.println("-----------------\n");
              break;
              
          case 'K':
              if (cur_node_ptr != node_4_ptr){
                Serial.println("Please check current node ID and control input");
                break;
              }        
              for (int i =0; i<UNIT_MEDIUM; i+=k){
                cur_node_ptr->actuator.write(cur_node_ptr->cur_angle - i);
                delay(500);
                }
              cur_node_ptr->cur_angle -= UNIT_MEDIUM;
              Serial.println("Current node's angle");
              Serial.println(cur_node_ptr->cur_angle);
              Serial.println("-----------------\n");
              break;
              
          case 'L':
              if (cur_node_ptr != node_4_ptr){
                Serial.println("Please check current node ID and control input");
                break;
              }        
              for (int i =0; i<UNIT_SMALL; i+=k){
                cur_node_ptr->actuator.write(cur_node_ptr->cur_angle - i);
                delay(500);
                }
              cur_node_ptr->cur_angle -= UNIT_SMALL;
              Serial.println("Current node's angle");
              Serial.println(cur_node_ptr->cur_angle);
              Serial.println("-----------------\n");
              break;
        
          default:
              Serial.println("Please check your typing input!");
              break;            
                                   
      }
    }
  }
 }
