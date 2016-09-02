fblock LE_INTEGER;
uuid{5e29-f32e-ddb8-9a07};
var
IN1:integer; in;
IN2:integer; in;
OUT:bool; out;
implementation
void main()
{
OUT = IN1 <= IN2? 1 : 0;
}
