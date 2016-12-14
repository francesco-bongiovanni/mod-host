/*
 * This file is part of mod-host.
 *
 * mod-host is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * mod-host is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with mod-host.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "mod-link.h"

#include "AudioEngine.hpp"
#include "ableton/link/HostTimeFilter.hpp"
#include <chrono>

class LinkTransport {
public:
    LinkTransport(double bpm, double bufferSize, double sampleRate)
        : link(bpm),
          engine(link),
          outputLatency(0),
          sampleTime(0)
    {
        outputLatency = std::chrono::microseconds(llround(1.0e6 * bufferSize / sampleRate));

        // testing
        setEnabled(true);
    }

    void setEnabled(bool enabled)
    {
        link.enable(enabled);
    }

    void setTempo(double tempo)
    {
        engine.setTempo(tempo);
    }

    void process(uint32_t frames, LinkTimeInfo* info)
    {
        const auto hostTime = hostTimeFilter.sampleTimeToHostTime(sampleTime);

        sampleTime += frames;

        const auto bufferBeginAtOutput = hostTime + outputLatency;

        engine.audioCallback(bufferBeginAtOutput, info);
    }

private:
    ableton::Link link;
    ableton::linkaudio::AudioEngine engine;

    ableton::link::HostTimeFilter<ableton::platforms::stl::Clock> hostTimeFilter;
    std::chrono::microseconds outputLatency;
    double sampleTime;
};

link_t* link_create(double bpm, uint32_t buffer_size, uint32_t sample_rate)
{
    LinkTransport* t;

    try {
        t = new LinkTransport(bpm, buffer_size, sample_rate);
    } catch (...) {
        return nullptr;
    }

    return (link_t*)t;
}

void link_enable(link_t* link, bool on)
{
    ((LinkTransport*)link)->setEnabled(on);
}

void link_set_tempo(link_t* link, double bpm)
{
    ((LinkTransport*)link)->setTempo(bpm);
}

void link_process(link_t* link, uint32_t frames, link_time_info_t* info)
{
    ((LinkTransport*)link)->process(frames, (LinkTimeInfo*)info);
}

void link_cleanup(link_t* link)
{
    delete (LinkTransport*)link;
}

#include "AudioEngine.cpp"
