/*
 ==============================================================================
 
 This file was auto-generated!
 
 ==============================================================================
 */

#include "MainComponent.h"

#define DEBUG_DUMP 0

//==============================================================================
MainComponent::MainComponent()
{
  PropertiesFile::Options options;
  options.applicationName = ProjectInfo::projectName;
  options.folderName = ProjectInfo::projectName;
  options.filenameSuffix = "settings";
  options.osxLibrarySubFolder = "Application Support";
  m_appProperties.setStorageParameters(options);
  
  setOpaque (true);

  addAndMakeVisible (m_midiInputListLabel);
  m_midiInputListLabel.setText ("MIDI Input  :", dontSendNotification);
  m_midiInputListLabel.attachToComponent (&m_midiInputList, true);

  addAndMakeVisible (m_midiInputList);
  m_midiInputList.setTextWhenNoChoicesAvailable ("No MIDI Inputs Enabled");
  auto midiInputs = MidiInput::getAvailableDevices();

  StringArray midiInputNames;
  for (auto input : midiInputs)
      midiInputNames.add (input.name);

  m_midiInputList.addItemList (midiInputNames, 1);
  m_midiInputList.onChange = [this] { setMidiInput (m_midiInputList.getSelectedItemIndex()); };

  String sInputDevice = m_appProperties.getUserSettings()->getValue("inputDevice");
  
  bool bInputSet = false;
  
  if(sInputDevice.length())
    bInputSet = SetInputDevice(midiInputs, sInputDevice);
  
  // if not found then look for Linnstrument
  if(!bInputSet)
    bInputSet = SetInputDevice(midiInputs, "Linnstrument");

  // if not found then look for Seaboard
  if(!bInputSet)
    bInputSet = SetInputDevice(midiInputs, "Linnstrument");

  // if no enabled devices were found just use the first one in the list
  if (!bInputSet)
      setMidiInput (0);
  

  addAndMakeVisible (m_midiOutputListLabel);
  m_midiOutputListLabel.setText ("MIDI Output :", dontSendNotification);
  m_midiOutputListLabel.attachToComponent (&m_midiOutputList, true);

  addAndMakeVisible (m_midiOutputList);
  m_midiOutputList.setTextWhenNoChoicesAvailable ("No MIDI Outputs Enabled");
  auto midiOutputs = MidiOutput::getAvailableDevices();

  StringArray midiOutputNames;
  for (auto output : midiOutputs)
      midiOutputNames.add (output.name);

  m_midiOutputList.addItemList (midiOutputNames, 1);
  m_midiOutputList.onChange = [this] { setMidiOutput (m_midiOutputList.getSelectedItemIndex()); };

  String sOutputDevice = m_appProperties.getUserSettings()->getValue("outputDevice");
  bool bOutputSet = false;

  if(sOutputDevice.length())
    bOutputSet = SetOutputDevice(midiOutputs, sOutputDevice);

  if(!bOutputSet)
    bOutputSet = SetOutputDevice(midiOutputs, "Hydrasynth");

  // if no enabled devices were found just use the first one in the list
  if (!bOutputSet)
      setMidiInput (0);

  
  addAndMakeVisible (m_midiModeLabel);
  m_midiModeLabel.setText ("MIDI Mode   :", dontSendNotification);
  m_midiModeLabel.attachToComponent (&m_modeNormal, true);

  
  addAndMakeVisible (m_modeNormal);
  addAndMakeVisible (m_modeMono);
  addAndMakeVisible (m_modeLow);
  addAndMakeVisible (m_modeHigh);
  addAndMakeVisible (m_modeLast);

  m_modeNormal.onClick = [this] { updateToggleState (&m_modeNormal,   "Normal"); };
  m_modeMono.onClick = [this] { updateToggleState (&m_modeNormal,   "Mono"); };
  m_modeLow.onClick = [this] { updateToggleState (&m_modeNormal,   "Low"); };
  m_modeHigh.onClick = [this] { updateToggleState (&m_modeNormal,   "High"); };
  m_modeLast.onClick = [this] { updateToggleState (&m_modeNormal,   "Last"); };

  m_modeNormal.setRadioGroupId (1001);
  m_modeMono.setRadioGroupId (1001);
  m_modeLow.setRadioGroupId (1001);
  m_modeHigh.setRadioGroupId (1001);
  m_modeLast.setRadioGroupId (1001);

  String sMode = m_appProperties.getUserSettings()->getValue("mode");
  if(sMode.length())
  {
    if(sMode == "Normal")
      m_modeNormal.setToggleState(true, true);
    else if (sMode == "Mono")
      m_modeMono.setToggleState(true, true);
    else if (sMode == "Low")
      m_modeLow.setToggleState(true, true);
    else if (sMode == "High")
      m_modeHigh.setToggleState(true, true);
    else if (sMode == "Last")
      m_modeLast.setToggleState(true, true);
  }
  else
    m_modeLast.setToggleState(true, true);
  
  for(uint8_t uChannel = 0; uChannel < 16; uChannel++)
    m_Channels[uChannel].SetChannel(uChannel);

  setSize (520, 120);

}

