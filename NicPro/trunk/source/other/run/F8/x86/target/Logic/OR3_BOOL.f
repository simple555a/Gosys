fblock OR3_BOOL;
uuid{05fd-0fc4-a278-38a7};
var
IN1:bool; in;
IN2:bool; in;
IN3:bool; in;
OUT:bool; out;
implementation
void main()
{
  OUT = IN1 || IN2 || IN3;
}
