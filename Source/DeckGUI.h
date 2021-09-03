#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "DJAudioPlayer.h"
#include "WaveformDisplay.h"

//==============================================================================
/*
*/
class DeckGUI    : public Component,
                   public Button::Listener, 
                   public Slider::Listener, 
                   public FileDragAndDropTarget,
                   public Timer
{
public:
    DeckGUI(DJAudioPlayer* player,
            AudioFormatManager& formatManagerToUse,
            AudioThumbnailCache& cacheToUse);
    ~DeckGUI();

    void paint (Graphics&) override;
    void resized() override;

     /** implement Button::Listener */
    void buttonClicked (Button *) override;

    /** implement Slider::Listener */
    void sliderValueChanged (Slider *slider) override;

    bool isInterestedInFileDrag (const StringArray &files) override;
    void filesDropped (const StringArray &files, int x, int y) override; 

    void timerCallback() override;

private:

    //TextButton playButton{"PLAY/STOP"};
    ImageButton playButton;
    TextButton loopButton{"LOOP"};
    TextButton loadButton{"LOAD"};
    TextButton prevButton{"PREV"};
    TextButton nextButton{"NEXT"};
  
    Slider volDial;
    Label volLabel;
    
    Slider speedSlider;
    Label speedLabel;
    
    Slider posSlider;

    DJAudioPlayer* player;
    WaveformDisplay waveformDisplay;
    
    bool isPlaying;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DeckGUI)
};
