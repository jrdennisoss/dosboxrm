
# Overview

The version of the ReelMagic driver that was implemented is 1.11

## Major Components

*  RMDEV.SYS          - Tells programs information about the ReelMagic driver and configuration.
*  FMPDRV.EXE         - This is the TSR driver for the ReelMagic hardware.
*  The Physical H/W   - FMPDRV.EXE (and possibly RMDEV.SYS) talk to this via port I/O and also probably DMA







# RMDEV.SYS
When loaded, this DOS device driver file responds to some INT 2F / DOS Multiplex calls. Mainly it's
purpose is to tell applications where they can find the driver as well as some other things about
the hardware. It also handles the audio mixer interfaces.

Note: The ReelMagic DOSBox code `reelmagic_driver.cpp` emulates this file's functionality and
therfore is not required for using the DOSBox ReelMagic emulator. Like many emulated functionalities
within DOSBox, there is no actual "RMDEV.SYS" file. Its functionality is permanently resident when
ReelMagic support is enabled.


## Function AX=9800h
The AX=9800h function has several subfunctions it responds to...

### Subfunction BX=0000 - Query Magic Number
Means for applications to discover if the ReelMagic driver and hardware is installed.
Replies with "RM" string by setting AX=524Dh

### Subfunction BX=0001 - Query Driver Version
Means for applications to query the installed ReelMagic driver version.
Replies with AH=major and AL=minor.
Since version 1.11 is the target here, I reply with AH=01h AL=0Bh

### Subfunction BX=0002 - Query I/O Base Address
Means for applications to query which I/O base address the ReelMagic card is at. From my limited research,
the port I/O size is 4 bytes. Currently replying with a totally incorrect value of AX=9800h. This way if
anything reads/writes to the port, the DOSBox debugger will be verbose about it. The default stock config
of a ReelMagic card sits at port 0x260 from what I can tell.

Note: As "FMPDRV.EXE" is fully emulated, this value is ignored so it does not really matter
      what it replies with.

### Subfunction BX=0003 - Probably Query IRQ 
Not 100% sure, but I think this is the IRQ number of the card. Currently replying with AX=11.

Note: As "FMPDRV.EXE" is fully emulated, this value is ignored so it does not really matter
      what it replies with.

### Subfunction BX=0004 - Query if MPEG/ReelMagic Audio Channel is Enabled (I think)
This impacts the enabled/disabled UI state of the MPEG slider in the "DOXMIX.EXE" utility. This
is either a query to say that we have MPEG audio channel for the mixer, or its replying with an
IRQ or location or something... AX=1 for yes, AX = 0 for no

### Subfunction BX=0007 - Query if PCM and CD Audio Channels are Enabled (I think)
This impacts the enabled/disabled UI state of the PCM and CD Audio sliders in the "DOXMIX.EXE"
utility. This is either a query to say that we have theses channels for the mixer, or its
replying with an IRQ or location or something... AX=1 for yes, AX = 0 for no

### Subfunction BX=0010 - Query Main Audio Left Channel Volume
Reply with the channel volume value in AX. 0 = off and 100 = max.

### Subfunction BX=0011 - Query Main Audio Right Channel Volume
Reply with the channel volume value in AX. 0 = off and 100 = max.

### Subfunction BX=0012 - Query MPEG Audio Left Channel Volume
Reply with the channel volume value in AX. 0 = off and 100 = max.

### Subfunction BX=0013 - Query MPEG Audio Right Channel Volume
Reply with the channel volume value in AX. 0 = off and 100 = max.

### Subfunction BX=0014 - Query SYNTH Audio Left Channel Volume
Reply with the channel volume value in AX. 0 = off and 100 = max.

### Subfunction BX=0015 - Query SYNTH Audio Right Channel Volume
Reply with the channel volume value in AX. 0 = off and 100 = max.

### Subfunction BX=0016 - Query PCM Audio Left Channel Volume
Reply with the channel volume value in AX. 0 = off and 100 = max.

### Subfunction BX=0017 - Query PCM Audio Right Channel Volume
Reply with the channel volume value in AX. 0 = off and 100 = max.

