 /* ========================================================================
  * Copyright (C) 2001  Vienna University of Technology
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the GNU Lesser General Public
  * License as published by the Free Software Foundation; either
  * version 2.1 of the License, or (at your option) any later version.
  *
  * This library is distributed in the hope that it will be useful,
  * but WITHOUT ANY WARRANTY; without even the implied warranty of
  * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  * Lesser General Public License for more details.
  *
  * You should have received a copy of the GNU Lesser General Public
  * License along with this library; if not, write to the Free Software
  * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
  *
  * For further information please contact Gerhard Reitmayr under
  * <reitmayr@ims.tuwien.ac.at> or write to Gerhard Reitmayr,
  * Vienna University of Technology, Favoritenstr. 9-11/188, A1090 Vienna,
  * Austria.
  * ========================================================================
  * PROJECT: OpenTracker
  * ======================================================================== */
/** source file for SpeechVoiceModule module.
  *
  * @author Reinhard Steiner
  *
  * $Header: /scratch/subversion/cvs2svn-0.1236/../cvs/opentracker/src/input/SpeechVoiceModule.cxx,v 1.1 2002/12/10 17:23:45 kaufmann Exp $
  * @file                                                                   */
 /* ======================================================================= */


//#include "stdafx.h"
#include "SpeechVoiceModule.h"
#include "SpeechVoice.h"
#include "SpeechCore.h"


// Disable Debug warning for std lib classes
#pragma warning( disable : 4786 )


#ifdef USE_SAPISPEECH
using namespace std;


void SpeechVoiceModule::Initialize()
{
  m_Voice = new CSpeechVoice;
  assert(m_Voice);
}


void SpeechVoiceModule::Destroy()
{
  // Release voice, if created
  if(m_Voice)
  {
    delete(m_Voice);
    m_Voice = 0;
  }
}



DWORD SpeechVoiceModule::GetNumVoices()
{
  assert(m_Voice);
  return(m_Voice->GetNumVoices());
}


const char* SpeechVoiceModule::GetVoiceName(DWORD p_Id, string &p_Name)
{
  assert(m_Voice);
  return(m_Voice->GetVoiceName(p_Id, p_Name));
}


void SpeechVoiceModule::SetVoice(DWORD p_Id)
{
  assert(m_Voice);
  m_Voice->SetVoice(p_Id);
}


void SpeechVoiceModule::SetVoice(const char *p_Name)
{
  assert(m_Voice);
  m_Voice->SetVoice(p_Name);
}


void SpeechVoiceModule::Speak(const char *p_Sentence, bool p_Async)
{
  assert(m_Voice);
  m_Voice->Speak(p_Sentence, p_Async);
}



#endif //ifdef USE_SAPISPEECH