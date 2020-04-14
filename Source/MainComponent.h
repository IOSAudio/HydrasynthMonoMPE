/*
 ==============================================================================
 
 This file was auto-generated!
 
 ==============================================================================
 */

#pragma once

#include <JuceHeader.h>
#include <list>

class MPEElement
{
public:
  void DebugDump(void)
  {
    printf("[%2u] N=%u, V=%u, X=%u, Y=%u, Z=%u\n", m_uChannel, m_uNote, m_uVelocity, m_uX, m_uY, m_uZ);
  }
  
  void SetChannel(uint8_t uChannel)
  {
    m_uChannel = uChannel;
  }
  
  void NoteOff(void)
  {
    m_bInUse = false;
  }
  
  void NoteOn(uint8_t uNote, uint8_t uVelocity)
  {
    m_bInUse = true;
    m_uNote = uNote;
    m_uVelocity = uVelocity;
  }

  void SetX(uint16_t uX)
  {
    m_uX = uX;
  }

  void SetY(uint8_t uY)
  {
    m_uY = uY;
  }

  void SetZ(uint8_t uZ)
  {
    m_uZ = uZ;
  }

  bool IsForChannel(uint8_t uChannel)
  {
    return m_uChannel == uChannel;
  }
  
  bool IsInUse(void)
  {
    return m_bInUse;
  }

  bool IsNote(uint8_t uNote)
  {
    return m_uNote == uNote;
  }

  uint8_t GetNote(void)
  {
    return m_uNote;
  }

  uint8_t GetChannel(void)
  {
    return m_uChannel;
  }

  uint8_t GetVelocity(void)
  {
    return m_uVelocity;
  }
  
  uint16_t GetX(void)
  {
    return m_uX;
  }

  uint8_t GetY(void)
  {
    return m_uY;
  }

  uint8_t GetZ(void)
  {
    return m_uZ;
  }

private:
  uint8_t   m_uChannel = 0;
  uint8_t   m_uNote = 0;
  uint8_t   m_uVelocity = 0;
  uint16_t  m_uX = 0;
  uint8_t   m_uY = 0;
  uint8_t   m_uZ = 0;
  bool      m_bInUse = false;

};

//==============================================================================
/*
 This component lives inside our window, and this is where you should put all
 your controls and content.
 */
class MainComponent   : public Component, private MidiInputCallback
{
public:
  //==============================================================================
  MainComponent() ;
  ~MainComponent() override;
  
  
  //==============================================================================
  void paint (Graphics& g) override;
  void resized() override;
  
  
  //==============================================================================
  void handleIncomingMidiMessage (MidiInput* source, const MidiMessage& message) override;

  
private:
  
  typedef std::pair<uint8_t, uint8_t>                       NoteChannelPair;
  typedef std::map<NoteChannelPair, MPEElement *>           NoteChannelMap;
  typedef std::map<NoteChannelPair, MPEElement *>::iterator NoteChannelMapIter;
  typedef std::list<NoteChannelPair>                        NoteChannelList;
  typedef std::list<NoteChannelPair>::iterator              NoteChannelListIter;

  AudioDeviceManager    m_deviceManager;
  ApplicationProperties m_appProperties;

  ComboBox m_midiInputList;
  ComboBox m_midiOutputList;
  
  Label m_midiInputListLabel;
  Label m_midiOutputListLabel;
  Label m_midiModeLabel;

  ToggleButton m_modeNormal {"Normal"};
  ToggleButton m_modeMono {"Mono"};
  ToggleButton m_modeLow {"Low"};
  ToggleButton m_modeHigh {"High"};
  ToggleButton m_modeLast {"Last"};

  
  int m_nLastInputIndex = 0;
  int m_nLastOutputIndex = 0;



  void setMidiInput (int index);
  void setMidiOutput (int index);
  
  void updateToggleState (Button* button, String name);
  
  enum {mNormal, mMono, mLast, mLow, mHigh} m_mode = mMono;
  
  std::unique_ptr<MidiOutput>  m_pMidiOutput = nullptr;

  MPEElement      m_Channels[16];
  NoteChannelMap  m_Notes;
  NoteChannelList m_NotesTimeOrdered;
  
  MPEElement      *m_pLastPlayedElement = nullptr;
  
  
  void NoteOn(uint8_t uChannel, uint8_t uNote, uint8_t uVelocity);
  void NoteOff(uint8_t uChannel, uint8_t uNote);
  
  bool HandleX(uint8_t uChannel, uint16_t uX);
  bool HandleY(uint8_t uChannel, uint16_t uY);
  bool HandleZ(uint8_t uChannel, uint16_t uZ);

  MPEElement *GetHighestElement(void);
  MPEElement *GetLowestElement(void);
  MPEElement *GetLastElement(void);
  MPEElement *GetPreviousElement(void);

  bool ChannelHasPlayingNote(uint8_t uChannel);

  void DebugDump(void);

  bool SetInputDevice(Array<MidiDeviceInfo> &midiInputs, String sInputDevice);
  bool SetOutputDevice(Array<MidiDeviceInfo> &midiOutputs, String sOutputDevice);

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
