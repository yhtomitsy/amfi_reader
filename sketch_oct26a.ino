uint32_t t = 0;
int buff[21] = {0};
double posX = 0;
double posY = 0;
double posZ = 0;
double quatX = 0;
double quatY = 0;
double quatZ = 0;
double quatW = 0;

enum packetState {
  unknown,
  oneDown,
  twoDown
};
packetState stat_ = unknown;


void setup() {
    Serial.begin(2000000);
    Serial1.begin(921600);
}

void loop() {
    while(Serial1.available()){
        char c = Serial1.read();
        if(c == 0x02 && stat_ == twoDown){
            stat_ = unknown;
        }
        else if(c == 0x0B && stat_ == unknown){
            stat_ = oneDown;
        }
        else if(c == 0x83 && stat_ == oneDown){
            stat_ = twoDown;
        }  
        if(c == 0x14 && stat_ == twoDown)
        {
            for(uint8_t i = 0; i < 21; i++)
            {
                buff[i] = (char)Serial1.read();
            }
            getData();
            if(abs(posX) < 10000  &&  abs(posY) < 10000 && abs(posZ) < 10000)
            {
                Serial.print(posX);Serial.print(",");Serial.print(posY);Serial.print(",");Serial.print(posZ);Serial.print(",");
                Serial.print(quatW);Serial.print(",");Serial.print(quatX);Serial.print(",");Serial.print(quatY);Serial.print(",");Serial.println(quatZ);
            }
            
        }
    }
    delay(10);
}

void getData(){
    /*posX = double(buff[0] | (buff[1] << 8) | (buff[2] << 16)) / 100.0;
    posY = double(buff[3] | (buff[4] << 8) | (buff[5] << 16)) / 100.0;
    posZ = double(buff[6] | (buff[7] << 8) | (buff[8] << 16)) / 100.0;
    quatX = double(buff[9] | (buff[10] << 8) | (buff[11] << 16)) / 1000000.0;
    quatY = double(buff[12] | (buff[13] << 8) | (buff[14] << 16)) / 1000000.0;
    quatZ = double(buff[15] | (buff[16] << 8) | (buff[17] << 16)) / 1000000.0;
    quatW = double(buff[18] | (buff[19] << 8) | (buff[20] << 16)) / 1000000.0;*/

    posX = twosComplement(buff[0], buff[1], buff[2]) / 100.0;
    posY = twosComplement(buff[3], buff[4], buff[5]) / 100.0;
    posZ = twosComplement(buff[6], buff[7], buff[8]) / 100.0;
    quatX = twosComplement(buff[9], buff[10], buff[11]) / 1000000.0;
    quatY = twosComplement(buff[12], buff[13], buff[14]) / 1000000.0;
    quatZ = twosComplement(buff[15], buff[16], buff[17]) / 1000000.0;
    quatW = twosComplement(buff[18], buff[19], buff[20]) / 1000000.0;
}

float twosComplement(uint8_t a, uint8_t b, uint8_t c){
  // convert three bytes of 2s complement into 24 bit signed integer
  int val = 0;
  if (c & 0x80) val = 0xff;
  val = (val << 8) | c;
  val = (val << 8) | b;
  val = (val << 8) | a;
  return (float)val;
}

