fblock DC_BA;
attribute(autoscan);
uuid{7b54-ccc7-bff1-2094};
var
	S : bool;in;			
	IN: float;in;
	
	OUT:float;out;

implementation

void main()
{
	if(S)
	{ OUT=IN; }
	
}