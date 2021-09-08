/*
  ==============================================================================

    PlaylistComponent.cpp
    Created: 1 Sep 2021 4:26:52pm
    Author:  Kimi

  ==============================================================================
*/

#include <JuceHeader.h>
#include "PlaylistComponent.h"
#include <algorithm>

//==============================================================================
std::vector<std::string> trackTitles;
std::vector<std::string> trackTitles_bk;

PlaylistComponent::PlaylistComponent()
{
    // TODO: why getWidth() cannot be used?
    tableComponent.getHeader().addColumn("Track title", 1, 200);
    tableComponent.getHeader().addColumn("Length", 2, 200);
    tableComponent.getHeader().addColumn("bpm", 3, 200);
    tableComponent.getHeader().addColumn("", 4, 200);
    tableComponent.setModel(this);
    addAndMakeVisible(tableComponent);
    
    addAndMakeVisible(inputArea);
    addAndMakeVisible(searchButton);
    searchButton.addListener(this);
    addAndMakeVisible(clearButton);
    clearButton.addListener(this);
    //searchButton.setText ("Search for track", juce::dontSendNotification);
    //searchButton.attachToComponent (&inputArea, true);
    //trackTitles.push_back("test");
}

PlaylistComponent::~PlaylistComponent()
{
}

void PlaylistComponent::paint (juce::Graphics& g)
{
    /* This demo code just fills the component's background and
       draws some placeholder text to get you started.

       You should replace everything in this method with your own
       drawing code..
    */
    // clear the background
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    g.setColour (juce::Colours::orangered);
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component

    g.setColour (juce::Colours::white);
    g.setFont (14.0f);
//    g.drawText ("PlaylistComponent", getLocalBounds(),juce::Justification::centred, true);   // draw some placeholder text
}

void PlaylistComponent::resized()
{
    tableComponent.setBoundsInset (BorderSize<int> (8));
    tableComponent.setBounds(0, getHeight() / 8, getWidth(), getHeight());
    searchButton.setBounds(0, 0, getWidth() / 4, getHeight() / 8);
    clearButton.setBounds(getWidth() / 4, 0, getWidth() / 4, getHeight() / 8);
    inputArea.setBounds(getWidth() / 4 * 3, 0, getWidth() / 4, getHeight() / 8);
}

int PlaylistComponent::getNumRows() {
    std::cout << std::addressof(trackTitles) << std::endl;
    return trackTitles.size();
}

void PlaylistComponent::paintRowBackground (Graphics& g,
                                            int rowNumber,
                                            int width,
                                            int height,
                                            bool rowIsSelected)
{
    if (rowIsSelected) {
        g.fillAll(Colours::orange);
    } else {
        g.fillAll(Colours::darkgrey);
    }
}

void PlaylistComponent::paintCell (Graphics& g,
                                   int rowNumber,
                                   int columnId,
                                   int width,
                                   int height,
                                   bool rowIsSelected)
{
    g.drawText(trackTitles[rowNumber], 2, 0, width - 4, height, Justification::centredLeft, true);
    //g.drawText("4min23s", 2, 0, width - 4, height, Justification::centredLeft, true);
}

Component* PlaylistComponent::refreshComponentForCell (int rowNumber,
                                    int columnId,
                                    bool isRowSelected,
                                    Component* existingComponentToUpdate)
{
    if (columnId == 4) {
        if (existingComponentToUpdate == nullptr)
        {
            TextButton* btn = new TextButton{"DELETE"};
            String id{std::to_string(rowNumber)};
            btn->setComponentID(id);
            btn->addListener(this);
            existingComponentToUpdate = btn;
        }
    }
    return existingComponentToUpdate;
}

void PlaylistComponent::buttonClicked(Button *button)
{
    if (button == &searchButton) {
        if ( !trackTitles.empty())
        {
            std::cout << "search " << std::endl;
            trackTitles_bk = trackTitles;
            for (int i = 0; i < trackTitles.size(); ++i) {
                if (trackTitles.at(i).find("Brotherhood") != std::string::npos) {
                    std::cout << "track is found" << std::endl;
                } else {
                    trackTitles.erase(trackTitles.begin() + i);
                }
            }
    //        if (std::find(trackTitles.begin(), trackTitles.end(), "Brotherhood.mp3") != trackTitles.end()) {
    //            std::cout << "track is found" << std::endl;
    //        } else {
    //            std::cout << "track not found" << std::endl;
    //        }
        }
    } else if (button == &clearButton) {
        if (!trackTitles_bk.empty()) {
            trackTitles = trackTitles_bk;
        }
    }
    else
    {
        // delete button clicked
        int id = std::stoi(button->getComponentID().toStdString());
        std::cout << "delete the track " << trackTitles[id] << std::endl;
        // Delete selected track from playlist
        trackTitles.erase(trackTitles.begin() + id);
    }
}

void PlaylistComponent::addToPlaylist(std::string trackName)
{
    std::cout << std::addressof(trackTitles) << std::endl;
    trackTitles.push_back(trackName);
    // delete the oldest track if there are more than 6 tracks in the playlist
    if (trackTitles.size() > 6)
    {
        trackTitles.erase(trackTitles.begin());
    }
    
    //tableComponent.repaintRow(1);
    tableComponent.updateContent();
    tableComponent.repaint();
    
    //tableComponent.repaint();
    //tableComponent.refreshComponentForRow(0, true, <#Component *existingComponentToUpdate#>)
}
