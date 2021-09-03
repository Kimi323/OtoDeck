/*
  ==============================================================================

    WaveformDisplay.cpp
    Created: 30 Aug 2021 8:52:27pm
    Author:  Kimi

  ==============================================================================
*/

#include <JuceHeader.h>
#include "WaveformDisplay.h"

//==============================================================================
WaveformDisplay::WaveformDisplay(AudioFormatManager& formatManagerToUse,
                                 AudioThumbnailCache& cacheToUse) :
                                 audioThumb(1000, formatManagerToUse, cacheToUse),
                                 fileLoaded(false),
                                 position(0)
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.
    audioThumb.addChangeListener(this);
    //audioplayhead->getCurrentPosition(currentPositionInfo);
}

WaveformDisplay::~WaveformDisplay()
{
}

void WaveformDisplay::paint (juce::Graphics& g)
{
    /* This demo code just fills the component's background and
       draws some placeholder text to get you started.

       You should replace everything in this method with your own
       drawing code..
    */

    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));   // clear the background

    g.setColour (juce::Colours::grey);
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component

    g.setColour (juce::Colours::orange);
    if (fileLoaded) {
        audioThumb.drawChannel(g,
                               getLocalBounds(),
                               0,
                               audioThumb.getTotalLength(),
                               0,
                               1.0f);
        // draw a rectangle for the playhead
        g.setColour(Colours::limegreen);
        g.drawRect(position * getWidth(), 0, getWidth() / 20, getHeight());
        g.drawText (std::to_string(currentPositionInfo.bpm), getLocalBounds(),
                    juce::Justification::centred, true);
    } else {
        // draw some placeholder text
        g.setFont (20.0f);
        g.drawText ("File not loaded...", getLocalBounds(),
                    juce::Justification::centred, true);
    }
}

void WaveformDisplay::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..

}

void WaveformDisplay::loadURL(URL audioURL) {
    std::cout << "waveformDisplay loadURL is called" << std::endl;
    audioThumb.clear();
    fileLoaded = audioThumb.setSource(new URLInputSource(audioURL));
}

void WaveformDisplay::changeListenerCallback (ChangeBroadcaster *source) {
    std::cout << "change recieved" << std::endl;
    repaint();
}

void WaveformDisplay::setPositionRelative(double pos) {
    if (pos != position) {
        position = pos;
        repaint();
    }
}