MainComponent::~MainComponent()
{
  m_deviceManager.removeMidiInputDeviceCallback (MidiInput::getAvailableDevices()[m_midiInputList.getSelectedItemIndex()].identifier, this);
  m_pMidiOutput.reset();

}

bool MainComponent::SetInputDevice(Array<MidiDeviceInfo> &midiInputs, String sInputDevice)
{
  bool bResult = false;
  for (auto input : midiInputs)
  {
    if(input.name.startsWithIgnoreCase(sInputDevice))
    {
      setMidiInput (midiInputs.indexOf (input));
      bResult = true;
      break;
    }
  }
  return bResult;
}

bool MainComponent::SetOutputDevice(Array<MidiDeviceInfo> &midiOutputs, String sOutputDevice)
{
  bool bResult = false;
  for (auto input : midiOutputs)
  {
    if(input.name.startsWithIgnoreCase(sOutputDevice))
    {
      setMidiOutput(midiOutputs.indexOf (input));
      bResult = true;
      break;
    }
  }
  return bResult;
}

//==============================================================================
void MainComponent::paint (Graphics& g)
{
  // (Our component is opaque, so we must completely fill the background with a solid colour)
  g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));
  
  // You can add your drawing code here!
}

void MainComponent::resized()
{
  auto area = getLocalBounds();

  m_midiInputList.setBounds (area.removeFromTop (36).removeFromRight (getWidth() - 100).reduced (8));
  m_midiOutputList.setBounds (area.removeFromTop (36).removeFromRight (getWidth() - 100).reduced (8));
  
  auto r = m_midiOutputList.getBounds();
  r.setTop(r.getTopLeft().getY()+36);
  int nButtonWidth = r.getWidth()/5;

  m_modeNormal.setBounds(r.getX(), r.getY(), nButtonWidth, 20);
  m_modeMono.setBounds(r.getX()+nButtonWidth, r.getY(), nButtonWidth, 20);
  m_modeLast.setBounds(r.getX()+nButtonWidth*2, r.getY(), nButtonWidth, 20);
  m_modeLow.setBounds(r.getX()+nButtonWidth*3, r.getY(), nButtonWidth, 20);
  m_modeHigh.setBounds(r.getX()+nButtonWidth*4, r.getY(), nButtonWidth, 20);
}

void MainComponent::updateToggleState(Button* button, String name)
{
  if(m_modeNormal.getToggleState())
    m_mode = mNormal;
  else if(m_modeMono.getToggleState())
    m_mode = mMono;
  else if(m_modeLast.getToggleState())
    m_mode = mLast;
  else if(m_modeLow.getToggleState())
    m_mode = mLow;
  else if(m_modeHigh.getToggleState())
    m_mode = mHigh;
  
  m_appProperties.getUserSettings()->setValue("mode", name);

}

//==============================================================================
void MainComponent::setMidiInput (int index)
{
  auto list = MidiInput::getAvailableDevices();
  
  m_deviceManager.removeMidiInputDeviceCallback(list[m_nLastInputIndex].identifier, this);
  
  auto newInput = list[index];
  
  if (! m_deviceManager.isMidiInputDeviceEnabled (newInput.identifier))
    m_deviceManager.setMidiInputDeviceEnabled (newInput.identifier, true);
  
  m_deviceManager.addMidiInputDeviceCallback (newInput.identifier, this);
  m_midiInputList.setSelectedId (index + 1, dontSendNotification);
  
  m_nLastInputIndex = index;
  
  m_appProperties.getUserSettings()->setValue("inputDevice", newInput.name);
}

