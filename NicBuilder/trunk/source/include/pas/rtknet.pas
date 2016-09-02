unit rtknet;

interface

const
  RTK_INIT_AS_SERVER = $1;
  RTK_SKIP_STANDARD_BUSES = $2;
function init_network(flag:Integer; localaddr:Pchar):boolean;stdcall;external 'librtknet.dll';
procedure uninit_network;stdcall;external 'librtknet.dll';

implementation

initialization
  // init_network(0,nil);

finalization
  // uninit_network;

end.