### Subfunction BX=001C - Query PCM Audio Left Channel Volume
Reply with the channel volume value in AX. 0 = off and 100 = max.

### Subfunction BX=001D - Query PCM Audio Right Channel Volume
Reply with the channel volume value in AX. 0 = off and 100 = max.



## Function AX=9801h
The AX=9801h function has several subfunctions it responds to... As far as I can tell,
these are just setters for the mixer.

### Subfunction BX=0010 - Set Main Audio Left Channel Volume
New volume level is in DX. Value will be between 0 (off) and 100 (max).

### Subfunction BX=0011 - Set Main Audio Right Channel Volume
New volume level is in DX. Value will be between 0 (off) and 100 (max).

### Subfunction BX=0012 - Set MPEG Audio Left Channel Volume
New volume level is in DX. Value will be between 0 (off) and 100 (max).

### Subfunction BX=0013 - Set MPEG Audio Right Channel Volume
New volume level is in DX. Value will be between 0 (off) and 100 (max).

### Subfunction BX=0014 - Set SYNTH Audio Left Channel Volume
New volume level is in DX. Value will be between 0 (off) and 100 (max).

### Subfunction BX=0015 - Set SYNTH Audio Right Channel Volume
New volume level is in DX. Value will be between 0 (off) and 100 (max).

### Subfunction BX=0016 - Set PCM Audio Left Channel Volume
New volume level is in DX. Value will be between 0 (off) and 100 (max).

### Subfunction BX=0017 - Set PCM Audio Right Channel Volume
New volume level is in DX. Value will be between 0 (off) and 100 (max).

### Subfunction BX=001C - Set PCM Audio Left Channel Volume
New volume level is in DX. Value will be between 0 (off) and 100 (max).

### Subfunction BX=001D - Set PCM Audio Right Channel Volume
New volume level is in DX. Value will be between 0 (off) and 100 (max).



## Function AX=9803h - Query Path to Driver EXE
Means for applications to query the path where they can find the driver ("FMPDRV.EXE") executable. The
path must be fully-qualified and end with a '\' character. For example, if "FMPDRV.EXE" is installed in
"Z:\FMPDRV.EXE" then this function must respond with "Z:\".

The output is to be written as a null-terminated string to caller-provided memory at address DX:BX,
and set AX=0 on success.


## Function AX=9803h - Unknown
I'm not quite sure what this does. It is possibly a call to reset the card, but I'm just speculating. If I
do NOT return with an AX=0 from this call, then the Return to Zork game spams a bunch of `driver_call(10h,...)`
calls to FMPDRV.EXE and things don't seem to quite function as expected.

## Function AX=98FF - Probably Driver Unload
This appears to be a reset / clean call that is invoked when a "FMPLOAD.COM /u" is called. Currently
returning AX=0



 








# FMPDRV.EXE

This is the main driver for the ReelMagic MPEG video decoders. When invoked, it installs a software
interrupt handler as a TSR which is respondible for handling all requests to the MPEG decoders. If invoked
with the "/u" command-line option, it unloads the TSR and the interrupt handler it previously installed.

This file must always be named "FMPDRV.EXE" but can exist in any path. The path to this EXE file is
provided by an INT 2F function AX=9803h call to RMDEV.SYS as documented above.


Note: The ReelMagic DOSBox code `reelmagic_driver.cpp` emulates this file's functionality. Since an actual
"FMPDRV.EXE" file must exist somewhere for things to work smoothly, a functional "Z:\FMPDRV.EXE" file is
provided when ReelMagic support is enabled.

## Loading

The Return to Zork game provides an "FMPLOAD.COM" executable which is responsible for finding and
executing "FMPDRV.EXE" before the game start, and unloading it after the game ends. Since the "FMPDRV.EXE"
string is hardcoded, a constraint is created to use this exact filename for our driver emulator. The
"FMPLOAD.COM" executable will call the correct "Z:\FMPDRV.EXE" file as it makes a call to the "RMDEV.SYS"
Function AX=9803h - Query Path to Driver EXE.


Alternatively, the "Z:\FMPDRV.EXE" file can be invoked in the DOSBox's `autoexec` config section as
subsequent calls to either the emulated "FMPDRV.EXE" or a real "FMPDRV.EXE" will be a no-op because the
drivers perform a check to make sure they are not already loaded.

