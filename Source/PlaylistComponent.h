/*
  ==============================================================================

    PlaylistComponent.h
    Created: 1 Sep 2021 4:26:52pm
    Author:  Kimi

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include <vector>
#include <string>

//==============================================================================
/*
*/
class PlaylistComponent  : public juce::Component,
                           public TableListBoxModel,
                           public Button::Listener
{
public:
    PlaylistComponent();
    ~PlaylistComponent() override;

    void paint (juce::Graphics&) override;
    void resized() override;

    int getNumRows() override;
    
    void paintRowBackground (Graphics& g,
                             int rowNumber,
                             int width,
                             int height,
                             bool rowIsSelected) override;
    
    void paintCell (Graphics& g,
                    int rowNumber,
                    int columnId,
                    int width,
                    int height,
                    bool rowIsSelected) override;
    
    Component* refreshComponentForCell (int rowNumber,
                                        int columnId,
                                        bool isRowSelected,
                                        Component* existingComponentToUpdate) override;
    
    void buttonClicked(Button *button) override;
    void addToPlaylist(std::string trackName);
    
private:
    TableListBox tableComponent;
    TextEditor inputArea;
    TextButton searchButton{"SEARCH"};
    TextButton clearButton{"CLEAR"};
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PlaylistComponent)
};
