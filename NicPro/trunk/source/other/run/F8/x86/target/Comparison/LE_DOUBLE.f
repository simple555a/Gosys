fblock LE_DOUBLE;
uuid{727b-1ce4-d17c-3209};
var
IN1:double; in;
IN2:double; in;
OUT:bool; out;
implementation
void main()
{
OUT = IN1 <= IN2? 1 : 0;
}
