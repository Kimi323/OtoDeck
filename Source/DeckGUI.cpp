#include "../JuceLibraryCode/JuceHeader.h"
#include "DeckGUI.h"

//==============================================================================
DeckGUI::DeckGUI(DJAudioPlayer* _player,
                 AudioFormatManager& formatManagerToUse,
                 AudioThumbnailCache& cacheToUse
                 ) : player(_player),
                     waveformDisplay(formatManagerToUse, cacheToUse),
                    isPlaying(false)
{

    addAndMakeVisible(playButton);
    addAndMakeVisible(loopButton);
    addAndMakeVisible(loadButton);
    addAndMakeVisible(prevButton);
    addAndMakeVisible(nextButton);
       
    addAndMakeVisible(volDial);
    volLabel.setText ("Volume", juce::dontSendNotification);
    volLabel.attachToComponent (&volDial, true);
    
    addAndMakeVisible(speedSlider);
    speedLabel.setText ("Speed", juce::dontSendNotification);
    speedLabel.attachToComponent (&speedSlider, true);
    
    addAndMakeVisible(posSlider);
    
    addAndMakeVisible(waveformDisplay);


    playButton.addListener(this);
    loopButton.addListener(this);
    loadButton.addListener(this);
    prevButton.addListener(this);
    nextButton.addListener(this);

    volDial.addListener(this);
    speedSlider.addListener(this);
    posSlider.addListener(this);


    volDial.setRange(0.0, 2.0);
    volDial.setValue(1.0);
    speedSlider.setRange(0.0, 100.0);
    posSlider.setRange(0.0, 1.0);
    
    startTimer(500);

}

DeckGUI::~DeckGUI()
{
    stopTimer();
}

void DeckGUI::paint (Graphics& g)
{
    /* This demo code just fills the component's background and
       draws some placeholder text to get you started.

       You should replace everything in this method with your own
       drawing code..
    */

    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));   // clear the background

    g.setColour (Colours::green);
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component

    g.setColour (Colours::white);
    g.setFont (14.0f);
    //g.drawText ("DeckGUI", getLocalBounds(), Justification::centred, true);   // draw some placeholder text
    
    // draw line
    //g.setColour (juce::Colours::green);
    //g.drawLine (15, 50, 15, getHeight() / 4 * 3, 5);
}

void DeckGUI::resized()
{
    double rowWidth = getWidth() / 6;
    double rowHeight = getHeight() / 6;
    double paddingX = rowWidth * 0.1;
    double paddingY = rowHeight * 0.1;
    
    // draw vertical slider for speed control
    speedSlider.setSliderStyle(Slider::SliderStyle::LinearVertical);
    speedSlider.setBounds(rowWidth, paddingY, rowWidth, rowHeight * 3);
    speedSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, rowWidth / 2, rowHeight / 4);
    speedSlider.setNumDecimalPlacesToDisplay(0);
    speedSlider.setTextValueSuffix ("%");
    
    // volDial
    volDial.setSliderStyle (juce::Slider::Rotary);
    volDial.setBounds(rowWidth * 3, paddingY, rowWidth * 3, rowHeight * 3);
    volDial.setTextBoxStyle(juce::Slider::TextBoxBelow, true, rowWidth / 2, rowHeight / 4);
    volDial.setNumDecimalPlacesToDisplay(2);
    
    // playButton
    playButton.setBounds(paddingX, rowHeight * 3.5, rowWidth, rowHeight);
    // load image in memory for button to use
    Image playButtonImg = ImageCache::getFromMemory (BinaryData::start_stop_button_png, BinaryData::start_stop_button_pngSize);
    playButton.setImages(false, true, false, playButtonImg, 1.0f, {}, {}, 1.0f, {}, {}, 1.0f, {});
    
    loopButton.setBounds(rowWidth * 1 + paddingX * 3, rowHeight * 3.5, rowWidth, rowHeight);
    loadButton.setBounds(rowWidth * 2 + paddingX * 5, rowHeight * 3.5, rowWidth, rowHeight);
    prevButton.setBounds(rowWidth * 3 + paddingX * 7, rowHeight * 3.5, rowWidth, rowHeight);
    nextButton.setBounds(rowWidth * 4 + paddingX * 9, rowHeight * 3.5, rowWidth, rowHeight);
    
    waveformDisplay.setBounds(0, rowHeight * 4.5, getWidth(), rowHeight);
    
    // posSlider without textbox
    posSlider.setBounds(0, rowHeight * 5.5, getWidth(), rowHeight / 2);
    posSlider.setTextBoxStyle(juce::Slider::NoTextBox, 0, 0, 0);
    

}

void DeckGUI::buttonClicked(Button* button)
{
    if (button == &playButton)
    {
        std::cout << "Play button was clicked " << std::endl;
        if (!isPlaying) {
            player->start();
            isPlaying = true;
        } else {
            player->stop();
            isPlaying = false;
        }
    }
     if (button == &loopButton)
    {
        std::cout << "loop button was clicked " << std::endl;
        // TODO: loop the current song

    }
    if (button == &loadButton)
    {
        FileChooser chooser{"Select a file..."};
        if (chooser.browseForFileToOpen())
        {
            // player is pointer, why only player is pointer?
            player->loadURL(URL{chooser.getResult()});
            // waveformDisplay is not a pointer
            waveformDisplay.loadURL(URL{chooser.getResult()});
        }
    }
}

void DeckGUI::sliderValueChanged (Slider *slider)
{
    if (slider == &volDial)
    {
        player->setGain(slider->getValue());
    }

    if (slider == &speedSlider)
    {
        player->setSpeed(slider->getValue());
    }
    
    if (slider == &posSlider)
    {
        player->setPositionRelative(slider->getValue());
    }
    
}

bool DeckGUI::isInterestedInFileDrag (const StringArray &files)
{
  std::cout << "DeckGUI::isInterestedInFileDrag" << std::endl;
  return true; 
}

void DeckGUI::filesDropped (const StringArray &files, int x, int y)
{
  std::cout << "DeckGUI::filesDropped" << std::endl;
  if (files.size() == 1)
  {
    player->loadURL(URL{File{files[0]}});
  }
}
    
void DeckGUI::timerCallback() {
    waveformDisplay.setPositionRelative(player->getPositionRelative());
}
