#ifndef TOOL_MIDIW32_H
#define TOOL_MIDIW32_H

# include <windows.h>
# include <mmsystem.h>

# define MIDINOERROR  MMSYSERR_NOERROR
# define MIDIBADDEVID MMSYSERR_BADDEVICEID
# define MIDIBADPARAM MMSYSERR_INVALPARAM
# define MIDINODRIVER MMSYSERR_NODRIVER
# define MIDIOUTOFMEM MMSYSERR_NOMEM
# define MIDISTILLPLAYING MIDIERR_STILLPLAYING
# define MIDIBADHANDLE MMSYSERR_INVALHANDLE


#define MIDIINHANDLE HMIDIIN
#define MIDIOUTHANDLE HMIDIOUT


#endif // TOOL_MIDIW32_H
