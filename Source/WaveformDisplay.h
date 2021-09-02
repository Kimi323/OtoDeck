/*
  ==============================================================================

    WaveformDisplay.h
    Created: 30 Aug 2021 8:52:27pm
    Author:  Kimi

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

//==============================================================================
/*
*/
class WaveformDisplay  : public Component,
                         public ChangeListener
{
public:
    WaveformDisplay(AudioFormatManager& formatManagerToUse,
                    AudioThumbnailCache& cacheToUse);
    ~WaveformDisplay() override;

    void paint (juce::Graphics&) override;
    void resized() override;
    
    void loadURL(URL audioURL);
    void changeListenerCallback (ChangeBroadcaster *source) override;
    /**set the relative position of the playhead */
    void setPositionRelative(double pos);
    
private:
    AudioThumbnail audioThumb;
    bool fileLoaded;
    double position;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (WaveformDisplay)
};
