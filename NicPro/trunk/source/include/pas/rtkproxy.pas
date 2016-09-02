unit rtkproxy;

interface
uses
  rtkdefs,
  librtdb;
const
  PROXY_ENABLE_BGLOADER = 1 shl 0;
  PROXY_ENABLE_DISPATCHER = 1 shl 1;
  
  function init_proxy(hWnd : cardinal; flags : cardinal):__bool;stdcall;
    external 'librtkproxy.dll' ;
  procedure uninit_proxy();stdcall;stdcall;
    external 'librtkproxy.dll' ;
  function discover_nodes(timeout:__uint):__uint;stdcall;
    external 'librtkproxy.dll' ;
  function discover_groups(node:PNodeKey; timeout:__uint):__uint;stdcall;
    external 'librtkproxy.dll' ;
  function discover_tags(node:PNodeKey; group:PGroupKey; timeout:__uint):__uint;stdcall;
    external 'librtkproxy.dll' ;
implementation

end.
