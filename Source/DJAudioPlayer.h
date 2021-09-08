#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

class DJAudioPlayer : public AudioSource {
  public:

    DJAudioPlayer(AudioFormatManager& _formatManager);
    ~DJAudioPlayer();

    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override;

    void loadURL(URL audioURL);
    void setGain(double gain);
    void setSpeed(double ratio);
    void setPosition(double posInSecs);
    void setPositionRelative(double pos);
    /** get the relative position (between 0 and 1) of the playhead */
    double getPositionRelative();
    /** get the total length in second */
    double getTrackLengthInSeconds();
    /** return status of playing or stopped*/
    bool isPlaying();
    bool isEndOfTrack();
    void start();
    void stop();

private:
    AudioFormatManager& formatManager;
    std::unique_ptr<AudioFormatReaderSource> readerSource;
    AudioTransportSource transportSource; 
    ResamplingAudioSource resampleSource{&transportSource, false, 2};

};




