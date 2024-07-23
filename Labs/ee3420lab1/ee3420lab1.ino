/*
 * ee3420lab1.ino 
 * written by: evan smith
 * this program will create in interactive console interface through the UART
 * it will implement a method to generate a perfect triangle
 */

void setup() 
{
  // put your setup code here, to run once:
  Serial.begin(115200, SERIAL_8N1);  
  // set up serial port for communication 
  // at 115200 baud with 8 data bits,
  // no parity bit, and 1 stop bit
  // The serial port on the host must match this
  Serial.setTimeout(100000); //gives time for input

}

void loop() 
{
  // put your main code here, to run repeatedly:
  Serial.println("Enter an integer between 1-65535");
  long int leg1 = Serial.parseInt();
  long int i = 1;

  if (leg1 > 0 && leg1 < 65536)
  {
    while(i <= 65535)
    {
      //calculates the hypotnuse of a right triangle
      //for values 1-65535
      double hyp = sqrt(pow(leg1,2) + pow(i,2));
      //if right triangle is produced it will print
      //the side lengths
      if (int(hyp) == hyp)
      {
        Serial.print(leg1);
        Serial.print(" ");
        Serial.print(i);
        Serial.print(" ");
        Serial.println(int(hyp)); 
      }
      i++;
    }
  }
//end
}
