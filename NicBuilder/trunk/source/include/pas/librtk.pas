unit librtk;

interface
uses
  rtkdefs;

type
  TRtkHandle = integer;

  procedure close_handle(handle : TRtkHandle);stdcall;
    external 'librtk.dll' ;
  function get_config_file:pchar;stdcall;external 'librtk.dll';
  function get_working_dir():pchar;stdcall;external 'librtk.dll';
  function get_exe_dir():pchar;stdcall;external 'librtk.dll';
  procedure set_working_dir(dir : pchar);stdcall;external 'librtk.dll';
  procedure free_buffer(b : pointer);stdcall;external 'librtk.dll';
implementation

end.
