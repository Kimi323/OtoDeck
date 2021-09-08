#include "../JuceLibraryCode/JuceHeader.h"
#include "DeckGUI.h"

//==============================================================================
DeckGUI::DeckGUI(DJAudioPlayer* _player,
                 AudioFormatManager& formatManagerToUse,
                 AudioThumbnailCache& cacheToUse
                 ) : player(_player),
                     waveformDisplay(formatManagerToUse, cacheToUse),
                     currentIndex(0),
                     isLooping(false)
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
    speedSlider.setRange(0.25, 5.0);
    speedSlider.setValue(1.00);
    
    //double maxPos = player->getTrackLengthInSeconds();
    posSlider.setRange(0.0, 1.0);
    posSlider.setValue(0.0);
    
    startTimer(50);

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
    
    waveformDisplay.setBounds(0, paddingY, getWidth(), rowHeight);
    
    // posSlider without textbox
    posSlider.setBounds(0, paddingY + rowHeight, getWidth(), rowHeight / 2);
    posSlider.setTextBoxStyle(juce::Slider::NoTextBox, 0, 0, 0);
    
    // draw vertical slider for speed control
    speedSlider.setSliderStyle(Slider::SliderStyle::LinearVertical);
    speedSlider.setBounds(rowWidth, rowHeight * 1.5, rowWidth, rowHeight * 3);
    speedSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, rowWidth, rowHeight / 4);
    speedSlider.setNumDecimalPlacesToDisplay(2);
    speedSlider.setTextValueSuffix ("x");
    
    // volDial
    volDial.setSliderStyle (juce::Slider::Rotary);
    volDial.setBounds(rowWidth * 3, rowHeight * 1.5, rowWidth * 3, rowHeight * 3);
    volDial.setTextBoxStyle(juce::Slider::TextBoxBelow, true, rowWidth, rowHeight / 4);
    volDial.setNumDecimalPlacesToDisplay(2);
    
    // playButton
    playButton.setBounds(paddingX, rowHeight * 5 - paddingY, rowWidth, rowHeight);
    // load image in memory for button to use
    Image playButtonImg = ImageCache::getFromMemory (BinaryData::start_stop_button_png, BinaryData::start_stop_button_pngSize);
    playButton.setImages(false, true, false, playButtonImg, 1.0f, {}, {}, 1.0f, {}, {}, 1.0f, {});
    
    loopButton.setBounds(rowWidth * 1 + paddingX * 3, rowHeight * 5 - paddingY, rowWidth, rowHeight);
    loadButton.setBounds(rowWidth * 2 + paddingX * 5, rowHeight * 5 - paddingY, rowWidth, rowHeight);
    prevButton.setBounds(rowWidth * 3 + paddingX * 7, rowHeight * 5 - paddingY, rowWidth, rowHeight);
    nextButton.setBounds(rowWidth * 4 + paddingX * 9, rowHeight * 5 - paddingY, rowWidth, rowHeight);
    

    

}

void DeckGUI::buttonClicked(Button* button)
{
    if (button == &playButton)
    {
        std::cout << "Play button was clicked " << std::endl;
        if (player->isPlaying()) {
            player->stop();
        } else {
            player->start();
        }
    }
    if (button == &loopButton)
    {
        if (!isLooping) {
            isLooping = true;
        } else {
            isLooping = false;
        }
        std::cout << "isLooping is set to" << isLooping << std::endl;
    }
    if (button == &loadButton)
    {
        FileChooser chooser{"Select a file..."};
        if (chooser.browseForFileToOpen())
        {
            // get track name and add to playlist
            std::string trackName = chooser.getResult().getFileNameWithoutExtension().toStdString();
            playlistComponent.addToPlaylist(trackName);
            
            // add current url into urlList
            urlList.push_back(URL{chooser.getResult()});
            currentIndex = urlList.size() - 1;
            // TODO:player is pointer, why only player is pointer?
            // load url of current selected track
            player->loadURL(URL{chooser.getResult()});
            // waveformDisplay is not a pointer
            waveformDisplay.loadURL(URL{chooser.getResult()});
        }
    }
    // if next button is clicked and at least one track loaded
    if (button == &nextButton && !urlList.empty())
    {
        // update current index when not the last track
        if (currentIndex < urlList.size() - 1)
        {
            currentIndex = currentIndex + 1;
        }
        // load url of next track
        player->loadURL(urlList.at(currentIndex));
        waveformDisplay.loadURL(urlList.at(currentIndex));
        player->start();
        std::cout << "nextButton clicked" << std::endl;
    }
    // if prev button is clicked and at least one track loaded
    if (button == &prevButton && !urlList.empty())
    {
        // update current index when not the last track
        if (currentIndex > 0)
        {
            currentIndex = currentIndex - 1;
        }
        // load url of next track
        player->loadURL(urlList.at(currentIndex));
        waveformDisplay.loadURL(urlList.at(currentIndex));
        player->start();
        std::cout << "prevButton clicked" << std::endl;
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
    double currentPos = player->getPositionRelative();
    waveformDisplay.setPositionRelative(currentPos);
    
    // before track is loaded, set posSlider value to 0.0
    if (isnan(currentPos)) {
        posSlider.setValue(0.0);
    } else {
        // update polSlider position
        posSlider.setValue(currentPos);
    }
    
    if (player->isEndOfTrack() && !urlList.empty() && isLooping) {
        player->loadURL(urlList.at(currentIndex));
        player->start();
    }
    
}