## Detection of FMPDRV.EXE From User Applications

User applicaions detect the ReelMagic "FMPDRV.EXE" driver TSR presence
and interrupt number to use by doing something like this:
```
for (int_num = 0x80; int_num < 0x100; ++int_num) {
  ivt_func_t ivt_callback_ptr = cpu_global_ivt[int_num];
  if (ivt_callback_ptr == NULL) continue;
  const char * str = ivt_callback_ptr; //yup you read that correctly,
                                       //we are casting a function pointer to a string...
  if (strcmp(&str[3], "FMPDriver") == 0) {
    return int_num; //we have found the FMPDriver at INT int_num
}
```

Or possibly something like this: (looking at you "FMPTEST.EXE")
```
for (int_num = 0x80; int_num < 0x100; ++int_num) {
  ivt_func_t ivt_callback_ptr = cpu_global_ivt[int_num];
  if (ivt_callback_ptr == NULL) continue;
  const char * str = ivt_callback_ptr; //yup you read that correctly,
                                       //we are casting a function pointer to a string...
  size_t strsize = (unsigned char)&str[2];
  if (strcmp(&str[3], "FMPDriver", strsize) == 0) {
    return int_num; //we have found the FMPDriver at INT int_num
}
```

## The "driver_call()" INT 80h+ API

This is the main API call entry point to the driver from user applications such as SPLAYER.EXE or Return
to Zork, which is used to control the ReelMagic MPEG video decoders. This is normally found at INT 80h,
however it appears that it can also be placed in a higher IVT slot if 80h is already occupied (!=0) by
something else. See above section "Detection of FMPDRV.EXE From User Applications" for the logic used in
user applications to determine which INT number this API is found on. For this project, I usually refer
to this API as the `driver_call()` function.

The prototype for this `driver_call()` function I use is:
```
uint32_t driver_call(uint8_t command, uint8_t media_handle, uint16_t subfunc, uint16_t param1, uint16_t param2);
```

The parameters are passed in as registers:
```
  BH = command
  BL = media_handle
  CX = subfunc
  AX = param1
  DX = param2
```

The 32-bit return value is stored in AX (low 16-bits) and DX (high 16-bits)

The `command` parameter is used to specify which command/function is used and the `subfunc` parameter is
essentially a sub-function for some of these calls. The `media_handle` parameter specifies which handle
the call is specifically for, or 00h if N/A or global parameter. A media handle is essentially a reference
to a "hardware" MPEG video player/decoder resource.

### Command/Function 01h - Open Media Handle
Opens a media handle (MPEG decoder / player) for the given DOS filepath, and returns the new media handle.
If the file does not exist, then a zero media handle is returned. The filepath is passed in via a pointer
to a null-terminated string; `param2` is the segment, and `param1` is the offset. If subfunc has the 1000h
flag set, then the filepath is NOT treated as null-terminated string, but rather the first byte being the
string length.

I have seen three different subfunction values passed into this function. Return to Zork always seems to
use 0002h,  "SPLAYER.EXE" uses 0001h, and "FMPTEST.EXE" uses 1001h. I'm not sure why the difference between,
the 1 and 2, but I currently accept anything for the subfunction parameter and just log a warning if I get
something that's not expected. One theory is that the 0002h tells the open function to parse any suffixed
arguments (e.g. "/l") on the filepath and the 0001h tells the open function that the filepath is to be
opened exactly as-is, but this is just a guess.