void MainComponent::setMidiOutput (int index)
{
  auto list = MidiOutput::getAvailableDevices();
    
  auto newOutput = list[index];
 
  m_midiOutputList.setSelectedId (index + 1, dontSendNotification);
  
  m_nLastInputIndex = index;
  
  m_pMidiOutput.reset();
  
  m_pMidiOutput = MidiOutput::openDevice(newOutput.identifier);

  m_appProperties.getUserSettings()->setValue("outputDevice", newOutput.name);

}

void MainComponent::handleIncomingMidiMessage (MidiInput* source, const MidiMessage& message)
{
  // for mono mode
  // sender only using 8 channels
  // 1. last note only sends CC74, chan pressure and PB
  // 2. Other notes store CC74, chan pressure, velocity and PB
  // 3. by default highest note is replayed after release of playing note like mono high
  // 4. if replayed note is the same we need to retrigger with old velocity (same note bug)
  // 5. resend CC74, chan pressure and PB of replayed note
  
  if(m_mode != mNormal)
  {
    if(message.isNoteOn())
    {
      NoteOn(message.getChannel(), message.getNoteNumber(), message.getVelocity());
      m_pMidiOutput->sendMessageNow(message);
    }
    else
    {
      if(message.isNoteOff())
      {
        MidiBuffer buffer;

        uint8_t uNote = message.getNoteNumber();
        bool bRemovingLastOrderedNode = false;
        if(!m_NotesTimeOrdered.empty())
        {
          NoteChannelPair nc = m_NotesTimeOrdered.back();
          bRemovingLastOrderedNode = (nc.first == uNote) && (nc.second == message.getChannel());
        }
        
        NoteOff(message.getChannel(), uNote);
        
        // have we a previous note
        MPEElement *pElement = GetPreviousElement();
        if(pElement)
        {
          // resend previous X, Y, Z
          uint8_t uChannel = pElement->GetChannel();
          buffer.addEvent(MidiMessage::pitchWheel(uChannel, pElement->GetX()), 0);
          buffer.addEvent(MidiMessage::controllerEvent(uChannel, 74, pElement->GetY()), 0);
          buffer.addEvent(MidiMessage::channelPressureChange(uChannel, pElement->GetZ()), 0);

          // do we need to retrigger note to fix HS bug
          if(pElement->IsNote(uNote))
          {
            buffer.addEvent(MidiMessage::noteOn(pElement->GetChannel(), pElement->GetNote(), pElement->GetVelocity()), 0);
          }
          else
          {
            // do we need to retrigger note for proper Mono/Last
            if(m_mode == mLast && bRemovingLastOrderedNode)
              buffer.addEvent(MidiMessage::noteOn(pElement->GetChannel(), pElement->GetNote(), pElement->GetVelocity()), 0);
            
            buffer.addEvent(message, 0); // original note off
          }
        }
        else
          buffer.addEvent(message, 0); // original note off

        // send the midi
        m_pMidiOutput->sendBlockOfMessagesNow(buffer);
      }
      else
      {
        if(message.isPitchWheel())
        {
          if(HandleX(message.getChannel(), message.getPitchWheelValue()))
            m_pMidiOutput->sendMessageNow(message);
        }
        else
        {
          if(message.isControllerOfType(74))
          {
            if(HandleY(message.getChannel(), message.getControllerValue()))
              m_pMidiOutput->sendMessageNow(message);
          }
          else
          {
            if(message.isChannelPressure())
            {
              if(HandleZ(message.getChannel(), message.getChannelPressureValue()))
                m_pMidiOutput->sendMessageNow(message);
            }
          }
        }
      }
    }
  }
  else
    m_pMidiOutput->sendMessageNow(message);
}


