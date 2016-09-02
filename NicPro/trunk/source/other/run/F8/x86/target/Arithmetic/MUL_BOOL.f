
fblock MUL_BOOL;
uuid{fd6d-7540-db61-86f1};
var
	A	:float; in;
	B	:bool; in;
	OUT	:float; out;

implementation

void main()
{
	OUT = (float)B * A;
}