As mentioned above, the filepath string could have a "suffix" appended to it. For some assets, the Return
to Zork game appends a "/l" (that's a lower-case "L", not the number one) to the filepath string. This is
only done on assets that are intended to loop such as scene backgrounds. The ReelMagic emulator implements
this behavior and put the new opened player into a "loop mode" when this flag set. In "loop mode" the
video will loop/play forever until it is destroyed.

### Command/Function 02h - Close Media Handle
Closes the given media handle and frees all resources associated with it. Always returns zero, but as far
as I can tell, noone checks the return value.

### Command/Function 03h - Control/Play Media Handle
This is either "control" or "play" media handle, I'm not sure which. I have only observed the following
sub-functions.

#### Subfunction 0001h - Start Playing
This tells the MPEG decoder to start playing the specified media handle. This always returns zero, but as
far as I can tell, noone checks the return value.

#### Subfunction 0004h - Start Playing in Loop
This tells the MPEG decoder to start playing the specified media handle. This will also put the player
into a "loop mode" before starting. See above "Command/Function 01h - Open Media Handle" for more
information on loop mode. This always returns zero, but as far as I can tell, noone checks the return value.


### Command/Function 04h - Unknown - Possibly Stop Media Handle
I'm not quite sure what this is. I see Return to Zork call this on a specific media handle usually before
it closes the handle. Currently returning 0;

### Command/Function 09h - Set Parameter
I'm pretty confident this is a "set parameter" function. For most of these I am just ignoring them as I do
not know exactly what they do and am returning zero. This can be called on a specific `media_handle` or
called globally by using a zero `media_handle`.

#### Subfunction 0109h - Unknown
Called from "SPLAYER.EXE" with a zero media handle and return value is not checked. I don't think I have
seen Return to Zork call this subfunction. Returning zero and ignoring for now.

#### Subfunction 0210h - Unknown
Called from Return to Zork with a zero media handle and some values in `param1` and `param2`. This might
be something like setting a user data pointer for the user callback function. (see below)

The return value does not appear to be checked. Returning zero and ignoring for now.

#### Subfunction 0408h - Unknown
Called from Return to Zork with a zero media handle and return value does not appear to be checked.
Returning zero and ignoring for now.

#### Subfunction 0409h - Unknown
Called from Return to Zork with a zero media handle and return value does not appear to be checked.
Returning zero and ignoring for now.

#### Subfunction 040Ch - Unknown
Called from Return to Zork with a zero media handle and return value does not appear to be checked.
Returning zero and ignoring for now.

#### Subfunction 040Dh - Unknown
Called from Return to Zork with a zero media handle and return value does not appear to be checked.
Returning zero and ignoring for now.

#### Subfunction 040Eh - Unknown
Called from "FMPTEST.EXE" with a zero media handle.
Returning zero and ignoring for now.

#### Subfunction 040Eh - Possibly Set Z-Order
Called from Return to Zork with a valid media handle and return value does not appear to be checked. I
think this is used to set the presentation z-order on a given media handle. The higher the number, the
further back (lower priority) the video output window is. Currently setting this value in the associated
player object with the media handle.

#### Subfunction 1409h - Set Display Size
Called from "FMPTEST.EXE" to set the display dimensions of the output video window. "param1" is width and
"param2" is height. Returning zero.

#### Subfunction 2408h - Set Display Position
Called from "FMPTEST.EXE" to set the display position of the output video window. "param1" is width and
"param2" is height. Returning zero.


### Command/Function 0Ah - Get Parameter / Status
This is a "get parameter" or "get status" or both function. Unlike the set functions, these get functions
can't be as easily ignored and must reply back to the user applications with valid values otherwise things
get screwed up real good real fast...


#### Subfunction 0108h - Query Something About Resource Availability ???
Not quite sure about this one, but "FMPTEST.EXE" wants a value of DX=0 and AX >=32h. If I don't give it
what it wants, then the program gives me a "Not enough memort" error. This is only seems to be called
with a zero (global) media handle.


#### Subfunction 0202h - Query File Validity
This is some kind of file validity check that "SPLAYER.EXE", "FMPTEST.EXE", and Return to Zork call after
opening a media handle. It would appear that a return value or 0x3 means "yes the file is good" and that
a return value of 0x0 means "the file is bad". In order for this emulator to return 0x3, the file must
contain at least one MPEG-1 decodable picture encapsulated in either an MPEG-PS or MPEG-ES format.

#### Subfunction 0204h - Query Play State
This is some kind play state query. Both "SPLAYER.EXE" and Return to Zork "busy poll" this call after
sending a play command (03h) to us. The "SPLAYER.EXE" tool spins while (return value & 0x03) == 0 and
Return to Zork spins on this while the return value == 0x14. Therfore, while the player associated with
the given media handle is actively playing a file, this will return 0x14, and 0x01 when playing of said
file has ended.

Nots: This subfunction will forever return 0x14 when invoked on a media handle which is in "loop mode"

#### Subfunction 0208h - Unknown but Critically Important
I'm not exactly sure what this is, however, if we don't return a zero value, things get screwed up bad.
Return to Zork calls this every once in a while on a given file handle. Currently, returning zero seems to
make things work as expected, although this behavior really should be understood further.

I've tried returing a variety of variables and often Return to Zork will start closing random file handles
via DOS/INT21! Sometimes it wacks the video and other times it hits something else. When returning zero,
there do not seem to by any unwarranted DOS/INT21 file close calls. 

Looking further into the Return to Zork code, the routine at 15B2:0131 is
what seems to screw things up. It is responsible for calling this routine,
then checking the return value for non-zero. If the return value is non-zero,
it does a bunch of stuff that puts us in a bad state, as well as it starts
calling function 09h with the same subfunction 0208h. For now, I think it's
pretty safe to say that returning 00000000 from this subfunction is the right
thing to do.

#### Subfunction 0403h - Likely Get Decoded Picture Dimensions
This is likely a call to get the decoded picture dimensions. The picture height is height in the
upper 16-bits (DX) and the picture width is returned in the lower 16-bits.

If it's not a getter for the decoded picture dimensions, then it's probably a getter for current
display size. (which would need to default to decoded picture dimensions)


### Command/Function 0Bh - Register User Callback Function
This registers a callback function for the API user. It would appear that the registered callback function
is to be called on certain driver/device events. See the "The User Callback Function" sub-section below
for more information.

The media handle and subfunction parameters are ignored, `param2` is the segment and `param1` is the
offset of the callback function pointer. Zero can be given to both `param2` and `param1` to disable.
Zero is always returned; it does not appear the return value is checked.

### Command/Function 0Dh - Unload FMPDRV.EXE
This is called by "FMPDRV.EXE" when the user passes a "/u" onto the command-line. Invoking this function
cleans up any open media handles and removes the driver's INT handler. The "RMDEV.SYS" INT 2F functions
still remain resident and functional. Zero is always returned.

### Command/Function 0Eh - Reset
This is the very first call that "SPLAYER.EXE" and Return to Zork do at application startup. Likey this is
a reset function as the return value does not appear to be checked.





## The User Callback "driver_callback()" Function
A "user callback function" can be registered via the `driver_call(0Bh, ...)` API. It looks like this
function is intended to be called back from the driver to the user application on certain events. The
prototype for this `driver_callback()` function appears to be:
```
void driver_callback(uint16_t command, uint8_t media_handle, uint8_t unknown1, uint32_t unknown2);
```

The calling convention is standard x86 "far call" where all function parameters are passed on the stack
along with the 16-bit segment and 16-bit offset for the return address.

I have not dug too deep into what all the valid commands/events are, but running the Return To Zork
`driver_callback()` function (found at 15B2:070B) through Ghidra shows that there are quite a few.
I have only currently implemented function #5 to fix the "RTZ Click-to-Skip Bug" mentioned below.
Ultimately, all these callbacks should be understood and implemented to ensure maximum general
ReelMagic emulated compatibility.

Side note: Return to Zork does a -1 on the `command` variable before comparing it. This leads me to
           beleive that zero may not be a valid value. 

### Command/Function 01h - Unknown
No idea what this does. I am not (yet) calling it.

### Command/Function 02h - Unknown
No idea what this does. I am not (yet) calling it.

### Command/Function 03h - Unknown
No idea what this does. I am not (yet) calling it.

### Command/Function 04h - Unknown
No idea what this does. I am not (yet) calling it.

### Command/Function 05h - Media Closed (or Possibly Media Stopped)
This appears to be a callback that is expected to happen once the driver has either closed or stopped
the media playing. I'm leaning towards this being an "I'm about to close the media handle" callback, but
at this point I am not 100% sure. However by invoking this callback when we are called to close a media
handle, this fixes "The RTZ Click-to-Skip Bug" mentioned below...

### Command/Function 09h - Unknown
No idea what this does. I am not (yet) calling it.

### Command/Function 0Ah - Unknown
No idea what this does. I am not (yet) calling it.

### Command/Function 0Bh - Unknown
No idea what this does. I am not (yet) calling it.



# The Physical H/W

For now, no port I/O calls have been implemented, but I left commented out some of the remaining port I/O
handler code I was tinkering with when debugging. For now, taking a similiar approach to what DOSBox did
with MSCDEX; implementation of an emulated driver resides in DOSBox.


# Known Issues and Limitations

As this API was generated from what I could get from the DOSBox debugger and Ghidra, there are
gonna be bugs and issues primarily because this is based on how I observed things interacting,
and not on a known spec.

## The RTZ Click-to-Skip Bug

This issue is big enough to note because this was the driving force for me to implement part of the
"user callback" functionality.

During the intro scene, if the mouse is clicked to skip the video, an issue is encountered where
the media handle #1 is closed twice. This would not be an issue, however, the first time the media
handle #1 is closed, the game still holds on to the handle #1 thinking its still open. Then, before
calling the second close of media handle #1, the next media handle for the next video  is opened,
which is given media handle #1 because the first one was closed. The game then closes media handle #1
right after opening the second video, which puts things into a never ending open-close loop for
all video assets in the game, resulting in black screens for the whole game.

This is a log of what happens:
```
REELMAGIC:FMPDRV.EXE driver_call(02h,01h,0h,0h,0h)=0h <-- mouse click generates this
  Note: Currentl implementation invokes user callback function #5 here
REELMAGIC:FMPDRV.EXE pre-exception driver_call(0Ah,01h,204h,0h,0h)
REELMAGIC:FMPDRV.EXE pre-exception driver_call(0Ah,01h,204h,0h,0h)
REELMAGIC:FMPDRV.EXE driver_call(04h,01h,0h,0h,0h)=0h
 Note: Or... Should we do the callback here? What is function 4?
REELMAGIC:FMPDRV.EXE pre-exception driver_call(0Ah,01h,208h,0h,0h)
  NOTE: new stream is opened here...
REELMAGIC:FMPDRV.EXE pre-exception driver_call(02h,01h,0h,0h,0h)
```

This could be worked around by "looping" the next free media handle instead of always starting
at index zero. This would work because by the time things were wrapped back to zero, the game
would have already had done the double-closed that handle.

The "right" way to do this seems to be firing the "user callback function" that was registered on
game start to notify the game that the handle is no longer playing and/or open. From what I can
tell, there is only one user callback command/sub-function which could be used to handle this
which is #5.

Roughly how the game handles #5:
```
driver_callback(command=0005h, media_handle=XXh, unknown1=00h, unknown2=00000000h)
  . driver_call(0xA,media_handle,0x208,0,0)
  . Calls FUN_15b2_0131();
    . driver_call(0xA,media_handle,0x208,0,0)
  . if (media_handle == DAT_1afe_18ec) DAT_1afe_18ec = 0;
```

The last part where `DAT_1afe_18ec` is zero'd releases the game's copy of the handle number and
therfore does not try to keep closing it after that is done.



# Example API Usage from SPLAYER.EXE

```
if (detect_driver_intnum() == 0) die; //stores the intnum in a global somewhere that driver_call() uses
driver_call(0xe,0,0,0,0);
driver_call(9,0,0x109,1,0);
mpeg_file_handle = driver_call(1,0,1,argv[1],argv_segment);
if (!mpeg_file_handle) die;
if (!driver_call(10,mpeg_file_handle,0x202,0,0)) {
  driver_call(2,mpeg_file_handle,0,0,0);
  die;
}
INT 10h AX = 0f00h -- Get videomode (store result of AL)
INT 10h AX = 0012h -- Set videomode to 12h; i think this is M_EGA 640x480 (see mode lists in "ints/int10_modes.cpp")
driver_call(3,mpeg_file_handle,1,0,0);
while ((driver_call(10,mpeg_file_handle,0x204,0,0) & 3) == 0);
driver_call(2,mpeg_file_handle,0,0,0);
INT 10h AX = 00??h -- Restore video mode from first INT10h call; AL is set to the stored result
```
