/*
 *  Callbacks.cpp - Collection of callback functions
 *  Copyright (C) 2014  Fundació i2CAT, Internet i Innovació digital a Catalunya
 *
 *  This file is part of liveMediaStreamer.
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 *  Authors:  Marc Palau <marc.palau@i2cat.net>,
 *            David Cassany <david.cassany@i2cat.net>
 *			  Martin German <martin.german@i2cat.net>
 */

#include "Callbacks.hh"
#include "Controller.hh"
#include "Filter.hh"
#include "Types.hh"
#include "Path.hh"
#include "Utils.hh"

namespace callbacks
{
    void connectToMixerCallback(char const* medium, unsigned short port)
    {
        PipelineManager* pipeMngr = Controller::getInstance()->pipelineManager();
        Path* path = NULL;
        int mixerID = -1;

        if (strcmp(medium, "audio") == 0) {
            path = new AudioDecoderPath(pipeMngr->getReceiverID(), (int)port);
            mixerID = pipeMngr->searchFilterIDByType(AUDIO_MIXER);

        } else if (strcmp(medium, "video") == 0) {
            path = new VideoDecoderPath(pipeMngr->getReceiverID(), (int)port);
            mixerID = pipeMngr->searchFilterIDByType(VIDEO_MIXER);
        }

        if (!path || mixerID == -1) {
            utils::errorMsg("No path or no mixer");
            return;
        }

        path->setDestinationFilter(mixerID, (int)port);

        if (!pipeMngr->connectPath(path)) {
            utils::errorMsg("Failed connection to path");
            return;
        }

        if (!pipeMngr->addPath(port, path)) {
            utils::errorMsg("Failed adding path");
            return;
        }

        return;
    }
    
    void connectToTransmitter(char const* medium, unsigned short port)
    {
        PipelineManager* pipeMngr = Controller::getInstance()->pipelineManager();
        Path* path = NULL;
        int transmitterID = pipeMngr->getTransmitterID();

        path = new Path(pipeMngr->getReceiverID(), (int)port);
        
        if (!path || transmitterID == -1) {
            utils::errorMsg("No path or transmitter");
            return;
        }

        path->setDestinationFilter(transmitterID, pipeMngr->getFilter(transmitterID)->generateReaderID());

        if (!pipeMngr->connectPath(path)) {
            utils::errorMsg("Failed connection to path");
            return;
        }

        if (!pipeMngr->addPath(port, path)) {
            utils::errorMsg("Failed adding path");
            return;
        }
        
        return;
    }
    
    void connectTranscoderToTransmitter(char const* medium, unsigned short port)
    {
        PipelineManager* pipeMngr = Controller::getInstance()->pipelineManager();
        Path* path = NULL;
        int transmitterID = pipeMngr->getTransmitterID();
        
        if (strcmp(medium, "audio") == 0) {
            path = new AudioTranscoderPath(pipeMngr->getReceiverID(), (int)port);
        } else if (strcmp(medium, "video") == 0) {
            path = new VideoTranscoderPath(pipeMngr->getReceiverID(), (int)port);
        }
        
        if (!path || transmitterID == -1) {
            utils::errorMsg("No path or no mixer");
            return;
        }
        
        path->setDestinationFilter(transmitterID, pipeMngr->getFilter(transmitterID)->generateReaderID());
        
        if (!pipeMngr->connectPath(path)) {
            utils::errorMsg("Failed connection to path");
            return;
        }
        
        if (!pipeMngr->addPath(port, path)) {
            utils::errorMsg("Failed adding path");
            return;
        }
        
        return;
    }

	void connectToDecoderCallback(char const* medium, unsigned short port)
	{
        PipelineManager* pipeMngr = Controller::getInstance()->pipelineManager();
        Path* path = NULL;

        path = new Path(pipeMngr->getReceiverID(), (int)port);
        
        if (!path == -1) {
            std::cerr << "[Callback] No path or no transmitter!" << std::endl;
            return;
        }

        if (!pipeMngr->addPath(port, path)) {
            //TODO: ERROR
            return;
        }
        
        return;
	}
}