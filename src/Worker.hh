/*
 *  Worker.cpp - Is the owner of a Processor thread
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
 *  Authors:  David Cassany <david.cassany@i2cat.net>
 *
 *
 */

#ifndef _WORKER_HH
#define _WORKER_HH

#include <thread>
#include <chrono>
#include <mutex>
#include <queue>

#include "Types.hh"
#include "Jzon.h"
#include "Utils.hh"
#include "Runnable.hh"

#define MAX_SLAVE 16

/*! Worker class is a pure abstract class that is mostly in charge of executing
    the process method of one or several filters in a dedicated thread. 
*/
class Worker {

public:

    /**
    * Creates a worker object
    */
    Worker();
    virtual ~Worker();

    /**
    * Starts worker thread
    * @return True if succeded and false if not
    */
    bool start();

    /**
    * Check worker running status
    * @return True if running and false if not
    */
    bool isRunning();

    /**
    * Tell its thread to stop and waits until done
    */
    virtual void stop();

    /**
    * Adds a processor to the worker
    * @param id processor ID
    * @param processor object to add
    * @return True if succeded and false if not
    */
    bool addProcessor(int id, Runnable *processor);

    /**
    * Removes a processor from the worker's list
    * @param id processor ID
    * @return True if succeded and false if not
    */
    bool removeProcessor(int id);

    /**
    * Get worker type
    * @return returns worker's type, see Types.hh
    */
    WorkerType getType(){return type;};

    /**
    * Get the number of processors
    * @return returns the number of current processors
    */
    size_t getProcessorsSize(){return processors.size();};

    /**
    * Fills the appropriate JSON state object
    * @param workerNode json reference to fill
    */
    void getState(Jzon::Object &workerNode);

protected:
    virtual void process();

    std::priority_queue<Runnable*, std::vector<Runnable*>, RunnableLess> processors;
    std::thread thread;
    bool run;
    bool enabled;

    WorkerType type;
    std::mutex mtx;
};

class LiveMediaWorker : public Worker {
public:
    /**
    * Creates a liveMediaWorker worker object, see worker types at Types.hh
    */
    LiveMediaWorker();

    /**
    * Tell its thread to stop and waits until done
    */
    void stop();
private:
    void process();
};

#endif
