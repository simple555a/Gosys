fblock OR6_BOOL;
uuid{05fd-0fc4-a278-38aa};
var
IN1:bool; in;
IN2:bool; in;
IN3:bool; in;
IN4:bool; in;
IN5:bool; in;
IN6:bool; in;
OUT:bool; out;
implementation
void main()
{
  OUT = IN1 || IN2 || IN3 || IN4 || IN5 || IN6;
}