void MainComponent::NoteOn(uint8_t uChannel, uint8_t uNote, uint8_t uVelocity)
{
  m_Channels[uChannel].NoteOn(uNote, uVelocity);
  m_Notes[std::make_pair(uNote, uChannel)] = &m_Channels[uChannel];
  m_NotesTimeOrdered.push_back(std::make_pair(uNote, uChannel));
  m_pLastPlayedElement = &m_Channels[uChannel];
#if DEBUG_DUMP
  DebugDump();
#endif
}

void MainComponent::NoteOff(uint8_t uChannel, uint8_t uNote)
{
  m_Channels[uChannel].NoteOff();
  auto i = m_Notes.find(std::make_pair(uNote, uChannel));
  if (i != m_Notes.end())
  {
	  if (i->second == m_pLastPlayedElement)
		  m_pLastPlayedElement = nullptr;

		m_Notes.erase(i);
	}
  
  auto o = std::find(m_NotesTimeOrdered.begin(), m_NotesTimeOrdered.end(), std::make_pair(uNote, uChannel));
  if(o != m_NotesTimeOrdered.end())
    m_NotesTimeOrdered.erase(o);

#if DEBUG_DUMP
  DebugDump();
#endif
}

MPEElement *MainComponent::GetHighestElement(void)
{
  MPEElement *pElement = nullptr;
  if(m_Notes.rbegin() != m_Notes.rend())
    pElement = m_Notes.rbegin()->second;

  return pElement;
}

MPEElement *MainComponent::GetLowestElement(void)
{
  MPEElement *pElement = nullptr;
  if(m_Notes.begin() != m_Notes.end())
    pElement = m_Notes.begin()->second;

  return pElement;
}


MPEElement *MainComponent::GetLastElement(void)
{
  MPEElement *pElement = nullptr;

  if(!m_NotesTimeOrdered.empty())
  {
    auto o = m_NotesTimeOrdered.back();
    
    auto i = m_Notes.find(o);
    if(i != m_Notes.end())
      pElement = i->second;
  }
  
  return pElement;
}

MPEElement *MainComponent::GetPreviousElement(void)
{
  MPEElement *pElement = nullptr;
  switch(m_mode)
  {
    case mMono:
    case mHigh:
      pElement = GetHighestElement();
      break;
      
    case mLast:
      pElement = GetLastElement();
      break;

    case mLow:
      pElement = GetLowestElement();
      break;
      
    default:
      break;
  }
  
  return pElement;
}


bool MainComponent::ChannelHasPlayingNote(uint8_t uChannel)
{
  bool bResult = false;
  
  // if channel is not in use rturn true
  // for mLow true if channel is lowest note
  // for mHigh true if channel is Highest note
  // for mLast true if channel is last note
  // for mMono true if channel is lastplayed note or Highest note

  
  if(m_Channels[uChannel].IsInUse())
  {
    if(m_mode == mMono && m_pLastPlayedElement)
    {
      bResult = m_pLastPlayedElement->GetChannel() == uChannel;
    }
    else
    {
      MPEElement *pElement = GetPreviousElement();
      if(pElement)
        bResult = pElement->IsForChannel(uChannel);
    }
  }
  else
    bResult = true;
  
#if DEBUG_DUMP
  DebugDump();
#endif

  return bResult;
}

bool MainComponent::HandleX(uint8_t uChannel, uint16_t uX)
{
  m_Channels[uChannel].SetX(uX);
  return ChannelHasPlayingNote(uChannel);
}

bool MainComponent::HandleY(uint8_t uChannel, uint16_t uY)
{
  m_Channels[uChannel].SetY(uY);
  return ChannelHasPlayingNote(uChannel);
}

bool MainComponent::HandleZ(uint8_t uChannel, uint16_t uZ)
{
  m_Channels[uChannel].SetZ(uZ);
  return ChannelHasPlayingNote(uChannel);
}

void MainComponent::DebugDump(void)
{
  printf("\n");
  if(m_pLastPlayedElement)
    printf("m_pLastPlayedElement = [%u, %u]\n", m_pLastPlayedElement->GetNote(), m_pLastPlayedElement->GetChannel());
  
  for(auto i : m_Notes)
  {
    i.second->DebugDump();
  }
  
  for(auto o : m_NotesTimeOrdered)
  {
    printf("[%u, %u], ", o.first, o.second);
  }
  
  printf("\n");
}

