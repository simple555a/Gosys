library pmcSeeker;

{ Important note about DLL memory management: ShareMem must be the
  first unit in your library's USES clause AND your project's (select
  Project-View Source) USES clause if your DLL exports any procedures or
  functions that pass strings as parameters or function results. This
  applies to all strings passed to and from your DLL--even those that
  are nested in records and classes. ShareMem is the interface unit to
  the BORLNDMM.DLL shared memory manager, which must be deployed along
  with your DLL. To avoid using BORLNDMM.DLL, pass string information
  using PChar or ShortString parameters. }

{$R 'version.res' 'version.rc'}

uses
  SysUtils,
  Classes,
  windows,
  ComCtrls,
  rtkdefs,
  librtdb,
  rtkproxy,
  Unit1 in 'Unit1.pas' {Form1},
  tagAttrs in 'tagAttrs.pas',
  SeekUtils in 'SeekUtils.pas',
  ExpFunctions in 'ExpFunctions.pas';

{$R *.res}

exports
  Discover index 1,
  GetFirstTag index 2,
  GetNextTag index 3,
  LoadSeeker index 4,
  UnloadSeeker index 5;
begin
end.
