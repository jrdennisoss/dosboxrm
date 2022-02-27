/*
 *  Copyright (C) 2022 Jon Dennis
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */

#ifndef DOSBOX_REELMAGIC_H
#define DOSBOX_REELMAGIC_H

#include "dosbox.h"


//
// video mixer stuff
//
struct ReelMagic_VideoMixerUnderlayProvider {
  virtual ~ReelMagic_VideoMixerUnderlayProvider() {}
  virtual void OnVerticalRefresh(void * const outputBuffer, const float fps) = 0;
  virtual bool   IsDisplayFullScreen() = 0;
  virtual Bit16u GetDisplayPositionWidth() = 0;
  virtual Bit16u GetDisplayPositionHeight() = 0;
  virtual Bit16u GetDisplaySizeWidth() = 0;
  virtual Bit16u GetDisplaySizeHeight() = 0;

  virtual Bit16u GetPictureWidth() const = 0;
  virtual Bit16u GetPictureHeight() const = 0;
  virtual Bit16u GetZOrder() const = 0;
};

void ReelMagic_RENDER_SetPal(Bit8u entry,Bit8u red,Bit8u green,Bit8u blue);
void ReelMagic_RENDER_SetSize(Bitu width,Bitu height,Bitu bpp,float fps,double ratio,bool dblw,bool dblh);
bool ReelMagic_RENDER_StartUpdate(void);
//void ReelMagic_RENDER_EndUpdate(bool abort);
//void ReelMagic_RENDER_DrawLine(const void *src);
typedef void (*ReelMagic_ScalerLineHandler_t)(const void *src);
extern ReelMagic_ScalerLineHandler_t ReelMagic_RENDER_DrawLine;

void ReelMagic_SetVideoMixerEnabled(const bool enabled);
void ReelMagic_PushVideoMixerUnderlayProvider(ReelMagic_VideoMixerUnderlayProvider& provider);
void ReelMagic_PopVideoMixerUnderlayProvider(ReelMagic_VideoMixerUnderlayProvider& provider);
void ReelMagic_VideoMixerUnderlayProviderZOrderUpdate(void);
void ReelMagic_InitVideoMixer(Section* /*sec*/);




//
// player stuff
//
struct ReelMagic_MediaPlayerFile {
  virtual ~ReelMagic_MediaPlayerFile() {}
  virtual const char *GetFileName() const = 0;
  virtual Bit32u GetFileSize() const = 0;
  virtual Bit32u Read(Bit8u *data, Bit32u amount) = 0;
  virtual void Seek(Bit32u pos, Bit32u type) = 0; // type can be either DOS_SEEK_SET || DOS_SEEK_CUR...
};
struct ReelMagic_MediaPlayer {
  virtual ~ReelMagic_MediaPlayer() {}
  virtual void SetDisplayPosition(const Bit16u x, const Bit16u y) = 0;
  virtual void SetDisplaySize(const Bit16u width, const Bit16u height) = 0;
  virtual void SetZOrder(const Bit16u value) = 0;
  virtual void SetLooping(const bool value) = 0;

  virtual bool IsFileValid() const = 0;
  virtual bool IsLooping() const = 0;
  virtual bool IsPlaying() const = 0;

  virtual Bit16u GetPictureWidth() const = 0;
  virtual Bit16u GetPictureHeight() const = 0;
  virtual Bit16u GetZOrder() const = 0;

  virtual void Play() = 0;
  virtual void Pause() = 0;
  virtual void Stop() = 0;
};

typedef Bit8u ReelMagic_MediaPlayer_Handle;

//note: once a player file object is handed to new/delete player, regardless of success, it will be cleaned up
ReelMagic_MediaPlayer_Handle ReelMagic_NewPlayer(struct ReelMagic_MediaPlayerFile * const playerFile);
void ReelMagic_DeletePlayer(const ReelMagic_MediaPlayer_Handle handle);
ReelMagic_MediaPlayer& ReelMagic_HandleToMediaPlayer(const ReelMagic_MediaPlayer_Handle handle); //throws on invalid handle
void ReelMagic_DeleteAllPlayers();

void ReelMagic_InitPlayerAudioMixer(void);




//
// driver and general stuff
//
void ReelMagic_Init(Section* /*sec*/);




#endif /* #ifndef DOSBOX_REELMAGIC_H */
